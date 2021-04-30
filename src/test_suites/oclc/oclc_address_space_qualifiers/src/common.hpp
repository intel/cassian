/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_OCLC_ADDRESS_SPACE_QUALIFIERS_COMMON_HPP
#define CASSIAN_OCLC_ADDRESS_SPACE_QUALIFIERS_COMMON_HPP

#include <cassian/runtime/device_properties.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cstddef>
#include <string>
#include <vector>

namespace ca = cassian;

int suggest_work_size(const std::string &type);

template <typename T>
std::vector<T> run_kernel(ca::Kernel kernel,
                          const std::vector<std::vector<T>> inputs,
                          ca::Runtime *runtime) {
  const size_t work_size = inputs[0].size();

  std::vector<ca::Buffer> buffers;

  for (const auto &input : inputs) {
    ca::Buffer input_buffer = runtime->create_buffer(sizeof(T) * input.size());
    runtime->write_buffer_from_vector(input_buffer, input);
    buffers.push_back(input_buffer);
  }

  int argument_index = 0;
  // Output is the last buffer
  buffers.push_back(runtime->create_buffer(sizeof(T) * work_size));
  for (auto &buffer : buffers) {
    runtime->set_kernel_argument(kernel, static_cast<int>(argument_index),
                                 buffer);
    argument_index++;
  }

  runtime->run_kernel(kernel, {work_size, 1, 1});

  std::vector<T> output = runtime->read_buffer_to_vector<T>(buffers.back());

  for (auto &buffer : buffers) {
    runtime->release_buffer(buffer);
  }

  return output;
}

template <typename T>
void run_kernel_read_write_buffer(ca::Kernel kernel, std::vector<T> &inout,
                                  ca::Runtime *runtime) {
  const size_t work_size = inout.size();

  ca::Buffer inout_buffer = runtime->create_buffer(sizeof(T) * inout.size());
  runtime->write_buffer_from_vector(inout_buffer, inout);
  runtime->set_kernel_argument(kernel, static_cast<int>(0), inout_buffer);

  runtime->run_kernel(kernel, {work_size, 1, 1}, {1, 1, 1});

  inout = runtime->read_buffer_to_vector<T>(inout_buffer);
  runtime->release_buffer(inout_buffer);
}

#endif
