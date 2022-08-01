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
  const std::string build_options = "-cl-std=CL3.0";
  return runtime->create_kernel(kernel_name, source, build_options,
                                program_type);
}

template <size_t N>
std::vector<uint32_t> run_kernel(const ca::Kernel &kernel,
                                 const std::array<size_t, N> &global_work_size,
                                 const std::array<size_t, N> &local_work_size,
                                 ca::Runtime *runtime) {
  std::vector<ca::Buffer> buffers;

  size_t total_work_items = 1;
  for (const auto &gws : global_work_size) {
    total_work_items *= gws;
  }

  ca::Buffer output_buffer =
      runtime->create_buffer(sizeof(uint32_t) * total_work_items);
  buffers.push_back(output_buffer);

  for (size_t i = 0; i < buffers.size(); ++i) {
    runtime->set_kernel_argument(kernel, static_cast<int>(i), buffers[i]);
  }

  runtime->run_kernel(kernel, global_work_size, local_work_size);

  std::vector<uint32_t> output =
      runtime->read_buffer_to_vector<uint32_t>(output_buffer);

  for (auto &buffer : buffers) {
    runtime->release_buffer(buffer);
  }

  return output;
}

template <size_t N>
std::vector<uint32_t>
get_reference(const std::array<size_t, N> &global_work_size,
              const std::array<size_t, N> &local_work_size) {
  size_t local_vector_size = 1;
  size_t global_vector_size = 1;
  for (size_t i = 0; i < N; i++) {
    local_vector_size *= local_work_size.at(i);
    global_vector_size *= global_work_size.at(i);
  }
  std::vector<uint32_t> local_linear_id(global_vector_size);
  for (int i = 0; i < global_vector_size / local_vector_size; i++) {
    const auto start = local_linear_id.begin() + (i * local_vector_size);
    const auto end = start + local_vector_size;
    std::iota(start, end, 0);
  }
  return local_linear_id;
}

template <size_t N>
void run_test(const ca::Kernel &kernel,
              const std::array<size_t, N> &global_work_size,
              const std::array<size_t, N> &local_work_size,
              ca::Runtime *runtime) {
  const std::vector<uint32_t> output =
      run_kernel(kernel, global_work_size, local_work_size, runtime);
  const std::vector<uint32_t> reference =
      get_reference(global_work_size, local_work_size);
  REQUIRE_THAT(output, Catch::Equals(reference));
}

template <size_t N>
void test_get_local_linear_id(const TestConfig &config,
                              const std::string &kernel_name) {
  ca::Runtime *runtime = config.runtime();
  const std::string program_type = config.program_type();
  const ca::Kernel kernel =
      create_kernel("kernels/oclc_work_item_functions/get_local_linear_id.cl",
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

TEST_CASE("get_local_linear_id", "") {
  SECTION("1D") {
    test_get_local_linear_id<1>(get_test_config(), "test_kernel");
  }
  SECTION("2D") {
    test_get_local_linear_id<2>(get_test_config(), "test_kernel");
  }
  SECTION("3D") {
    test_get_local_linear_id<3>(get_test_config(), "test_kernel");
  }
}

} // namespace