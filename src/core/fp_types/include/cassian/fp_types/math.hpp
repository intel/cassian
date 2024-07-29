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
 * Checks whether the value is Inf. Switches between std isinf function and
 * cassian isinf function.
 *
 * @tparam T the type to check.
 * @param[in] value typed value to check.
 * @returns bool returned whether the value is Inf.
 */
template <typename T> bool isinf(T const &value) {
  if constexpr (is_custom_type_v<T>) {
    return isinf(value);
  } else {
    return std::isinf(value);
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
  } else if constexpr (std::is_unsigned_v<T>) {
    return value;
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

template <typename T> T acos(T const &value) {
  if constexpr (is_custom_type_v<T>) {
    static_assert(std::is_same_v<T, cassian::Half>);
    return acos(value);
  } else {
    return std::acos(value);
  }
}

template <typename T> T acosh(T const &value) {
  if constexpr (is_custom_type_v<T>) {
    static_assert(std::is_same_v<T, cassian::Half>);
    return acosh(value);
  } else {
    return std::acosh(value);
  }
}

template <typename T> T sin(T const &value) {
  if constexpr (is_custom_type_v<T>) {
    static_assert(std::is_same_v<T, cassian::Half>);
    return sin(value);
  } else {
    return std::sin(value);
  }
}

template <typename T> T sinh(T const &value) {
  if constexpr (is_custom_type_v<T>) {
    static_assert(std::is_same_v<T, cassian::Half>);
    return sinh(value);
  } else {
    return std::sinh(value);
  }
}

template <typename T> T asin(T const &value) {
  if constexpr (is_custom_type_v<T>) {
    static_assert(std::is_same_v<T, cassian::Half>);
    return asin(value);
  } else {
    return std::asin(value);
  }
}

template <typename T> T asinh(T const &value) {
  if constexpr (is_custom_type_v<T>) {
    static_assert(std::is_same_v<T, cassian::Half>);
    return asinh(value);
  } else {
    return std::asinh(value);
  }
}

template <typename T> T tan(T const &value) {
  if constexpr (is_custom_type_v<T>) {
    static_assert(std::is_same_v<T, cassian::Half>);
    return tan(value);
  } else {
    return std::tan(value);
  }
}

template <typename T> T tanh(T const &value) {
  if constexpr (is_custom_type_v<T>) {
    static_assert(std::is_same_v<T, cassian::Half>);
    return tanh(value);
  } else {
    return std::tanh(value);
  }
}

template <typename T> T atan(T const &value) {
  if constexpr (is_custom_type_v<T>) {
    static_assert(std::is_same_v<T, cassian::Half>);
    return atan(value);
  } else {
    return std::atan(value);
  }
}

template <typename T> T atan2(T const &value_a, T const &value_b) {
  if constexpr (is_custom_type_v<T>) {
    static_assert(std::is_same_v<T, cassian::Half>);
    return atan2(value_a, value_b);
  } else {
    return std::atan2(value_a, value_b);
  }
}

template <typename T> T cbrt(T const &value) {
  if constexpr (is_custom_type_v<T>) {
    static_assert(std::is_same_v<T, cassian::Half>);
    return cbrt(value);
  } else {
    return std::cbrt(value);
  }
}

template <typename T> T copysign(T const &value_a, T const &value_b) {
  if constexpr (is_custom_type_v<T>) {
    static_assert(std::is_same_v<T, cassian::Half>);
    return copysign(value_a, value_b);
  } else {
    return std::copysign(value_a, value_b);
  }
}

template <typename T> T atanh(T const &value) {
  if constexpr (is_custom_type_v<T>) {
    static_assert(std::is_same_v<T, cassian::Half>);
    return atanh(value);
  } else {
    return std::atanh(value);
  }
}

template <typename T> T cos(T const &value) {
  if constexpr (is_custom_type_v<T>) {
    static_assert(std::is_same_v<T, cassian::Half>);
    return cos(value);
  } else {
    return std::cos(value);
  }
}

template <typename T> T cosh(T const &value) {
  if constexpr (is_custom_type_v<T>) {
    static_assert(std::is_same_v<T, cassian::Half>);
    return cosh(value);
  } else {
    return std::cosh(value);
  }
}

template <typename T> T fabs(T const &value) {
  if constexpr (is_custom_type_v<T>) {
    static_assert(std::is_same_v<T, cassian::Half>);
    return fabs(value);
  } else {
    return std::fabs(value);
  }
}

template <typename T> T tgamma(T const &value) {
  if constexpr (is_custom_type_v<T>) {
    static_assert(std::is_same_v<T, cassian::Half>);
    return tgamma(value);
  } else {
    return std::tgamma(value);
  }
}

template <typename T> T fmod(T const &value_a, T const &value_b) {
  if constexpr (is_custom_type_v<T>) {
    static_assert(std::is_same_v<T, cassian::Half>);
    return fmod(value_a, value_b);
  } else {
    return std::fmod(value_a, value_b);
  }
}

template <typename T> T fmax(T const &value_a, T const &value_b) {
  if constexpr (is_custom_type_v<T>) {
    static_assert(std::is_same_v<T, cassian::Half>);
    return fmax(value_a, value_b);
  } else {
    return std::fmax(value_a, value_b);
  }
}

template <typename T> T fmin(T const &value_a, T const &value_b) {
  if constexpr (is_custom_type_v<T>) {
    static_assert(std::is_same_v<T, cassian::Half>);
    return fmin(value_a, value_b);
  } else {
    return std::fmin(value_a, value_b);
  }
}

template <typename T> T pow(T const &value_a, T const &value_b) {
  if constexpr (is_custom_type_v<T>) {
    static_assert(std::is_same_v<T, cassian::Half>);
    return pow(value_a, value_b);
  } else {
    return std::pow(value_a, value_b);
  }
}

template <typename T_1, typename T_2>
T_1 pow(T_1 const &value_a, T_2 const &value_b) {
  if constexpr (is_custom_type_v<T_1>) {
    static_assert(std::is_same_v<T_1, cassian::Half>);
    return pow(value_a, value_b);
  } else {
    return std::pow(value_a, value_b);
  }
}

template <typename T> T remainder(T const &value_a, T const &value_b) {
  if constexpr (is_custom_type_v<T>) {
    static_assert(std::is_same_v<T, cassian::Half>);
    return remainder(value_a, value_b);
  } else {
    return std::remainder(value_a, value_b);
  }
}

template <typename T> T rint(T const &value) {
  if constexpr (is_custom_type_v<T>) {
    static_assert(std::is_same_v<T, cassian::Half>);
    return rint(value);
  } else {
    return std::rint(value);
  }
}

template <typename T> T floor(T const &value) {
  if constexpr (is_custom_type_v<T>) {
    static_assert(std::is_same_v<T, cassian::Half>);
    return floor(value);
  } else {
    return std::floor(value);
  }
}

template <typename T> T ceil(T const &value) {
  if constexpr (is_custom_type_v<T>) {
    static_assert(std::is_same_v<T, cassian::Half>);
    return ceil(value);
  } else {
    return std::ceil(value);
  }
}

template <typename T> T trunc(T const &value) {
  if constexpr (is_custom_type_v<T>) {
    static_assert(std::is_same_v<T, cassian::Half>);
    return trunc(value);
  } else {
    return std::trunc(value);
  }
}

template <typename T> T round(T const &value) {
  if constexpr (is_custom_type_v<T>) {
    static_assert(std::is_same_v<T, cassian::Half>);
    return round(value);
  } else {
    return std::round(value);
  }
}

template <typename OUT, typename IN> OUT ilogb(const IN &value) {
  if constexpr (is_custom_type_v<IN>) {
    static_assert(std::is_same_v<IN, cassian::Half>);
    return OUT(ilogb(value));
  } else {
    return OUT(std::ilogb(value));
  }
}

template <typename T> T erfc(T const &value) {
  if constexpr (is_custom_type_v<T>) {
    static_assert(std::is_same_v<T, cassian::Half>);
    return erfc(value);
  } else {
    return std::erfc(value);
  }
}

template <typename T> T erf(T const &value) {
  if constexpr (is_custom_type_v<T>) {
    static_assert(std::is_same_v<T, cassian::Half>);
    return erf(value);
  } else {
    return std::erf(value);
  }
}

template <typename T> T exp(T const &value) {
  if constexpr (is_custom_type_v<T>) {
    static_assert(std::is_same_v<T, cassian::Half>);
    return exp(value);
  } else {
    return std::exp(value);
  }
}

template <typename T> T exp2(T const &value) {
  if constexpr (is_custom_type_v<T>) {
    static_assert(std::is_same_v<T, cassian::Half>);
    return exp2(value);
  } else {
    return std::exp2(value);
  }
}

template <typename T> T expm1(T const &value) {
  if constexpr (is_custom_type_v<T>) {
    static_assert(std::is_same_v<T, cassian::Half>);
    return expm1(value);
  } else {
    return std::expm1(value);
  }
}

template <typename T> T fdim(T const &value_a, T const &value_b) {
  if constexpr (is_custom_type_v<T>) {
    static_assert(std::is_same_v<T, cassian::Half>);
    return fdim(value_a, value_b);
  } else {
    return std::fdim(value_a, value_b);
  }
}

template <typename T>
T fma(T const &value_a, T const &value_b, T const &value_c) {
  if constexpr (is_custom_type_v<T>) {
    static_assert(std::is_same_v<T, cassian::Half>);
    return fma(value_a, value_b, value_c);
  } else {
    return std::fma(value_a, value_b, value_c);
  }
}

template <typename T, typename T_1 = T>
T ldexp(T const &value_a, T_1 const &value_b) {
  if constexpr (is_custom_type_v<T>) {
    static_assert(std::is_same_v<T, cassian::Half>);
    return ldexp(value_a, value_b);
  } else {
    return std::ldexp(value_a, value_b);
  }
}

template <typename T> T hypot(T const &value_a, T const &value_b) {
  if constexpr (is_custom_type_v<T>) {
    static_assert(std::is_same_v<T, cassian::Half>);
    return hypot(value_a, value_b);
  } else {
    return std::hypot(value_a, value_b);
  }
}

template <typename T> T lgamma(T const &value) {
  if constexpr (is_custom_type_v<T>) {
    static_assert(std::is_same_v<T, cassian::Half>);
    return lgamma(value);
  } else {
    return std::lgamma(value);
  }
}

template <typename T> T log(T const &value) {
  if constexpr (is_custom_type_v<T>) {
    static_assert(std::is_same_v<T, cassian::Half>);
    return log(value);
  } else {
    return std::log(value);
  }
}

template <typename T> T log2(T const &value) {
  if constexpr (is_custom_type_v<T>) {
    static_assert(std::is_same_v<T, cassian::Half>);
    return log2(value);
  } else {
    return std::log2(value);
  }
}

template <typename T> T log10(T const &value) {
  if constexpr (is_custom_type_v<T>) {
    static_assert(std::is_same_v<T, cassian::Half>);
    return log10(value);
  } else {
    return std::log10(value);
  }
}

template <typename T> T log1p(T const &value) {
  if constexpr (is_custom_type_v<T>) {
    static_assert(std::is_same_v<T, cassian::Half>);
    return log1p(value);
  } else {
    return std::log1p(value);
  }
}

template <typename T> T logb(T const &value) {
  if constexpr (is_custom_type_v<T>) {
    static_assert(std::is_same_v<T, cassian::Half>);
    return logb(value);
  } else {
    return std::logb(value);
  }
}

template <typename T> T nearbyint(T const &value) {
  if constexpr (is_custom_type_v<T>) {
    static_assert(std::is_same_v<T, cassian::Half>);
    return nearbyint(value);
  } else {
    return std::nearbyint(value);
  }
}
} // namespace cassian

#endif
