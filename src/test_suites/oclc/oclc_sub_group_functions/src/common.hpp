/*
 * Copyright (C) 2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_OCLC_SUB_GROUP_FUNCTIONS_COMMON_HPP
#define CASSIAN_OCLC_SUB_GROUP_FUNCTIONS_COMMON_HPP

#include <cassian/catch2_utils/catch2_utils.hpp>
#include <cassian/random/random.hpp>
#include <cassian/runtime/openclc_type_tuples.hpp>
#include <cassian/runtime/openclc_types.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cassian/test_harness/test_harness.hpp>
#include <cassian/utility/metaprogramming.hpp>
#include <cassian/utility/utility.hpp>
#include <string>
#include <test_config.hpp>
#include <vector>

namespace ca = cassian;

ca::Kernel create_kernel(const std::string &path,
                         const std::string &kernel_name,
                         const std::string &build_options, ca::Runtime *runtime,
                         const std::string &program_type);
std::string get_kernel_name(const std::string &name);

struct ImageConfig {
  ca::ImageType type;
  ca::ImageDimensions dim;
  ca::ImageFormat format;
  ca::ImageChannelOrder order;
};

struct ArgData {
  void *data;
  size_t data_size;
  size_t data_count;

  bool is_image;
  // image_descriptor
  ImageConfig image_config;
  // memory flags
  bool is_local_memory;
  ArgData(void *data = nullptr, size_t data_size = 0, size_t data_count = 0,
          bool is_image = false, bool is_local_memory = false)
      : data(data), data_size(data_size), data_count(data_count),
        is_image(is_image), image_config(image_config),
        is_local_memory(is_local_memory) {}
};

template <typename T> struct KernelDescriptor {
  KernelDescriptor()
      : args(0), local_mem_size(0), kernel_name(""), kernel_file_name(""),
        kernel_func_name(""), kernel_build_options(""),
        change_prefix_for_types(false), change_prefix_for_all_types(false),
        delta_size(0) {}
  std::string get_build_options(std::string func_name) {
    if (change_prefix_for_all_types) {
      func_name = "intel_" + func_name;
    } else if (change_prefix_for_types) {
      if (T::device_type == std::string("char") ||
          T::device_type == std::string("uchar") ||
          T::device_type == std::string("short") ||
          T::device_type == std::string("ushort")) {
        func_name = "intel_" + func_name;
      }
    }
    std::string use_all_func;
    using vector_type_check = typename T::host_type;
    if (ca::is_vector_v<vector_type_check>) {
      use_all_func = "all";
    }
    std::string build_options =
        std::string("-cl-std=CL3.0") + std::string(" -D DATA_TYPE=") +
        T::device_type + std::string(" -D FUNC_NAME=") + func_name +
        std::string(" -D USE_ALL=") + use_all_func +
        std::string(" -D DELTA_SIZE=") + std::to_string(delta_size);
    return build_options;
  };
  size_t args;
  ArgData arg1, arg2, arg3;
  size_t local_mem_size;
  uint32_t delta_size;
  std::string kernel_name;
  std::string kernel_file_name;
  std::string kernel_func_name;
  std::string kernel_build_options;
  bool change_prefix_for_types;
  bool change_prefix_for_all_types;
};

int suggest_work_size(const std::string &type);
template <typename T, typename TEST_TYPE, size_t N>
std::vector<std::vector<T>>
run_kernel(const ca::Kernel &kernel,
           const std::array<size_t, N> global_work_size,
           const std::array<size_t, N> local_work_size,
           KernelDescriptor<TEST_TYPE> test_description, ca::Runtime *runtime) {

  std::vector<ca::Buffer> buffers;
  std::vector<std::vector<T>> outputs;

  for (uint8_t args_count = 1; args_count <= test_description.args;
       args_count++) {
    if (args_count == 1) {
      if (!test_description.arg1.is_image) {
        ca::Buffer buffer =
            runtime->create_buffer(test_description.arg1.data_size);
        buffers.push_back(buffer);
        runtime->write_buffer(buffer, test_description.arg1.data);
        runtime->set_kernel_argument(kernel, 0, buffer);
      }
    } else if (args_count == 2) {
      if (!test_description.arg2.is_image) {
        ca::Buffer buffer =
            runtime->create_buffer(test_description.arg2.data_size);
        buffers.push_back(buffer);
        runtime->write_buffer(buffer, test_description.arg2.data);
        runtime->set_kernel_argument(kernel, 1, buffer);
      }
    } else if (args_count == 3) {
      if (!test_description.arg3.is_image) {
        ca::Buffer buffer =
            runtime->create_buffer(test_description.arg3.data_size);
        buffers.push_back(buffer);
        runtime->write_buffer(buffer, test_description.arg3.data);
        runtime->set_kernel_argument(kernel, 2, buffer);
      }
    }
  }

  runtime->run_kernel(kernel, global_work_size, local_work_size);

  for (auto each_buffer : buffers) {
    std::vector<T> output = runtime->read_buffer_to_vector<T>(each_buffer);
    outputs.push_back(output);
    runtime->release_buffer(each_buffer);
  }

  return outputs;
}

template <typename T, typename TEST_TYPE, size_t N>
std::vector<std::vector<T>>
run_test(KernelDescriptor<TEST_TYPE> test_description,
         const std::array<size_t, N> global_work_size,
         const std::array<size_t, N> local_work_size, ca::Runtime *runtime,
         const std::string program_type) {
  std::string build_options =
      test_description.get_build_options(test_description.kernel_func_name);
  const ca::Kernel kernel = create_kernel(test_description.kernel_file_name,
                                          test_description.kernel_name,
                                          build_options, runtime, program_type);

  const std::vector<std::vector<T>> outputs = run_kernel<T, TEST_TYPE>(
      kernel, global_work_size, local_work_size, test_description, runtime);
  runtime->release_kernel(kernel);
  return outputs;
}

template <size_t N>
void calculate_dimensions(std::array<size_t, N> &global_work_size,
                          std::array<size_t, N> &local_work_size,
                          size_t &global_work_size_total,
                          const size_t global_work_size_per_dimension,
                          ca::Runtime *runtime) {

  for (auto &gws : global_work_size) {
    gws = global_work_size_per_dimension;
  }

  // TODO: Support multiple work groups
  std::array<size_t, N> max_group_size = {};
  if constexpr (N >= 1) {
    max_group_size[0] = static_cast<size_t>(
        runtime->get_device_property(ca::DeviceProperty::max_group_size_x));
  }
  if constexpr (N >= 2) {
    max_group_size[1] = static_cast<size_t>(
        runtime->get_device_property(ca::DeviceProperty::max_group_size_y));
  }
  if constexpr (N >= 3) {
    max_group_size[2] = static_cast<size_t>(
        runtime->get_device_property(ca::DeviceProperty::max_group_size_z));
  }
  const auto max_total_group_size = static_cast<size_t>(
      runtime->get_device_property(ca::DeviceProperty::max_total_group_size));
  local_work_size = ca::Runtime::get_max_local_work_size(
      global_work_size, max_group_size, max_total_group_size);

  const size_t min_work_group_count = 2;
  for (size_t i = 0; i < N; ++i) {
    if (global_work_size.at(i) <= local_work_size.at(i)) {
      global_work_size.at(i) = local_work_size.at(i) * min_work_group_count;
    }
  }

  for (size_t i = 0; i < N; ++i) {
    global_work_size_total *= global_work_size.at(i);
  }
}
struct LocalWorkgroupInfo {
  size_t workgroups_count;
  size_t workgroup_size_scalar;
};

template <size_t N>
LocalWorkgroupInfo
calculate_work_groups_num(std::array<size_t, N> &global_work_size,
                          std::array<size_t, N> &local_work_size) {

  LocalWorkgroupInfo workgroup_info;
  size_t gws = 1;
  size_t lws = 1;

  for (size_t i = 0; i < N; ++i) {
    gws = gws * global_work_size[i];
    lws = lws * local_work_size[i];
  }
  workgroup_info.workgroups_count = gws / lws;
  workgroup_info.workgroup_size_scalar = lws;
  return workgroup_info;
}

template <typename TEST_TYPE, size_t N>
void test_subgroup_generic(const TestConfig &config,
                           const std::string func_name,
                           const uint32_t max_delta_size = 1) {
  ca::Runtime *runtime = config.runtime();
  const std::string program_type = config.program_type();

  const size_t global_work_size_per_dimension = config.work_size();
  std::array<size_t, N> global_work_size = {};
  std::array<size_t, N> local_work_size = {};
  size_t global_work_size_total = 1;

  calculate_dimensions(global_work_size, local_work_size,
                       global_work_size_total, global_work_size_per_dimension,
                       runtime);

  for (uint32_t delta_size = 1; delta_size <= max_delta_size; delta_size++) {
    std::vector<uint32_t> input_data_values(global_work_size_total, 1);
    KernelDescriptor<TEST_TYPE> kernel_description;
    kernel_description.kernel_name = get_kernel_name(func_name);
    kernel_description.kernel_file_name =
        "kernels/oclc_sub_group_functions/" + func_name + ".cl";
    kernel_description.kernel_func_name = func_name;
    kernel_description.args = 1;
    kernel_description.arg1.data = input_data_values.data();
    kernel_description.arg1.data_count = input_data_values.size();
    kernel_description.arg1.data_size =
        input_data_values.size() * sizeof(uint32_t);
    kernel_description.change_prefix_for_all_types = true;
    kernel_description.delta_size = delta_size;

    const std::vector<std::vector<uint32_t>> outputs =
        run_test<uint32_t, TEST_TYPE, N>(kernel_description, global_work_size,
                                         local_work_size, runtime,
                                         program_type);
    const std::vector<uint32_t> reference(kernel_description.arg1.data_count,
                                          1);
    REQUIRE_THAT(outputs[0], Catch::Equals(reference));
  }
}

using ScalarTestTypes = std::tuple<ca::clc_long_t, ca::clc_ulong_t,
                                   ca::clc_half_t, ca::clc_double_t>;

using GenericTestTypes =
    ca::TupleConcat<ca::TypesInt, ca::TypesUint, ca::TypesFloat,
                    ScalarTestTypes, ca::TypesChar, ca::TypesUchar,
                    ca::TypesShort, ca::TypesUshort>::type;

template <typename TestType> auto test_name() {
  return std::string(TestType::type_name);
}

#endif
