/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_OCLC_OPERATORS_COMMON_HPP
#define CASSIAN_OCLC_OPERATORS_COMMON_HPP

#include <cassian/runtime/runtime.hpp>
#include <cassian/vector/vector.hpp>
#include <catch2/catch.hpp>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

class UnknownOperatorException : public std::runtime_error {
  using std::runtime_error::runtime_error;
};

int suggest_work_size(const std::string &type);

template <typename OUTPUT_TYPE, typename INPUT_TYPE>
std::vector<OUTPUT_TYPE> run_kernel(const cassian::Kernel &kernel,
                                    const std::vector<INPUT_TYPE> &input,
                                    cassian::Runtime *runtime) {
  const size_t work_size = input.size();

  std::vector<cassian::Buffer> buffers;

  cassian::Buffer input_buffer =
      runtime->create_buffer(sizeof(INPUT_TYPE) * input.size());
  runtime->write_buffer_from_vector(input_buffer, input);
  buffers.push_back(input_buffer);

  // Output is the last buffer
  buffers.push_back(runtime->create_buffer(sizeof(OUTPUT_TYPE) * work_size));

  for (size_t i = 0; i < buffers.size(); ++i) {
    runtime->set_kernel_argument(kernel, static_cast<int>(i), buffers[i]);
  }

  runtime->run_kernel(kernel, work_size);

  std::vector<OUTPUT_TYPE> output =
      runtime->read_buffer_to_vector<OUTPUT_TYPE>(buffers.back());

  for (auto &buffer : buffers) {
    runtime->release_buffer(buffer);
  }

  return output;
}

template <typename OUTPUT_TYPE, typename A_TYPE, typename B_TYPE>
std::vector<OUTPUT_TYPE>
run_kernel(const cassian::Kernel &kernel, const std::vector<A_TYPE> &input_a,
           const std::vector<B_TYPE> &input_b, cassian::Runtime *runtime) {
  const size_t work_size = input_a.size();

  std::vector<cassian::Buffer> buffers;

  cassian::Buffer input_a_buffer =
      runtime->create_buffer(sizeof(A_TYPE) * input_a.size());
  runtime->write_buffer_from_vector(input_a_buffer, input_a);
  buffers.push_back(input_a_buffer);

  cassian::Buffer input_b_buffer =
      runtime->create_buffer(sizeof(B_TYPE) * input_b.size());
  runtime->write_buffer_from_vector(input_b_buffer, input_b);
  buffers.push_back(input_b_buffer);

  // Output is the last buffer
  buffers.push_back(runtime->create_buffer(sizeof(OUTPUT_TYPE) * work_size));

  for (size_t i = 0; i < buffers.size(); ++i) {
    runtime->set_kernel_argument(kernel, static_cast<int>(i), buffers[i]);
  }

  runtime->run_kernel(kernel, work_size);

  std::vector<OUTPUT_TYPE> output =
      runtime->read_buffer_to_vector<OUTPUT_TYPE>(buffers.back());

  for (auto &buffer : buffers) {
    runtime->release_buffer(buffer);
  }

  return output;
}

template <typename T, typename cassian::EnableIfIsFloatingPoint<T> = 0>
void compare(const std::vector<T> &output, const std::vector<T> &reference) {
  REQUIRE_THAT(output, Catch::Approx(reference));
}

template <typename T, typename cassian::EnableIfIsIntegral<T> = 0>
void compare(const std::vector<T> &output, const std::vector<T> &reference) {
  REQUIRE_THAT(output, Catch::Equals(reference));
}

template <typename T, typename cassian::EnableIfIsVector<T> = 0>
void compare(const std::vector<T> &output, const std::vector<T> &reference) {
  // TODO: Provide custom matcher for Vectors
  using scalar_type = cassian::scalar_type_v<T>;
  std::vector<scalar_type> flat_output;
  for (const auto &e : output) {
    for (size_t i = 0; i < e.size(); ++i) {
      flat_output.push_back(e[i]);
    }
  }
  std::vector<scalar_type> flat_reference;
  for (const auto &e : reference) {
    for (size_t i = 0; i < e.size(); ++i) {
      flat_reference.push_back(e[i]);
    }
  }
  compare(flat_output, flat_reference);
}

#endif
