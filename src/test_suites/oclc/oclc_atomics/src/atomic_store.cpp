/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <algorithm>
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

template <typename T> struct TestCase {
  using test_type = T;
  using test_host_type = typename T::host_type;

  MemoryType memory_type = MemoryType::global;

  FunctionType function_type = FunctionType::implicit;
  MemoryOrder memory_order = MemoryOrder::relaxed;
  MemoryScope memory_scope = MemoryScope::device;

  int global_work_size = 0;
  int local_work_size = 0;

  ca::Runtime *runtime = nullptr;
  std::string program_type;

  std::vector<test_host_type> input;
};

std::string get_kernel_path(const MemoryType memory_type) {
  switch (memory_type) {
  case MemoryType::global:
    return "kernels/oclc_atomics/atomic_store_global.cl";
  case MemoryType::local:
    return "kernels/oclc_atomics/atomic_store_local.cl";
  default:
    return "unknown";
  }
}

template <typename TEST_TYPE>
std::string get_build_options(const int local_work_size,
                              const FunctionType function_type,
                              const MemoryOrder memory_order,
                              const MemoryScope memory_scope) {
  std::string build_options = " -cl-std=CL3.0" +
                              atomic_type_build_option<TEST_TYPE>() +
                              data_type_build_option<TEST_TYPE>() +
                              work_group_size_build_option(local_work_size);

  if (function_type == FunctionType::implicit) {
  } else if (function_type == FunctionType::explicit_memory_order) {
    build_options += memory_order_build_option(memory_order);
  } else if (function_type == FunctionType::explicit_memory_scope) {
    build_options += memory_order_build_option(memory_order) +
                     memory_scope_build_option(memory_scope);
  }

  return build_options;
}

template <typename T>
std::vector<T> run_kernel(const ca::Kernel &kernel, const int global_work_size,
                          const int local_work_size,
                          const std::vector<T> &input, ca::Runtime *runtime) {
  std::vector<ca::Buffer> buffers;

  ca::Buffer input_buffer = runtime->create_buffer(sizeof(T) * input.size());
  runtime->write_buffer_from_vector(input_buffer, input);
  buffers.push_back(input_buffer);

  ca::Buffer output_buffer = runtime->create_buffer(sizeof(T) * input.size());
  buffers.push_back(output_buffer);

  for (size_t i = 0; i < buffers.size(); ++i) {
    runtime->set_kernel_argument(kernel, static_cast<int>(i), buffers[i]);
  }

  runtime->run_kernel(kernel, global_work_size, local_work_size);

  std::vector<T> output = runtime->read_buffer_to_vector<T>(output_buffer);

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
  if (ca::should_skip_test(requirements, *test_case.runtime)) {
    return;
  }

  const std::string kernel_path = get_kernel_path(test_case.memory_type);
  const std::string build_options = get_build_options<test_type>(
      test_case.local_work_size, test_case.function_type,
      test_case.memory_order, test_case.memory_scope);
  const ca::Kernel kernel = create_kernel(
      kernel_path, build_options, test_case.runtime, test_case.program_type);

  const std::vector<test_host_type> output =
      run_kernel(kernel, test_case.global_work_size, test_case.local_work_size,
                 test_case.input, test_case.runtime);
  test_case.runtime->release_kernel(kernel);

  REQUIRE_THAT(output, Catch::Equals(test_case.input));
}

template <typename TEST_CASE_TYPE>
void test_signatures(TEST_CASE_TYPE test_case,
                     const std::vector<MemoryType> &memory_types,
                     const std::vector<FunctionType> &function_types,
                     const std::vector<MemoryOrder> &memory_orders,
                     const std::vector<MemoryScope> &memory_scopes) {
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
            for (const auto memory_scope : memory_scopes) {
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

TEMPLATE_TEST_CASE("atomic_store_signatures", "", ca::clc_int_t, ca::clc_uint_t,
                   ca::clc_long_t, ca::clc_ulong_t, ca::clc_half_t,
                   ca::clc_float_t, ca::clc_double_t, ca::clc_intptr_t,
                   ca::clc_uintptr_t, ca::clc_size_t, ca::clc_ptrdiff_t) {
  const TestConfig &config = get_test_config();

  using test_case_type = TestCase<TestType>;
  auto test_case = create_test_case<test_case_type>(config);
  if (should_skip(test_case)) {
    return;
  }

  test_case.input =
      ca::generate_vector<typename test_case_type::test_host_type>(
          test_case.global_work_size, 0);
  test_signatures(
      test_case, memory_types_all, function_types_all,
      {MemoryOrder::relaxed, MemoryOrder::acquire, MemoryOrder::seq_cst},
      memory_scopes_all);
}

} // namespace
