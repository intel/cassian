/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <algorithm>
#include <array>
#include <cassian/random/random.hpp>
#include <cassian/runtime/openclc_types.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cassian/test_harness/test_harness.hpp>
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

template <typename T> std::string get_build_options() {
  std::string build_options = std::string("-cl-std=CL3.0") +
                              std::string(" -D DATA_TYPE=") + T::device_type;
  return build_options;
}

ca::Kernel create_kernel(const std::string &path,
                         const std::string &kernel_name,
                         const std::string &build_options, ca::Runtime *runtime,
                         const std::string &program_type) {
  const std::string source = ca::load_text_file(ca::get_asset(path));
  return runtime->create_kernel(kernel_name, source, build_options,
                                program_type);
}

std::string get_kernel_name(const size_t n) {
  return "test_kernel_" + std::to_string(n);
}

template <typename T, size_t N>
std::vector<T> run_kernel(const ca::Kernel &kernel,
                          const std::array<size_t, N> global_work_size,
                          const std::array<size_t, N> local_work_size,
                          const std::vector<T> &input_value,
                          const std::array<std::vector<size_t>, N> &input_id,
                          ca::Runtime *runtime) {
  std::vector<ca::Buffer> buffers;

  ca::Buffer input_value_buffer =
      runtime->create_buffer(sizeof(T) * input_value.size());
  runtime->write_buffer_from_vector(input_value_buffer, input_value);
  buffers.push_back(input_value_buffer);

  for (size_t i = 0; i < input_id.size(); ++i) {
    ca::Buffer buffer =
        runtime->create_buffer(sizeof(size_t) * input_id.at(i).size());
    runtime->write_buffer_from_vector(buffer, input_id.at(i));
    buffers.push_back(buffer);
  }

  ca::Buffer output_buffer =
      runtime->create_buffer(sizeof(T) * input_value.size());
  buffers.push_back(output_buffer);

  for (size_t i = 0; i < buffers.size(); ++i) {
    runtime->set_kernel_argument(kernel, static_cast<int>(i), buffers[i]);
  }

  runtime->run_kernel(kernel, global_work_size, local_work_size);

  std::vector<T> output = runtime->read_buffer_to_vector<T>(output_buffer);

  for (auto &buffer : buffers) {
    runtime->release_buffer(buffer);
  }

  return output;
}

template <size_t N>
size_t to_linear_id(const std::array<size_t, N> &id,
                    const std::array<size_t, N> &size) {
  size_t linear_id = 0;
  size_t stride = 1;
  for (size_t i = 0; i < N; ++i) {
    linear_id += id.at(i) * stride;
    stride *= size.at(i);
  }
  return linear_id;
}

template <typename T>
std::vector<T> get_reference(const std::vector<T> &input_value,
                             const std::array<std::vector<size_t>, 3> &input_id,
                             const std::array<size_t, 3> global_work_size,
                             const std::array<size_t, 3> local_work_size) {
  std::vector<T> output(input_value.size());

  std::array<size_t, 3> work_group_count = {};
  for (size_t i = 0; i < work_group_count.size(); ++i) {
    work_group_count.at(i) = global_work_size.at(i) / local_work_size.at(i);
  }

  for (size_t wg_z = 0; wg_z < work_group_count[2]; ++wg_z) {
    for (size_t wg_y = 0; wg_y < work_group_count[1]; ++wg_y) {
      for (size_t wg_x = 0; wg_x < work_group_count[0]; ++wg_x) {
        std::array<size_t, 3> work_group_id = {wg_x, wg_y, wg_z};

        // Get value per work-group
        std::array<size_t, 3> value_id = {};
        for (size_t i = 0; i < 3; ++i) {
          const size_t broadcast_id = input_id.at(i)[work_group_id.at(i)];
          value_id.at(i) =
              work_group_id.at(i) * local_work_size.at(i) + broadcast_id;
        }
        const size_t value_id_linear = to_linear_id(value_id, global_work_size);
        T value = input_value[value_id_linear];

        // Propagate value to all work-items in the work-group
        for (size_t wi_z = 0; wi_z < local_work_size[2]; ++wi_z) {
          for (size_t wi_y = 0; wi_y < local_work_size[1]; ++wi_y) {
            for (size_t wi_x = 0; wi_x < local_work_size[0]; ++wi_x) {
              std::array<size_t, 3> work_item_id = {wi_x, wi_y, wi_z};

              std::array<size_t, 3> output_id = {};
              for (size_t i = 0; i < 3; ++i) {
                output_id.at(i) = work_group_id.at(i) * local_work_size.at(i) +
                                  work_item_id.at(i);
              }
              const size_t output_id_linear =
                  to_linear_id(output_id, global_work_size);
              output[output_id_linear] = value;
            }
          }
        }
      }
    }
  }

  return output;
}

template <typename T>
std::vector<T> get_reference(const std::vector<T> &input_value,
                             const std::array<std::vector<size_t>, 2> &input_id,
                             const std::array<size_t, 2> global_work_size,
                             const std::array<size_t, 2> local_work_size) {
  const std::array<size_t, 3> gws = {global_work_size[0], global_work_size[1],
                                     1};
  const std::array<size_t, 3> lws = {local_work_size[0], local_work_size[1], 1};
  const std::array<std::vector<size_t>, 3> in_id = {
      input_id[0], input_id[1], {0}};
  return get_reference(input_value, in_id, gws, lws);
}

