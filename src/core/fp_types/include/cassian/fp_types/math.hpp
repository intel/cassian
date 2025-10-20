/*
 * Copyright (C) 2021-2024 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_FP_TYPES_MATH_HPP
#define CASSIAN_FP_TYPES_MATH_HPP

#include <cassian/fp_types/half.hpp>
#include <cassian/fp_types/type_traits.hpp>
#include <cmath>

namespace cassian {

/**
 * Computes the absolute value of a typed value. Nullifies unsigned types.
 *
 * @tparam T the type to use.
 * @param[in] value object to use.
 * @returns absolute value.
 */
template <typename T> T abs(T const &value) {
  using std::abs;
  if constexpr (std::is_unsigned_v<T>) {
    return value;
  } else {
    return abs(value);
  }
}

template <typename T> T flush_to_zero(T const &value) {
  using std::copysign;
  using std::fpclassify;
  if (fpclassify(value) == FP_SUBNORMAL) {
    return copysign(T(0.0), value);
  }
  return value;
}
} // namespace cassian

#endif
