/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/fp_types/half.hpp>
#include <cassian/random/random.hpp>
#include <cmath>
#include <cstdint>
#include <limits>
#include <random>

namespace cassian {

template <>
int8_t generate_value<int8_t>(const int8_t min, const int8_t max,
                              const int seed) {
  return static_cast<int8_t>(generate_value<int32_t>(min, max, seed));
}

template <>
uint8_t generate_value<uint8_t>(const uint8_t min, const uint8_t max,
                                const int seed) {
  return static_cast<uint8_t>(generate_value<uint32_t>(min, max, seed));
}

template <>
float generate_value<float>(const float min, const float max, const int seed) {
  static std::default_random_engine engine(seed);
  std::uniform_real_distribution<float> distribution(min, max);
  return distribution(engine);
}

template <>
float generate_value<float>(const float min, const float max, const int seed,
                            const std::vector<float> &except) {
  auto value = generate_value<float>(min, max, seed);
  while (!except.empty() &&
         std::find(except.begin(), except.end(), value) != except.end()) {
    value = generate_value<float>(min, max, seed);
  }
  return value;
}

template <>
double generate_value<double>(const double min, const double max,
                              const int seed) {
  static std::default_random_engine engine(seed);
  std::uniform_real_distribution<double> distribution(min, max);
  return distribution(engine);
}

template <>
double generate_value<double>(const double min, const double max,
                              const int seed,
                              const std::vector<double> &except) {
  auto value = generate_value<double>(min, max, seed);
  while (!except.empty() &&
         std::find(except.begin(), except.end(), value) != except.end()) {
    value = generate_value<double>(min, max, seed);
  }
  return value;
}

template <>
bfloat generate_value<bfloat>(const bfloat min, const bfloat max,
                              const int seed) {
  const auto f_min = static_cast<float>(min);
  const auto f_max = static_cast<float>(max);
  return static_cast<bfloat>(generate_value<float>(f_min, f_max, seed));
}

template <>
half generate_value<half>(const half min, const half max, const int seed) {
  const auto f_min = static_cast<float>(min);
  const auto f_max = static_cast<float>(max);
  return static_cast<half>(generate_value<float>(f_min, f_max, seed));
}

} // namespace cassian
