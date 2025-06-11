/*
 * Copyright (C) 2025 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/utility/matchers.hpp>
#include <cstring>

namespace cassian {

template <> bool nan_sensitive_eq(const float lhs, const float rhs) {
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

template <> bool nan_sensitive_eq(const double lhs, const double rhs) {
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

} // namespace cassian
