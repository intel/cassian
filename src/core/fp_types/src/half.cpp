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
  uint32_t bits = 0;
  std::memcpy(&bits, &v, sizeof(v));

  constexpr uint32_t float_sign_mask = 0x80000000;
  constexpr uint32_t float_exp_mask = 0x7f800000;
  constexpr uint32_t float_mantissa_mask = 0x007fffff;
  constexpr unsigned float_exp_shift = 23;
  constexpr uint8_t float_bias = 127;
  constexpr uint8_t float_inf_nan_exp = 0xff;

  constexpr uint8_t half_bias = 15;
  constexpr uint8_t half_exp_shift = 10;
  constexpr uint8_t half_inf_nan_exp = 0x1f;
  constexpr int8_t half_min_exp = -14;
  constexpr int8_t half_min_exp_denorm = -24;
  constexpr uint16_t half_implicit_one = 0x0400;
  constexpr uint16_t half_nan_quiet = 1 << 9;

  constexpr int float_mantissa_shift = 13;
  constexpr uint32_t half_next_unsaved_mask = 1 << (float_mantissa_shift - 1);
  constexpr uint32_t half_rest_unsaved_mask = half_next_unsaved_mask - 1;

  const uint16_t sign = (bits & float_sign_mask) >> 16;
  const uint32_t mantissa32 = bits & float_mantissa_mask;
  const uint8_t biased_exp32 = (bits & float_exp_mask) >> float_exp_shift;

  uint16_t biased_exp16 = 0;
  uint16_t mantissa16 = 0;

  if (biased_exp32 == float_inf_nan_exp) { // NaN or Inf
    biased_exp16 = half_inf_nan_exp;

    if (mantissa32 != 0) { // NaN, making it quiet
      mantissa16 = mantissa32 >> float_mantissa_shift | half_nan_quiet;
    }
  } else if (biased_exp32 != 0) {
    const int8_t exp = biased_exp32 - float_bias;

    if (exp < half_min_exp_denorm) {
      // too small, flush to zero
      mantissa16 = 0;
    } else if (exp < half_min_exp) {
      // small float numbers map to half denormals
      biased_exp16 = 0;
      // TODO: Round to nearest or even
      mantissa16 = half_implicit_one >> -(exp + float_mantissa_shift + 1) |
                   mantissa32 >> -(exp + 1);
    } else if (exp <= 15) {
      biased_exp16 = exp + half_bias;
      mantissa16 = mantissa32 >> float_mantissa_shift;

      // Round to nearest or even
      const bool next = (mantissa32 & half_next_unsaved_mask) != 0;
      const bool rest = (mantissa32 & half_rest_unsaved_mask) != 0;
      const bool is_odd = (mantissa16 & 1) != 0;

      if (next && (is_odd || rest)) {
        mantissa16 += 1;
      }
    } else {
      // large float maps to inf
      biased_exp16 = half_inf_nan_exp;
    }
  }

  data = sign + (biased_exp16 << half_exp_shift) + mantissa16;
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
