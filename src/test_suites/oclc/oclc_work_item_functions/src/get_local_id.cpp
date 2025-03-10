/*
 * Copyright (C) 2021-2024 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <algorithm>
#include <cassian/logging/logging.hpp>
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

enum class TestType { basic, complete };

enum class WorkGroupType { uniform, non_uniform };

template <size_t N> struct WorkSize {
  std::array<size_t, N> global_work_size, local_work_size;
};

template <size_t N>
WorkSize<N> get_work_size(const TestConfig &config, ca::Runtime *runtime) {
  WorkSize<N> work_size = {};
  const size_t global_work_size_per_dimension = config.work_size();
  for (auto &gws : work_size.global_work_size) {
    gws = global_work_size_per_dimension;
  }

  std::array<size_t, N> max_group_size = get_max_group_size<N>(runtime);
  const auto max_total_group_size = static_cast<size_t>(
      runtime->get_device_property(ca::DeviceProperty::max_total_group_size));
  work_size.local_work_size = ca::Runtime::get_max_local_work_size(
      work_size.global_work_size, max_group_size, max_total_group_size);
  return work_size;
}

template <size_t N> WorkGroupType get_work_group_type(WorkSize<N> work_size) {
  size_t global_size_x = work_size.global_work_size[0];
  size_t global_size_y = 1;
  size_t global_size_z = 1;
  size_t local_size_x = work_size.local_work_size[0];
  size_t local_size_y = 1;
  size_t local_size_z = 1;
  if (N > 1) {
    global_size_y = work_size.global_work_size.at(1);
    local_size_y = work_size.local_work_size.at(1);
  }
  if (N > 2) {
    global_size_z = work_size.global_work_size.at(2);
    local_size_z = work_size.local_work_size.at(2);
  }
  if (global_size_x % local_size_x == 0 && global_size_y % local_size_y == 0 &&
      global_size_z % local_size_z == 0) {
    return WorkGroupType::uniform;
  }
  return WorkGroupType::non_uniform;
}

std::string get_kernel_name(const size_t n, const std::string &kernel_name) {
  return kernel_name + '_' + std::to_string(n);
}

template <size_t N>
std::array<std::vector<uint32_t>, N>
run_kernel(const ca::Kernel &kernel,
           const std::array<size_t, N> &global_work_size,
           const std::array<size_t, N> &local_work_size, ca::Runtime *runtime) {
  std::vector<ca::Buffer> buffers;

  for (const auto &gws : global_work_size) {
    ca::Buffer buffer = runtime->create_buffer(sizeof(uint32_t) * gws);
    buffers.push_back(buffer);
  }

  for (size_t i = 0; i < buffers.size(); ++i) {
    runtime->set_kernel_argument(kernel, static_cast<int>(i), buffers[i]);
  }

  runtime->run_kernel(kernel, global_work_size, local_work_size);

  std::array<std::vector<uint32_t>, N> output = {};
  for (size_t i = 0; i < buffers.size(); ++i) {
    output.at(i) = runtime->read_buffer_to_vector<uint32_t>(buffers[i]);
  }

  for (auto &buffer : buffers) {
    runtime->release_buffer(buffer);
  }

  return output;
}

template <size_t N>
std::array<std::vector<uint32_t>, N>
get_reference(const WorkSize<N> work_size) {
  std::array<std::vector<uint32_t>, N> output = {};
  for (size_t i = 0; i < N; ++i) {
    std::vector<uint32_t> local_id(work_size.global_work_size.at(i));
    for (size_t j = 0; j < local_id.size(); ++j) {
      local_id.at(j) = j % work_size.local_work_size.at(i);
    }
    output.at(i) = local_id;
  }
  return output;
}

template <size_t N>
void run_test(const ca::Kernel &kernel, const WorkSize<N> work_size,
              ca::Runtime *runtime) {
  const std::array<std::vector<uint32_t>, N> output = run_kernel(
      kernel, work_size.global_work_size, work_size.local_work_size, runtime);
  const std::array<std::vector<uint32_t>, N> reference =
      get_reference(work_size);
  for (size_t i = 0; i < N; ++i) {
    REQUIRE_THAT(output.at(i), Catch::Equals(reference.at(i)));
  }
}

template <size_t N>
void run_complete_test(const ca::Kernel &kernel, const WorkSize<N> work_size,
                       ca::Runtime *runtime,
                       const WorkGroupType work_group_type) {
  size_t global_size_x = work_size.global_work_size[0];
  size_t global_size_y = 1;
  size_t global_size_z = 1;
  size_t local_size_x = work_size.local_work_size[0];
  size_t local_size_y = 1;
  size_t local_size_z = 1;
  if (N > 1) {
    global_size_y = work_size.global_work_size.at(1);
    local_size_y = work_size.local_work_size.at(1);
  }
  if (N > 2) {
    global_size_z = work_size.global_work_size.at(2);
    local_size_z = work_size.local_work_size.at(2);
  }

  for (size_t gz = 1; gz <= global_size_z; gz++) {
    for (size_t gy = 1; gy <= global_size_y / gz; gy++) {
      for (size_t gx = 1; gx <= (global_size_x / gy) / gz; gx++) {
        for (size_t lz = 1; lz <= gz && lz <= local_size_z; lz++) {
          for (size_t ly = 1; ly <= gy && ly <= local_size_y; ly++) {
            for (size_t lx = 1; lx <= gx && lx <= local_size_x; lx++) {
              WorkSize<N> work_group_size = {};
              work_group_size.global_work_size.at(0) = gx;
              work_group_size.local_work_size.at(0) = lx;
              if (N > 1) {
                work_group_size.global_work_size.at(1) = gy;
                work_group_size.local_work_size.at(1) = ly;
              }
              if (N > 2) {
                work_group_size.global_work_size.at(2) = gz;
                work_group_size.local_work_size.at(2) = lz;
              }
              if (work_group_type == get_work_group_type(work_group_size)) {
                run_test(kernel, work_group_size, runtime);
              }
            }
          }
        }
      }
    }
  }
}

template <size_t N>
void test_get_local_id(const TestConfig &config, const std::string &kernel_name,
                       const TestType test_type) {
  ca::Runtime *runtime = config.runtime();
  const std::string program_type = config.program_type();

  const std::string path = "kernels/oclc_work_item_functions/get_local_id.cl";
  const std::string source = ca::load_text_file(ca::get_asset(path));
  const std::string build_options = get_build_options(config.simd());
  const ca::Kernel kernel = runtime->create_kernel(
      get_kernel_name(N, kernel_name), source, build_options, program_type);
  const WorkSize<N> work_size = get_work_size<N>(config, runtime);

  SECTION("uniform") {
    if (test_type == TestType::complete) {
      run_complete_test(kernel, work_size, runtime, WorkGroupType::uniform);
    } else {
      if (get_work_group_type(work_size) == WorkGroupType::uniform) {
        run_test(kernel, work_size, runtime);
      }
    }
  }

  SECTION("non-uniform") {
    ca::Requirements requirements;
    requirements.feature(ca::Feature::non_uniform_work_group);
    if (ca::should_skip_test(requirements, *runtime)) {
      ca::logging::info() << "Non-uniform work groups not supported.\n";
      return;
    }
    if (test_type == TestType::complete) {
      run_complete_test(kernel, work_size, runtime, WorkGroupType::non_uniform);
    } else {
      if (get_work_group_type(work_size) == WorkGroupType::non_uniform) {
        run_test(kernel, work_size, runtime);
      }
    }
  }

  runtime->release_kernel(kernel);
}

TEST_CASE("get_local_id", "") {
  const TestConfig &config = get_test_config();
  if (should_skip(config)) {
    return;
  }

  SECTION("1D") {
    test_get_local_id<1>(config, "test_kernel", TestType::basic);
  }
  SECTION("2D") {
    test_get_local_id<2>(config, "test_kernel", TestType::basic);
  }
  SECTION("3D") {
    test_get_local_id<3>(config, "test_kernel", TestType::basic);
  }
}

TEST_CASE("get_local_id - complete", "") {
  const TestConfig &config = get_test_config();
  if (should_skip(config)) {
    return;
  }

  SECTION("1D") {
    test_get_local_id<1>(config, "test_kernel", TestType::complete);
  }
  SECTION("2D") {
    test_get_local_id<2>(config, "test_kernel", TestType::complete);
  }
  SECTION("3D") {
    test_get_local_id<3>(config, "test_kernel", TestType::complete);
  }
}

TEST_CASE("get_local_id - wrappers", "") {
  const TestConfig &config = get_test_config();
  ca::Requirements requirements;
  requirements.feature(parse_simd(config.simd()));
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  SECTION("3D") {
    test_get_local_id<3>(config, "test_kernel_wrappers", TestType::basic);
  }
}

} // namespace