template <typename T>
std::vector<T> get_reference(const std::vector<T> &input_value,
                             const std::array<std::vector<size_t>, 1> &input_id,
                             const std::array<size_t, 1> &global_work_size,
                             const std::array<size_t, 1> &local_work_size) {
  const std::array<size_t, 3> gws = {global_work_size[0], 1, 1};
  const std::array<size_t, 3> lws = {local_work_size[0], 1, 1};
  const std::array<std::vector<size_t>, 3> in_id = {input_id[0], {0}, {0}};
  return get_reference(input_value, in_id, gws, lws);
}

template <typename T, size_t N>
void run_test(const std::vector<T> &input_value,
              const std::array<std::vector<size_t>, N> &input_id,
              const ca::Kernel &kernel,
              const std::array<size_t, N> global_work_size,
              const std::array<size_t, N> local_work_size,
              ca::Runtime *runtime) {
  const std::vector<T> output =
      run_kernel(kernel, global_work_size, local_work_size, input_value,
                 input_id, runtime);
  const std::vector<T> reference =
      get_reference(input_value, input_id, global_work_size, local_work_size);
  REQUIRE_THAT(output, Catch::Equals(reference));
}

template <typename TEST_TYPE, size_t N>
void test_work_group_broadcast(const TestConfig &config) {
  using test_host_type = typename TEST_TYPE::host_type;

  ca::Runtime *runtime = config.runtime();
  const std::string program_type = config.program_type();

  const std::string kernel_name = get_kernel_name(N);
  const std::string build_options = get_build_options<TEST_TYPE>();
  const ca::Kernel kernel =
      create_kernel("kernels/oclc_work_group_functions/work_group_broadcast.cl",
                    kernel_name, build_options, runtime, program_type);

  const size_t global_work_size_per_dimension = config.work_size();
  std::array<size_t, N> global_work_size = {};
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
  const std::array<size_t, N> local_work_size =
      ca::Runtime::get_max_local_work_size(global_work_size, max_group_size,
                                           max_total_group_size);

  const size_t min_work_group_count = 2;
  for (size_t i = 0; i < N; ++i) {
    if (global_work_size.at(i) <= local_work_size.at(i)) {
      global_work_size.at(i) = local_work_size.at(i) * min_work_group_count;
    }
  }

  size_t global_work_size_total = 1;
  for (size_t i = 0; i < N; ++i) {
    global_work_size_total *= global_work_size.at(i);
  }

  std::array<size_t, N> work_group_count = {};
  for (size_t i = 0; i < N; ++i) {
    work_group_count.at(i) = global_work_size.at(i) / local_work_size.at(i);
  }

  SECTION("value from first local work-item in each dimension is broadcasted") {
    std::vector<test_host_type> input_value(global_work_size_total);
    std::iota(input_value.begin(), input_value.end(), test_host_type(0));
    std::array<std::vector<size_t>, N> input_id;
    for (size_t i = 0; i < N; ++i) {
      input_id.at(i) = std::vector<size_t>(work_group_count.at(i), 0);
    }
    run_test(input_value, input_id, kernel, global_work_size, local_work_size,
             runtime);
  }

  SECTION("random ids") {
    std::vector<test_host_type> input_value(global_work_size_total);
    std::iota(input_value.begin(), input_value.end(), test_host_type(0));
    std::array<std::vector<size_t>, N> input_id;
    for (size_t i = 0; i < N; ++i) {
      input_id.at(i) = ca::generate_vector<size_t>(
          work_group_count.at(i), 0, local_work_size.at(i) - 1, 0);
    }
    run_test(input_value, input_id, kernel, global_work_size, local_work_size,
             runtime);
  }

  SECTION("random values") {
    const std::vector<test_host_type> input_value =
        ca::generate_vector<test_host_type>(global_work_size_total, 0);
    std::array<std::vector<size_t>, N> input_id;
    for (size_t i = 0; i < N; ++i) {
      input_id.at(i) = std::vector<size_t>(work_group_count.at(i), 0);
    }
    run_test(input_value, input_id, kernel, global_work_size, local_work_size,
             runtime);
  }

  SECTION("random ids and values") {
    const std::vector<test_host_type> input_value =
        ca::generate_vector<test_host_type>(global_work_size_total, 0);
    std::array<std::vector<size_t>, N> input_id;
    for (size_t i = 0; i < input_id.size(); ++i) {
      input_id.at(i) = ca::generate_vector<size_t>(
          work_group_count.at(i), 0, local_work_size.at(i) - 1, 0);
    }
    run_test(input_value, input_id, kernel, global_work_size, local_work_size,
             runtime);
  }

  runtime->release_kernel(kernel);
}

TEMPLATE_TEST_CASE("work_group_broadcast", "", ca::clc_int_t, ca::clc_uint_t,
                   ca::clc_long_t, ca::clc_ulong_t, ca::clc_float_t,
                   ca::clc_double_t) {
  const TestConfig &config = get_test_config();

  ca::Requirements requirements;
  requirements.arithmetic_type<TestType>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  SECTION("1D") { test_work_group_broadcast<TestType, 1>(get_test_config()); }

  SECTION("2D") { test_work_group_broadcast<TestType, 2>(get_test_config()); }

  SECTION("3D") { test_work_group_broadcast<TestType, 3>(get_test_config()); }
}

} // namespace
