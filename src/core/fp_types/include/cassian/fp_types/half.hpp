/*
 * Copyright (C) 2021-2024 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_FP_TYPES_HALF_HPP
#define CASSIAN_FP_TYPES_HALF_HPP

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
 * Half class representing half precision floating point type
 *
 * Data layout: 1 sign bit, 5 exponent bits, 10 mantissa bits
 */
class Half {
public:
  /**
   * Default constructor
   */
  constexpr Half() : data(0) {}

  /**
   * Constructor from float
   *
   * @param[in] v value to be converted to half
   */
  explicit Half(float v);

  /**
   * Constructor from float with stochastic rounding
   *
   * @param[in] v value to be converted to half with stochastic rounding
   * @param[in] random randomly generated value to be used as rounding input
   * @sa random_value
   */
  Half(float v, const float random);

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
   * @param[in] v value to be converted to half
   */
  template <typename T>
  explicit Half(EnableConstructionFrom<T> v) : Half(static_cast<float>(v)) {}

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
   * Assign operator.
   */
  template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
  Half &operator=(T value) {
    *this = Half(static_cast<float>(value));
    return *this;
  }

  /**
   * Logical not operator.
   */
  bool operator!() const { return !static_cast<bool>(*this); }

  /**
   * Unary plus operator.
   */
  constexpr Half operator+() const { return *this; }

  /**
   * Unary minus operator.
   */
  constexpr Half operator-() const {
    auto r = *this;
    r.data ^= 1 << 15;
    return r;
  }

  /**
   * Equal operator.
   *
   * @param[in] rhs Value to compare against.
   * @returns true if both values are equal.
   */
  bool operator==(const Half &rhs) const;

  /**
   * Not equal operator.
   *
   * @param[in] rhs Value to compare against.
   * @returns true if values are not equal bitwise.
   */
  bool operator!=(const Half &rhs) const;

  /**
   * Less than operator.
   *
   * @param[in] rhs Value to compare.
   * @returns true if this is less than rhs.
   */
  bool operator<(const Half &rhs) const;

  /**
   * Greater than operator.
   *
   * @param[in] rhs Value to compare.
   * @returns true if this is greater than rhs.
   */
  bool operator>(const Half &rhs) const;

  /**
   * Less than or equal operator.
   *
   * @param[in] rhs Value to compare.
   * @returns true if this is less than or equal to rhs.
   */
  bool operator<=(const Half &rhs) const;

  /**
   * Greater than or equal operator.
   *
   * @param[in] rhs Value to compare.
   * @returns true if this is greater than or equal to rhs.
   */
  bool operator>=(const Half &rhs) const;

  /**
   * Nan sensitive equal
   *
   * @returns bool value - result of comparison
   */
  bool nan_sensitive_eq(const Half &rhs) const;

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
  static constexpr Half encode(storage_t v) {
    Half r;
    r.data = v;
    return r;
  }

  /**
   * Get binary representation of a half precision value
   *
   * @returns binary representation
   */
  constexpr storage_t decode() const { return data; }

protected:
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

  /**
   * Metafunction to enable arithmetic operators for Half and integer mix
   *
   * @tparam T type
   */
  template <typename T>
  using EnableHalfArithmeticIfIntegral =
      std::enable_if_t<std::is_integral_v<T>, Half>;

  /**
   * Metafunction to enable arithmetic operators for Half and floating point mix
   *
   * @tparam T type
   */
  template <typename T>
  using EnableHalfArithmeticIfFloatingPoint =
      std::enable_if_t<std::is_floating_point_v<T>, T>;

  /**
   * Addition operator
   *
   * @param[in] lhs left-hand side value
   * @param[in] rhs right-hand side value
   * @returns sum of lhs and rhs
   */
  friend Half operator+(Half lhs, Half rhs);

