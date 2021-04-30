/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_UTILITY_MATH_HPP
#define CASSIAN_UTILITY_MATH_HPP

#include <cassian/vector/vector.hpp>
#include <cmath>
#include <limits>
#include <numeric>

namespace cassian {

/**
 * Computes dot product of two vectors
 * @param[in] a input vector.
 * @param[in] b input vector.
 */
template <typename T, EnableIfIsVector<T> = 0>
scalar_type_v<T> dot_product(const T &a, const T &b) {
  return std::inner_product(a.begin(), a.begin() + T::vector_size, b.begin(),
                            scalar_type_v<T>(0ULL));
}

/**
 * Computes dot product of two scalars
 * @param[in] a input scalar.
 * @param[in] b input scalar.
 */
template <typename T, EnableIfIsScalar<T> = 0>
T dot_product(const T &a, const T &b) {
  return a * b;
}

/**
 * Normalizes a vector. Returns a vector in the same direction
 * as `a` but with a length of 1.
 * @param[in] a input vector.
 * @returns vector.
 */
template <typename T, EnableIfIsVector<T> = 0> T normalize(const T &a) {
  using scalar_type = scalar_type_v<T>;
  for (auto i = 0; i < T::vector_size; i++) {
    if (std::isnan(a[i])) {
      return T(std::numeric_limits<scalar_type>::quiet_NaN());
    }
  }
  T result = a;
  if (std::find_if(a.begin(), a.begin() + T::vector_size,
                   [](const scalar_type &val) { return std::isinf(val); }) !=
      a.begin() + T::vector_size) {
    for (size_t i = 0; i < T::vector_size; i++) {
      result[i] = std::isinf(a[i]) ? std::copysign(1.0, a[i]) : 0.0 * a[i];
    }
  }
  long double norm = 0.0;
  for (size_t i = 0; i < T::vector_size; i++) {
    norm += static_cast<long double>(result[i]) *
            static_cast<long double>(result[i]);
  }
  if (norm == 0.0) {
    return result;
  }
  norm = std::sqrt(norm);
  for (size_t i = 0; i < T::vector_size; i++) {
    result[i] /= norm;
  }
  return result;
}

template <typename T, EnableIfIsScalar<T> = 0> T normalize(const T &a) {
  if (std::isnan(a)) {
    return std::numeric_limits<T>::quiet_NaN();
  }
  if (std::isinf(a)) {
    T result = std::isinf(a) ? std::copysign(1.0, a) : 0.0 * a;
    return result;
  }
  if (a == 0) {
    return a;
  }
  return std::copysign(1.0, a);
}

} // namespace cassian
#endif