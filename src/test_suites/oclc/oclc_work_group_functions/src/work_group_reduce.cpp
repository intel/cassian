/*
 * Copyright (C) 2025 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */
#include "cassian/runtime/device_properties.hpp"
#include "cassian/runtime/property_checks.hpp"
#include <algorithm>
#include <cassian/random/random.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cassian/test_harness/test_harness.hpp>
#include <cassian/utility/utility.hpp>
#include <catch2/catch.hpp>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <numeric>
#include <string>
#include <test_config.hpp>
#include <type_traits>
#include <vector>

namespace ca = cassian;

namespace {

template <typename T>
using FillFunction = std::function<void(std::vector<T> &)>;

template <typename T>
using ReduceReferenceFunction =
    std::function<std::vector<T>(const std::vector<T> &)>;

template <typename T> struct WorkGroupTestConfig {
  size_t group_size{};
  size_t group_cnt{};
  T fold_identity{};
  unsigned max_ulp_diff{};
  FillFunction<T> fill_buffer;
  ReduceReferenceFunction<T> reduce_reference_function;
  std::string kernel_path;
  std::string kernel_name;
  std::string operation;
  std::string program_type;
};

template <typename TestType>
void test_work_group_reduce(
    ca::Runtime *runtime,
    const WorkGroupTestConfig<typename TestType::host_type> &config) {
  using HostType = typename TestType::host_type;

  const std::string source =
      ca::load_text_file(ca::get_asset(config.kernel_path));
  ca::Kernel kernel = runtime->create_kernel(
      config.kernel_name, source,
      std::string("-cl-std=CL3.0 -D DATA_TYPE=") + TestType::device_type +
          std::string(" -D OPERATION=") + config.operation,
      config.program_type);

  const size_t global_work_size = config.group_size * config.group_cnt;
  std::vector<HostType> input(global_work_size, static_cast<HostType>(0));
  config.fill_buffer(input);

  ca::Buffer input_buffer =
      runtime->create_buffer(sizeof(HostType) * global_work_size);
  runtime->write_buffer_from_vector(input_buffer, input);

  ca::Buffer output_buffer =
      runtime->create_buffer(sizeof(HostType) * config.group_cnt);
  runtime->write_buffer_from_vector(
      output_buffer, std::vector<HostType>{config.fold_identity});

  runtime->set_kernel_argument(kernel, 0, input_buffer);
  runtime->set_kernel_argument(kernel, 1, output_buffer);

  runtime->run_kernel(kernel, std::array<size_t, 2>{global_work_size, 1},
                      std::array<size_t, 2>{config.group_size, 1});
  std::vector<HostType> output =
      runtime->read_buffer_to_vector<HostType>(output_buffer);

  std::vector<HostType> reference_output =
      config.reduce_reference_function(input);
  if constexpr (std::is_integral_v<HostType>) {
    REQUIRE_THAT(output, Catch::Matchers::Equals(reference_output));
  } else {
    REQUIRE(std::equal(
        output.begin(), output.end(), reference_output.begin(),
        reference_output.end(),
        [&config](const HostType &output, const HostType &expected) {
          return Catch::WithinULP(output, config.max_ulp_diff).match(expected);
        }));
  }
}

TEMPLATE_TEST_CASE("work_group_reduce", "", ca::clc_int_t, ca::clc_uint_t,
                   ca::clc_long_t, ca::clc_ulong_t, ca::clc_float_t,
                   ca::clc_double_t) {
  using HostType = typename TestType::host_type;
  using CIT = typename std::vector<HostType>::const_iterator;
  using GroupReduceFunction =
      typename std::function<HostType(CIT group_begin, CIT group_end)>;

  const TestConfig &test_config = get_test_config();
  WorkGroupTestConfig<HostType> config;

  auto *runtime = test_config.runtime();
  auto fill_random = [](std::vector<HostType> &buffer) {
    buffer = ca::generate_vector<HostType>(
        buffer.size(), std::numeric_limits<HostType>::min(),
        std::numeric_limits<HostType>::max(), Catch::rngSeed());
  };

  auto fill_iota = [](std::vector<HostType> &buffer) {
    std::iota(buffer.begin(), buffer.end(), 0);
  };

  auto apply_reference_per_group = [&](const std::vector<HostType> &buffer,
                                       GroupReduceFunction group_reduce) {
    std::vector<HostType> reference_output;
    reference_output.reserve(config.group_cnt * config.group_size);
    for (auto group_id = 0; group_id < config.group_cnt; group_id++) {
      auto group_begin = buffer.begin() + group_id * config.group_size;
      auto group_end = group_begin + config.group_size;
      reference_output.push_back(group_reduce(group_begin, group_end));
    }
    return reference_output;
  };

  size_t compute_unit_cnt =
      runtime->get_device_property(cassian::DeviceProperty::max_compute_units);

  config.group_size = runtime->get_device_property(
      cassian::DeviceProperty::max_total_group_size);
  config.kernel_path = "kernels/oclc_work_group_functions/work_group_reduce.cl";
  config.kernel_name = "wg_reduce";
  config.program_type = test_config.program_type();

  SECTION("reduce add") {
    config.operation = "add";
    config.fold_identity = 0;
    config.max_ulp_diff = 0;

    config.fill_buffer = [](std::vector<HostType> &buffer) {
      std::fill(buffer.begin(), buffer.end(), 1);
    };

    config.reduce_reference_function =
        [&](const std::vector<HostType> &buffer) {
          return apply_reference_per_group(
              buffer, [](CIT group_begin, CIT group_end) {
                return std::accumulate(group_begin, group_end,
                                       static_cast<HostType>(0));
              });
        };

    SECTION("reduce add trivial single group") {
      config.group_cnt = 1;
      test_work_group_reduce<TestType>(runtime, config);
    }

    SECTION("reduce add trivial multiple groups") {
      config.group_cnt = compute_unit_cnt;
      test_work_group_reduce<TestType>(runtime, config);
    }

    SECTION("reduce add multiple groups with random input") {
      config.fill_buffer = fill_random;
      config.group_cnt = compute_unit_cnt;
      test_work_group_reduce<TestType>(runtime, config);
    }
  }

  SECTION("reduce min") {
    config.operation = "min";
    config.fold_identity = std::numeric_limits<HostType>::max();
    config.max_ulp_diff = 0;

    config.fill_buffer = fill_iota;

    config.reduce_reference_function =
        [&](const std::vector<HostType> &buffer) {
          return apply_reference_per_group(
              buffer, [](CIT group_begin, CIT group_end) {
                return *std::min_element(group_begin, group_end);
              });
        };

    SECTION("reduce min trivial single group") {
      config.group_cnt = 1;
      test_work_group_reduce<TestType>(runtime, config);
    }

    SECTION("reduce min trivial multiple groups") {
      config.group_cnt = compute_unit_cnt;
      test_work_group_reduce<TestType>(runtime, config);
    }

    SECTION("reduce min multiple groups with random input") {
      config.fill_buffer = fill_random;
      config.group_cnt = compute_unit_cnt;
      test_work_group_reduce<TestType>(runtime, config);
    }
  }

  SECTION("reduce max") {
    config.operation = "max";
    config.fold_identity = std::numeric_limits<HostType>::min();
    config.max_ulp_diff = 0;

    config.fill_buffer = fill_iota;

    config.reduce_reference_function =
        [&](const std::vector<HostType> &buffer) {
          return apply_reference_per_group(
              buffer, [](CIT group_begin, CIT group_end) {
                return *std::max_element(group_begin, group_end);
              });
        };

    SECTION("reduce max trivial single group") {
      config.group_cnt = 1;
      test_work_group_reduce<TestType>(runtime, config);
    }

    SECTION("reduce max trivial multiple groups") {
      config.group_cnt = compute_unit_cnt;
      test_work_group_reduce<TestType>(runtime, config);
    }

    SECTION("reduce max multiple groups with random input") {
      config.fill_buffer = fill_random;
      config.group_cnt = compute_unit_cnt;
      test_work_group_reduce<TestType>(runtime, config);
    }
  }
}

} // namespace
