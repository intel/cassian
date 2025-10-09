/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_FP_TYPES_TFLOAT_HPP
#define CASSIAN_FP_TYPES_TFLOAT_HPP

#include <cmath>
#include <cstdint>
#include <limits>
#include <string>
#include <type_traits>

#include <cassian/fp_types/type_traits.hpp>

/**
 * Cassian namespace.
 */
namespace cassian {

/**
 * Tfloat class representing reduced precision 32-bit floating point type
 *
 * Data layout: 1 sign bit, 8 exponent bits, 10 mantissa bits
 * Remaining bits are set to 0
 */
class Tfloat {
public:
  /**
   * Default constructor
   */
  constexpr Tfloat() : data(0) {}

  /**
   * Constructor from float
   *
   * @param[in] v value to be converted to Tfloat
   */
  explicit Tfloat(float v);

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
   * @param[in] v value to be converted to tfloat
   */
  template <typename T>
  explicit Tfloat(EnableConstructionFrom<T> v)
      : Tfloat(static_cast<float>(v)) {}

  /**
   * Conversion operator to float
   *
   * @returns value converted to float
   */
  explicit operator float() const;

  /**
   * Equal operator.
   *
   * @param[in] rhs Value to compare against.
   * @returns true if both values are equal.
   */
  bool operator==(const Tfloat &rhs) const;

  /**
   * Not equal operator.
   *
   * @param[in] rhs Value to compare against.
   * @returns true if values are not equal bitwise.
   */
  bool operator!=(const Tfloat &rhs) const;

  /**
   * Less than operator.
   *
   * @param[in] rhs Value to compare.
   * @returns true if this is less than rhs.
   */
  bool operator<(const Tfloat &rhs) const;

  /**
   * Greater than operator.
   *
   * @param[in] rhs Value to compare.
   * @returns true if this is greater than rhs.
   */
  bool operator>(const Tfloat &rhs) const;

  /**
   * Less than or equal operator.
   *
   * @param[in] rhs Value to compare.
   * @returns true if this is less than or equal to rhs.
   */
  bool operator<=(const Tfloat &rhs) const;

  /**
   * Greater than or equal operator.
   *
   * @param[in] rhs Value to compare.
   * @returns true if this is greater than or equal to rhs.
   */
  bool operator>=(const Tfloat &rhs) const;

  /**
   * Unary plus operator.
   */
  Tfloat operator+() const;

  /**
   * Unary minus operator.
   */
  Tfloat operator-() const;

  /**
   * Nan sensitive equal
   *
   * @returns bool value - result of comparison
   */
  bool nan_sensitive_eq(const Tfloat &rhs) const;

  /**
   * Metadata needed to distinguish vector-like data types.
   */
  using is_custom_type = int;

  /**
   * Internal storage type
   */
  using storage_t = uint32_t;

