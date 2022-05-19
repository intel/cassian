/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cstring>
#include <iomanip>
#include <sstream>

#include <cassian/fp_types/tfloat.hpp>

namespace cassian {

Tfloat::Tfloat(float v) {
  uint32_t tmp = 0;
  uint32_t remainder = 0;
  const uint32_t remainder_mask = 0x00001fff;
  std::memcpy(&tmp, &v, sizeof(uint32_t)); // Type punning
  remainder = tmp & remainder_mask;
  tmp &= (~remainder_mask); // Fit into 19 bits
  const uint32_t exponent_mask = 0x7f800000;
  const uint32_t mantissa_mask = 0x007fe000;
  const bool is_nan_or_inf = (tmp & exponent_mask) == exponent_mask;

  if (!is_nan_or_inf) {
    const uint32_t highest_temp = 0xffffe000;
    const uint32_t remainder_highest_bit = 0x00001000;
    const uint32_t last_bit = 0x00002000;

    if ((remainder > remainder_highest_bit && tmp != highest_temp) ||
        (remainder == remainder_highest_bit && tmp != highest_temp &&
         (tmp & last_bit) != 0)) { // Round to nearest or even
      tmp += last_bit;             // Add 19th bit
    }
  } else {
    if (remainder != 0) { // Make sure NaN is not lost by cropping
      tmp |= mantissa_mask;
    }
  }
  data = tmp;
}

Tfloat::operator float() const {
  float tmp_f = 0;
  std::memcpy(&tmp_f, &data, sizeof(uint32_t));
  return tmp_f;
}

bool Tfloat::operator==(const Tfloat &rhs) const {
  const auto f_lhs = static_cast<float>(*this);
  const auto f_rhs = static_cast<float>(rhs);
  return f_lhs == f_rhs;
}

bool Tfloat::operator!=(const Tfloat &rhs) const { return !(*this == rhs); };

bool Tfloat::operator<(const Tfloat &rhs) const {
  const auto f_lhs = static_cast<float>(*this);
  const auto f_rhs = static_cast<float>(rhs);
  return f_lhs < f_rhs;
}

bool Tfloat::operator>(const Tfloat &rhs) const { return rhs < *this; };

bool Tfloat::operator<=(const Tfloat &rhs) const { return !(*this > rhs); };

bool Tfloat::operator>=(const Tfloat &rhs) const { return !(*this < rhs); };

Tfloat operator+(Tfloat lhs, Tfloat rhs) {
  return Tfloat(static_cast<float>(lhs) + static_cast<float>(rhs));
};

Tfloat operator-(Tfloat lhs, Tfloat rhs) {
  return Tfloat(static_cast<float>(lhs) - static_cast<float>(rhs));
};

Tfloat operator*(Tfloat lhs, Tfloat rhs) {
  return Tfloat(static_cast<float>(lhs) * static_cast<float>(rhs));
};

Tfloat operator/(Tfloat lhs, Tfloat rhs) {
  return Tfloat(static_cast<float>(lhs) / static_cast<float>(rhs));
};

Tfloat Tfloat::operator+() const {
  return Tfloat::encode(data);
}

Tfloat Tfloat::operator-() const {
  const uint32_t sign_mask = 0x80000000;
  return Tfloat::encode(data ^ sign_mask);
}

bool Tfloat::nan_sensitive_eq(const Tfloat &rhs) const {
  const int32_t type_mask = 0xffffe000;
  const int32_t exponent_mask = 0x7f800000;
  const int32_t mantissa_mask = 0x007fe000;
  const bool this_nan =
      (data & exponent_mask) == exponent_mask && (data & mantissa_mask) != 0;
  const bool rhs_nan = (rhs.data & exponent_mask) == exponent_mask &&
                       (rhs.data & mantissa_mask) != 0;
  if (this_nan && rhs_nan) {
    return true;
  }
  return (data & type_mask) == (rhs.data & type_mask);
}

std::string to_string(const Tfloat &value) {
  std::stringstream ss;
  ss << std::noshowbase << "0x" << std::setfill('0') << std::setw(8) << std::hex
     << value.decode();
  return ss.str();
}

std::ostream &operator<<(std::ostream &os, const Tfloat &value) {
  os << to_string(value);
  return os;
}

bool isnan(Tfloat value) {
  const int32_t exponent_mask = 0x7f800000;
  const int32_t mantissa_mask = 0x007fe000;
  return (value.decode() & exponent_mask) == exponent_mask &&
         (value.decode() & mantissa_mask) != 0;
}

Tfloat abs(Tfloat value) {
  const uint32_t sign_mask = 0x80000000;
  return Tfloat::encode(value.decode() & ~sign_mask);
}

Tfloat sqrt(Tfloat value) {
  return Tfloat(std::sqrt(static_cast<float>(value)));
}

Tfloat nextafter(const Tfloat from, const Tfloat to) {
  return (from < to)
             ? Tfloat(from + std::numeric_limits<Tfloat>::epsilon())
             : (from > to
                    ? Tfloat(from - std::numeric_limits<Tfloat>::epsilon())
                    : Tfloat(to));
}

} // namespace cassian
