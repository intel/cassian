/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <algorithm>
#include <cassian/catch2_utils/catch2_utils.hpp>
#include <cassian/runtime/openclc_type_tuples.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cassian/test_harness/test_harness.hpp>
#include <cassian/utility/utility.hpp>
#include <catch2/catch.hpp>
#include <common.hpp>
#include <cstddef>
#include <numeric>
#include <string>
#include <test_config.hpp>
#include <vector>

namespace ca = cassian;

namespace {

std::string get_kernel_path(const AddressSpace address_space) {
  return std::string("kernels/oclc_vector_load_store_functions/vload_") +
         to_string(address_space) + ".cl";
}

template <typename VECTOR_TYPE, typename SCALAR_TYPE>
std::string get_build_options(const AddressSpace address_space) {
  std::string build_options =
      std::string(" -D SCALAR_TYPE=") + SCALAR_TYPE::device_type +
      std::string(" -D VECTOR_TYPE=") + VECTOR_TYPE::device_type;
  if (address_space == AddressSpace::clc_generic) {
    build_options += std::string(" -cl-std=CL3.0");
  }
  build_options += " -D FUNCTION=vload" +
                   std::to_string(VECTOR_TYPE::host_type::vector_size);
  build_options +=
      " -D VECTOR_SIZE=" + std::to_string(VECTOR_TYPE::host_type::vector_size);
  return build_options;
}

ca::Kernel create_kernel(const std::string &path,
                         const std::string &build_options, ca::Runtime *runtime,
                         const std::string &program_type) {
  const std::string source = ca::load_text_file(ca::get_asset(path));
  const std::string kernel_name = "test_kernel";
  return runtime->create_kernel(kernel_name, source, build_options,
                                program_type);
}

template <typename VECTOR_TYPE, typename SCALAR_TYPE>
std::vector<VECTOR_TYPE>
run_kernel(const ca::Kernel &kernel, const size_t global_work_size,
           const size_t local_work_size, const std::vector<SCALAR_TYPE> &input,
           const AddressSpace address_space, ca::Runtime *runtime) {
  std::vector<ca::Buffer> buffers;

  // TODO: Extract size_in_bytes function
  ca::Buffer input_buffer =
      runtime->create_buffer(sizeof(SCALAR_TYPE) * input.size());
  runtime->write_buffer_from_vector(input_buffer, input);
  buffers.push_back(input_buffer);

  ca::Buffer output_buffer =
      runtime->create_buffer(sizeof(VECTOR_TYPE) * global_work_size);
  buffers.push_back(output_buffer);

  for (size_t i = 0; i < buffers.size(); ++i) {
    runtime->set_kernel_argument(kernel, static_cast<int>(i), buffers[i]);
  }
  if (address_space == AddressSpace::clc_local) {
    const ca::LocalMemory local_memory(
        sizeof(SCALAR_TYPE) * VECTOR_TYPE::vector_size * local_work_size);
    runtime->set_kernel_argument(kernel, static_cast<int>(buffers.size()),
                                 local_memory);
  }

  runtime->run_kernel(kernel, global_work_size);

  std::vector<VECTOR_TYPE> output =
      runtime->read_buffer_to_vector<VECTOR_TYPE>(output_buffer);

  for (auto &buffer : buffers) {
    runtime->release_buffer(buffer);
  }

  return output;
}

template <typename VECTOR_TYPE, typename SCALAR_TYPE>
std::vector<VECTOR_TYPE> get_reference(const std::vector<SCALAR_TYPE> &input,
                                       const size_t global_work_size) {
  std::vector<VECTOR_TYPE> output(global_work_size);
  const size_t element_count = VECTOR_TYPE::vector_size;
  for (size_t i = 0; i < global_work_size; ++i) {
    std::vector<SCALAR_TYPE> element(element_count);
    for (size_t j = 0; j < element_count; ++j) {
      const size_t offset = i * element_count + j;
      element[j] = input[offset];
    }
    output[i] = VECTOR_TYPE(element);
  }
  return output;
}

template <typename VECTOR_TYPE, typename SCALAR_TYPE>
void run_test(const std::vector<SCALAR_TYPE> &input, const ca::Kernel &kernel,
              const size_t global_work_size, const size_t local_work_size,
              const AddressSpace address_space, ca::Runtime *runtime) {
  const std::vector<VECTOR_TYPE> output = run_kernel<VECTOR_TYPE>(
      kernel, global_work_size, local_work_size, input, address_space, runtime);
  const std::vector<VECTOR_TYPE> reference =
      get_reference<VECTOR_TYPE>(input, global_work_size);
  REQUIRE_THAT(output, Catch::Equals(reference));
}

template <typename T>
void test_vload(const TestConfig &config, const AddressSpace address_space) {
  ca::Runtime *runtime = config.runtime();
  const std::string program_type = config.program_type();

  using vector_type = T;
  using scalar_type = typename vector_type::scalar_type;
  using vector_host_type = typename vector_type::host_type;
  using scalar_host_type = typename scalar_type::host_type;

  const std::string kernel_path = get_kernel_path(address_space);
  const std::string build_options =
      get_build_options<vector_type, scalar_type>(address_space);
  const ca::Kernel kernel =
      create_kernel(kernel_path, build_options, runtime, program_type);

  const size_t global_work_size = config.work_size();
  // TODO: Use Runtime::get_max_local_work_size
  const size_t local_work_size =
      global_work_size > 256 ? 256 : global_work_size;

  const size_t number_of_scalars =
      global_work_size * vector_host_type::vector_size;
  std::vector<scalar_host_type> input(number_of_scalars);
  std::iota(input.begin(), input.end(), 0);

  run_test<vector_host_type>(input, kernel, global_work_size, local_work_size,
                             address_space, runtime);

  runtime->release_kernel(kernel);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("vload_global", "", ca::VectorTypes,
                                    test_name<TestType>) {
  const TestConfig &config = get_test_config();
  using scalar_type = typename TestType::scalar_type;

  ca::Requirements requirements;
  requirements.arithmetic_type<scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  test_vload<TestType>(get_test_config(), AddressSpace::clc_global);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("vload_local", "", ca::VectorTypes,
                                    test_name<TestType>) {
  const TestConfig &config = get_test_config();
  using scalar_type = typename TestType::scalar_type;

  ca::Requirements requirements;
  requirements.arithmetic_type<scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  test_vload<TestType>(get_test_config(), AddressSpace::clc_local);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("vload_private", "", ca::VectorTypes,
                                    test_name<TestType>) {
  const TestConfig &config = get_test_config();
  using scalar_type = typename TestType::scalar_type;

  ca::Requirements requirements;
  requirements.arithmetic_type<scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  test_vload<TestType>(get_test_config(), AddressSpace::clc_private);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("vload_constant", "", ca::VectorTypes,
                                    test_name<TestType>) {
  const TestConfig &config = get_test_config();
  using scalar_type = typename TestType::scalar_type;

  ca::Requirements requirements;
  requirements.arithmetic_type<scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  test_vload<TestType>(get_test_config(), AddressSpace::clc_constant);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("vload_generic", "", ca::VectorTypes,
                                    test_name<TestType>) {
  const TestConfig &config = get_test_config();
  using scalar_type = typename TestType::scalar_type;

  ca::Requirements requirements;
  requirements.arithmetic_type<scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  test_vload<TestType>(get_test_config(), AddressSpace::clc_generic);
}

} // namespace
