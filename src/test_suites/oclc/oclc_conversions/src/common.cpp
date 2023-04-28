/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/runtime/feature.hpp>
#include <cassian/runtime/runtime.hpp>
#include <common.hpp>

namespace {

float uint32_to_float(uint32_t v) {
  float tmp = 0;
  std::memcpy(&tmp, &v, sizeof(uint32_t));
  return tmp;
}

bool nan_sensitive_eq(const float lhs, const float rhs) {
  int32_t lhs_data = 0;
  std::memcpy(&lhs_data, &lhs, sizeof(int32_t));
  int32_t rhs_data = 0;
  std::memcpy(&rhs_data, &rhs, sizeof(int32_t));
  const int32_t exponent_mask = 0x7f800000;
  const int32_t mantissa_mask = 0x007fffff;
  const bool this_nan = (lhs_data & exponent_mask) == exponent_mask &&
                        (lhs_data & mantissa_mask) != 0;
  const bool rhs_nan = (rhs_data & exponent_mask) == exponent_mask &&
                       (rhs_data & mantissa_mask) != 0;
  if (this_nan && rhs_nan) {
    return true;
  }
  return lhs_data == rhs_data;
}

bool nan_sensitive_eq(const double lhs, const double rhs) {
  int64_t lhs_data = 0;
  std::memcpy(&lhs_data, &lhs, sizeof(int64_t));
  int64_t rhs_data = 0;
  std::memcpy(&rhs_data, &rhs, sizeof(int64_t));
  const int64_t exponent_mask = 0x7ff0000000000000;
  const int64_t mantissa_mask = 0xfffffffffffff;
  const bool this_nan = (lhs_data & exponent_mask) == exponent_mask &&
                        (lhs_data & mantissa_mask) != 0;
  const bool rhs_nan = (rhs_data & exponent_mask) == exponent_mask &&
                       (rhs_data & mantissa_mask) != 0;
  if (this_nan && rhs_nan) {
    return true;
  }
  return lhs_data == rhs_data;
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

void set_rounding_mode(RoundingMode rounding_mode) {
  switch (rounding_mode) {
  case RoundingMode::round_default:
  case RoundingMode::round_to_nearest_even: {
    fesetround(FE_TONEAREST);
    return;
  }
  case RoundingMode::round_toward_negative_infinity: {
    fesetround(FE_DOWNWARD);
    return;
  }
  case RoundingMode::round_toward_positive_infinity: {
    fesetround(FE_UPWARD);
    return;
  }
  case RoundingMode::round_toward_zero: {
    fesetround(FE_TOWARDZERO);
    return;
  }
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

template <typename T>
class NanSensitiveEqMatcher : public Catch::MatcherBase<T> {
  T value;

public:
  explicit NanSensitiveEqMatcher(const T v) : value(v) {}

  bool match(const T &v) const override { return v.nan_sensitive_eq(value); }

  std::string describe() const override {
    std::ostringstream ss;
    ss << "is NaN sensitively equal to " << value;
    return ss.str();
  }
};

template <>
class NanSensitiveEqMatcher<float> : public Catch::MatcherBase<float> {
  float value;

public:
  explicit NanSensitiveEqMatcher(const float v) : value(v) {}

  bool match(const float &v) const override {
    return nan_sensitive_eq(v, value);
  }

  std::string describe() const override {
    std::ostringstream ss;
    ss << "is NaN sensitively equal to " << value;
    return ss.str();
  }
};

template <>
class NanSensitiveEqMatcher<double> : public Catch::MatcherBase<double> {
  double value;

public:
  explicit NanSensitiveEqMatcher(const double v) : value(v) {}

  bool match(const double &v) const override {
    return nan_sensitive_eq(v, value);
  }

  std::string describe() const override {
    std::ostringstream ss;
    ss << "is NaN sensitively equal to " << value;
    return ss.str();
  }
};

template <typename T>
inline NanSensitiveEqMatcher<T> nan_sensitive_equal(const T value) {
  return NanSensitiveEqMatcher<T>(value);
}

template <>
void compare(const std::vector<float> &output,
             const std::vector<float> &reference) {
  for (int i = 0; i < output.size(); ++i) {
    REQUIRE_THAT(output[i], nan_sensitive_equal(reference[i]));
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
    REQUIRE_THAT(output[i], nan_sensitive_equal(reference[i]));
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
