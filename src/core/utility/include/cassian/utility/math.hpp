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
  scalar_type_v<T> result = 0;
  for (auto i = 0; i < T::vector_size; i++) {
    result = std::fma(a[i], b[i], result);
  }
  return result;
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
 * Computes cross product of two vectors
 * @param[in] a input vector.
 * @param[in] b input vector.
 * @returns vector.
 */
template <typename T, EnableIfIsVector<T> = 0>
T cross_product(const T &a, const T &b) {
  T result;
  result[0] = (a[1] * b[2]) - (a[2] * b[1]);
  result[1] = (a[2] * b[0]) - (a[0] * b[2]);
  result[2] = (a[0] * b[1]) - (a[1] * b[0]);
  if (result.size() == 4) {
    result[3] = 0;
  }
  return result;
}

template <typename T, EnableIfIsScalar<T> = 0>
T cross_product(const T &a, const T &b) {
  return a * b;
}

/**
 * Computes distance between two vectors
 * @param[in] a input vector.
 * @param[in] b input vector.
 * @returns scalar.
 */
template <typename T, EnableIfIsVector<T> = 0>
scalar_type_v<T> distance(const T &a, const T &b) {
  scalar_type_v<T> result = 0.0;
  for (int i = 0; i < T::vector_size; i++) {
    result += std::pow(a[i] - b[i], 2);
  }
  return std::sqrt(result);
}

template <typename T, EnableIfIsScalar<T> = 0>
T distance(const T &a, const T &b) {
  return std::abs(a - b);
}

/**
 * Computes length of vector
 * @param[in] a input vector.
 * @returns scalar.
 */
template <typename T, EnableIfIsVector<T> = 0>
scalar_type_v<T> length(const T &a) {
  scalar_type_v<T> length = 0.0;
  for (int i = 0; i < T::vector_size; i++) {
    length += std::pow(a[i], 2);
  }
  return std::sqrt(length);
}

template <typename T, EnableIfIsScalar<T> = 0> T length(const T &a) {
  return std::sqrt(std::pow(a, 2));
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