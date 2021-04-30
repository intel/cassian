/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "common.hpp"
#include "test_config.hpp"
#include <cassian/cli/cli.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cassian/utility/utility.hpp>
#include <catch2/catch.hpp>
#include <cstdint>
#include <string>
#include <vector>

namespace ca = cassian;

std::vector<work_size_data>
test_reqd_work_group_size(const std::array<size_t, 3> &global_ws,
                          const std::array<size_t, 3> &local_ws,
                          const std::array<size_t, 3> &reqd_wgs,
                          ca::Runtime *runtime,
                          const std::string &program_type) {
  const std::string source = ca::load_text_file(ca::get_asset(
      "kernels/oclc_function_qualifiers/reqd_work_group_size.cl"));
  std::stringstream build_options;
  build_options << "-DX=" << std::to_string(reqd_wgs[0])
                << " -DY=" << std::to_string(reqd_wgs[1])
                << " -DZ=" << std::to_string(reqd_wgs[2]);
  ca::Kernel kernel = runtime->create_kernel(
      "attr_reqd_work_group_size", source, build_options.str(), program_type);
  auto output_vector =
      run_kernel_reqd<work_size_data>(kernel, global_ws, local_ws, runtime);
  runtime->release_kernel(kernel);
  return output_vector;
}

std::vector<work_size_data>
get_reference_data(const std::array<size_t, 3> &ws_data, const size_t &size) {
  std::vector<work_size_data> ws_data_vector(size);
  for (auto i = 0U; i < size; i++) {
    ws_data_vector[i].work_size_x = ws_data[0];
    ws_data_vector[i].work_size_y = ws_data[1];
    ws_data_vector[i].work_size_z = ws_data[2];
  }
  return ws_data_vector;
}

std::array<size_t, 3> create_global_work_size(const uint32_t &work_size,
                                              const uint32_t &dim) {
  std::array<size_t, 3> array = {1, 1, 1};
  for (int i = 0; i < dim; i++) {
    array.at(i) = work_size;
  }
  return array;
}

void run_section(const TestConfig &config,
                 const std::array<size_t, 3> &work_size_limits,
                 const size_t &max_work_size, const uint32_t &work_size,
                 const uint32_t &dim) {
  const auto global_ws = create_global_work_size(work_size, dim);
  const auto local_ws = ca::Runtime::get_max_local_work_size(
      global_ws, work_size_limits, max_work_size);
  const auto distributed_local_ws =
      get_distributed_work_size(global_ws, work_size_limits, local_ws, dim);
  SECTION("local work size with reqd_work_group_size does match") {
    ca::logging::debug() << "reqd_work_group_size is "
                         << ca::to_string(distributed_local_ws) << '\n';
    const auto ws_data_result = test_reqd_work_group_size(
        global_ws, distributed_local_ws, distributed_local_ws, config.runtime(),
        config.program_type());
    const auto reference =
        get_reference_data(distributed_local_ws, ws_data_result.size());
    REQUIRE_THAT(reference, Catch::Equals(ws_data_result));
  }
  SECTION("local work size and reqd_work_group_size does not match") {
    const auto reqd_wgs = get_different_reqd_size(distributed_local_ws);
    ca::logging::debug() << "reqd_work_group_size is "
                         << ca::to_string(reqd_wgs) << '\n';
    REQUIRE_THROWS(test_reqd_work_group_size(global_ws, distributed_local_ws,
                                             reqd_wgs, config.runtime(),
                                             config.program_type()));
  }
}

TEST_CASE("kernel reqd_work_group_size attribute", "") {
  const TestConfig &config = get_test_config();
  const auto work_size = static_cast<uint32_t>(config.work_size());
  const auto work_size_2d = static_cast<uint32_t>(config.work_size_2d());
  const auto work_size_3d = static_cast<uint32_t>(config.work_size_3d());
  const std::array<uint32_t, 3> work_sizes = {work_size, work_size_2d,
                                              work_size_3d};
  const std::array<size_t, 3> work_size_limits = {
      static_cast<size_t>(config.runtime()->get_device_property(
          ca::DeviceProperty::max_group_size_x)),
      static_cast<size_t>(config.runtime()->get_device_property(
          ca::DeviceProperty::max_group_size_y)),
      static_cast<size_t>(config.runtime()->get_device_property(
          ca::DeviceProperty::max_group_size_z))};
  const auto max_work_size =
      static_cast<size_t>(config.runtime()->get_device_property(
          ca::DeviceProperty::max_total_group_size));
  for (auto i = 0U; i < work_sizes.size(); i++) {
    DYNAMIC_SECTION("with " << i + 1 << " dimension") {
      run_section(config, work_size_limits, max_work_size, work_sizes.at(i),
                  i + 1);
    }
  }
}