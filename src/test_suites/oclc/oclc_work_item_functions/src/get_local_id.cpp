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
void test_get_local_id(const TestConfig &config, const std::string &kernel_name,
                       const TestType test_type, bool link) {
  run_test_of_type<N>(config, get_kernel_name(N, kernel_name), test_type, link,
                      "kernels/oclc_work_item_functions/get_local_id",
                      run_test<N>);
}

TEST_CASE("get_local_id", "") {
  const TestConfig &config = get_test_config();
  if (should_skip(config)) {
    return;
  }

  SECTION("1D") {
    test_get_local_id<1>(config, "test_kernel", TestType::basic, false);
  }
  SECTION("2D") {
    test_get_local_id<2>(config, "test_kernel", TestType::basic, false);
  }
  SECTION("3D") {
    test_get_local_id<3>(config, "test_kernel", TestType::basic, false);
  }
}

TEST_CASE("get_local_id - linked wrappers", "") {
  const TestConfig &config = get_test_config();
  ca::Requirements requirements;
  requirements.feature(parse_simd(config.simd()));
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  SECTION("1D") {
    test_get_local_id<1>(config, "test_kernel_wrappers", TestType::basic, true);
  }

  SECTION("2D") {
    test_get_local_id<2>(config, "test_kernel_wrappers", TestType::basic, true);
  }

  SECTION("3D") {
    test_get_local_id<3>(config, "test_kernel_wrappers", TestType::basic, true);
  }
}

TEST_CASE("get_local_id - complete", "") {
  const TestConfig &config = get_test_config();
  if (should_skip(config)) {
    return;
  }

  SECTION("1D") {
    test_get_local_id<1>(config, "test_kernel", TestType::complete, false);
  }
  SECTION("2D") {
    test_get_local_id<2>(config, "test_kernel", TestType::complete, false);
  }
  SECTION("3D") {
    test_get_local_id<3>(config, "test_kernel", TestType::complete, false);
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
    test_get_local_id<3>(config, "test_kernel_wrappers", TestType::basic,
                         false);
  }
}

TEST_CASE("get_local_id - linked wrappers complete", "") {
  const TestConfig &config = get_test_config();
  ca::Requirements requirements;
  requirements.feature(parse_simd(config.simd()));
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  SECTION("1D") {
    test_get_local_id<1>(config, "test_kernel_wrappers", TestType::complete,
                         true);
  }

  SECTION("2D") {
    test_get_local_id<2>(config, "test_kernel_wrappers", TestType::complete,
                         true);
  }

  SECTION("3D") {
    test_get_local_id<3>(config, "test_kernel_wrappers", TestType::complete,
                         true);
  }
}

} // namespace
