/*
 * Copyright (C) 2021-2024 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_OCLC_WORK_ITEM_FUNCTIONS_COMMON_HPP
#define CASSIAN_OCLC_WORK_ITEM_FUNCTIONS_COMMON_HPP

#include <cassian/logging/logging.hpp>
#include <cassian/runtime/feature.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cassian/test_harness/test_harness.hpp>
#include <cassian/utility/utility.hpp>
#include <catch2/catch.hpp>
#include <common.hpp>
#include <string>
#include <test_config.hpp>

namespace ca = cassian;

int suggest_work_size(const std::string &type);
std::string get_build_options(const std::string &simd);
ca::Feature parse_simd(const std::string &simd);
bool should_skip(const TestConfig &config);

template <size_t N>
std::array<size_t, N> get_max_group_size(ca::Runtime *runtime) {
  std::array<size_t, N> max_group_size = {};
  static_assert(N <= 3, "It is not supported for more than 3 dimensions");
  if constexpr (N >= 1) {
    max_group_size[0] = static_cast<size_t>(
        runtime->get_device_property(ca::DeviceProperty::max_group_size_x));
  }
  if constexpr (N >= 2) {
    max_group_size[1] = static_cast<size_t>(
        runtime->get_device_property(ca::DeviceProperty::max_group_size_y));
  }
  if constexpr (N == 3) {
    max_group_size[2] = static_cast<size_t>(
        runtime->get_device_property(ca::DeviceProperty::max_group_size_z));
  }
  return max_group_size;
}

enum class WorkGroupType { uniform, non_uniform };

template <size_t N> struct WorkSize {
  std::array<size_t, N> global_work_size, local_work_size;
};

template <size_t N>
WorkSize<N> get_work_size(const TestConfig &config, ca::Runtime *runtime) {
  WorkSize<N> work_size = {};
  const size_t global_work_size_per_dimension = config.work_size();
  for (auto &gws : work_size.global_work_size) {
    gws = global_work_size_per_dimension;
  }

  std::array<size_t, N> max_group_size = get_max_group_size<N>(runtime);
  const auto max_total_group_size = static_cast<size_t>(
      runtime->get_device_property(ca::DeviceProperty::max_total_group_size));
  work_size.local_work_size = ca::Runtime::get_max_local_work_size(
      work_size.global_work_size, max_group_size, max_total_group_size);
  return work_size;
}

template <size_t N> WorkGroupType get_work_group_type(WorkSize<N> work_size) {
  size_t global_size_x = work_size.global_work_size[0];
  size_t global_size_y = 1;
  size_t global_size_z = 1;
  size_t local_size_x = work_size.local_work_size[0];
  size_t local_size_y = 1;
  size_t local_size_z = 1;
  if (N > 1) {
    global_size_y = work_size.global_work_size.at(1);
    local_size_y = work_size.local_work_size.at(1);
  }
  if (N > 2) {
    global_size_z = work_size.global_work_size.at(2);
    local_size_z = work_size.local_work_size.at(2);
  }
  if (global_size_x % local_size_x == 0 && global_size_y % local_size_y == 0 &&
      global_size_z % local_size_z == 0) {
    return WorkGroupType::uniform;
  }
  return WorkGroupType::non_uniform;
}

template <size_t N>
using run_test_t = void(const ca::Kernel &, const WorkSize<N>, ca::Runtime *);

template <size_t N>
void run_complete_test(const std::function<run_test_t<N>> &run_test,
                       const ca::Kernel &kernel, const WorkSize<N> work_size,
                       ca::Runtime *runtime,
                       const WorkGroupType work_group_type) {
  size_t global_size_x = work_size.global_work_size[0];
  size_t global_size_y = 1;
  size_t global_size_z = 1;
  size_t local_size_x = work_size.local_work_size[0];
  size_t local_size_y = 1;
  size_t local_size_z = 1;
  if (N > 1) {
    global_size_y = work_size.global_work_size.at(1);
    local_size_y = work_size.local_work_size.at(1);
  }
  if (N > 2) {
    global_size_z = work_size.global_work_size.at(2);
    local_size_z = work_size.local_work_size.at(2);
  }

  for (size_t gz = 1; gz <= global_size_z; gz++) {
    for (size_t gy = 1; gy <= global_size_y / gz; gy++) {
      for (size_t gx = 1; gx <= (global_size_x / gy) / gz; gx++) {
        for (size_t lz = 1; lz <= gz && lz <= local_size_z; lz++) {
          for (size_t ly = 1; ly <= gy && ly <= local_size_y; ly++) {
            for (size_t lx = 1; lx <= gx && lx <= local_size_x; lx++) {
              WorkSize<N> work_group_size = {};
              work_group_size.global_work_size.at(0) = gx;
              work_group_size.local_work_size.at(0) = lx;
              if (N > 1) {
                work_group_size.global_work_size.at(1) = gy;
                work_group_size.local_work_size.at(1) = ly;
              }
              if (N > 2) {
                work_group_size.global_work_size.at(2) = gz;
                work_group_size.local_work_size.at(2) = lz;
              }
              if (work_group_type == get_work_group_type(work_group_size)) {
                run_test(kernel, work_group_size, runtime);
              }
            }
          }
        }
      }
    }
  }
}

enum class TestType { basic, complete };

template <size_t N>
void run_test_of_type(const TestConfig &config, const std::string &kernel_name,
                      const TestType test_type, bool link,
                      const std::string &source_base_path,
                      const std::function<run_test_t<N>> &run_test,
                      const std::string &extra_build_options = "") {
  ca::Runtime *runtime = config.runtime();
  const std::string program_type = config.program_type();

  const std::string build_options =
      get_build_options(config.simd()) + " " + extra_build_options;
  ca::Kernel kernel{};
  if (link) {
    const std::string path_import = source_base_path + "_import.cl";
    const std::string path_export = source_base_path + "_export.cl";
    const std::string source_import =
        ca::load_text_file(ca::get_asset(path_import));
    const std::string source_export =
        ca::load_text_file(ca::get_asset(path_export));

    std::vector<cassian::ProgramDescriptor> programs;
    programs.reserve(2);
    programs.emplace_back(source_import, build_options, program_type);
    programs.emplace_back(source_export, build_options, program_type);

    const std::string linker_options = "";
    kernel = runtime->create_kernel_from_multiple_programs(
        kernel_name, programs, linker_options);
  } else {
    const std::string path = source_base_path + ".cl";
    const std::string source = ca::load_text_file(ca::get_asset(path));

    kernel = runtime->create_kernel(kernel_name, source, build_options,
                                    program_type);
  }
  const WorkSize<N> work_size = get_work_size<N>(config, runtime);

  SECTION("uniform") {
    if (test_type == TestType::complete) {
      run_complete_test(run_test, kernel, work_size, runtime,
                        WorkGroupType::uniform);
    } else {
      if (get_work_group_type(work_size) == WorkGroupType::uniform) {
        run_test(kernel, work_size, runtime);
      }
    }
  }

  SECTION("non-uniform") {
    ca::Requirements requirements;
    requirements.feature(ca::Feature::non_uniform_work_group);
    if (ca::should_skip_test(requirements, *runtime)) {
      ca::logging::info() << "Non-uniform work groups not supported.\n";
      return;
    }
    if (test_type == TestType::complete) {
      run_complete_test(run_test, kernel, work_size, runtime,
                        WorkGroupType::non_uniform);
    } else {
      if (get_work_group_type(work_size) == WorkGroupType::non_uniform) {
        run_test(kernel, work_size, runtime);
      }
    }
  }

  runtime->release_kernel(kernel);
}

#endif
