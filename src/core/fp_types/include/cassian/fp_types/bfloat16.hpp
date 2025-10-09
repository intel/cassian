/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_FP_TYPES_BFLOAT16_HPP
#define CASSIAN_FP_TYPES_BFLOAT16_HPP

#include <cmath>
#include <cstdint>
#include <limits>
#include <string>

#include <cassian/fp_types/type_traits.hpp>

/**
 * Cassian namespace.
 */
namespace cassian {

/**
 * Bfloat16 class representing half precision floating point type
 *
 * Data layout: 1 sign bit, 8 exponent bits, 7 mantissa bits
 */
class Bfloat16 {
public:
  /**
   * Default constructor
   */
  constexpr Bfloat16() : data(0) {}

  /**
   * Constructor from float
   *
   * @param[in] v value to be converted to bfloat16
   */
  explicit Bfloat16(float v);

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
   * @param[in] v value to be converted to bfloat16
   */
  template <typename T>
  explicit Bfloat16(EnableConstructionFrom<T> v)
      : Bfloat16(static_cast<float>(v)) {}

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
  bool operator==(const Bfloat16 &rhs) const;

  /**
   * Not equal operator.
   *
   * @param[in] rhs Value to compare against.
   * @returns true if values are not equal bitwise.
   */
  bool operator!=(const Bfloat16 &rhs) const;

  /**
   * Less than operator.
   *
   * @param[in] rhs Value to compare.
   * @returns true if this is less than rhs.
   */
  bool operator<(const Bfloat16 &rhs) const;

  /**
   * Greater than operator.
   *
   * @param[in] rhs Value to compare.
   * @returns true if this is greater than rhs.
   */
  bool operator>(const Bfloat16 &rhs) const;

  /**
   * Less than or equal operator.
   *
   * @param[in] rhs Value to compare.
   * @returns true if this is less than or equal to rhs.
   */
  bool operator<=(const Bfloat16 &rhs) const;

  /**
   * Greater than or equal operator.
   *
   * @param[in] rhs Value to compare.
   * @returns true if this is greater than or equal to rhs.
   */
  bool operator>=(const Bfloat16 &rhs) const;

  /**
   * Unary plus operator.
   */
  Bfloat16 operator+() const;

  /**
   * Unary minus operator.
   */
  Bfloat16 operator-() const;

