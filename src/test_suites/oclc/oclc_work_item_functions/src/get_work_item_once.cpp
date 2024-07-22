/*
 * Copyright (C) 2024 Intel Corporation
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
template <size_t N>
std::vector<uint32_t> run_kernel(const ca::Kernel &kernel,
                                 const std::array<size_t, N> &global_work_size,
                                 const std::array<size_t, N> &local_work_size,
                                 ca::Runtime *runtime) {

  size_t total_work_items = 1;
  for (const auto &gws : global_work_size) {
    total_work_items *= gws;
  }

  ca::Buffer output_buffer =
      runtime->create_buffer(sizeof(uint32_t) * total_work_items);
  std::vector<uint32_t> zeros(total_work_items, 0);
  runtime->write_buffer_from_vector(output_buffer, zeros);
  runtime->set_kernel_argument(kernel, 0, output_buffer);
  runtime->run_kernel(kernel, global_work_size, local_work_size);
  std::vector<uint32_t> output =
      runtime->read_buffer_to_vector<uint32_t>(output_buffer);
  runtime->release_buffer(output_buffer);
  return output;
}

template <size_t N>
std::vector<uint32_t>
get_reference(const std::array<size_t, N> &global_work_size,
              const std::array<size_t, N> &local_work_size,
              const std::string &kenel_name) {
  size_t local_vector_size = 1;
  size_t global_vector_size = 1;
  for (size_t i = 0; i < N; i++) {
    local_vector_size *= local_work_size.at(i);
    global_vector_size *= global_work_size.at(i);
  }
  std::vector<uint32_t> reference_vector(global_vector_size, 0);
  if (kenel_name == "test_kernel_global") {
    std::fill(reference_vector.begin(), reference_vector.end(), 1);
  } else {
    for (int i = 0; i < local_vector_size; i++) {
      reference_vector[i] = global_vector_size / local_vector_size;
    }
  }
  return reference_vector;
}

template <size_t N>
void run_test(const ca::Kernel &kernel,
              const std::array<size_t, N> &global_work_size,
              const std::array<size_t, N> &local_work_size,
              ca::Runtime *runtime, const std::string &kernel_name) {
  const std::vector<uint32_t> output =
      run_kernel(kernel, global_work_size, local_work_size, runtime);
  const std::vector<uint32_t> reference =
      get_reference(global_work_size, local_work_size, kernel_name);
  REQUIRE_THAT(output, Catch::Equals(reference));
}

template <size_t N>
void test_get_work_item_once(const TestConfig &config,
                             const std::string &kernel_name) {
  ca::Runtime *runtime = config.runtime();
  const std::string program_type = config.program_type();

  const std::string path =
      "kernels/oclc_work_item_functions/get_work_item_once.cl";
  const std::string source = ca::load_text_file(ca::get_asset(path));
  const std::string build_options =
      get_build_options(config.simd()) + " -cl-std=CL3.0";
  const ca::Kernel kernel =
      runtime->create_kernel(kernel_name, source, build_options, program_type);

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

  run_test(kernel, global_work_size, local_work_size, runtime, kernel_name);

  runtime->release_kernel(kernel);
}

TEST_CASE("get_work_item_once", "") {
  const TestConfig &config = get_test_config();
  if (should_skip(config)) {
    return;
  }

  SECTION("1D - global") {
    test_get_work_item_once<1>(config, "test_kernel_global");
  }
  SECTION("2D - global") {
    test_get_work_item_once<2>(config, "test_kernel_global");
  }
  SECTION("3D - global") {
    test_get_work_item_once<3>(config, "test_kernel_global");
  }

  SECTION("1D - local") {
    test_get_work_item_once<1>(config, "test_kernel_local");
  }
  SECTION("2D - local") {
    test_get_work_item_once<2>(config, "test_kernel_local");
  }
  SECTION("3D - local") {
    test_get_work_item_once<3>(config, "test_kernel_local");
  }
}
} // namespace
