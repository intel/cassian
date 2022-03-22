/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_FP_TYPES_BFLOAT_HPP
#define CASSIAN_FP_TYPES_BFLOAT_HPP

#include <cstdint>
#include <limits>

#include <cassian/fp_types/type_traits.hpp>

/**
 * Cassian namespace.
 */
namespace cassian {

/**
 * Bfloat class representing half precision floating point type
 *
 * Data layout: 1 sign bit, 8 exponent bits, 7 mantissa bits
 */
class Bfloat {
public:
  /**
   * Default constructor
   */
  constexpr Bfloat() : data(0) {}

  /**
   * Constructor from float
   *
   * @param[in] v value to be converted to bfloat
   */
  explicit Bfloat(float v);

  /**
   * Metafunction to enable a constructor declaration if T is integral, double
   * or long double.
   *
   * @tparam T type.
   */
  template <typename T>
  using EnableConstructionFrom =
      std::enable_if_t<std::is_integral_v<T> || std::is_same_v<T, double> ||
                           std::is_same_v<T, long double>,
                       T>;

  /**
   * Constructor from integral types, double and long double
   *
   * @param[in] v value to be converted to bfloat
   */
  template <typename T>
  explicit Bfloat(EnableConstructionFrom<T> v)
      : Bfloat(static_cast<float>(v)) {}

  /**
   * Conversion operator to float
   *
   * @returns value converted to float
   */
  explicit operator float() const;

  /**
   * Conversion operator to integral types, double and long double
   *
   * @returns value converted to T
   */
  template <typename T> explicit operator T() const { return cast<T>(); }

  /**
   * Equal operator.
   *
   * @param[in] rhs Value to compare against.
   * @returns true if both values are equal.
   */
  bool operator==(const Bfloat &rhs) const;

  /**
   * Not equal operator.
   *
   * @param[in] rhs Value to compare against.
   * @returns true if values are not equal bitwise.
   */
  bool operator!=(const Bfloat &rhs) const;

  /**
   * Less than operator.
   *
   * @param[in] rhs Value to compare.
   * @returns true if this is less than rhs.
   */
  bool operator<(const Bfloat &rhs) const;

  /**
   * Greater than operator.
   *
   * @param[in] rhs Value to compare.
   * @returns true if this is greater than rhs.
   */
  bool operator>(const Bfloat &rhs) const;

  /**
   * Less than or equal operator.
   *
   * @param[in] rhs Value to compare.
   * @returns true if this is less than or equal to rhs.
   */
  bool operator<=(const Bfloat &rhs) const;

  /**
   * Greater than or equal operator.
   *
   * @param[in] rhs Value to compare.
   * @returns true if this is greater than or equal to rhs.
   */
  bool operator>=(const Bfloat &rhs) const;

  /**
   * Unary plus operator.
   */
  Bfloat operator+() const;

  /**
   * Unary minus operator.
   */
  Bfloat operator-() const;

  /**
   * Addition operator
   *
   * @param[in] rhs value to add
   * @returns sum of lhs and rhs
   */
  Bfloat operator+(const Bfloat &rhs) const;

  /**
   * Subtraction operator
   *
   * @param[in] rhs value to subtract
   * @returns difference between lhs and rhs
   */
  Bfloat operator-(const Bfloat &rhs) const;

  /**
   * Nan sensitive equal
   *
   * @returns bool value - result of comparison
   */
  bool nan_sensitive_eq(const Bfloat &rhs) const;

  /**
   * Metadata needed to distinguish vector-like data types.
   */
  using is_custom_type = int;

  /**
   * Internal storage type
   */
  using storage_t = uint16_t;

  /**
   * Convert binary representation into Half object
   *
   * @param[in] v binary representation to be converted to half
   * @returns half object
   */
  static constexpr Bfloat encode(storage_t v) {
    Bfloat r;
    r.data = v;
    return r;
  }

  /**
   * Get binary representation of a half precision value
   *
   * @returns binary representation
   */
  constexpr storage_t decode() const { return data; }

private:
  /**
   * Metafunction to enable a cast operator declaration if T is integral, double
   * or long double.
   *
   * @tparam T type.
   */
  template <typename T>
  using EnableCastTo =
      std::enable_if_t<std::is_integral_v<T> || std::is_same_v<T, double> ||
                           std::is_same_v<T, long double>,
                       T>;

  /**
   * Cast to T type.
   *
   * @tparam T type.
   */
  template <typename T> EnableCastTo<T> cast() const {
    return static_cast<T>(static_cast<float>(*this));
  }

