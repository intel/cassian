/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/runtime/feature.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cassian/utility/matchers.hpp>
#include <catch2/catch.hpp>
#include <common.hpp>

namespace ca = cassian;

namespace {

float uint32_to_float(uint32_t v) {
  float tmp = 0;
  std::memcpy(&tmp, &v, sizeof(uint32_t));
  return tmp;
}

} // namespace

std::string to_string(RoundingMode rounding_mode) {
  switch (rounding_mode) {
  case RoundingMode::round_to_nearest_even:
    return "round_to_nearest_even";
  case RoundingMode::round_toward_zero:
    return "round_toward_zero";
  case RoundingMode::round_toward_positive_infinity:
    return "round_toward_positive_infinity";
  case RoundingMode::round_toward_negative_infinity:
    return "round_toward_negative_infinity";
  case RoundingMode::round_default:
    return "round_default";
  default:
    throw UnknownRoundingModeException("Unknown rounding mode");
  }
}

std::string function_suffix(RoundingMode rounding_mode) {
  switch (rounding_mode) {
  case RoundingMode::round_to_nearest_even:
    return "_rte";
  case RoundingMode::round_toward_zero:
    return "_rtz";
  case RoundingMode::round_toward_positive_infinity:
    return "_rtp";
  case RoundingMode::round_toward_negative_infinity:
    return "_rtn";
  case RoundingMode::round_default:
    return "";
  default:
    throw UnknownRoundingModeException("Unknown rounding mode");
  }
}

std::string to_string(OverflowHandling overflow_handling) {
  switch (overflow_handling) {
  case OverflowHandling::overflow_saturation:
    return "overflow_saturation";
  case OverflowHandling::overflow_default:
    return "overflow_default";
  default:
    throw UnknownOverflowHandlingException("Unknown overflow handling");
  }
}

std::string function_suffix(OverflowHandling overflow_handling) {
  switch (overflow_handling) {
  case OverflowHandling::overflow_saturation:
    return "_sat";
  case OverflowHandling::overflow_default:
    return "";
  default:
    throw UnknownOverflowHandlingException("Unknown overflow handling");
  }
}

template <>
void compare(const std::vector<float> &output,
             const std::vector<float> &reference) {
  for (int i = 0; i < output.size(); ++i) {
    REQUIRE_THAT(output[i], ca::nan_sensitive_equal(reference[i]));
  }
}

template <>
void compare(const std::vector<ca::Half> &output,
             const std::vector<ca::Half> &reference) {
  for (int i = 0; i < output.size(); ++i) {
    REQUIRE_THAT(output[i], nan_sensitive_equal(reference[i]));
  }
}

template <>
void compare(const std::vector<double> &output,
             const std::vector<double> &reference) {
  for (int i = 0; i < output.size(); ++i) {
    REQUIRE_THAT(output[i], ca::nan_sensitive_equal(reference[i]));
  }
}

const float float_certain_value = -0.15625F;
const float float_two = 2.0F;
const uint32_t float_lowest_denorm = 0x00000001;
const uint32_t float_nan = 0x7f800001;
const uint32_t float_to_be_rounded_down = 0x7f7aefff;
const uint32_t float_to_be_rounded_even = 0x7f7af000;
const uint32_t float_to_be_rounded_up = 0x7f7af001;
const uint32_t float_to_be_rounded_max = 0xffffffff;
