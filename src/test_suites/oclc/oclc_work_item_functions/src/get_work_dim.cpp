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
  const std::string build_options;
  return runtime->create_kernel(kernel_name, source, build_options,
                                program_type);
}

template <size_t N>
std::vector<uint32_t> run_kernel(const ca::Kernel &kernel,
                                 const std::array<size_t, N> &global_work_size,
                                 ca::Runtime *runtime) {
  std::vector<ca::Buffer> buffers;

  size_t total_work_items = 0;
  for (const auto &gws : global_work_size) {
    total_work_items += gws;
  }

  ca::Buffer output_buffer =
      runtime->create_buffer(sizeof(uint32_t) * total_work_items);
  buffers.push_back(output_buffer);

  for (size_t i = 0; i < buffers.size(); ++i) {
    runtime->set_kernel_argument(kernel, static_cast<int>(i), buffers[i]);
  }

  runtime->run_kernel(kernel, global_work_size);

  std::vector<uint32_t> output =
      runtime->read_buffer_to_vector<uint32_t>(output_buffer);

  for (auto &buffer : buffers) {
    runtime->release_buffer(buffer);
  }

  return output;
}

template <size_t N>
std::vector<uint32_t>
get_reference(const std::array<size_t, N> &global_work_size) {
  size_t total_work_items = 0;
  for (const auto &gws : global_work_size) {
    total_work_items += gws;
  }
  return std::vector<uint32_t>(total_work_items, N);
}

template <size_t N>
void run_test(const ca::Kernel &kernel,
              const std::array<size_t, N> &global_work_size,
              ca::Runtime *runtime) {
  const std::vector<uint32_t> output =
      run_kernel(kernel, global_work_size, runtime);
  const std::vector<uint32_t> reference = get_reference(global_work_size);
  REQUIRE_THAT(output, Catch::Equals(reference));
}

template <size_t N> void test_get_work_dim(const TestConfig &config) {
  ca::Runtime *runtime = config.runtime();
  const std::string program_type = config.program_type();

  const ca::Kernel kernel =
      create_kernel("kernels/oclc_work_item_functions/get_work_dim.cl", runtime,
                    program_type);

  const size_t global_work_size_per_dimension = config.work_size();
  std::array<size_t, N> global_work_size = {};
  for (auto &gws : global_work_size) {
    gws = global_work_size_per_dimension;
  }

  run_test(kernel, global_work_size, runtime);

  runtime->release_kernel(kernel);
}

TEST_CASE("get_work_dim", "") {
  SECTION("1D") { test_get_work_dim<1>(get_test_config()); }
  SECTION("2D") { test_get_work_dim<2>(get_test_config()); }
  SECTION("3D") { test_get_work_dim<3>(get_test_config()); }
}

} // namespace
