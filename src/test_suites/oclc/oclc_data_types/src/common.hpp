/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_OCLC_DATA_TYPES_COMMON_HPP
#define CASSIAN_OCLC_DATA_TYPES_COMMON_HPP

#include <cassian/runtime/runtime.hpp>
#include <utility>
#include <vector>

template <typename T>
void run_kernel(const cassian::Kernel &kernel, std::vector<T> &output,
                int output_count, cassian::Runtime *runtime,
                bool use_local_mem = false) {
  cassian::Buffer output_buffer =
      runtime->create_buffer(sizeof(T) * output_count);
  cassian::LocalMemory local_memory(sizeof(T) * output_count);

  runtime->set_kernel_argument(kernel, 0, output_buffer);
  if (use_local_mem) {
    runtime->set_kernel_argument(kernel, 1, local_memory);
  }

  runtime->run_kernel(kernel, 1);
  output = runtime->read_buffer_to_vector<T>(output_buffer);

  runtime->release_buffer(output_buffer);
}

template <typename TestType> auto test_name() {
  return std::string(TestType::type_name);
}

#endif
