/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_OCLC_FUNCTION_QUALIFIERS_COMMON_HPP
#define CASSIAN_OCLC_FUNCTION_QUALIFIERS_COMMON_HPP

#include "test_config.hpp"
#include <cassian/logging/logging.hpp>
#include <cassian/runtime/runtime.hpp>
#include <catch2/catch.hpp>
#include <tuple>
#include <utility>
#include <vector>

namespace ca = cassian;

int suggest_work_size(const std::string &type);
int suggest_2d_work_size(const std::string &type);
int suggest_3d_work_size(const std::string &type);

struct work_size_data {
  uint32_t work_size_x;
  uint32_t work_size_y;
  uint32_t work_size_z;
  work_size_data() = default;
  bool operator!=(const work_size_data &rhs) const {
    return work_size_x != rhs.work_size_x || work_size_y != rhs.work_size_y ||
           work_size_z != rhs.work_size_z;
  }
};

std::array<size_t, 3>
get_different_reqd_size(const std::array<size_t, 3> &work_size);

template <typename T>
std::vector<T> run_kernel_reqd(const cassian::Kernel &kernel,
                               const std::array<size_t, 3> &global_ws,
                               const std::array<size_t, 3> &local_ws,
                               cassian::Runtime *runtime) {
  size_t size = 1;
  std::for_each(global_ws.begin(), global_ws.end(),
                [&size](size_t in) { size *= in; });
  ca::Buffer buffer = runtime->create_buffer(size * sizeof(T));
  runtime->set_kernel_argument(kernel, 0, buffer);
  runtime->run_kernel(kernel, global_ws, local_ws);
  std::vector<T> output = runtime->read_buffer_to_vector<T>(buffer);
  runtime->release_buffer(buffer);
  return output;
}

template <typename T>
std::vector<T> run_kernel_multiple(const cassian::Kernel &kernel,
                                   const std::vector<T> &input,
                                   const std::array<size_t, 3> &work_size,
                                   cassian::Runtime *runtime) {
  cassian::Buffer input_buffer =
      runtime->create_buffer(input.size() * sizeof(T));
  cassian::Buffer output_buffer = runtime->create_buffer(
      input.size() * sizeof(T), ca::AccessQualifier::write_only);
  runtime->write_buffer_from_vector(input_buffer, input);
  runtime->set_kernel_argument(kernel, 0, input_buffer);
  runtime->set_kernel_argument(kernel, 1, output_buffer);
  runtime->run_kernel(kernel, work_size);
  std::vector<T> output = runtime->read_buffer_to_vector<T>(output_buffer);
  runtime->release_buffer(output_buffer);
  runtime->release_buffer(input_buffer);
  return output;
}

std::array<size_t, 3>
get_distributed_work_size(const std::array<size_t, 3> &global_ws,
                          const std::array<size_t, 3> &work_size_limits,
                          const std::array<size_t, 3> &local_ws,
                          const int &work_dim);

namespace Catch {
std::string work_size_data_to_string(work_size_data const &value);
template <> struct StringMaker<work_size_data> {
  static std::string convert(work_size_data const &value) {
    return work_size_data_to_string(value);
  }
};
} // namespace Catch
#endif
