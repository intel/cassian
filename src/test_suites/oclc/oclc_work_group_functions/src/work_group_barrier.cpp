/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <algorithm>
#include <cassian/random/random.hpp>
#include <cassian/runtime/runtime.hpp>
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

ca::Kernel create_kernel(const std::string &path,
                         const std::string &kernel_name,
                         const std::string &memory_scope,
                         const int local_work_size, ca::Runtime *runtime,
                         const std::string &program_type) {
  const std::string source = ca::load_text_file(ca::get_asset(path));
  const std::string build_options =
      "-cl-std=CL3.0 -DLOCAL_WORK_SIZE=" + std::to_string(local_work_size) +
      " -DMEMORY_SCOPE=" + memory_scope;
  return runtime->create_kernel(kernel_name, source, build_options,
                                program_type);
}

std::vector<int> run_kernel(const ca::Kernel &kernel,
                            const int global_work_size,
                            const int local_work_size,
                            const std::vector<int> &input,
                            ca::Runtime *runtime) {
  std::vector<ca::Buffer> buffers;

  ca::Buffer input_buffer = runtime->create_buffer(sizeof(int) * input.size());
  runtime->write_buffer_from_vector(input_buffer, input);
  buffers.push_back(input_buffer);

  ca::Buffer output_buffer =
      runtime->create_buffer(sizeof(int) * global_work_size);
  buffers.push_back(output_buffer);

  for (size_t i = 0; i < buffers.size(); ++i) {
    runtime->set_kernel_argument(kernel, static_cast<int>(i), buffers[i]);
  }

  runtime->run_kernel(kernel, global_work_size, local_work_size);

  std::vector<int> output = runtime->read_buffer_to_vector<int>(output_buffer);

  for (auto &buffer : buffers) {
    runtime->release_buffer(buffer);
  }

  return output;
}

std::vector<int> get_reference(const std::vector<int> &input,
                               const int global_work_size,
                               const int local_work_size) {
  std::vector<int> output(input.size());
  const int work_group_count = global_work_size / local_work_size;
  for (int work_group = 0; work_group < work_group_count; ++work_group) {
    int value = 1;
    for (int work_item = 0; work_item < local_work_size; ++work_item) {
      const int id = work_group * local_work_size + work_item;
      output[id] = input[id];
    }
  }
  for (int i = 0; i < 10; ++i) {
    std::vector<int> tmp(output.begin(), output.end());
    for (int work_item = 0; work_item < global_work_size; ++work_item) {
      const int id = work_item;
      if (id + 1 < global_work_size) {
        tmp[id] += output[id + 1];
      } else {
        tmp[id] += output[0];
      }
    }
    output.swap(tmp);
  }
  return output;
}

void run_test(const std::vector<int> &input, const ca::Kernel &kernel,
              const int global_work_size, const int local_work_size,
              ca::Runtime *runtime) {
  const std::vector<int> output =
      run_kernel(kernel, global_work_size, local_work_size, input, runtime);
  const std::vector<int> reference =
      get_reference(input, global_work_size, local_work_size);
  REQUIRE_THAT(output, Catch::Equals(reference));
}

void test_work_group_barrier(const TestConfig &config) {
  ca::Runtime *runtime = config.runtime();
  const std::string program_type = config.program_type();

  size_t global_work_size = config.work_size();

  // TODO: Support multiple work groups
  const auto max_group_size = static_cast<size_t>(
      runtime->get_device_property(ca::DeviceProperty::max_group_size_x));
  const auto max_total_group_size = static_cast<size_t>(
      runtime->get_device_property(ca::DeviceProperty::max_total_group_size));
  const size_t local_work_size = ca::Runtime::get_max_local_work_size(
      global_work_size, max_group_size, max_total_group_size);

  const size_t min_work_group_count = 2;
  if (global_work_size == local_work_size) {
    global_work_size = local_work_size * min_work_group_count;
  }
  const size_t work_group_count = global_work_size / local_work_size;

  const std::string kernel_path =
      "kernels/oclc_work_group_functions/work_group_barrier.cl";

  SECTION("global") {
    std::array<std::string, 2> memory_scopes = {"memory_scope_work_group",
                                                "memory_scope_device"};
    for (auto &memory_scope : memory_scopes) {
      const ca::Kernel kernel =
          create_kernel(kernel_path, "test_kernel_global", memory_scope,
                        local_work_size, runtime, program_type);
      DYNAMIC_SECTION("specific value - " + memory_scope) {
        const std::vector<int> input(global_work_size, 1);
        run_test(input, kernel, global_work_size, local_work_size, runtime);
      }
      runtime->release_kernel(kernel);
    }
  }
  SECTION("local") {
    const ca::Kernel kernel = create_kernel(
        kernel_path, "test_kernel_local", "memory_scope_work_group",
        local_work_size, runtime, program_type);
    SECTION("specific value") {
      const std::vector<int> input(global_work_size, 1);
      run_test(input, kernel, global_work_size, local_work_size, runtime);
    }
    runtime->release_kernel(kernel);
  }
}

TEST_CASE("work_group_barrier", "") {
  test_work_group_barrier(get_test_config());
}

} // namespace
