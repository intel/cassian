/*
 * Copyright (C) 2021-2024 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cmath>
#include <cstring>

#include <cassian/random/random.hpp>

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

float generate_random_quiet_nan(const int seed) {
  static std::default_random_engine engine(seed);
  std::uniform_int_distribution<uint32_t> mantissa_dis(0, 0x7FFFFF);
  std::uniform_int_distribution<uint32_t> sign_dis(0, 1);
  const uint32_t random_mantissa =
      mantissa_dis(engine) |
      0x400000; // Set the MSB of the mantissa for quiet NaN
  const uint32_t exponent_bits =
      (sign_dis(engine) != 0) ? 0x7F800000 : 0xFF800000; // Random sign
  const uint32_t nan_bits = exponent_bits | random_mantissa;

  float nan = std::numeric_limits<float>::quiet_NaN();
  std::memcpy(&nan, &nan_bits, sizeof(nan));

  return nan;
}

template <>
float generate_value<float>(const float min, const float max, const int seed) {
  static std::default_random_engine engine(seed);

  if (std::isnan(min) || std::isnan(max)) {
    throw std::runtime_error("NaN value passed to generate_value");
  }

  if (std::isinf(min) || std::isinf(max)) {
    throw std::runtime_error("INF value passed to generate_value");
  }

  if (min > max) {
    throw std::runtime_error("min bigger than max passed to generate_value");
  }

  if (min == max) {
    return min;
  }

  std::uniform_int_distribution<uint32_t> distribution(0, UINT32_MAX);
  uint32_t random_bits = distribution(engine);

  float random_float =
      static_cast<float>(random_bits) / static_cast<float>(UINT32_MAX);
  float result = 0.0F;
  if (min < 0 && max > 0) {
    float positive_part = random_float * max;
    float negative_part = (1.0F - random_float) * min;
    result = positive_part + negative_part;
  } else {
    result = min + random_float * (max - min);
  }

  return result;
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

  if (std::isnan(min) || std::isnan(max)) {
    throw std::runtime_error("NaN value passed to generate_value");
  }

  if (std::isinf(min) || std::isinf(max)) {
    throw std::runtime_error("INF value passed to generate_value");
  }

  if (min > max) {
    throw std::runtime_error("min bigger than max passed to generate_value");
  }

  if (min == max) {
    return min;
  }

  std::uniform_int_distribution<uint64_t> distribution(0, UINT64_MAX);
  uint64_t random_bits = distribution(engine);

  double random_double =
      static_cast<double>(random_bits) / static_cast<double>(UINT64_MAX);
  double result = 0.0;
  if (min < 0 && max > 0) {
    double positive_part = random_double * max;
    double negative_part = (1.0 - random_double) * min;
    result = positive_part + negative_part;
  } else {
    result = min + random_double * (max - min);
  }

  return result;
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
bfloat16 generate_value<bfloat16>(const bfloat16 min, const bfloat16 max,
                                  const int seed) {
  const auto f_min = static_cast<float>(min);
  const auto f_max = static_cast<float>(max);
  return static_cast<bfloat16>(generate_value<float>(f_min, f_max, seed));
}

template <>
half generate_value<half>(const half min, const half max, const int seed) {
  const auto f_min = static_cast<float>(min);
  const auto f_max = static_cast<float>(max);
  return static_cast<half>(generate_value<float>(f_min, f_max, seed));
}

template <>
tfloat generate_value<tfloat>(const tfloat min, const tfloat max,
                              const int seed) {
  const auto f_min = static_cast<float>(min);
  const auto f_max = static_cast<float>(max);
  return static_cast<tfloat>(generate_value<float>(f_min, f_max, seed));
}

} // namespace cassian
