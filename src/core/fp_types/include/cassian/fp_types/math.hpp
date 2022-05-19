/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_FP_TYPES_MATH_HPP
#define CASSIAN_FP_TYPES_MATH_HPP

#include <cassian/fp_types/type_traits.hpp>
#include <cmath>

namespace cassian {

/**
 * Checks whether the value is NaN. Switches between std isnan function and
 * cassian isnan function.
 *
 * @tparam T the type to check.
 * @param[in] value typed value to check.
 * @returns bool returned whether the value is NaN.
 */
template <typename T> bool isnan(T const &value) {
  if constexpr (is_custom_type_v<T>) {
    return isnan(value);
  } else {
    return std::isnan(value);
  }
}

/**
 * Computes the next representable value of from in the direction of to.
 * Switches between std nextafter function and cassian nextafter function.
 *
 * @tparam T the type to use.
 * @param[in] from base value.
 * @param[in] to value toward which the return value is approximated.
 * @returns the next representable value.
 */
template <typename T> T nextafter(T const &from, T const &to) {
  if constexpr (is_custom_type_v<T>) {
    return nextafter(from, to);
  } else {
    return std::nextafter(from, to);
  }
}

/**
 * Computes the absolute value of a typed value. Switches between std abs
 * function and cassian abs function.
 *
 * @tparam T the type to use.
 * @param[in] value object to use.
 * @returns absolute value.
 */
template <typename T> T abs(T const &value) {
  if constexpr (is_custom_type_v<T>) {
    return abs(value);
  } else {
    return std::abs(value);
  }
}

/**
 * Computes the square root of a typed value. Switches between std sqrt
 * function and cassian sqrt function.
 *
 * @tparam T the type to use.
 * @param[in] value object to use.
 * @returns square root.
 */
template <typename T> T sqrt(T const &value) {
  if constexpr (is_custom_type_v<T>) {
    return sqrt(value);
  } else {
    return std::sqrt(value);
  }
}

} // namespace cassian

#endif
