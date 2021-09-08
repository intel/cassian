/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_REFERENCE_DP4A_HPP
#define CASSIAN_REFERENCE_DP4A_HPP

#include <cstddef>
#include <cstdint>
#include <vector>

#include <cassian/vector/vector.hpp>

/**
 * Cassian namespace.
 */
namespace cassian {

/**
 * Compute dp4a reference from Vector elements.
 *
 * @overload
 */
template <typename A_TYPE, typename B_TYPE>
std::vector<int32_t> dp4a(const std::vector<Vector<A_TYPE, 4>> &input_a,
                          const std::vector<Vector<B_TYPE, 4>> &input_b,
                          const std::vector<int32_t> &input_c) {
  std::vector<int32_t> output(input_c.size(), 0);
  for (size_t i = 0; i < output.size(); ++i) {
    output[i] = input_c[i];
    for (int j = 0; j < 4; ++j) {
      output[i] += input_a[i][j] * input_b[i][j];
    }
  }
  return output;
}

/**
 * Compute dp4a reference from regular elements.
 *
 * This function implements algorithm for computing `dot(a, b) + c`.
 *
 * @tparam A_TYPE the type of input_a.
 * @tparam B_TYPE the type of input_b.
 * @param[in] input_a a operand.
 * @param[in] input_b b operand.
 * @param[in] input_c c operand.
 * @returns `dot(a, b) + c`.
 */
template <typename A_TYPE, typename B_TYPE>
std::vector<int32_t> dp4a(const std::vector<A_TYPE> &input_a,
                          const std::vector<B_TYPE> &input_b,
                          const std::vector<int32_t> &input_c);

} // namespace cassian
#endif
