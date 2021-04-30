/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <algorithm>
#include <cassian/cli/cli.hpp>
#include <cassian/runtime/device_properties.hpp>
#include <cassian/runtime/runtime.hpp>
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

ca::Kernel create_kernel(const std::string &path, const std::string &name,
                         cassian::Runtime *runtime,
                         const std::string &program_type) {
  const std::string source = ca::load_text_file(ca::get_asset(path));
  return runtime->create_kernel(name, source, "", program_type);
}

std::vector<int> generate_input(const size_t elements) {
  std::vector<int> input(elements);
  std::iota(input.begin(), input.end(), 0);
  return input;
}

std::vector<int>
run_kernel(const cassian::Kernel &kernel, const size_t global_work_size,
           const size_t local_work_size, const std::vector<int> &input,
           const int elements_per_work_item, cassian::Runtime *runtime) {
  std::vector<ca::Buffer> buffers;

  ca::Buffer input_buffer = runtime->create_buffer(sizeof(int) * input.size());
  runtime->write_buffer_from_vector(input_buffer, input);
  buffers.push_back(input_buffer);

  const ca::LocalMemory local_memory(sizeof(int) * local_work_size *
                                     elements_per_work_item);

  ca::Buffer output_buffer = runtime->create_buffer(
      sizeof(int) * global_work_size * elements_per_work_item);
  buffers.push_back(output_buffer);

  runtime->set_kernel_argument(kernel, 0, input_buffer);
  runtime->set_kernel_argument(kernel, 1, local_memory);
  runtime->set_kernel_argument(kernel, 2, elements_per_work_item);
  runtime->set_kernel_argument(kernel, 3, output_buffer);

  runtime->run_kernel(kernel, global_work_size, local_work_size);

  std::vector<int> output = runtime->read_buffer_to_vector<int>(output_buffer);

  for (auto &buffer : buffers) {
    runtime->release_buffer(buffer);
  }

  return output;
}

TEST_CASE("local argument", "") {
  const TestConfig &config = get_test_config();

  ca::Runtime *runtime = config.runtime();
  const std::string program_type = config.program_type();

  const size_t global_work_size = config.work_size();
  const size_t local_work_size = get_local_work_size(global_work_size, runtime);

  SECTION("read write with local memory sharing") {
    const size_t elements_per_work_item = 1;
    const std::vector<int> input =
        generate_input(global_work_size * elements_per_work_item);

    ca::Kernel kernel =
        create_kernel("kernels/oclc_address_space_qualifiers/local.cl",
                      "test_kernel", runtime, program_type);
    const std::vector<int> output =
        run_kernel(kernel, global_work_size, local_work_size, input,
                   elements_per_work_item, runtime);
    runtime->release_kernel(kernel);

    REQUIRE_THAT(output, Catch::Equals(input));
  }

  SECTION("local memory max size", "") {
    const int max_local_memory_size =
        runtime->get_device_property(ca::DeviceProperty::max_local_memory_size);
    const size_t elements_per_work_item =
        max_local_memory_size / local_work_size / sizeof(int);
    const std::vector<int> input =
        generate_input(global_work_size * elements_per_work_item);

    ca::Kernel kernel =
        create_kernel("kernels/oclc_address_space_qualifiers/local.cl",
                      "test_kernel", runtime, program_type);
    const std::vector<int> output =
        run_kernel(kernel, global_work_size, local_work_size, input,
                   elements_per_work_item, runtime);
    runtime->release_kernel(kernel);

    REQUIRE_THAT(output, Catch::Equals(input));
  }
}
