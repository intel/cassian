/*
 * Copyright (C) 2022-2024 Intel Corporation
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
get_reference(const WorkSize<N> &work_size) {
  std::array<std::vector<uint32_t>, N> output = {};
  bool is_non_uniform =
      get_work_group_type(work_size) == WorkGroupType::non_uniform;
  for (size_t i = 0; i < N; ++i) {
    std::vector<uint32_t> local_size(work_size.global_work_size.at(i),
                                     work_size.local_work_size.at(i));
    if (is_non_uniform) {
      int remainder_size =
          work_size.global_work_size.at(i) % work_size.local_work_size.at(i);
      for (int j = 0; j < remainder_size; j++) {
        local_size[local_size.size() - 1 - j] = remainder_size;
      }
    }
    output.at(i) = local_size;
  }
  return output;
}

template <size_t N>
void run_test(const ca::Kernel &kernel, const WorkSize<N> &work_size,
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
void test_get_local_size(const TestConfig &config, TestType test_type,
                         bool link) {
  run_test_of_type<N>(config, get_kernel_name(N), test_type, link,
                      "kernels/oclc_work_item_functions/get_local_size",
                      run_test<N>);
}

TEST_CASE("get_local_size", "") {
  const TestConfig &config = get_test_config();
  if (should_skip(config)) {
    return;
  }

  SECTION("1D") { test_get_local_size<1>(config, TestType::basic, false); }
  SECTION("2D") { test_get_local_size<2>(config, TestType::basic, false); }
  SECTION("3D") { test_get_local_size<3>(config, TestType::basic, false); }
}

TEST_CASE("get_local_size - linked wrappers", "") {
  const TestConfig &config = get_test_config();
  if (should_skip(config)) {
    return;
  }

  SECTION("1D") { test_get_local_size<1>(config, TestType::basic, true); }
  SECTION("2D") { test_get_local_size<2>(config, TestType::basic, true); }
  SECTION("3D") { test_get_local_size<3>(config, TestType::basic, true); }
}

TEST_CASE("get_local_size - complete", "") {
  const TestConfig &config = get_test_config();
  if (should_skip(config)) {
    return;
  }

  SECTION("1D") { test_get_local_size<1>(config, TestType::complete, false); }
  SECTION("2D") { test_get_local_size<2>(config, TestType::complete, false); }
  SECTION("3D") { test_get_local_size<3>(config, TestType::complete, false); }
}

TEST_CASE("get_local_size - linked wrappers complete", "") {
  const TestConfig &config = get_test_config();
  if (should_skip(config)) {
    return;
  }

  SECTION("1D") { test_get_local_size<1>(config, TestType::complete, true); }
  SECTION("2D") { test_get_local_size<2>(config, TestType::complete, true); }
  SECTION("3D") { test_get_local_size<3>(config, TestType::complete, true); }
}

} // namespace
