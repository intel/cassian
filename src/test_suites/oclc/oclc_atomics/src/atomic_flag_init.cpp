/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <algorithm>
#include <cassian/random/random.hpp>
#include <cassian/runtime/openclc_types.hpp>
#include <cassian/runtime/openclc_utils.hpp>
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
};

std::string get_kernel_path() {
  return "kernels/oclc_atomics/atomic_flag_init.cl";
}

template <typename TEST_TYPE> std::string get_build_options() {
  std::string build_options =
      " -cl-std=CL3.0" + data_type_build_option<TEST_TYPE>();
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
  const std::string build_options = get_build_options<test_type>();
  const ca::Kernel kernel = create_kernel(
      kernel_path, build_options, test_case.runtime, test_case.program_type);

  const auto output =
      run_kernel<test_host_type>(kernel, test_case.global_work_size,
                                 test_case.local_work_size, test_case.runtime);
  test_case.runtime->release_kernel(kernel);

  REQUIRE(output == 0);
}

TEST_CASE("ATOMIC_FLAG_INIT", "") {
  const TestConfig &config = get_test_config();

  using test_case_type = TestCase<ca::clc_int_t>;
  auto test_case = create_test_case<test_case_type>(config);
  if (!ca::check_optional_openclc_feature_support(
          test_case.runtime, test_case.program_type,
          "__opencl_c_program_scope_global_variables")) {
    return;
  }
  run_test(test_case);
}

} // namespace