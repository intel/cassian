/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <algorithm>
#include <cassian/catch2_utils/catch2_utils.hpp>
#include <cassian/cli/cli.hpp>
#include <cassian/runtime/openclc_type_tuples.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cassian/test_harness/test_harness.hpp>
#include <cassian/utility/metaprogramming.hpp>
#include <cassian/utility/utility.hpp>
#include <catch2/catch.hpp>
#include <cstddef>
#include <numeric>
#include <string>
#include <test_config.hpp>
#include <vector>

namespace ca = cassian;

namespace {

template <typename T> int get_reference() {
  using data_host_type = typename T::host_type;
  if constexpr (ca::is_vector_v<data_host_type>) {
    return data_host_type::size_in_memory;
  }
  return 1;
}

template <typename T> std::string get_build_options() {
  const std::string clc_data_type = T::device_type;
  std::string build_options = " -D DATA_TYPE=" + clc_data_type;
  return build_options;
}

ca::Kernel create_kernel(const std::string &path, const std::string &name,
                         const std::string &build_options,
                         cassian::Runtime *runtime,
                         const std::string &program_type) {
  const std::string source = ca::load_text_file(ca::get_asset(path));
  return runtime->create_kernel(name, source, build_options, program_type);
}

std::vector<int> generate_input(const size_t elements) {
  std::vector<int> input(elements);
  std::iota(input.begin(), input.end(), 0);
  return input;
}

template <typename T>
std::vector<int> run_data_type_kernel(const cassian::Kernel &kernel,
                                      const size_t global_work_size,
                                      const std::vector<T> &input,
                                      cassian::Runtime *runtime) {
  std::vector<ca::Buffer> buffers;

  ca::Buffer input_buffer = runtime->create_buffer(sizeof(T) * input.size());
  runtime->write_buffer_from_vector(input_buffer, input);
  buffers.push_back(input_buffer);

  ca::Buffer output_buffer =
      runtime->create_buffer(sizeof(int) * global_work_size);
  buffers.push_back(output_buffer);

  runtime->set_kernel_argument(kernel, 0, input_buffer);
  runtime->set_kernel_argument(kernel, 1, output_buffer);

  runtime->run_kernel(kernel, global_work_size);

  std::vector<int> output = runtime->read_buffer_to_vector<int>(output_buffer);

  for (auto &buffer : buffers) {
    runtime->release_buffer(buffer);
  }

  return output;
}

template <typename T>
std::vector<int> run_pure_type_kernel(const cassian::Kernel &kernel,
                                      const size_t global_work_size,
                                      cassian::Runtime *runtime) {
  std::vector<ca::Buffer> buffers;

  ca::Buffer output_buffer =
      runtime->create_buffer(sizeof(int) * global_work_size);
  buffers.push_back(output_buffer);

  runtime->set_kernel_argument(kernel, 0, output_buffer);

  runtime->run_kernel(kernel, global_work_size);

  std::vector<int> output = runtime->read_buffer_to_vector<int>(output_buffer);

  for (auto &buffer : buffers) {
    runtime->release_buffer(buffer);
  }

  return output;
}

using GentypeTypes = ca::TupleConcat<ca::ScalarTypes, ca::VectorTypes>::type;

template <typename T> std::string test_name() {
  return std::string(T::type_name);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("vec_step_data_type", "", GentypeTypes,
                                    test_name<TestType>) {
  const TestConfig &config = get_test_config();

  ca::Requirements requirements;
  requirements.arithmetic_type<typename TestType::scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  ca::Runtime *runtime = config.runtime();
  const std::string program_type = config.program_type();
  const size_t global_work_size = config.work_size();

  using data_host_type = typename TestType::host_type;
  const std::vector<data_host_type> input(global_work_size,
                                          data_host_type(123));

  const std::string build_options = get_build_options<TestType>();
  ca::Kernel kernel =
      create_kernel("kernels/oclc_miscellaneous_vector_functions/vec_step.cl",
                    "test_data_type", build_options, runtime, program_type);
  const std::vector<int> output =
      run_data_type_kernel(kernel, global_work_size, input, runtime);
  runtime->release_kernel(kernel);

  const std::vector<int> reference =
      std::vector<int>(global_work_size, get_reference<TestType>());
  REQUIRE_THAT(output, Catch::Equals(reference));
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("vec_step_pure_type", "", GentypeTypes,
                                    test_name<TestType>) {
  const TestConfig &config = get_test_config();

  ca::Requirements requirements;
  requirements.arithmetic_type<typename TestType::scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  ca::Runtime *runtime = config.runtime();
  const std::string program_type = config.program_type();
  const size_t global_work_size = config.work_size();

  const std::string build_options = get_build_options<TestType>();
  ca::Kernel kernel =
      create_kernel("kernels/oclc_miscellaneous_vector_functions/vec_step.cl",
                    "test_pure_type", build_options, runtime, program_type);
  using data_host_type = typename TestType::host_type;
  const std::vector<int> output =
      run_pure_type_kernel<data_host_type>(kernel, global_work_size, runtime);
  runtime->release_kernel(kernel);

  const std::vector<int> reference =
      std::vector<int>(global_work_size, get_reference<TestType>());
  REQUIRE_THAT(output, Catch::Equals(reference));
}

} // namespace
