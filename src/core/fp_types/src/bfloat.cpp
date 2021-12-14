/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cstring>

#include <cassian/fp_types/bfloat.hpp>

namespace cassian {

Bfloat::Bfloat(float v) {
  uint32_t tmp = 0;
  uint16_t remainder = 0;
  const uint32_t remainder_mask = 0x0000ffff;
  const int remainder_bits = 16;
  std::memcpy(&tmp, &v, sizeof(uint32_t)); // Type punning
  remainder = tmp & remainder_mask;
  tmp = tmp >> remainder_bits; // Fit into 16 bits
  const int16_t exponent_mask = 0x7f80;
  const int16_t mantissa_mask = 0x007f;
  const bool is_nan_or_inf = (tmp & exponent_mask) == exponent_mask;

  if (!is_nan_or_inf) {
    const int highest_temp = 0xffff;
    const int remainder_highest_bit = 0x8000;

    if ((remainder > remainder_highest_bit && tmp != highest_temp) ||
        (remainder == remainder_highest_bit && tmp != highest_temp &&
         tmp % 2 == 1)) { // Round to nearest or even
      ++tmp;
    }
  } else {
    if (remainder != 0) { // Make sure NaN is not lost by cropping
      tmp |= mantissa_mask;
    }
  }
  data = 0;
  std::memcpy(&data, &tmp, sizeof(uint16_t));
}

Bfloat::operator float() const {
  float tmp_f = 0;
  uint32_t tmp_32 = data;
  const int remainder_bits = 16;
  tmp_32 = tmp_32 << remainder_bits;
  std::memcpy(&tmp_f, &tmp_32, sizeof(uint32_t));
  return tmp_f;
}

bool Bfloat::operator==(const Bfloat &rhs) const {
  const auto f_lhs = static_cast<float>(*this);
  const auto f_rhs = static_cast<float>(rhs);
  return f_lhs == f_rhs;
}

bool Bfloat::operator!=(const Bfloat &rhs) const { return !(*this == rhs); };

bool Bfloat::operator<(const Bfloat &rhs) const {
  const auto f_lhs = static_cast<float>(*this);
  const auto f_rhs = static_cast<float>(rhs);
  return f_lhs < f_rhs;
}

bool Bfloat::operator>(const Bfloat &rhs) const { return rhs < *this; };

bool Bfloat::operator<=(const Bfloat &rhs) const { return !(*this > rhs); };

bool Bfloat::operator>=(const Bfloat &rhs) const { return !(*this < rhs); };

bool Bfloat::nan_sensitive_eq(const Bfloat &rhs) const {
  const int16_t exponent_mask = 0x7f80;
  const int16_t mantissa_mask = 0x007f;
  const bool this_nan =
      (data & exponent_mask) == exponent_mask && (data & mantissa_mask) != 0;
  const bool rhs_nan = (rhs.data & exponent_mask) == exponent_mask &&
                       (rhs.data & mantissa_mask) != 0;
  if (this_nan && rhs_nan) {
    return true;
  }
  return data == rhs.data;
}

} // namespace cassian
