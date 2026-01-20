/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/logging/logging.hpp>
#include <cassian/random/random.hpp>
#include <cassian/runtime/openclc_types.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cassian/test_harness/test_harness.hpp>
#include <cassian/utility/utility.hpp>
#include <catch2/catch.hpp>
#include <common.hpp>
#include <cstddef>
#include <string>
#include <test_config.hpp>
#include <vector>

namespace ca = cassian;

namespace {

template <typename T> struct Input {
  std::vector<T> value;
  std::vector<T> operand;
};

template <typename T> struct TestCase {
  using test_type = T;
  using test_host_type = typename T::host_type;

  MemoryType memory_type = MemoryType::global;

  FunctionType function_type = FunctionType::implicit;
  MemoryOrder memory_order = MemoryOrder::relaxed;
  MemoryScope memory_scope = MemoryScope::device;
  AddressSpaceCastMode cast_mode = AddressSpaceCastMode::original;

  int global_work_size = 0;
  int local_work_size = 0;

  ca::Runtime *runtime = nullptr;
  std::string program_type;

  Input<test_host_type> input;
};

template <typename T> Input<T> generate_input(const int size) {
  Input<T> input;
  input.value = ca::generate_vector<T>(size, 0);
  input.operand = ca::generate_vector<T>(size, 0);
  return input;
}

std::string get_kernel_path(const MemoryType memory_type) {
  switch (memory_type) {
  case MemoryType::global:
    return "kernels/oclc_atomics/atomic_exchange_global.cl";
  case MemoryType::local:
    return "kernels/oclc_atomics/atomic_exchange_local.cl";
  default:
    return "unknown";
  }
}

template <typename TEST_TYPE>
std::string
get_build_options(const int local_work_size, const FunctionType function_type,
                  const MemoryOrder memory_order,
                  const MemoryScope memory_scope, const MemoryType memory_type,
                  const AddressSpaceCastMode cast_mode) {
  std::string build_options =
      " -cl-std=CL3.0" + atomic_type_build_option<TEST_TYPE>() +
      data_type_build_option<TEST_TYPE>() +
      work_group_size_build_option(local_work_size) +
      pointer_to_named_address_space_atomic_cast_to_generic_build_option<
          TEST_TYPE>(memory_type, cast_mode);

  if (function_type == FunctionType::explicit_memory_order) {
    build_options += memory_order_build_option(memory_order);
  } else if (function_type == FunctionType::explicit_memory_scope) {
    build_options += memory_order_build_option(memory_order) +
                     memory_scope_build_option(memory_scope);
  }

  return build_options;
}

template <typename T> struct Output {
  std::vector<T> result;
  std::vector<T> fetched;
};

template <typename T>
Output<T> run_kernel(const ca::Kernel &kernel, const int global_work_size,
                     const int local_work_size, const Input<T> &input,
                     ca::Runtime *runtime) {
  std::vector<ca::Buffer> buffers;

  ca::Buffer value_buffer =
      runtime->create_buffer(sizeof(T) * input.value.size());
  runtime->write_buffer_from_vector(value_buffer, input.value);
  buffers.push_back(value_buffer);

  ca::Buffer operand_buffer =
      runtime->create_buffer(sizeof(T) * input.operand.size());
  runtime->write_buffer_from_vector(operand_buffer, input.operand);
  buffers.push_back(operand_buffer);

  ca::Buffer fetched_buffer =
      runtime->create_buffer(sizeof(T) * input.value.size());
  buffers.push_back(fetched_buffer);

  for (size_t i = 0; i < buffers.size(); ++i) {
    runtime->set_kernel_argument(kernel, static_cast<int>(i), buffers[i]);
  }

  runtime->run_kernel(kernel, global_work_size, local_work_size);

  Output<T> output;
  output.result = runtime->read_buffer_to_vector<T>(value_buffer);
  output.fetched = runtime->read_buffer_to_vector<T>(fetched_buffer);

  for (auto &buffer : buffers) {
    runtime->release_buffer(buffer);
  }

  return output;
}

template <typename TEST_CASE_TYPE> void run_test(TEST_CASE_TYPE test_case) {
  using test_type = typename TEST_CASE_TYPE::test_type;
  using test_host_type = typename TEST_CASE_TYPE::test_host_type;

  ca::Requirements requirements;
  function_type_requirements(requirements, test_case.program_type,
                             test_case.function_type);
  memory_scope_requirements(requirements, test_case.program_type,
                            test_case.memory_scope);
  memory_order_requirements(requirements, test_case.program_type,
                            test_case.memory_order);
  cast_mode_requirements(requirements, test_case.program_type,
                         test_case.cast_mode);
  if (ca::should_skip_test(requirements, *test_case.runtime)) {
    return;
  }

  const std::string kernel_path = get_kernel_path(test_case.memory_type);
  const std::string build_options = get_build_options<test_type>(
      test_case.local_work_size, test_case.function_type,
      test_case.memory_order, test_case.memory_scope, test_case.memory_type,
      test_case.cast_mode);
  const ca::Kernel kernel = create_kernel(
      kernel_path, build_options, test_case.runtime, test_case.program_type);

  const Output<test_host_type> output =
      run_kernel(kernel, test_case.global_work_size, test_case.local_work_size,
                 test_case.input, test_case.runtime);
  test_case.runtime->release_kernel(kernel);

  REQUIRE_THAT(output.fetched, Catch::Equals(test_case.input.value));
  REQUIRE_THAT(output.result, Catch::Equals(test_case.input.operand));
}

template <typename TEST_CASE_TYPE>
void test_signatures(TEST_CASE_TYPE test_case,
                     const std::vector<MemoryType> &memory_types,
                     const std::vector<FunctionType> &function_types,
                     const std::vector<MemoryOrder> &memory_orders,
                     const std::vector<MemoryScope> &memory_scope,
                     const std::vector<AddressSpaceCastMode> &cast_modes) {
  for (const auto cast_mode : cast_modes) {
    test_case.cast_mode = cast_mode;
    SECTION(to_string(cast_mode)) {
      for (const auto memory_type : memory_types) {
        test_case.memory_type = memory_type;
        SECTION(to_string(memory_type)) {
          for (const auto function_type : function_types) {
            test_case.function_type = function_type;
            SECTION(to_string(function_type)) {
              if (function_type == FunctionType::implicit) {
                run_test(test_case);
              } else if (function_type == FunctionType::explicit_memory_order) {
                for (const auto memory_order : memory_orders) {
                  test_case.memory_order = memory_order;
                  SECTION(to_string(memory_order)) { run_test(test_case); }
                }
              } else if (function_type == FunctionType::explicit_memory_scope) {
                for (const auto memory_scope : memory_scope) {
                  test_case.memory_scope = memory_scope;
                  SECTION(to_string(memory_scope)) {
                    for (const auto memory_order : memory_orders) {
                      test_case.memory_order = memory_order;
                      SECTION(to_string(memory_order)) { run_test(test_case); }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

TEMPLATE_TEST_CASE("atomic_exchange_signatures", "", ca::clc_int_t,
                   ca::clc_uint_t, ca::clc_long_t, ca::clc_ulong_t,
                   ca::clc_half_t, ca::clc_float_t, ca::clc_double_t,
                   ca::clc_intptr_t, ca::clc_uintptr_t, ca::clc_size_t,
                   ca::clc_ptrdiff_t) {
  const TestConfig &config = get_test_config();

  using test_case_type = TestCase<TestType>;
  auto test_case = create_test_case<test_case_type>(config);
  if (should_skip(test_case)) {
    return;
  }

  test_case.input = generate_input<typename test_case_type::test_host_type>(
      test_case.global_work_size);
  test_signatures(test_case, memory_types_all, function_types_all,
                  memory_orders_all, memory_scopes_all,
                  address_space_casts_modes_all);
}

} // namespace