  /**
   * Nan sensitive equal
   *
   * @returns bool value - result of comparison
   */
  bool nan_sensitive_eq(const Bfloat16 &rhs) const;

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
  static constexpr Bfloat16 encode(storage_t v) {
    Bfloat16 r;
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

  template <typename T>
  using EnableBfloat16ArithmeticIfIntegral =
      std::enable_if_t<std::is_integral_v<T>, Bfloat16>;

  template <typename T>
  using EnableBfloat16ArithmeticIfFloatingPoint =
      std::enable_if_t<std::is_floating_point_v<T>, T>;

  /**
   * Addition operator
   *
   * @param[in] lhs left-hand side value
   * @param[in] rhs right-hand side value
   * @returns sum of lhs and rhs
   */
  friend Bfloat16 operator+(Bfloat16 lhs, Bfloat16 rhs);

  /**
   * @overload
   */
  template <typename T>
  friend EnableBfloat16ArithmeticIfIntegral<T> operator+(T lhs, Bfloat16 rhs) {
    return static_cast<Bfloat16>(lhs) + rhs;
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableBfloat16ArithmeticIfIntegral<T> operator+(Bfloat16 lhs, T rhs) {
    return lhs + static_cast<Bfloat16>(rhs);
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableBfloat16ArithmeticIfFloatingPoint<T> operator+(T lhs,
                                                              Bfloat16 rhs) {
    return lhs + static_cast<T>(rhs);
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableBfloat16ArithmeticIfFloatingPoint<T> operator+(Bfloat16 lhs,
                                                              T rhs) {
    return static_cast<T>(lhs) + rhs;
  }

  /**
   * Subtraction operator
   *
   * @param[in] lhs left-hand side value
   * @param[in] rhs right-hand side value
   * @returns difference between lhs and rhs
   */
  friend Bfloat16 operator-(Bfloat16 lhs, Bfloat16 rhs);

  /**
   * @overload
   */
  template <typename T>
  friend EnableBfloat16ArithmeticIfIntegral<T> operator-(T lhs, Bfloat16 rhs) {
    return static_cast<Bfloat16>(lhs) - rhs;
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableBfloat16ArithmeticIfIntegral<T> operator-(Bfloat16 lhs, T rhs) {
    return lhs - static_cast<Bfloat16>(rhs);
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableBfloat16ArithmeticIfFloatingPoint<T> operator-(T lhs,
                                                              Bfloat16 rhs) {
    return lhs - static_cast<T>(rhs);
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableBfloat16ArithmeticIfFloatingPoint<T> operator-(Bfloat16 lhs,
                                                              T rhs) {
    return static_cast<T>(lhs) - rhs;
  }

  /**
   * Multiplication operator
   *
   * @param[in] lhs left-hand side value
   * @param[in] rhs right-hand side value
   * @returns product of lhs and rhs
   */
  friend Bfloat16 operator*(Bfloat16 lhs, Bfloat16 rhs);

  /**
   * @overload
   */
  template <typename T>
  friend EnableBfloat16ArithmeticIfIntegral<T> operator*(T lhs, Bfloat16 rhs) {
    return static_cast<Bfloat16>(lhs) * rhs;
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableBfloat16ArithmeticIfIntegral<T> operator*(Bfloat16 lhs, T rhs) {
    return lhs * static_cast<Bfloat16>(rhs);
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableBfloat16ArithmeticIfFloatingPoint<T> operator*(T lhs,
                                                              Bfloat16 rhs) {
    return lhs * static_cast<T>(rhs);
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableBfloat16ArithmeticIfFloatingPoint<T> operator*(Bfloat16 lhs,
                                                              T rhs) {
    return static_cast<T>(lhs) * rhs;
  }

  /**
   * Division operator
   *
   * @param[in] lhs left-hand side value
   * @param[in] rhs right-hand side value
   * @returns quotient
   */
  friend Bfloat16 operator/(Bfloat16 lhs, Bfloat16 rhs);

  /**
   * @overload
   */
  template <typename T>
  friend EnableBfloat16ArithmeticIfIntegral<T> operator/(T lhs, Bfloat16 rhs) {
    return static_cast<Bfloat16>(lhs) / rhs;
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableBfloat16ArithmeticIfIntegral<T> operator/(Bfloat16 lhs, T rhs) {
    return lhs / static_cast<Bfloat16>(rhs);
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableBfloat16ArithmeticIfFloatingPoint<T> operator/(T lhs,
                                                              Bfloat16 rhs) {
    return lhs / static_cast<T>(rhs);
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableBfloat16ArithmeticIfFloatingPoint<T> operator/(Bfloat16 lhs,
                                                              T rhs) {
    return static_cast<T>(lhs) / rhs;
  }

  /**
   * Convert Bfloat16 to string representation.
   *
   * @param[in] value object to convert.
   */
  friend std::string to_string(const Bfloat16 &value);

  /**
   * Append string representation of Bfloat16 object to a stream.
   *
   * @param[in] os stream to use.
   * @param[in] value object to convert.
   * @returns used stream.
   */
  friend std::ostream &operator<<(std::ostream &os, const Bfloat16 &value);
};

/**
 * Alias for type name
 */
using bfloat16 = Bfloat16;

static_assert(sizeof(bfloat16) == 2, "wrong size of bfloat16 type");

template <> struct is_floating_point<Bfloat16> : std::true_type {};

template <> struct is_custom_type<Bfloat16> : std::true_type {};

/**
 * Checks whether the value is NaN.
 *
 * @param[in] value object to use.
 */
bool isnan(Bfloat16 value);

/**
 * Computes the absolute value of a Bfloat16 value arg.
 *
 * @param[in] value object to use.
 * @returns absolute value.
 */
Bfloat16 abs(Bfloat16 value);

/**
 * Computes the square root of a Bfloat16 value arg.
 *
 * @param[in] value object to use.
 * @returns square root.
 */
Bfloat16 sqrt(Bfloat16 value);

/**
 * Computes the next representable value of from in the direction of to.
 *
 * @param[in] from base value.
 * @param[in] to value toward which the return value is approximated.
 * @returns the next representable value.
 */
Bfloat16 nextafter(const Bfloat16 from, const Bfloat16 to);

} // namespace cassian

/**
 * Std namespace.
 */
namespace std {
/**
 * numeric limits specializations
 */
template <> class numeric_limits<cassian::Bfloat16> {
public:
  /**
   * std::numeric_limits is specialized
   */
  static constexpr bool is_specialized = true;

  /**
   * cassian::Bfloat16 is signed
   */
  static constexpr bool is_signed = true;

  /**
   * cassian::Bfloat16 is not integer
   */
  static constexpr bool is_integer = false;

  /**
   * cassian::Bfloat16 is inexact
   */
  static constexpr bool is_exact = false;

  /**
   * cassian::Bfloat16 has inifinity
   */
  static constexpr bool has_infinity = true;

  /**
   * cassian::Bfloat16 has quiet NaN
   */
  static constexpr bool has_quiet_NaN = true;

  /**
   * cassian::Bfloat16 has signaling NaN
   */
  static constexpr bool has_signaling_NaN = true;

  /**
   * cassian::Bfloat16 has denormals
   */
  static constexpr bool has_denorm = true;

  /**
   * cassian::Bfloat16 does not detect loss of precision as denormalization loss
   */
  static constexpr bool has_denorm_loss = false;

  /**
   * cassian::Bfloat16 uses rounding to nearest even
   */
  static constexpr std::float_round_style round_style = std::round_to_nearest;

  /**
   * cassian::Bfloat16 is IEC 559/IEEE 754 type
   */
  static constexpr bool is_iec559 = true;

  /**
   * cassian::Bfloat16 is bounded
   */
  static constexpr bool is_bounded = true;

  /**
   * cassian::Bfloat16 does not handle overflows with modulo arithmetic
   */
  static constexpr bool is_modulo = false;

  /**
   * number of radix digits
   */
  static constexpr int digits = 8;

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
  static constexpr int min_exponent = -125;

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
  static constexpr cassian::Bfloat16 min() {
    return cassian::Bfloat16::encode(0x0080);
  };
  /**
   * maximum value
   */
  static constexpr cassian::Bfloat16 max() {
    return cassian::Bfloat16::encode(0x7f7f);
  };
  /**
   * lowest value
   */
  static constexpr cassian::Bfloat16 lowest() {
    return cassian::Bfloat16::encode(0xff7f);
  };
  /**
   * difference between 1.0 and the next representable value
   */
  static cassian::Bfloat16 epsilon() {
    return cassian::Bfloat16::encode(0x3c00);
  }
  /**
   * maximum rounding error
   */
  static cassian::Bfloat16 round_error() { return cassian::Bfloat16(0.5f); }
  /**
   * infinity value
   */
  static constexpr cassian::Bfloat16 infinity() {
    return cassian::Bfloat16::encode(0x7f80);
  };
  /**
   * quiet nan value
   */
  // specific quiet and signaling NaN encoding is architecture dependent
  static constexpr cassian::Bfloat16 quiet_NaN() {
    return cassian::Bfloat16::encode(0x7f81);
  };
  /**
   * signaling nan value
   */
  static constexpr cassian::Bfloat16 signaling_NaN() {
    // in F16C x86 extension f32 sNaN is converted to f16 qNaN
    // ref: Intel 64 and IA-32 Architectures Software Developer's Manual
    // Volume 1, order number 325462-069US, table 14-9
    return quiet_NaN();
  };
  /**
   * lowest denorm value
   */
  static constexpr cassian::Bfloat16 denorm_min() {
    return cassian::Bfloat16::encode(0x0001);
  };
};

} // namespace std
#endif
