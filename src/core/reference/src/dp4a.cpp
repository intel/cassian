/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/reference/dp4a.hpp>
#include <cassian/utility/utility.hpp>
#include <cassian/vector/vector.hpp>

namespace cassian {

template <>
std::vector<int32_t> dp4a(const std::vector<int32_t> &input_a,
                          const std::vector<int32_t> &input_b,
                          const std::vector<int32_t> &input_c) {
  std::vector<Vector<int8_t, 4>> input_a_vector(input_a.size());
  for (size_t i = 0; i < input_a.size(); ++i) {
    input_a_vector[i] = Vector<int8_t, 4>(split_bytes<int8_t>(input_a[i]));
  }

  std::vector<Vector<int8_t, 4>> input_b_vector(input_b.size());
  for (size_t i = 0; i < input_b.size(); ++i) {
    input_b_vector[i] = Vector<int8_t, 4>(split_bytes<int8_t>(input_b[i]));
  }

  return dp4a(input_a_vector, input_b_vector, input_c);
}

template <>
std::vector<int> dp4a(const std::vector<uint32_t> &input_a,
                      const std::vector<int32_t> &input_b,
                      const std::vector<int32_t> &input_c) {
  std::vector<Vector<uint8_t, 4>> input_a_vector(input_a.size());
  for (size_t i = 0; i < input_a.size(); ++i) {
    input_a_vector[i] = Vector<uint8_t, 4>(split_bytes<uint8_t>(input_a[i]));
  }

  std::vector<Vector<int8_t, 4>> input_b_vector(input_b.size());
  for (size_t i = 0; i < input_b.size(); ++i) {
    input_b_vector[i] = Vector<int8_t, 4>(split_bytes<int8_t>(input_b[i]));
  }

  return dp4a(input_a_vector, input_b_vector, input_c);
}

template <>
std::vector<int> dp4a(const std::vector<int32_t> &input_a,
                      const std::vector<uint32_t> &input_b,
                      const std::vector<int32_t> &input_c) {
  std::vector<Vector<int8_t, 4>> input_a_vector(input_a.size());
  for (size_t i = 0; i < input_a.size(); ++i) {
    input_a_vector[i] = Vector<int8_t, 4>(split_bytes<int8_t>(input_a[i]));
  }

  std::vector<Vector<uint8_t, 4>> input_b_vector(input_b.size());
  for (size_t i = 0; i < input_b.size(); ++i) {
    input_b_vector[i] = Vector<uint8_t, 4>(split_bytes<uint8_t>(input_b[i]));
  }

  return dp4a(input_a_vector, input_b_vector, input_c);
}

template <>
std::vector<int> dp4a(const std::vector<uint32_t> &input_a,
                      const std::vector<uint32_t> &input_b,
                      const std::vector<int32_t> &input_c) {
  std::vector<Vector<uint8_t, 4>> input_a_vector(input_a.size());
  for (size_t i = 0; i < input_a.size(); ++i) {
    input_a_vector[i] = Vector<uint8_t, 4>(split_bytes<uint8_t>(input_a[i]));
  }

  std::vector<Vector<uint8_t, 4>> input_b_vector(input_b.size());
  for (size_t i = 0; i < input_b.size(); ++i) {
    input_b_vector[i] = Vector<uint8_t, 4>(split_bytes<uint8_t>(input_b[i]));
  }

  return dp4a(input_a_vector, input_b_vector, input_c);
}

} // namespace cassian
