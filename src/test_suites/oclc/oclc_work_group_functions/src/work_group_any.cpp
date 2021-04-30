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

ca::Kernel create_kernel(const std::string &path, ca::Runtime *runtime,
                         const std::string &program_type) {
  const std::string source = ca::load_text_file(ca::get_asset(path));
  const std::string kernel_name = "test_kernel";
  const std::string build_options = "-cl-std=CL3.0";
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
    int value = 0;
    for (int work_item = 0; work_item < local_work_size; ++work_item) {
      const int id = work_group * local_work_size + work_item;
      if (input[id] != 0) {
        value = 1;
        break;
      }
    }
    for (int work_item = 0; work_item < local_work_size; ++work_item) {
      const int id = work_group * local_work_size + work_item;
      output[id] = value;
    }
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

void test_work_group_any(const TestConfig &config) {
  ca::Runtime *runtime = config.runtime();
  const std::string program_type = config.program_type();

  const ca::Kernel kernel =
      create_kernel("kernels/oclc_work_group_functions/work_group_any.cl",
                    runtime, program_type);

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

  SECTION(
      "all work-items in all work-groups have the same non-zero predicate") {
    const std::vector<int> input(global_work_size, 2);
    run_test(input, kernel, global_work_size, local_work_size, runtime);
  }

  SECTION(
      "all work-items in all work-groups have different non-zero predicate") {
    std::vector<int> input(global_work_size);
    std::iota(input.begin(), input.end(), 1);
    run_test(input, kernel, global_work_size, local_work_size, runtime);
  }

  SECTION("all work-items in all work-groups have zero predicate") {
    const std::vector<int> input(global_work_size, 0);
    run_test(input, kernel, global_work_size, local_work_size, runtime);
  }

  SECTION("all work-items in a work-group have the same non-zero predicate") {
    std::vector<int> input(global_work_size);
    for (size_t work_group = 0; work_group < work_group_count; ++work_group) {
      const int value = static_cast<int>(work_group + 10);
      for (size_t work_item = 0; work_item < local_work_size; ++work_item) {
        const size_t id = work_group * local_work_size + work_item;
        input[id] = value;
      }
    }
    run_test(input, kernel, global_work_size, local_work_size, runtime);
  }

  SECTION("all work-items in a work-group have different non-zero predicate") {
    std::vector<int> input(global_work_size);
    for (size_t work_group = 0; work_group < work_group_count; ++work_group) {
      for (size_t work_item = 0; work_item < local_work_size; ++work_item) {
        const size_t id = work_group * local_work_size + work_item;
        const int value = static_cast<int>(work_item + 10);
        input[id] = value;
      }
    }
    run_test(input, kernel, global_work_size, local_work_size, runtime);
  }

  SECTION("all work-items in a work-group have zero predicate") {
    std::vector<int> input(global_work_size);
    for (size_t work_group = 0; work_group < work_group_count; ++work_group) {
      const int value = 0;
      for (size_t work_item = 0; work_item < local_work_size; ++work_item) {
        const size_t id = work_group * local_work_size + work_item;
        input[id] = value;
      }
    }
    run_test(input, kernel, global_work_size, local_work_size, runtime);
  }

  SECTION("single work-item in a work-group has non-zero predicate, others "
          "have zero predicate") {
    std::vector<int> input(global_work_size);
    for (size_t work_group = 0; work_group < work_group_count; ++work_group) {
      for (size_t work_item = 0; work_item < local_work_size; ++work_item) {
        const size_t id = work_group * local_work_size + work_item;
        input[id] = work_item == 0 ? 2 : 0;
      }
    }
    run_test(input, kernel, global_work_size, local_work_size, runtime);
  }

  SECTION("single work-item in a work-group has zero predicate, others have "
          "non-zero predicate") {
    std::vector<int> input(global_work_size);
    for (size_t work_group = 0; work_group < work_group_count; ++work_group) {
      for (size_t work_item = 0; work_item < local_work_size; ++work_item) {
        const size_t id = work_group * local_work_size + work_item;
        input[id] = work_item == 0 ? 0 : 2;
      }
    }
    run_test(input, kernel, global_work_size, local_work_size, runtime);
  }

  SECTION("random values") {
    const std::vector<int> input =
        ca::generate_vector<int>(global_work_size, 0);
    run_test(input, kernel, global_work_size, local_work_size, runtime);
  }

  runtime->release_kernel(kernel);
}

TEST_CASE("work_group_any", "") { test_work_group_any(get_test_config()); }

} // namespace