  /**
   * Convert binary representation into Tfloat object
   *
   * @param[in] v binary representation to be converted to tfloat
   * @returns tfloat object
   */
  static constexpr Tfloat encode(storage_t v) {
    Tfloat r;
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
  using EnableTfloatArithmeticIfIntegral =
      std::enable_if_t<std::is_integral_v<T>, Tfloat>;

  template <typename T>
  using EnableTfloatArithmeticIfFloatingPoint =
      std::enable_if_t<std::is_floating_point_v<T>, T>;

  /**
   * Addition operator
   *
   * @param[in] lhs left-hand side value
   * @param[in] rhs right-hand side value
   * @returns sum of lhs and rhs
   */
  friend Tfloat operator+(Tfloat lhs, Tfloat rhs);

  /**
   * @overload
   */
  template <typename T>
  friend EnableTfloatArithmeticIfIntegral<T> operator+(T lhs, Tfloat rhs) {
    return static_cast<Tfloat>(lhs) + rhs;
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableTfloatArithmeticIfIntegral<T> operator+(Tfloat lhs, T rhs) {
    return lhs + static_cast<Tfloat>(rhs);
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableTfloatArithmeticIfFloatingPoint<T> operator+(T lhs, Tfloat rhs) {
    return lhs + static_cast<T>(rhs);
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableTfloatArithmeticIfFloatingPoint<T> operator+(Tfloat lhs, T rhs) {
    return static_cast<T>(lhs) + rhs;
  }

  /**
   * Subtraction operator
   *
   * @param[in] lhs left-hand side value
   * @param[in] rhs right-hand side value
   * @returns difference between lhs and rhs
   */
  friend Tfloat operator-(Tfloat lhs, Tfloat rhs);

  /**
   * @overload
   */
  template <typename T>
  friend EnableTfloatArithmeticIfIntegral<T> operator-(T lhs, Tfloat rhs) {
    return static_cast<Tfloat>(lhs) - rhs;
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableTfloatArithmeticIfIntegral<T> operator-(Tfloat lhs, T rhs) {
    return lhs - static_cast<Tfloat>(rhs);
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableTfloatArithmeticIfFloatingPoint<T> operator-(T lhs, Tfloat rhs) {
    return lhs - static_cast<T>(rhs);
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableTfloatArithmeticIfFloatingPoint<T> operator-(Tfloat lhs, T rhs) {
    return static_cast<T>(lhs) - rhs;
  }

  /**
   * Multiplication operator
   *
   * @param[in] lhs left-hand side value
   * @param[in] rhs right-hand side value
   * @returns product of lhs and rhs
   */
  friend Tfloat operator*(Tfloat lhs, Tfloat rhs);

  /**
   * @overload
   */
  template <typename T>
  friend EnableTfloatArithmeticIfIntegral<T> operator*(T lhs, Tfloat rhs) {
    return static_cast<Tfloat>(lhs) * rhs;
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableTfloatArithmeticIfIntegral<T> operator*(Tfloat lhs, T rhs) {
    return lhs * static_cast<Tfloat>(rhs);
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableTfloatArithmeticIfFloatingPoint<T> operator*(T lhs, Tfloat rhs) {
    return lhs * static_cast<T>(rhs);
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableTfloatArithmeticIfFloatingPoint<T> operator*(Tfloat lhs, T rhs) {
    return static_cast<T>(lhs) * rhs;
  }

  /**
   * Division operator
   *
   * @param[in] lhs left-hand side value
   * @param[in] rhs right-hand side value
   * @returns quotient
   */
  friend Tfloat operator/(Tfloat lhs, Tfloat rhs);

  /**
   * @overload
   */
  template <typename T>
  friend EnableTfloatArithmeticIfIntegral<T> operator/(T lhs, Tfloat rhs) {
    return static_cast<Tfloat>(lhs) / rhs;
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableTfloatArithmeticIfIntegral<T> operator/(Tfloat lhs, T rhs) {
    return lhs / static_cast<Tfloat>(rhs);
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableTfloatArithmeticIfFloatingPoint<T> operator/(T lhs, Tfloat rhs) {
    return lhs / static_cast<T>(rhs);
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableTfloatArithmeticIfFloatingPoint<T> operator/(Tfloat lhs, T rhs) {
    return static_cast<T>(lhs) / rhs;
  }

  /**
   * Convert Tfloat to string representation.
   *
   * @param[in] value object to convert.
   */
  friend std::string to_string(const Tfloat &value);

  /**
   * Append string representation of Tfloat object to a stream.
   *
   * @param[in] os stream to use.
   * @param[in] value object to convert.
   * @returns used stream.
   */
  friend std::ostream &operator<<(std::ostream &os, const Tfloat &value);
};

/**
 * Alias for type name
 */
using tfloat = Tfloat;

template <> struct is_floating_point<Tfloat> : std::true_type {};

template <> struct is_custom_type<Tfloat> : std::true_type {};

/**
 * Checks whether the value is NaN.
 *
 * @param[in] value object to use.
 */
bool isnan(Tfloat value);

/**
 * Computes the absolute value of a Tfloat value arg.
 *
 * @param[in] value object to use.
 * @returns absolute value.
 */
Tfloat abs(Tfloat value);

/**
 * Computes the square root of a Tfloat value arg.
 *
 * @param[in] value object to use.
 * @returns square root.
 */
Tfloat sqrt(Tfloat value);

/**
 * Computes the next representable value of from in the direction of to.
 *
 * @param[in] from base value.
 * @param[in] to value toward which the return value is approximated.
 * @returns the next representable value.
 */
Tfloat nextafter(const Tfloat from, const Tfloat to);

} // namespace cassian

/**
 * Std namespace.
 */
namespace std {
/**
 * numeric limits specializations
 */
template <> class numeric_limits<cassian::Tfloat> {
public:
  /**
   * std::numeric_limits is specialized
   */
  static constexpr bool is_specialized = true;

  /**
   * cassian::Tfloat is signed
   */
  static constexpr bool is_signed = true;

  /**
   * cassian::Tfloat is not integer
   */
  static constexpr bool is_integer = false;

  /**
   * cassian::Tfloat is inexact
   */
  static constexpr bool is_exact = false;

  /**
   * cassian::Tfloat has inifinity
   */
  static constexpr bool has_infinity = true;

  /**
   * cassian::Tfloat has quiet NaN
   */
  static constexpr bool has_quiet_NaN = true;

  /**
   * cassian::Tfloat has signaling NaN
   */
  static constexpr bool has_signaling_NaN = true;

  /**
   * cassian::Tfloat has denormals
   */
  static constexpr bool has_denorm = true;

  /**
   * cassian::Tfloat does not detect loss of precision as denormalization loss
   */
  static constexpr bool has_denorm_loss = false;

  /**
   * cassian::Tfloat uses rounding to nearest even
   */
  static constexpr std::float_round_style round_style = std::round_to_nearest;

  /**
   * cassian::Tfloat is IEC 559/IEEE 754 type
   */
  static constexpr bool is_iec559 = true;

  /**
   * cassian::Tfloat is bounded
   */
  static constexpr bool is_bounded = true;

  /**
   * cassian::Tfloat does not handle overflows with modulo arithmetic
   */
  static constexpr bool is_modulo = false;

  /**
   * number of radix digits
   */
  static constexpr int digits = 11;

  /**
   * number of decimal digits
   */
  static constexpr int digits10 = 6;

  /**
   * number of decimal digits necessary to differentiate all values of this type
   */
  static constexpr int max_digits10 = 9;

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
  static cassian::Tfloat min() { return cassian::Tfloat::encode(0x00800000); }
  /**
   * maximum value
   */
  static cassian::Tfloat max() { return cassian::Tfloat::encode(0x7f7fe000); }
  /**
   * lowest value
   */
  static constexpr cassian::Tfloat lowest() {
    return cassian::Tfloat::encode(0xff7fe000);
  }
  /**
   * difference between 1.0 and the next representable value
   */
  static cassian::Tfloat epsilon() {
    // encode(0x3f802000) - encode(0x3f800000);
    return cassian::Tfloat::encode(0x3a800000);
  }
  /**
   * maximum rounding error
   */
  static cassian::Tfloat round_error() { return cassian::Tfloat(0.5f); }
  /**
   * infinity value
   */
  static constexpr cassian::Tfloat infinity() {
    return cassian::Tfloat::encode(0x7f800000);
  };
  /**
   * quiet nan value
   */
  // specific quiet and signaling NaN encoding is architecture dependent
  static constexpr cassian::Tfloat quiet_NaN() {
    return cassian::Tfloat::encode(0x7f802000);
  };
  /**
   * signaling nan value
   */
  static constexpr cassian::Tfloat signaling_NaN() { return quiet_NaN(); };
  /**
   * lowest denorm value
   */
  static constexpr cassian::Tfloat denorm_min() {
    return cassian::Tfloat::encode(0x00002000);
  };
};

} // namespace std
#endif
