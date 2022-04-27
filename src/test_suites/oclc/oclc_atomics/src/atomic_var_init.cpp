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

  int global_work_size = 0;
  int local_work_size = 0;

  ca::Runtime *runtime = nullptr;
  std::string program_type;

  test_host_type input = 0;
};

std::string get_kernel_path() {
  return "kernels/oclc_atomics/atomic_var_init.cl";
}

template <typename TEST_HOST_TYPE>
std::string input_build_option(TEST_HOST_TYPE input) {
  return std::string(" -D INPUT=") + std::to_string(input);
}

template <typename TEST_CASE_TYPE>
std::string get_build_options(TEST_CASE_TYPE test_case) {
  using test_type = typename TEST_CASE_TYPE::test_type;
  using test_host_type = typename TEST_CASE_TYPE::test_host_type;
  std::string build_options =
      " -cl-std=CL3.0" + atomic_type_build_option<test_type>() +
      data_type_build_option<test_type>() +
      input_build_option<test_host_type>(test_case.input);
  return build_options;
}

template <typename test_host_type>
test_host_type run_kernel(const ca::Kernel &kernel, const int global_work_size,
                          const int local_work_size, ca::Runtime *runtime) {
  ca::Buffer output_buffer = runtime->create_buffer(sizeof(test_host_type));
  runtime->set_kernel_argument(kernel, static_cast<int>(0), output_buffer);

  runtime->run_kernel(kernel, global_work_size, local_work_size);

  test_host_type output;
  runtime->read_buffer(output_buffer, &output);

  runtime->release_buffer(output_buffer);
  return output;
}

template <typename TEST_CASE_TYPE> void run_test(TEST_CASE_TYPE test_case) {
  using test_type = typename TEST_CASE_TYPE::test_type;
  using test_host_type = typename TEST_CASE_TYPE::test_host_type;

  const std::string kernel_path = get_kernel_path();
  const std::string build_options =
      get_build_options<TEST_CASE_TYPE>(test_case);
  const ca::Kernel kernel = create_kernel(
      kernel_path, build_options, test_case.runtime, test_case.program_type);

  const auto output =
      run_kernel<test_host_type>(kernel, test_case.global_work_size,
                                 test_case.local_work_size, test_case.runtime);
  test_case.runtime->release_kernel(kernel);

  REQUIRE(output == test_case.input);
}

TEMPLATE_TEST_CASE("ATOMIC_VAR_INIT", "", ca::clc_int_t, ca::clc_uint_t,
                   ca::clc_long_t, ca::clc_ulong_t, ca::clc_float_t,
                   ca::clc_double_t, ca::clc_intptr_t, ca::clc_uintptr_t,
                   ca::clc_size_t, ca::clc_ptrdiff_t) {
  const TestConfig &config = get_test_config();

  using test_case_type = TestCase<TestType>;
  auto test_case = create_test_case<test_case_type>(config);
  if (should_skip(test_case)) {
    return;
  }

  test_case.input =
      ca::generate_value<typename test_case_type::test_host_type>(0);

  run_test(test_case);
}

} // namespace