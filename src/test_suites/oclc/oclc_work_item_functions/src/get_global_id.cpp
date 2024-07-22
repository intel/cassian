/*
 * Copyright (C) 2021-2024 Intel Corporation
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

std::string get_kernel_name(const size_t n, const std::string &kernel_name) {
  return kernel_name + '_' + std::to_string(n);
}

template <size_t N>
std::array<std::vector<uint32_t>, N>
run_kernel(const ca::Kernel &kernel,
           const std::array<size_t, N> &global_work_size,
           ca::Runtime *runtime) {
  std::vector<ca::Buffer> buffers;

  for (const auto &gws : global_work_size) {
    ca::Buffer buffer = runtime->create_buffer(sizeof(uint32_t) * gws);
    buffers.push_back(buffer);
  }

  for (size_t i = 0; i < buffers.size(); ++i) {
    runtime->set_kernel_argument(kernel, static_cast<int>(i), buffers[i]);
  }

  runtime->run_kernel(kernel, global_work_size);

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
get_reference(const std::array<size_t, N> &global_work_size) {
  std::array<std::vector<uint32_t>, N> output = {};
  for (size_t i = 0; i < N; ++i) {
    std::vector<uint32_t> global_id(global_work_size.at(i));
    std::iota(global_id.begin(), global_id.end(), 0);
    output.at(i) = global_id;
  }
  return output;
}

template <size_t N>
void run_test(const ca::Kernel &kernel,
              const std::array<size_t, N> &global_work_size,
              ca::Runtime *runtime) {
  const std::array<std::vector<uint32_t>, N> output =
      run_kernel(kernel, global_work_size, runtime);
  const std::array<std::vector<uint32_t>, N> reference =
      get_reference(global_work_size);
  for (size_t i = 0; i < N; ++i) {
    REQUIRE_THAT(output.at(i), Catch::Equals(reference.at(i)));
  }
}

template <size_t N>
void test_get_global_id(const TestConfig &config,
                        const std::string &kernel_name) {
  ca::Runtime *runtime = config.runtime();
  const std::string program_type = config.program_type();
  const std::string path = "kernels/oclc_work_item_functions/get_global_id.cl";
  const std::string source = ca::load_text_file(ca::get_asset(path));
  const std::string build_options = get_build_options(config.simd());
  const ca::Kernel kernel = runtime->create_kernel(
      get_kernel_name(N, kernel_name), source, build_options, program_type);

  const size_t global_work_size_per_dimension = config.work_size();
  std::array<size_t, N> global_work_size = {};
  for (auto &gws : global_work_size) {
    gws = global_work_size_per_dimension;
  }

  run_test(kernel, global_work_size, runtime);

  runtime->release_kernel(kernel);
}

TEST_CASE("get_global_id", "") {
  const TestConfig &config = get_test_config();
  if (should_skip(config)) {
    return;
  }

  SECTION("1D") { test_get_global_id<1>(config, "test_kernel"); }
  SECTION("2D") { test_get_global_id<2>(config, "test_kernel"); }
  SECTION("3D") { test_get_global_id<3>(config, "test_kernel"); }
}

TEST_CASE("get_global_id - wrappers", "") {
  const TestConfig &config = get_test_config();
  if (should_skip(config)) {
    return;
  }

  SECTION("3D") { test_get_global_id<3>(config, "test_kernel_wrappers"); }
}

} // namespace
