/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/fp_types/half.hpp>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <sstream>

namespace cassian {

Half::Half(float v) {
  const int float_exponent_offset = 127;
  const int half_exponent_offset = 15;
  const int max_half_exponent_value = 15;
  const uint32_t sign_mask = 0x80000000;
  const uint32_t exponent_float_mask = 0x7f800000;
  const uint32_t mantissa_float_mask = 0x007fe000;
  const uint32_t remainder_float_mask = 0x00001fff;
  const int sign_float_shift = 31;
  const int exponent_float_shift = 23;
  const int mantissa_float_shift = 13;
  uint32_t tmp = 0;
  std::memcpy(&tmp, &v, sizeof(uint32_t));
  const uint16_t sign = (tmp & sign_mask) >> sign_float_shift;
  uint16_t exponent = (tmp & exponent_float_mask) >> exponent_float_shift;
  uint16_t mantissa = (tmp & mantissa_float_mask) >> mantissa_float_shift;
  const uint16_t remainder = (tmp & remainder_float_mask);
  const uint16_t highest_float_exponent = 0xff;
  const uint16_t highest_half_exponent = 0x1f;
  const int sign_half_shift = 15;
  const int exponent_half_shift = 10;
  const uint16_t exponent_half_mask = 0x7c00;
  const uint16_t mantissa_half_mask = 0x3ff;
  if (exponent != 0 && exponent != highest_float_exponent) {
    if (exponent <= float_exponent_offset - half_exponent_offset) {
      // underflow to zero
      data = sign << sign_half_shift;
      return;
    }
    if (exponent > float_exponent_offset + max_half_exponent_value) {
      // overflow to inf
      data = (sign << sign_half_shift) | exponent_half_mask;
      return;
    }
    exponent -= float_exponent_offset - half_exponent_offset;
  } else if (exponent == highest_float_exponent) {
    exponent = highest_half_exponent;
  }
  const int highest_remainder_bit = 0x1000;
  // round to nearest or even
  if ((remainder > highest_remainder_bit && mantissa != mantissa_half_mask) ||
      (remainder == highest_remainder_bit && mantissa != mantissa_half_mask &&
       mantissa % 2 == 1)) {
    ++mantissa;
  }
  data =
      (sign << sign_half_shift) | (exponent << exponent_half_shift) | mantissa;
}

Half::operator float() const {
  const uint32_t float16_exp_shift = (23 - 10);
  const uint32_t float16_exp_mask = 0x7c00;
  const uint32_t float32_exp_mask = 0x7f800000;
  const uint32_t float16_mantissa_mask = 0x03ff;
  // -9 is needed because later we do multiplication of floats
  // 127 - 15 - 9 = 103, float exponent: 103 - 127 = -24
  // smallest half denorm = 2^-14 * 2^-10 = 2^-24
  // 2^-24 * float(mantissa = 0x0001) = 2^-24 * 1.0
  const uint32_t float16_to_32_bias_diff_denorm = ((127 - 15 - 9) << 23);
  const uint32_t float16_to_32_bias_diff = ((127 - 15) << 10);
  const uint32_t float16_sign_mask = 0x8000;
  const uint32_t exp = data & float16_exp_mask;
  const uint32_t mantissa = data & float16_mantissa_mask;
  const uint32_t sign = (data & float16_sign_mask) << 16;
  const uint32_t nan = (exp == float16_exp_mask ? float32_exp_mask : 0U);
  // 0.0
  if ((exp | mantissa) == 0) {
    return *reinterpret_cast<const float *>(&sign);
  }
  // normals
  if (exp != 0) {
    uint32_t tmp =
        (((exp + float16_to_32_bias_diff) | mantissa) << float16_exp_shift) |
        nan | sign;
    return *reinterpret_cast<float *>(&tmp);
  }
  // subnormals
  float tmpf =
      *reinterpret_cast<const float *>(&float16_to_32_bias_diff_denorm) *
      static_cast<float>(mantissa);
  *reinterpret_cast<uint32_t *>(&tmpf) |= sign;
  return tmpf;
}

bool Half::operator==(const Half &rhs) const {
  const auto f_lhs = static_cast<float>(*this);
  const auto f_rhs = static_cast<float>(rhs);
  return f_lhs == f_rhs;
}

bool Half::operator!=(const Half &rhs) const { return !(*this == rhs); };

bool Half::operator<(const Half &rhs) const {
  const auto f_lhs = static_cast<float>(*this);
  const auto f_rhs = static_cast<float>(rhs);
  return f_lhs < f_rhs;
}

bool Half::operator>(const Half &rhs) const { return rhs < *this; };

bool Half::operator<=(const Half &rhs) const { return !(*this > rhs); };

bool Half::operator>=(const Half &rhs) const { return !(*this < rhs); };

bool Half::nan_sensitive_eq(const Half &rhs) const {
  const uint16_t exponent_mask = 0x7c00;
  const uint16_t mantissa_mask = 0x03ff;
  const bool this_nan =
      (data & exponent_mask) == exponent_mask && (data & mantissa_mask) != 0;
  const bool rhs_nan = (rhs.data & exponent_mask) == exponent_mask &&
                       (rhs.data & mantissa_mask) != 0;
  if (this_nan && rhs_nan) {
    return true;
  }
  return data == rhs.data;
}

Half operator+(Half lhs, Half rhs) {
  return Half(static_cast<float>(lhs) + static_cast<float>(rhs));
}

Half operator-(Half lhs, Half rhs) {
  return Half(static_cast<float>(lhs) - static_cast<float>(rhs));
}

Half operator*(Half lhs, Half rhs) {
  return Half(static_cast<float>(lhs) * static_cast<float>(rhs));
}

Half operator/(Half lhs, Half rhs) {
  return Half(static_cast<float>(lhs) / static_cast<float>(rhs));
}

std::string to_string(const Half &value) {
  std::stringstream ss;
  ss << std::noshowbase << "0x" << std::setfill('0') << std::setw(4) << std::hex
     << value.decode();
  return ss.str();
}

std::ostream &operator<<(std::ostream &os, const Half &value) {
  os << to_string(value);
  return os;
}

Half abs(Half value) { return Half::encode(value.decode() & ~(1 << 15)); }

Half sqrt(Half value) { return Half(std::sqrt(static_cast<float>(value))); }

} // namespace cassian
