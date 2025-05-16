/*
 * Copyright (C) 2021-2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cstring>
#include <iomanip>
#include <sstream>

#include <cassian/fp_types/bfloat16.hpp>

namespace cassian {

Bfloat16::Bfloat16(float v) {
  constexpr int remainder_bits = 16;
  constexpr uint32_t remainder_mask = (1 << remainder_bits) - 1;
  constexpr uint32_t exponent_mask = 0x7f80;
  constexpr uint32_t mantissa_mask = 0x007f;

  uint32_t tmp = 0;
  uint32_t remainder = 0;

  std::memcpy(&tmp, &v, sizeof(uint32_t)); // Type punning
  remainder = tmp & remainder_mask;
  tmp = tmp >> remainder_bits; // Fit into 16 bits

  const bool is_nan_or_inf = (tmp & exponent_mask) == exponent_mask;

  if (!is_nan_or_inf) {
    const int remainder_highest_bit = 0x8000;

    if ((remainder > remainder_highest_bit) ||
        (remainder == remainder_highest_bit &&
         tmp % 2 == 1)) { // Round to nearest or even
      ++tmp;
    }
  } else {
    if (remainder != 0) { // Make sure NaN is not lost by cropping
      tmp |= mantissa_mask;
    }
  }

  data = static_cast<uint16_t>(tmp);
}

Bfloat16::operator float() const {
  float tmp_f = 0;
  uint32_t tmp_32 = data;
  const int remainder_bits = 16;
  tmp_32 = tmp_32 << remainder_bits;
  std::memcpy(&tmp_f, &tmp_32, sizeof(uint32_t));
  return tmp_f;
}

bool Bfloat16::operator==(const Bfloat16 &rhs) const {
  const auto f_lhs = static_cast<float>(*this);
  const auto f_rhs = static_cast<float>(rhs);
  return f_lhs == f_rhs;
}

bool Bfloat16::operator!=(const Bfloat16 &rhs) const {
  return !(*this == rhs);
};

bool Bfloat16::operator<(const Bfloat16 &rhs) const {
  const auto f_lhs = static_cast<float>(*this);
  const auto f_rhs = static_cast<float>(rhs);
  return f_lhs < f_rhs;
}

bool Bfloat16::operator>(const Bfloat16 &rhs) const { return rhs < *this; };

bool Bfloat16::operator<=(const Bfloat16 &rhs) const { return !(*this > rhs); };

bool Bfloat16::operator>=(const Bfloat16 &rhs) const { return !(*this < rhs); };

Bfloat16 operator+(Bfloat16 lhs, Bfloat16 rhs) {
  return Bfloat16(static_cast<float>(lhs) + static_cast<float>(rhs));
};

Bfloat16 operator-(Bfloat16 lhs, Bfloat16 rhs) {
  return Bfloat16(static_cast<float>(lhs) - static_cast<float>(rhs));
};

Bfloat16 operator*(Bfloat16 lhs, Bfloat16 rhs) {
  return Bfloat16(static_cast<float>(lhs) * static_cast<float>(rhs));
};

Bfloat16 operator/(Bfloat16 lhs, Bfloat16 rhs) {
  return Bfloat16(static_cast<float>(lhs) / static_cast<float>(rhs));
};

Bfloat16 Bfloat16::operator+() const { return Bfloat16::encode(data); }

Bfloat16 Bfloat16::operator-() const {
  const uint16_t sign_mask = 0x8000;
  return Bfloat16::encode(data ^ sign_mask);
}

bool Bfloat16::nan_sensitive_eq(const Bfloat16 &rhs) const {
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

std::string to_string(const Bfloat16 &value) {
  std::stringstream ss;
  ss << std::noshowbase << "0x" << std::setfill('0') << std::setw(4) << std::hex
     << +value.decode();
  return ss.str();
}

std::ostream &operator<<(std::ostream &os, const Bfloat16 &value) {
  os << to_string(value);
  return os;
}

bool isnan(Bfloat16 value) {
  const int16_t exponent_mask = 0x7f80;
  const int16_t mantissa_mask = 0x007f;
  return (value.decode() & exponent_mask) == exponent_mask &&
         (value.decode() & mantissa_mask) != 0;
}

Bfloat16 abs(Bfloat16 value) {
  const uint16_t sign_mask = 0x8000;
  return Bfloat16::encode(value.decode() & ~sign_mask);
}

Bfloat16 sqrt(Bfloat16 value) {
  return Bfloat16(std::sqrt(static_cast<float>(value)));
}

Bfloat16 nextafter(const Bfloat16 from, const Bfloat16 to) {
  if (isnan(from) || isnan(to)) {
    return from + to;
  }
  if (from == to) {
    return to;
  }
  if (from == static_cast<Bfloat16>(0.0F)) {
    return (to > static_cast<Bfloat16>(0.0F))
               ? std::numeric_limits<Bfloat16>::denorm_min()
               : -std::numeric_limits<Bfloat16>::denorm_min();
  }

  int16_t from_data = from.decode();
  int16_t to_data = to.decode();

  if (from_data >= 0) {
    from_data += (to_data >= from_data) ? 1 : -1;
  } else {
    from_data += (to_data >= from_data && to_data < 0) ? 1 : -1;
  }

  return Bfloat16::encode(from_data);
}

} // namespace cassian
