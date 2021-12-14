/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_RANDOM_RANDOM_HPP
#define CASSIAN_RANDOM_RANDOM_HPP

#include <algorithm>
#include <cstdint>
#include <limits>
#include <random>
#include <vector>

#include <cassian/fp_types/bfloat.hpp>
#include <cassian/fp_types/half.hpp>
#include <cassian/vector/vector.hpp>

/**
 * Cassian namespace.
 */
namespace cassian {

/**
 * Generate random scalar value in range [min, max].
 *
 * @param[in] min Minimum value.
 * @param[in] max Maximum value.
 * @param[in] seed Random seed.
 * @returns generated value.
 * @tparam T type of generated value.
 */
template <typename T, typename cassian::EnableIfIsScalar<T> = 0>
T generate_value(const T min, const T max, const int seed) {
  static std::default_random_engine engine(seed);
  std::uniform_int_distribution<T> distribution(min, max);
  return distribution(engine);
}

/**
 * Generate random scalar value in range [min, max] \ except.
 *
 * @param[in] min Minimum value.
 * @param[in] max Maximum value.
 * @param[in] seed Random seed.
 * @param[in] except except values vector.
 * @returns generated value.
 * @tparam T type of generated value.
 */
template <typename T, typename cassian::EnableIfIsScalar<T> = 0>
T generate_value(const T min, const T max, const int seed,
                 const std::vector<T> &except) {
  auto value = generate_value(min, max, seed);
  while (!except.empty() &&
         std::find(except.begin(), except.end(), value) != except.end()) {
    value = generate_value(min, max, seed);
  }
  return value;
}

/**
 * Generate random Vector value in range [min, max]. If specialization
 * is floating point the range is [min, max). Use std::nextafter as max
 * parameter to close interval.
 *
 * @param[in] min Minimum scalar value.
 * @param[in] max Maximum scalar value.
 * @param[in] seed Random seed.
 * @returns generated value.
 * @tparam T type of generated value.
 */
template <typename T, typename cassian::EnableIfIsVector<T> = 0>
T generate_value(const scalar_type_v<T> min, const scalar_type_v<T> max,
                 const int seed) {
  using scalar_type = scalar_type_v<T>;
  std::vector<scalar_type> data(T::vector_size);
  std::generate(data.begin(), data.end(),
                [&] { return generate_value<scalar_type>(min, max, seed); });
  return T(data);
}

/**
 * Generate random Vector value in range [min, max] \ except. If specialization
 * is floating point the range is [min, max) \ except. Use std::nextafter as max
 * parameter to close interval.
 *
 * @param[in] min Minimum scalar value.
 * @param[in] max Maximum scalar value.
 * @param[in] seed Random seed.
 * @param[in] except except values vector.
 * @returns generated value.
 * @tparam T type of generated value.
 */
template <typename T, typename cassian::EnableIfIsVector<T> = 0>
T generate_value(const scalar_type_v<T> min, const scalar_type_v<T> max,
                 const int seed, const std::vector<scalar_type_v<T>> &except) {
  T generated_value;
  for (auto i = 0; i < generated_value.size(); i++) {
    generated_value[i] =
        generate_value<scalar_type_v<T>>(min, max, seed, except);
  }
  return generated_value;
}

/**
 * Specialization for int8_t.
 *
 * @overload
 */
template <>
int8_t generate_value<int8_t>(const int8_t min, const int8_t max,
                              const int seed);

/**
 * Specialization for uint8_t.
 *
 * @overload
 */
template <>
uint8_t generate_value<uint8_t>(const uint8_t min, const uint8_t max,
                                const int seed);

/**
 * Specialization for float.
 *
 * @overload
 */
template <>
float generate_value<float>(const float min, const float max, const int seed);
/**
 * Specialization for float.
 *
 * @overload
 */
template <>
float generate_value<float>(const float min, const float max, const int seed,
                            const std::vector<float> &except);

/**
 * Specialization for double.
 *
 * @overload
 */
template <>
double generate_value<double>(const double min, const double max,
                              const int seed);

/**
 * Specialization for double.
 *
 * @overload
 */
template <>
double generate_value<double>(const double min, const double max,
                              const int seed,
                              const std::vector<double> &except);

/**
 * Specialization for bfloat.
 *
 * @overload
 */
template <>
bfloat generate_value<bfloat>(const bfloat min, const bfloat max,
                              const int seed);

/**
 * Specialization for half.
 *
 * @overload
 */
template <>
half generate_value<half>(const half min, const half max, const int seed);

/**
 * Generate random value.
 *
 * @param[in] seed Random seed.
 * @returns generated value.
 * @tparam T type of generated value.
 */
template <typename T, typename cassian::EnableIfIsScalar<T> = 0>
T generate_value(const int seed) {
  const T min = std::numeric_limits<T>::min();
  const T max = std::numeric_limits<T>::max();
  return generate_value(min, max, seed);
}

/**
 * Generate random Vector value.
 *
 * @overload
 */
template <typename T, typename cassian::EnableIfIsVector<T> = 0>
T generate_value(const int seed) {
  using scalar_type = scalar_type_v<T>;
  const scalar_type min = std::numeric_limits<scalar_type>::min();
  const scalar_type max = std::numeric_limits<scalar_type>::max();
  return generate_value<T>(min, max, seed);
}

/**
 * Generate vector of random scalar values in range [min, max].
 *
 * @param[in] size Number of values to generate.
 * @param[in] min Minimum value.
 * @param[in] max Maximum value.
 * @param[in] seed Random seed.
 * @returns vector of generated values.
 * @tparam T type of generated values.
 */
template <typename T, typename cassian::EnableIfIsScalar<T> = 0>
std::vector<T> generate_vector(const int size, const T min, const T max,
                               const int seed) {
  std::vector<T> data(size);
  std::generate(data.begin(), data.end(),
                [&] { return generate_value<T>(min, max, seed); });
  return data;
}

/**
 * Generate vector of random Vector values in range [min, max].
 *
 * @param[in] size Number of values to generate.
 * @param[in] min Minimum scalar value.
 * @param[in] max Maximum scalar value.
 * @param[in] seed Random seed.
 * @returns vector of generated values.
 * @tparam T type of generated values.
 */
template <typename T, typename cassian::EnableIfIsVector<T> = 0>
std::vector<T> generate_vector(const int size, const scalar_type_v<T> min,
                               const scalar_type_v<T> max, const int seed) {
  std::vector<T> data(size);
  std::generate(data.begin(), data.end(),
                [&] { return generate_value<T>(min, max, seed); });
  return data;
}

/**
 * Generate vector of random values.
 *
 * @param[in] size Number of values to generate.
 * @param[in] seed Random seed.
 * @returns vector of generated values.
 * @tparam T type of generated value.
 */
template <typename T>
std::vector<T> generate_vector(const int size, const int seed) {
  std::vector<T> data(size);
  std::generate(data.begin(), data.end(),
                [&] { return generate_value<T>(seed); });
  return data;
}

} // namespace cassian

#endif
