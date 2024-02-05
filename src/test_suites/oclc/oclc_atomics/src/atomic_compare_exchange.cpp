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
#include <random>
#include <string>
#include <test_config.hpp>
#include <vector>

namespace ca = cassian;

namespace {

template <typename T> struct Input {
  std::vector<T> value;
  std::vector<T> expected;
  std::vector<T> desired;
};

template <typename T> struct TestCase {
  using test_type = T;
  using test_host_type = typename T::host_type;

  MemoryType memory_type = MemoryType::global;

  FunctionType function_type = FunctionType::implicit;
  MemoryOrder success_memory_order = MemoryOrder::relaxed;
  MemoryOrder failure_memory_order = MemoryOrder::relaxed;
  MemoryScope memory_scope = MemoryScope::device;

  ComparisonType comparison_type = ComparisonType::strong;
  ComparisonResult comparison_result = ComparisonResult::success;

  int global_work_size = 0;
  int local_work_size = 0;

  ca::Runtime *runtime = nullptr;
  std::string program_type;

  Input<test_host_type> input;
};

template <typename T>
Input<T> generate_input(const int size,
                        const ComparisonResult expected_result) {
  Input<T> input;
  input.value = ca::generate_vector<T>(size, 0);
  input.desired = ca::generate_vector<T>(size, 0);
  if (expected_result == ComparisonResult::success) {
    input.expected = input.value;
  } else if (expected_result == ComparisonResult::failure) {
    input.expected = ca::generate_vector<T>(size, 0);
  }
  return input;
}

std::string get_kernel_path(const MemoryType memory_type) {
  switch (memory_type) {
  case MemoryType::global:
    return "kernels/oclc_atomics/atomic_compare_exchange_global.cl";
  case MemoryType::local:
    return "kernels/oclc_atomics/atomic_compare_exchange_local.cl";
  default:
    return "unknown";
  }
}

std::string
success_memory_order_build_option(const MemoryOrder success_memory_order) {
  return std::string(" -D SUCCESS_MEMORY_ORDER=") +
         to_string(success_memory_order);
}

std::string
failure_memory_order_build_option(const MemoryOrder failure_memory_order) {
  return std::string(" -D FAILURE_MEMORY_ORDER=") +
         to_string(failure_memory_order);
}

std::string comparison_type_build_option(const ComparisonType comparison_type) {
  return std::string(" -D COMPARISON_TYPE=") + to_string(comparison_type);
}

template <typename TEST_TYPE>
std::string get_build_options(const int local_work_size,
                              const FunctionType function_type,
                              const MemoryOrder success_memory_order,
                              const MemoryOrder failure_memory_order,
                              const MemoryScope memory_scope,
                              const ComparisonType comparison_type) {
  std::string build_options = " -cl-std=CL3.0" +
                              atomic_type_build_option<TEST_TYPE>() +
                              data_type_build_option<TEST_TYPE>() +
                              comparison_type_build_option(comparison_type) +
                              work_group_size_build_option(local_work_size);
  if (function_type == FunctionType::explicit_memory_order) {
    build_options += success_memory_order_build_option(success_memory_order) +
                     failure_memory_order_build_option(failure_memory_order);
  } else if (function_type == FunctionType::explicit_memory_scope) {
    build_options += success_memory_order_build_option(success_memory_order) +
                     failure_memory_order_build_option(failure_memory_order) +
                     memory_scope_build_option(memory_scope);
  }

  return build_options;
}

template <typename T> struct Output {
  std::vector<T> value;
  std::vector<T> fetched;
  std::vector<int32_t> result;
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

  ca::Buffer expected_buffer =
      runtime->create_buffer(sizeof(T) * input.expected.size());
  runtime->write_buffer_from_vector(expected_buffer, input.expected);
  buffers.push_back(expected_buffer);

  ca::Buffer desired_buffer =
      runtime->create_buffer(sizeof(T) * input.desired.size());
  runtime->write_buffer_from_vector(desired_buffer, input.desired);
  buffers.push_back(desired_buffer);

  ca::Buffer result_buffer =
      runtime->create_buffer(sizeof(int32_t) * global_work_size);
  buffers.push_back(result_buffer);

  for (size_t i = 0; i < buffers.size(); ++i) {
    runtime->set_kernel_argument(kernel, static_cast<int>(i), buffers[i]);
  }

  runtime->run_kernel(kernel, global_work_size, local_work_size);

  Output<T> output;
  output.value = runtime->read_buffer_to_vector<T>(value_buffer);
  output.fetched = runtime->read_buffer_to_vector<T>(expected_buffer);
  output.result = runtime->read_buffer_to_vector<int32_t>(result_buffer);

  for (auto &buffer : buffers) {
    runtime->release_buffer(buffer);
  }

  return output;
}

template <typename TEST_CASE_TYPE> void run_test(TEST_CASE_TYPE test_case) {
  using test_type = typename TEST_CASE_TYPE::test_type;
  using test_host_type = typename TEST_CASE_TYPE::test_host_type;

  const std::string kernel_path = get_kernel_path(test_case.memory_type);
  const std::string build_options = get_build_options<test_type>(
      test_case.local_work_size, test_case.function_type,
      test_case.success_memory_order, test_case.failure_memory_order,
      test_case.memory_scope, test_case.comparison_type);
  const ca::Kernel kernel = create_kernel(
      kernel_path, build_options, test_case.runtime, test_case.program_type);

  const Output<test_host_type> output =
      run_kernel(kernel, test_case.global_work_size, test_case.local_work_size,
                 test_case.input, test_case.runtime);
  test_case.runtime->release_kernel(kernel);

  REQUIRE_THAT(output.fetched, Catch::Equals(test_case.input.value));
  REQUIRE_THAT(output.result,
               Catch::Equals(to_bool_vector(test_case.comparison_result,
                                            test_case.global_work_size)));

  if (test_case.comparison_result == ComparisonResult::failure) {
    REQUIRE_THAT(output.value, Catch::Equals(test_case.input.value));
  } else if (test_case.comparison_result == ComparisonResult::success) {
    REQUIRE_THAT(output.value, Catch::Equals(test_case.input.desired));
  }
}

template <typename TEST_CASE_TYPE>
void set_memory_orders(TEST_CASE_TYPE test_case,
                       const std::vector<MemoryOrder> &failure_memory_orders,
                       const std::vector<MemoryOrder> &success_memory_orders) {
  for (const auto success_memory_order : success_memory_orders) {
    test_case.success_memory_order = success_memory_order;
    SECTION("success_" + to_string(success_memory_order)) {
      if (!is_memory_order_supported(test_case.runtime, test_case.program_type,
                                     success_memory_order)) {
        ca::logging::info()
            << to_string(success_memory_order) << " section skipped\n";
        continue;
      }
      for (const auto failure_memory_order : failure_memory_orders) {
        test_case.failure_memory_order = failure_memory_order;
        SECTION("failure_" + to_string(failure_memory_order)) {
          if (failure_memory_order > success_memory_order) {
            continue;
          }
          run_test(test_case);
        }
      }
    }
  }
}

template <typename TEST_CASE_TYPE>
void test_signatures(TEST_CASE_TYPE test_case,
                     const std::vector<MemoryType> &memory_types,
                     const std::vector<FunctionType> &functions_types,
                     const std::vector<MemoryOrder> &failure_memory_orders,
                     const std::vector<MemoryOrder> &success_memory_orders,
                     const std::vector<MemoryScope> &memory_scopes,
                     const std::vector<ComparisonType> &comparison_types,
                     const std::vector<ComparisonResult> &comparison_results) {
  for (const auto memory_type : memory_types) {
    test_case.memory_type = memory_type;
    SECTION(to_string(memory_type)) {
      for (const auto comparison_result : comparison_results) {
        test_case.comparison_result = comparison_result;
        test_case.input =
            generate_input<typename TEST_CASE_TYPE::test_host_type>(
                test_case.global_work_size, comparison_result);
        SECTION(to_string(comparison_result)) {
          for (const auto comparison_type : comparison_types) {
            test_case.comparison_type = comparison_type;
            SECTION(to_string(comparison_type)) {
              for (const auto function_type : functions_types) {
                test_case.function_type = function_type;
                SECTION(to_string(function_type)) {
                  if (!is_function_type_supported(test_case.runtime,
                                                  test_case.program_type,
                                                  function_type)) {
                    ca::logging::info()
                        << to_string(function_type) << " section skipped\n";
                    continue;
                  }
                  if (function_type == FunctionType::implicit) {
                    run_test(test_case);
                  } else if (function_type ==
                             FunctionType::explicit_memory_order) {
                    set_memory_orders(test_case, failure_memory_orders,
                                      success_memory_orders);
                  } else if (function_type ==
                             FunctionType::explicit_memory_scope) {
                    for (const auto memory_scope : memory_scopes) {
                      test_case.memory_scope = memory_scope;
                      SECTION(to_string(memory_scope)) {
                        if (!is_memory_scope_supported(test_case.runtime,
                                                       test_case.program_type,
                                                       memory_scope)) {
                          ca::logging::info() << to_string(memory_scope)
                                              << " section skipped\n";
                          continue;
                        }
                        set_memory_orders(test_case, failure_memory_orders,
                                          success_memory_orders);
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
}

TEMPLATE_TEST_CASE("atomic_compare_exchange_signatures", "", ca::clc_int_t,
                   ca::clc_uint_t, ca::clc_long_t, ca::clc_ulong_t,
                   ca::clc_intptr_t, ca::clc_uintptr_t, ca::clc_size_t,
                   ca::clc_ptrdiff_t) {
  const TestConfig &config = get_test_config();

  using test_case_type = TestCase<TestType>;
  auto test_case = create_test_case<test_case_type>(config);
  if (should_skip(test_case)) {
    return;
  }

  test_signatures<test_case_type>(
      test_case, memory_types_all, function_types_all,
      {MemoryOrder::relaxed, MemoryOrder::acquire, MemoryOrder::seq_cst},
      memory_orders_all,
      {MemoryScope::work_group, MemoryScope::device,
       MemoryScope::all_svm_devices},
      comparison_types_all, comparison_results_all);
}

} // namespace