/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <algorithm>
#include <cassian/catch2_utils/catch2_utils.hpp>
#include <cassian/cli/cli.hpp>
#include <cassian/runtime/device_properties.hpp>
#include <cassian/runtime/openclc_type_tuples.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cassian/test_harness/test_harness.hpp>
#include <cassian/utility/metaprogramming.hpp>
#include <cassian/utility/utility.hpp>
#include <catch2/catch.hpp>
#include <cstdlib>
#include <numeric>
#include <string>
#include <test_config.hpp>
#include <vector>

namespace ca = cassian;

size_t get_local_work_size(const size_t global_work_size,
                           cassian::Runtime *runtime) {
  const auto max_group_size = static_cast<size_t>(
      runtime->get_device_property(ca::DeviceProperty::max_group_size_x));
  const auto max_total_group_size = static_cast<size_t>(
      runtime->get_device_property(ca::DeviceProperty::max_total_group_size));
  return ca::Runtime::get_max_local_work_size(global_work_size, max_group_size,
                                              max_total_group_size);
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

template <typename T> std::vector<T> generate_input(const size_t elements) {
  std::vector<T> input(elements);
  std::iota(input.begin(), input.end(), T(0));
  return input;
}

template <typename T>
std::vector<T>
run_kernel(const cassian::Kernel &kernel, const size_t global_work_size,
           const size_t local_work_size, const std::vector<T> &input,
           const int elements_per_work_item, cassian::Runtime *runtime) {
  std::vector<ca::Buffer> buffers;

  ca::Buffer input_buffer = runtime->create_buffer(sizeof(T) * input.size());
  runtime->write_buffer_from_vector(input_buffer, input);
  buffers.push_back(input_buffer);

  const ca::LocalMemory local_memory(sizeof(T) * local_work_size *
                                     elements_per_work_item);

  ca::Buffer output_buffer = runtime->create_buffer(
      sizeof(T) * global_work_size * elements_per_work_item);
  buffers.push_back(output_buffer);

  runtime->set_kernel_argument(kernel, 0, input_buffer);
  runtime->set_kernel_argument(kernel, 1, elements_per_work_item);
  runtime->set_kernel_argument(kernel, 2, local_memory);
  runtime->set_kernel_argument(kernel, 3, output_buffer);

  runtime->run_kernel(kernel, global_work_size, local_work_size);

  std::vector<T> output = runtime->read_buffer_to_vector<T>(output_buffer);

  for (auto &buffer : buffers) {
    runtime->release_buffer(buffer);
  }

  return output;
}

template <typename T>
void run_test(const std::string &kernel_path, const std::string &kernel_name,
              const int elements_per_work_item, const TestConfig &config) {
  ca::Runtime *runtime = config.runtime();
  const std::string program_type = config.program_type();

  size_t global_work_size = config.work_size();
  const size_t local_work_size = get_local_work_size(global_work_size, runtime);
  // TODO: Support multiple work groups
  global_work_size *= 2;

  using data_host_type = typename T::host_type;
  const std::vector<data_host_type> input =
      generate_input<data_host_type>(global_work_size * elements_per_work_item);

  const std::string build_options = get_build_options<T>();
  ca::Kernel kernel = create_kernel(kernel_path, kernel_name, build_options,
                                    runtime, program_type);
  const std::vector<data_host_type> output =
      run_kernel(kernel, global_work_size, local_work_size, input,
                 elements_per_work_item, runtime);
  runtime->release_kernel(kernel);

  REQUIRE_THAT(output, Catch::Equals(input));
}

template <typename T>
void test_elements_per_work_item(const std::string &kernel_path,
                                 const std::string &kernel_name,
                                 const TestConfig &config) {
  const std::vector<int> elements_per_work_item = {1, 2};
  for (const auto i : elements_per_work_item) {
    DYNAMIC_SECTION("elements_per_work_item_" << i) {
      run_test<T>(kernel_path, kernel_name, i, config);
    }
  }
}

template <typename T> void test_global_to_local(const TestConfig &config) {
  test_elements_per_work_item<T>(
      "kernels/oclc_async_copy_functions/async_work_group_copy.cl",
      "test_global_to_local", config);
}

template <typename T> void test_local_to_global(const TestConfig &config) {
  test_elements_per_work_item<T>(
      "kernels/oclc_async_copy_functions/async_work_group_copy.cl",
      "test_local_to_global", config);
}

template <typename T>
void test_global_to_local_to_global(const TestConfig &config) {
  test_elements_per_work_item<T>(
      "kernels/oclc_async_copy_functions/async_work_group_copy.cl",
      "test_global_to_local_to_global", config);
}

template <typename T>
void test_async_work_group_copy(const TestConfig &config) {
  SECTION("global_to_local") { test_global_to_local<T>(config); }
  SECTION("local_to_global") { test_local_to_global<T>(config); }
  SECTION("global_local_to_global") {
    test_global_to_local_to_global<T>(config);
  }
}

using GentypeTypes = ca::TupleConcat<ca::ScalarTypes, ca::VectorTypes>::type;

template <typename T> std::string test_name() {
  return std::string(T::type_name);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("async_work_group_copy", "", GentypeTypes,
                                    test_name<TestType>) {
  const TestConfig &config = get_test_config();

  ca::Requirements requirements;
  requirements.arithmetic_type<typename TestType::scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  test_async_work_group_copy<TestType>(config);
}
