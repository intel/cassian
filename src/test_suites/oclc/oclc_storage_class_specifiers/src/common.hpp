/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_OCLC_STORAGE_CLASS_SPECIFIERS_COMMON_HPP
#define CASSIAN_OCLC_STORAGE_CLASS_SPECIFIERS_COMMON_HPP

#include <cassian/runtime/device_properties.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cstddef>
#include <numeric>
#include <string>
#include <vector>

namespace ca = cassian;

int suggest_work_size(const std::string &type);

template <typename T>
std::vector<T> run_kernel(const std::vector<T> &input, ca::Runtime *runtime,
                          ca::Kernel kernel) {
  ca::Buffer buffer_in = runtime->create_buffer(input.size() * sizeof(T));
  runtime->write_buffer(buffer_in, input.data());
  ca::Buffer buffer_out = runtime->create_buffer(input.size() * sizeof(T));

  runtime->set_kernel_argument(kernel, static_cast<int>(0), buffer_in);
  runtime->set_kernel_argument(kernel, static_cast<int>(1), buffer_out);
  runtime->run_kernel(kernel, input.size());

  std::vector<T> output = runtime->read_buffer_to_vector<T>(buffer_out);
  runtime->release_buffer(buffer_in);
  runtime->release_buffer(buffer_out);
  return output;
}

#endif