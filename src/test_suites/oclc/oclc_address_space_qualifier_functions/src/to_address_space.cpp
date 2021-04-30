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
#include <cstdint>
#include <string>
#include <test_config.hpp>
#include <vector>

namespace ca = cassian;

namespace {

template <typename T> std::string get_build_options() {
  const std::string clc_data_type = T::device_type;
  std::string build_options =
      " -D DATA_TYPE=" + clc_data_type + " -cl-std=CL3.0";
  return build_options;
}

enum class AddressSpace { clc_global, clc_local, clc_private };

std::string get_kernel_path(const AddressSpace address_space) {
  const std::string base = "kernels/oclc_address_space_qualifier_functions/";
  switch (address_space) {
  case AddressSpace::clc_global:
    return base + "to_global.cl";
  case AddressSpace::clc_local:
    return base + "to_local.cl";
  case AddressSpace::clc_private:
    return base + "to_private.cl";
  default:
    return "";
  }
}

std::string get_non_const_kernel_name() { return "test_non_const"; }

std::string get_const_kernel_name() { return "test_const"; }

ca::Kernel create_kernel(const std::string &path, const std::string &name,
                         const std::string &build_options,
                         cassian::Runtime *runtime,
                         const std::string &program_type) {
  const std::string source = ca::load_text_file(ca::get_asset(path));
  return runtime->create_kernel(name, source, build_options, program_type);
}

template <typename T>
std::vector<uintptr_t> run_kernel_without_input(const cassian::Kernel &kernel,
                                                const size_t global_work_size,
                                                cassian::Runtime *runtime) {
  std::vector<ca::Buffer> buffers;

  ca::Buffer output_buffer =
      runtime->create_buffer(sizeof(uintptr_t) * global_work_size);
  buffers.push_back(output_buffer);

  runtime->set_kernel_argument(kernel, 0, output_buffer);

  runtime->run_kernel(kernel, global_work_size);

  std::vector<uintptr_t> output =
      runtime->read_buffer_to_vector<uintptr_t>(output_buffer);

  for (auto &buffer : buffers) {
    runtime->release_buffer(buffer);
  }

  return output;
}

template <typename T>
std::vector<uintptr_t> run_kernel_with_input(const cassian::Kernel &kernel,
                                             const size_t global_work_size,
                                             cassian::Runtime *runtime) {
  std::vector<ca::Buffer> buffers;

  ca::Buffer input_buffer = runtime->create_buffer(sizeof(T) * 1);
  buffers.push_back(input_buffer);

  ca::Buffer output_buffer =
      runtime->create_buffer(sizeof(uintptr_t) * global_work_size);
  buffers.push_back(output_buffer);

  runtime->set_kernel_argument(kernel, 0, input_buffer);
  runtime->set_kernel_argument(kernel, 1, output_buffer);

  runtime->run_kernel(kernel, global_work_size);

  std::vector<uintptr_t> output =
      runtime->read_buffer_to_vector<uintptr_t>(output_buffer);

  for (auto &buffer : buffers) {
    runtime->release_buffer(buffer);
  }

  return output;
}

template <typename T>
std::vector<uintptr_t>
run_kernel(const AddressSpace address_space, const cassian::Kernel &kernel,
           const size_t global_work_size, cassian::Runtime *runtime) {
  switch (address_space) {
  case AddressSpace::clc_global:
    return run_kernel_with_input<T>(kernel, global_work_size, runtime);
  case AddressSpace::clc_local:
  case AddressSpace::clc_private:
    return run_kernel_without_input<T>(kernel, global_work_size, runtime);
  default:
    return std::vector<uintptr_t>();
  }
}

template <typename T>
void test_address_space(const AddressSpace address_space,
                        const std::string &kernel_name,
                        const TestConfig &config) {
  ca::Runtime *runtime = config.runtime();
  const std::string program_type = config.program_type();

  const std::string kernel_path = get_kernel_path(address_space);
  const std::string build_options = get_build_options<T>();
  ca::Kernel kernel = create_kernel(kernel_path, kernel_name, build_options,
                                    runtime, program_type);

  const size_t global_work_size = config.work_size();
  using data_host_type = typename T::host_type;
  const std::vector<uintptr_t> output = run_kernel<data_host_type>(
      address_space, kernel, global_work_size, runtime);

  runtime->release_kernel(kernel);

  REQUIRE_THAT(output, !Catch::VectorContains(uintptr_t(0)));
}

template <typename T> void test_to_global(const TestConfig &config) {
  const AddressSpace address_space = AddressSpace::clc_global;
  SECTION("non_const") {
    test_address_space<T>(address_space, get_non_const_kernel_name(), config);
  }
  SECTION("const") {
    test_address_space<T>(address_space, get_const_kernel_name(), config);
  }
}

template <typename T> void test_to_local(const TestConfig &config) {
  const AddressSpace address_space = AddressSpace::clc_local;
  SECTION("non_const") {
    test_address_space<T>(address_space, get_non_const_kernel_name(), config);
  }
  SECTION("const") {
    test_address_space<T>(address_space, get_const_kernel_name(), config);
  }
}

template <typename T> void test_to_private(const TestConfig &config) {
  const AddressSpace address_space = AddressSpace::clc_private;
  SECTION("non_const") {
    test_address_space<T>(address_space, get_non_const_kernel_name(), config);
  }
  SECTION("const") {
    test_address_space<T>(address_space, get_const_kernel_name(), config);
  }
}

using GentypeTypes = ca::TupleConcat<ca::ScalarTypes, ca::VectorTypes>::type;

template <typename T> std::string test_name() {
  return std::string(T::type_name);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("to_global", "", GentypeTypes,
                                    test_name<TestType>) {
  const TestConfig &config = get_test_config();

  ca::Requirements requirements;
  requirements.arithmetic_type<TestType>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  test_to_global<TestType>(config);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("to_local", "", GentypeTypes,
                                    test_name<TestType>) {
  const TestConfig &config = get_test_config();

  ca::Requirements requirements;
  requirements.arithmetic_type<TestType>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  test_to_local<TestType>(config);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("to_private", "", GentypeTypes,
                                    test_name<TestType>) {
  const TestConfig &config = get_test_config();

  ca::Requirements requirements;
  requirements.arithmetic_type<TestType>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  test_to_private<TestType>(config);
}

} // namespace
