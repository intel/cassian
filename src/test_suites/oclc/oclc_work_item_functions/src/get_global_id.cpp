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
                         const std::string &kernel_name, ca::Runtime *runtime,
                         const std::string &program_type) {
  const std::string source = ca::load_text_file(ca::get_asset(path));
  const std::string build_options;
  return runtime->create_kernel(kernel_name, source, build_options,
                                program_type);
}

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
    std::vector<uint32_t> o(global_work_size.at(i));
    std::iota(o.begin(), o.end(), 0);
    output.at(i) = o;
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

  const ca::Kernel kernel =
      create_kernel("kernels/oclc_work_item_functions/get_global_id.cl",
                    get_kernel_name(N, kernel_name), runtime, program_type);

  const size_t global_work_size_per_dimension = config.work_size();
  std::array<size_t, N> global_work_size = {};
  for (auto &gws : global_work_size) {
    gws = global_work_size_per_dimension;
  }

  run_test(kernel, global_work_size, runtime);

  runtime->release_kernel(kernel);
}

TEST_CASE("get_global_id", "") {
  SECTION("1D") { test_get_global_id<1>(get_test_config(), "test_kernel"); }
  SECTION("2D") { test_get_global_id<2>(get_test_config(), "test_kernel"); }
  SECTION("3D") { test_get_global_id<3>(get_test_config(), "test_kernel"); }
}

TEST_CASE("get_global_id - wrappers", "") {
  SECTION("3D") {
    test_get_global_id<3>(get_test_config(), "test_kernel_wrappers");
  }
}

} // namespace