  /**
   * @overload
   */
  template <typename T>
  friend EnableHalfArithmeticIfIntegral<T> operator+(T lhs, Half rhs) {
    return static_cast<Half>(lhs) + rhs;
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableHalfArithmeticIfIntegral<T> operator+(Half lhs, T rhs) {
    return lhs + static_cast<Half>(rhs);
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableHalfArithmeticIfFloatingPoint<T> operator+(T lhs, Half rhs) {
    return lhs + static_cast<T>(rhs);
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableHalfArithmeticIfFloatingPoint<T> operator+(Half lhs, T rhs) {
    return static_cast<T>(lhs) + rhs;
  }

  /**
   * Subtruction operator
   *
   * @param[in] lhs left-hand side value
   * @param[in] rhs right-hand side value
   * @returns difference between lhs and rhs
   */
  friend Half operator-(Half lhs, Half rhs);

  /**
   * @overload
   */
  template <typename T>
  friend EnableHalfArithmeticIfIntegral<T> operator-(T lhs, Half rhs) {
    return static_cast<Half>(lhs) - rhs;
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableHalfArithmeticIfIntegral<T> operator-(Half lhs, T rhs) {
    return lhs - static_cast<Half>(rhs);
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableHalfArithmeticIfFloatingPoint<T> operator-(T lhs, Half rhs) {
    return lhs - static_cast<T>(rhs);
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableHalfArithmeticIfFloatingPoint<T> operator-(Half lhs, T rhs) {
    return static_cast<T>(lhs) - rhs;
  }

  /**
   * Multiplication operator
   *
   * @param[in] lhs left-hand side value
   * @param[in] rhs right-hand side value
   * @returns product of lhs and rhs
   */
  friend Half operator*(Half lhs, Half rhs);

  /**
   * @overload
   */
  template <typename T>
  friend EnableHalfArithmeticIfIntegral<T> operator*(T lhs, Half rhs) {
    return static_cast<Half>(lhs) * rhs;
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableHalfArithmeticIfIntegral<T> operator*(Half lhs, T rhs) {
    return lhs * static_cast<Half>(rhs);
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableHalfArithmeticIfFloatingPoint<T> operator*(T lhs, Half rhs) {
    return lhs * static_cast<T>(rhs);
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableHalfArithmeticIfFloatingPoint<T> operator*(Half lhs, T rhs) {
    return static_cast<T>(lhs) * rhs;
  }

  /**
   * Division operator
   *
   * @param[in] lhs left-hand side value
   * @param[in] rhs right-hand side value
   * @returns quotient
   */
  friend Half operator/(Half lhs, Half rhs);

  /**
   * @overload
   */
  template <typename T>
  friend EnableHalfArithmeticIfIntegral<T> operator/(T lhs, Half rhs) {
    return static_cast<Half>(lhs) / rhs;
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableHalfArithmeticIfIntegral<T> operator/(Half lhs, T rhs) {
    return lhs / static_cast<Half>(rhs);
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableHalfArithmeticIfFloatingPoint<T> operator/(T lhs, Half rhs) {
    return lhs / static_cast<T>(rhs);
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableHalfArithmeticIfFloatingPoint<T> operator/(Half lhs, T rhs) {
    return static_cast<T>(lhs) / rhs;
  }

  /**
   * Metafunction to enable comparison operators for Half and integer mix
   *
   * @tparam T type
   */
  template <typename T>
  using EnableHalfComparisonIfIntegral =
      std::enable_if_t<std::is_integral_v<T>, bool>;

  /**
   * Metafunction to enable comparison operators for Half and floating point mix
   *
   * @tparam T type
   */
  template <typename T>
  using EnableHalfComparisonIfFloatingPoint =
      std::enable_if_t<std::is_floating_point_v<T>, bool>;

  /**
   * Equal operator.
   *
   * @param[in] lhs Value to compare against.
   * @param[in] rhs Value to compare against.
   * @returns true if both values are equal.
   */
  template <typename T>
  friend EnableHalfComparisonIfIntegral<T> operator==(T lhs, Half rhs) {
    return static_cast<Half>(lhs) == rhs;
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableHalfComparisonIfIntegral<T> operator==(Half lhs, T rhs) {
    return lhs == static_cast<Half>(rhs);
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableHalfComparisonIfFloatingPoint<T> operator==(T lhs, Half rhs) {
    return lhs == static_cast<T>(rhs);
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableHalfComparisonIfFloatingPoint<T> operator==(Half lhs, T rhs) {
    return static_cast<T>(lhs) == rhs;
  }

  /**
   * Not equal operator.
   *
   * @param[in] lhs Value to compare against.
   * @param[in] rhs Value to compare against.
   * @returns true if values are not equal.
   */
  template <typename T>
  friend EnableHalfComparisonIfIntegral<T> operator!=(T lhs, Half rhs) {
    return static_cast<Half>(lhs) != rhs;
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableHalfComparisonIfIntegral<T> operator!=(Half lhs, T rhs) {
    return lhs != static_cast<Half>(rhs);
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableHalfComparisonIfFloatingPoint<T> operator!=(T lhs, Half rhs) {
    return lhs != static_cast<T>(rhs);
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableHalfComparisonIfFloatingPoint<T> operator!=(Half lhs, T rhs) {
    return static_cast<T>(lhs) != rhs;
  }

  /**
   * Less than operator.
   *
   * @param[in] lhs Value to compare against.
   * @param[in] rhs Value to compare against.
   * @returns true if lhs is less than rhs.
   */
  template <typename T>
  friend EnableHalfComparisonIfIntegral<T> operator<(T lhs, Half rhs) {
    return static_cast<Half>(lhs) < rhs;
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableHalfComparisonIfIntegral<T> operator<(Half lhs, T rhs) {
    return lhs < static_cast<Half>(rhs);
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableHalfComparisonIfFloatingPoint<T> operator<(T lhs, Half rhs) {
    return lhs < static_cast<T>(rhs);
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableHalfComparisonIfFloatingPoint<T> operator<(Half lhs, T rhs) {
    return static_cast<T>(lhs) < rhs;
  }

  /**
   * Greater than operator.
   *
   * @param[in] lhs Value to compare against.
   * @param[in] rhs Value to compare against.
   * @returns true if lhs is greater than rhs.
   */
  template <typename T>
  friend EnableHalfComparisonIfIntegral<T> operator>(T lhs, Half rhs) {
    return static_cast<Half>(lhs) > rhs;
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableHalfComparisonIfIntegral<T> operator>(Half lhs, T rhs) {
    return lhs > static_cast<Half>(rhs);
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableHalfComparisonIfFloatingPoint<T> operator>(T lhs, Half rhs) {
    return lhs > static_cast<T>(rhs);
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableHalfComparisonIfFloatingPoint<T> operator>(Half lhs, T rhs) {
    return static_cast<T>(lhs) > rhs;
  }

  /**
   * Less than or equal operator.
   *
   * @param[in] lhs Value to compare against.
   * @param[in] rhs Value to compare against.
   * @returns true if lhs is less than or equal to rhs.
   */
  template <typename T>
  friend EnableHalfComparisonIfIntegral<T> operator<=(T lhs, Half rhs) {
    return static_cast<Half>(lhs) <= rhs;
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableHalfComparisonIfIntegral<T> operator<=(Half lhs, T rhs) {
    return lhs <= static_cast<Half>(rhs);
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableHalfComparisonIfFloatingPoint<T> operator<=(T lhs, Half rhs) {
    return lhs <= static_cast<T>(rhs);
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableHalfComparisonIfFloatingPoint<T> operator<=(Half lhs, T rhs) {
    return static_cast<T>(lhs) <= rhs;
  }

  /**
   * Greater than or equal operator.
   *
   * @param[in] lhs Value to compare against.
   * @param[in] rhs Value to compare against.
   * @returns true if lhs is greater than or equal to rhs.
   */
  template <typename T>
  friend EnableHalfComparisonIfIntegral<T> operator>=(T lhs, Half rhs) {
    return static_cast<Half>(lhs) >= rhs;
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableHalfComparisonIfIntegral<T> operator>=(Half lhs, T rhs) {
    return lhs >= static_cast<Half>(rhs);
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableHalfComparisonIfFloatingPoint<T> operator>=(T lhs, Half rhs) {
    return lhs >= static_cast<T>(rhs);
  }

  /**
   * @overload
   */
  template <typename T>
  friend EnableHalfComparisonIfFloatingPoint<T> operator>=(Half lhs, T rhs) {
    return static_cast<T>(lhs) >= rhs;
  }

  /**
   * Convert Half to string representation.
   *
   * @param[in] value object to convert.
   */
  friend std::string to_string(const Half &value);

  /**
   * Append string representation of Half object to a stream.
   *
   * @param[in] os stream to use.
   * @param[in] value object to convert.
   * @returns used stream.
   */
  friend std::ostream &operator<<(std::ostream &os, const Half &value);

  /**
   * Checks whether the value is NaN.
   *
   * @param[in] value object to use.
   */
  friend bool isnan(Half value);

  /**
   * Checks whether the value is Inf.
   *
   * @param[in] value object to use.
   */
  friend bool isinf(Half value);

  /**
   * Checks whether the value is denormalized.
   *
   * @param[in] value object to use.
   */
  friend bool isdenorm(Half value);

  /**
   * Computes the absolute value of a Half value arg.
   *
   * @param[in] value object to use.
   * @returns absolute value.
   */
  friend Half abs(Half value);

  /**
   * Computes the square root of a Half value arg.
   *
   * @param[in] value object to use.
   * @returns square root.
   */
  friend Half sqrt(Half value);

  /**
   * Computes the next representable value of from in the direction of to.
   *
   * @param[in] from base value.
   * @param[in] to value toward which the return value is approximated.
   * @returns the next representable value.
   */
  friend Half nextafter(const Half from, const Half to);

  friend Half acos(Half value);

  friend Half acosh(Half value);

  friend Half sin(Half value);

  friend Half sinh(Half value);

  friend Half asin(Half value);

  friend Half asinh(Half value);

  friend Half tan(Half value);

  friend Half tanh(Half value);

  friend Half atan(Half value);

  friend Half atan2(Half value_a, Half value_b);

  friend Half atanh(Half value);

  friend Half cbrt(Half value);

  friend Half copysign(Half value_a, Half value_b);

  friend Half cos(Half value);

  friend Half cosh(Half value);

  friend Half fabs(Half value);

  friend Half tgamma(Half value);

  friend Half fmod(Half value_a, Half value_b);

  friend Half fmax(Half value_a, Half value_b);

  friend Half fmin(Half value_a, Half value_b);

  friend Half pow(Half value_a, Half value_b);

  friend Half pow(Half value_a, int value_b);

  friend Half remainder(Half value_a, Half value_b);

  friend Half rint(Half value);

  friend Half trunc(Half value);

  friend Half round(Half value);

  friend int ilogb(Half value);

  friend Half erfc(Half value);

  friend Half erf(Half value);

  friend Half exp(Half value);

  friend Half exp2(Half value);

  friend Half expm1(Half value);

  friend Half fdim(Half value_a, Half value_b);

  friend Half fma(Half value_a, Half value_b, Half value_c);

  friend Half ldexp(Half value_a, int value_b);

  friend Half hypot(Half value_a, Half value_b);

  friend Half lgamma(Half value);

  friend Half log(Half value);

  friend Half log2(Half value);

  friend Half log10(Half value);

  friend Half log1p(Half value);

  friend Half logb(Half value);

  friend Half floor(Half value);

  friend Half ceil(Half value);

  friend Half nearbyint(Half value);

  /**
   * Returns zero if value is denormalized. Otherwise, returns value.
   *
   * @param[in] value object to use.
   * @returns zero or value.
   */
  friend Half flush_to_zero(Half value);
};

/**
 * Alias for type name
 */
using half = Half;

template <> struct is_floating_point<Half> : std::true_type {};

template <> struct is_custom_type<Half> : std::true_type {};

} // namespace cassian

/**
 * Std namespace.
 */
namespace std {

/**
 * numeric limits specializations
 */
template <> class numeric_limits<cassian::Half> {
public:
  /**
   * std::numeric_limits is specialized
   */
  static constexpr bool is_specialized = true;

  /**
   * cassian::Half is signed
   */
  static constexpr bool is_signed = true;

  /**
   * cassian::Half is not integer
   */
  static constexpr bool is_integer = false;

  /**
   * cassian::Half is inexact
   */
  static constexpr bool is_exact = false;

  /**
   * cassian::Half has inifinity
   */
  static constexpr bool has_infinity = true;

  /**
   * cassian::Half has quiet NaN
   */
  static constexpr bool has_quiet_NaN = true;

  /**
   * cassian::Half has signaling NaN
   */
  static constexpr bool has_signaling_NaN = true;

  /**
   * cassian::Half has denormals
   */
  static constexpr bool has_denorm = true;

  /**
   * cassian::Half does not detect loss of precision as denormalization loss
   */
  static constexpr bool has_denorm_loss = false;

  /**
   * cassian::Half uses rounding to nearest even
   */
  static constexpr std::float_round_style round_style = std::round_to_nearest;

  /**
   * cassian::Half is IEC 559/IEEE 754 type
   */
  static constexpr bool is_iec559 = true;

  /**
   * cassian::Half is bounded
   */
  static constexpr bool is_bounded = true;

  /**
   * cassian::Half does not handle overflows with modulo arithmetic
   */
  static constexpr bool is_modulo = false;

  /**
   * number of radix digits
   */
  static constexpr int digits = 11;

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
  static constexpr int min_exponent = -13;

  /**
   * the smallest negative power of ten that is a valid normalized
   * floating-point value
   */
  static constexpr int min_exponent10 = -5;

  /**
   * one more than the largest integer power of the radix that is a valid finite
   * floating-point value
   */
  static constexpr int max_exponent = 15;

  /**
   * the largest integer power of 10 that is a valid finite floating-point value
   */
  static constexpr int max_exponent10 = 5;

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
  static constexpr cassian::Half min() {
    return cassian::Half::encode(0x0400);
  };
  /**
   * maximum value
   */
  static constexpr cassian::Half max() {
    return cassian::Half::encode(0x7bff);
  };
  /**
   * lowest value
   */
  static constexpr cassian::Half lowest() {
    return cassian::Half::encode(0xfbff);
  };
  /**
   * difference between 1.0 and the next representable value
   */
  static constexpr cassian::Half epsilon() {
    // cassian::Half::encode(0x3c01) - cassian::Half::encode(0x3c00);
    return cassian::Half::encode(0x1400);
  }
  /**
   * maximum rounding error
   */
  static constexpr cassian::Half round_error() { // cassian::Half(0.5f)
    return cassian::Half::encode(0x3800);
  }
  /**
   * infinity value
   */
  static constexpr cassian::Half infinity() {
    return cassian::Half::encode(0x7c00);
  };
  /**
   * quiet nan value
   */
  // specific quiet and signaling NaN encoding is architecture dependent
  static constexpr cassian::Half quiet_NaN() {
    return cassian::Half::encode(0x7c01);
  };
  /**
   * signaling nan value
   */
  static constexpr cassian::Half signaling_NaN() {
    // in F16C x86 extension f32 sNaN is converted to f16 qNaN
    // ref: Intel 64 and IA-32 Architectures Software Developer's Manual
    // Volume 1, order number 325462-069US, table 14-9
    return quiet_NaN();
  };
  /**
   * lowest denorm value
   */
  static constexpr cassian::Half denorm_min() {
    return cassian::Half::encode(0x0001);
  };
};

} // namespace std
#endif
