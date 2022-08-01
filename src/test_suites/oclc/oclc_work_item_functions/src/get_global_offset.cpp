/*
 * Copyright (C) 2022 Intel Corporation
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
                         const std::string &kernel_name, ca::Runtime *runtime,
                         const std::string &program_type) {
  const std::string source = ca::load_text_file(ca::get_asset(path));
  const std::string build_options;
  return runtime->create_kernel(kernel_name, source, build_options,
                                program_type);
}

std::string get_kernel_name(const size_t n) {
  return "test_kernel_" + std::to_string(n);
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
get_reference(const std::array<size_t, N> &global_work_size,
              const std::array<size_t, N> &local_work_size) {
  std::array<size_t, N> work_group_count = {};

  for (size_t i = 0; i < N; ++i) {
    work_group_count.at(i) = global_work_size.at(i) / local_work_size.at(i);
  }

  std::array<std::vector<uint32_t>, N> output = {};
  for (size_t i = 0; i < N; ++i) {
    std::vector<uint32_t> global_offset(global_work_size.at(i));
    std::vector<uint32_t> local_id(global_work_size.at(i));
    std::vector<uint32_t> global_id(global_work_size.at(i));

    for (size_t j = 0; j < work_group_count.at(i); ++j) {
      const auto begin = local_id.begin() + j * local_work_size.at(i);
      const auto end = begin + local_work_size.at(i);
      std::iota(local_id.begin(), local_id.end(), 0);
    }
    std::iota(global_id.begin(), global_id.end(), 0);
    std::transform(global_id.begin(), global_id.end(), local_id.begin(),
                   global_offset.begin(), std::minus<uint32_t>());
    output.at(i) = global_offset;
  }
  return output;
}

template <size_t N>
void run_test(const ca::Kernel &kernel,
              const std::array<size_t, N> &global_work_size,
              const std::array<size_t, N> &local_work_size,
              ca::Runtime *runtime) {
  const std::array<std::vector<uint32_t>, N> output =
      run_kernel(kernel, global_work_size, local_work_size, runtime);
  const std::array<std::vector<uint32_t>, N> reference =
      get_reference(global_work_size, local_work_size);
  for (size_t i = 0; i < N; ++i) {
    REQUIRE_THAT(output.at(i), Catch::Equals(reference.at(i)));
  }
}

template <size_t N> void test_get_global_offset(const TestConfig &config) {
  ca::Runtime *runtime = config.runtime();
  const std::string program_type = config.program_type();

  const std::string kernel_name = get_kernel_name(N);
  const ca::Kernel kernel =
      create_kernel("kernels/oclc_work_item_functions/get_global_offset.cl",
                    kernel_name, runtime, program_type);

  const size_t global_work_size_per_dimension = config.work_size();
  std::array<size_t, N> global_work_size = {};
  for (auto &gws : global_work_size) {
    gws = global_work_size_per_dimension;
  }

  std::array<size_t, N> max_group_size = get_max_group_size<N>(runtime);
  const auto max_total_group_size = static_cast<size_t>(
      runtime->get_device_property(ca::DeviceProperty::max_total_group_size));
  const std::array<size_t, N> local_work_size =
      ca::Runtime::get_max_local_work_size(global_work_size, max_group_size,
                                           max_total_group_size);

  run_test(kernel, global_work_size, local_work_size, runtime);

  runtime->release_kernel(kernel);
}

TEST_CASE("get_global_offset", "") {
  SECTION("1D") { test_get_global_offset<1>(get_test_config()); }
  SECTION("2D") { test_get_global_offset<2>(get_test_config()); }
  SECTION("3D") { test_get_global_offset<3>(get_test_config()); }
}

} // namespace