  /**
   * Binary representation
   */
  storage_t data;
};

/**
 * Alias for type name
 */
using bfloat = Bfloat;

template <> struct is_floating_point<Bfloat> : std::true_type {};

} // namespace cassian

/**
 * Std namespace.
 */
namespace std {
/**
 * numeric limits specializations
 */
template <> class numeric_limits<cassian::Bfloat> {
public:
  /**
   * std::numeric_limits is specialized
   */
  static constexpr bool is_specialized = true;

  /**
   * cassian::Bfloat is signed
   */
  static constexpr bool is_signed = true;

  /**
   * cassian::Bfloat is not integer
   */
  static constexpr bool is_integer = false;

  /**
   * cassian::Bfloat is inexact
   */
  static constexpr bool is_exact = false;

  /**
   * cassian::Bfloat has inifinity
   */
  static constexpr bool has_infinity = true;

  /**
   * cassian::Bfloat has quiet NaN
   */
  static constexpr bool has_quiet_NaN = true;

  /**
   * cassian::Bfloat has signaling NaN
   */
  static constexpr bool has_signaling_NaN = true;

  /**
   * cassian::Bfloat has denormals
   */
  static constexpr bool has_denorm = true;

  /**
   * cassian::Bfloat does not detect loss of precision as denormalization loss
   */
  static constexpr bool has_denorm_loss = false;

  /**
   * cassian::Bfloat uses rounding to nearest even
   */
  static constexpr std::float_round_style round_style = std::round_to_nearest;

  /**
   * cassian::Bfloat is IEC 559/IEEE 745 type
   */
  static constexpr bool is_iec559 = true;

  /**
   * cassian::Bfloat is bounded
   */
  static constexpr bool is_bounded = true;

  /**
   * cassian::Bfloat does not handle overflows with modulo arithmetic
   */
  static constexpr bool is_modulo = false;

  /**
   * number of radix digits
   */
  static constexpr int digits = 7;

  /**
   * number of decimal digits
   */
  static constexpr int digits10 = 4;

  /**
   * number of decimal digits necessary to differentiate all values of this type
   */
  static constexpr int max_digits10 = 4;

  /**
   * radix or integer base
   */
  static constexpr int radix = std::numeric_limits<float>::radix;

  /**
   * one more than the smallest negative power of the radix that is a valid
   * normalized floating-point value
   */
  static constexpr int min_exponent = -126;

  /**
   * the smallest negative power of ten that is a valid normalized
   * floating-point value
   */
  static constexpr int min_exponent10 = -37;

  /**
   * one more than the largest integer power of the radix that is a valid finite
   * floating-point value
   */
  static constexpr int max_exponent = 127;

  /**
   * the largest integer power of 10 that is a valid finite floating-point value
   */
  static constexpr int max_exponent10 = 38;

  /**
   * identifies types which can cause arithmetic operations to trap
   */
  static constexpr bool traps = false;

  /**
   * identifies floating-point types that detect tinyness before rounding
   */
  static constexpr bool tinyness_before = false;

  /**
   * minimum value
   */
  static constexpr cassian::Bfloat min() {
    return cassian::Bfloat::encode(0x0080);
  };
  /**
   * maximum value
   */
  static constexpr cassian::Bfloat max() {
    return cassian::Bfloat::encode(0x7f7f);
  };
  /**
   * lowest value
   */
  static constexpr cassian::Bfloat lowest() {
    return cassian::Bfloat::encode(0xff7f);
  };
  /**
   * difference between 1.0 and the next representable value
   */
  static cassian::Bfloat epsilon() {
    return cassian::Bfloat(std::numeric_limits<float>::epsilon());
  }
  /**
   * maximum rounding error
   */
  static cassian::Bfloat round_error() { return cassian::Bfloat(0.5f); }
  /**
   * infinity value
   */
  static constexpr cassian::Bfloat infinity() {
    return cassian::Bfloat::encode(0x7f80);
  };
  /**
   * quiet nan value
   */
  // specific quiet and signaling NaN encoding is architecture dependent
  static constexpr cassian::Bfloat quiet_NaN() {
    return cassian::Bfloat::encode(0x7f81);
  };
  /**
   * signaling nan value
   */
  static constexpr cassian::Bfloat signaling_NaN() {
    // in F16C x86 extension f32 sNaN is converted to f16 qNaN
    // ref: Intel 64 and IA-32 Architectures Software Developer's Manual
    // Volume 1, order number 325462-069US, table 14-9
    return quiet_NaN();
  };
  /**
   * lowest denorm value
   */
  static constexpr cassian::Bfloat denorm_min() {
    return cassian::Bfloat::encode(0x0001);
  };
};

} // namespace std
#endif
