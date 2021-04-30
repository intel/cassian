/*
 * Copyright (C) 2021 Intel Corporation
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
  constexpr Half() : data(0){};

  /**
   * Constructor from float
   *
   * @param[in] v value to be converted to half
   */
  explicit Half(float v);

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
};

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
Half operator+(Half lhs, Half rhs);

/**
 * @overload
 */
template <typename T>
EnableHalfArithmeticIfIntegral<T> operator+(T lhs, Half rhs) {
  return static_cast<Half>(lhs) + rhs;
}

/**
 * @overload
 */
template <typename T>
EnableHalfArithmeticIfIntegral<T> operator+(Half lhs, T rhs) {
  return lhs + static_cast<Half>(rhs);
}

/**
 * @overload
 */
template <typename T>
EnableHalfArithmeticIfFloatingPoint<T> operator+(T lhs, Half rhs) {
  return lhs + static_cast<T>(rhs);
}

/**
 * @overload
 */
template <typename T>
EnableHalfArithmeticIfFloatingPoint<T> operator+(Half lhs, T rhs) {
  return static_cast<T>(lhs) + rhs;
}

/**
 * Subtruction operator
 *
 * @param[in] lhs left-hand side value
 * @param[in] rhs right-hand side value
 * @returns difference between lhs and rhs
 */
Half operator-(Half lhs, Half rhs);

/**
 * @overload
 */
template <typename T>
EnableHalfArithmeticIfIntegral<T> operator-(T lhs, Half rhs) {
  return static_cast<Half>(lhs) - rhs;
}

/**
 * @overload
 */
template <typename T>
EnableHalfArithmeticIfIntegral<T> operator-(Half lhs, T rhs) {
  return lhs - static_cast<Half>(rhs);
}

/**
 * @overload
 */
template <typename T>
EnableHalfArithmeticIfFloatingPoint<T> operator-(T lhs, Half rhs) {
  return lhs - static_cast<T>(rhs);
}

/**
 * @overload
 */
template <typename T>
EnableHalfArithmeticIfFloatingPoint<T> operator-(Half lhs, T rhs) {
  return static_cast<T>(lhs) - rhs;
}

/**
 * Multiplication operator
 *
 * @param[in] lhs left-hand side value
 * @param[in] rhs right-hand side value
 * @returns product of lhs and rhs
 */
Half operator*(Half lhs, Half rhs);

/**
 * @overload
 */
template <typename T>
EnableHalfArithmeticIfIntegral<T> operator*(T lhs, Half rhs) {
  return static_cast<Half>(lhs) * rhs;
}

/**
 * @overload
 */
template <typename T>
EnableHalfArithmeticIfIntegral<T> operator*(Half lhs, T rhs) {
  return lhs * static_cast<Half>(rhs);
}

/**
 * @overload
 */
template <typename T>
EnableHalfArithmeticIfFloatingPoint<T> operator*(T lhs, Half rhs) {
  return lhs * static_cast<T>(rhs);
}

/**
 * @overload
 */
template <typename T>
EnableHalfArithmeticIfFloatingPoint<T> operator*(Half lhs, T rhs) {
  return static_cast<T>(lhs) * rhs;
}

/**
 * Division operator
 *
 * @param[in] lhs left-hand side value
 * @param[in] rhs right-hand side value
 * @returns quotient
 */
Half operator/(Half lhs, Half rhs);

/**
 * @overload
 */
template <typename T>
EnableHalfArithmeticIfIntegral<T> operator/(T lhs, Half rhs) {
  return static_cast<Half>(lhs) / rhs;
}

/**
 * @overload
 */
template <typename T>
EnableHalfArithmeticIfIntegral<T> operator/(Half lhs, T rhs) {
  return lhs / static_cast<Half>(rhs);
}

/**
 * @overload
 */
template <typename T>
EnableHalfArithmeticIfFloatingPoint<T> operator/(T lhs, Half rhs) {
  return lhs / static_cast<T>(rhs);
}

/**
 * @overload
 */
template <typename T>
EnableHalfArithmeticIfFloatingPoint<T> operator/(Half lhs, T rhs) {
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
EnableHalfComparisonIfIntegral<T> operator==(T lhs, Half rhs) {
  return static_cast<Half>(lhs) == rhs;
}

/**
 * @overload
 */
template <typename T>
EnableHalfComparisonIfIntegral<T> operator==(Half lhs, T rhs) {
  return lhs == static_cast<Half>(rhs);
}

/**
 * @overload
 */
template <typename T>
EnableHalfComparisonIfFloatingPoint<T> operator==(T lhs, Half rhs) {
  return lhs == static_cast<T>(rhs);
}

/**
 * @overload
 */
template <typename T>
EnableHalfComparisonIfFloatingPoint<T> operator==(Half lhs, T rhs) {
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
EnableHalfComparisonIfIntegral<T> operator!=(T lhs, Half rhs) {
  return static_cast<Half>(lhs) != rhs;
}

/**
 * @overload
 */
template <typename T>
EnableHalfComparisonIfIntegral<T> operator!=(Half lhs, T rhs) {
  return lhs != static_cast<Half>(rhs);
}

/**
 * @overload
 */
template <typename T>
EnableHalfComparisonIfFloatingPoint<T> operator!=(T lhs, Half rhs) {
  return lhs != static_cast<T>(rhs);
}

/**
 * @overload
 */
template <typename T>
EnableHalfComparisonIfFloatingPoint<T> operator!=(Half lhs, T rhs) {
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
EnableHalfComparisonIfIntegral<T> operator<(T lhs, Half rhs) {
  return static_cast<Half>(lhs) < rhs;
}

/**
 * @overload
 */
template <typename T>
EnableHalfComparisonIfIntegral<T> operator<(Half lhs, T rhs) {
  return lhs < static_cast<Half>(rhs);
}

/**
 * @overload
 */
template <typename T>
EnableHalfComparisonIfFloatingPoint<T> operator<(T lhs, Half rhs) {
  return lhs < static_cast<T>(rhs);
}

/**
 * @overload
 */
template <typename T>
EnableHalfComparisonIfFloatingPoint<T> operator<(Half lhs, T rhs) {
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
EnableHalfComparisonIfIntegral<T> operator>(T lhs, Half rhs) {
  return static_cast<Half>(lhs) > rhs;
}

/**
 * @overload
 */
template <typename T>
EnableHalfComparisonIfIntegral<T> operator>(Half lhs, T rhs) {
  return lhs > static_cast<Half>(rhs);
}

/**
 * @overload
 */
template <typename T>
EnableHalfComparisonIfFloatingPoint<T> operator>(T lhs, Half rhs) {
  return lhs > static_cast<T>(rhs);
}

/**
 * @overload
 */
template <typename T>
EnableHalfComparisonIfFloatingPoint<T> operator>(Half lhs, T rhs) {
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
EnableHalfComparisonIfIntegral<T> operator<=(T lhs, Half rhs) {
  return static_cast<Half>(lhs) <= rhs;
}

/**
 * @overload
 */
template <typename T>
EnableHalfComparisonIfIntegral<T> operator<=(Half lhs, T rhs) {
  return lhs <= static_cast<Half>(rhs);
}

/**
 * @overload
 */
template <typename T>
EnableHalfComparisonIfFloatingPoint<T> operator<=(T lhs, Half rhs) {
  return lhs <= static_cast<T>(rhs);
}

/**
 * @overload
 */
template <typename T>
EnableHalfComparisonIfFloatingPoint<T> operator<=(Half lhs, T rhs) {
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
EnableHalfComparisonIfIntegral<T> operator>=(T lhs, Half rhs) {
  return static_cast<Half>(lhs) >= rhs;
}

/**
 * @overload
 */
template <typename T>
EnableHalfComparisonIfIntegral<T> operator>=(Half lhs, T rhs) {
  return lhs >= static_cast<Half>(rhs);
}

/**
 * @overload
 */
template <typename T>
EnableHalfComparisonIfFloatingPoint<T> operator>=(T lhs, Half rhs) {
  return lhs >= static_cast<T>(rhs);
}

/**
 * @overload
 */
template <typename T>
EnableHalfComparisonIfFloatingPoint<T> operator>=(Half lhs, T rhs) {
  return static_cast<T>(lhs) >= rhs;
}

/**
 * Alias for type name
 */
using half = Half;

template <> struct is_floating_point<Half> : std::true_type {};

/**
 * Convert Half to string representation.
 *
 * @param[in] value object to convert.
 */
std::string to_string(const Half &value);

/**
 * Append string representation of Half object to a stream.
 *
 * @param[in] os stream to use.
 * @param[in] value object to convert.
 * @returns used stream.
 */
std::ostream &operator<<(std::ostream &os, const Half &value);

/**
 * Computes the absolute value of a Half value arg.
 *
 * @param[in] value object to use.
 * @returns absolute value.
 */
Half abs(Half value);

/**
 * Computes the square root of a Half value arg.
 *
 * @param[in] value object to use.
 * @returns square root.
 */
Half sqrt(Half value);

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
   * cassian::Half is IEC 559/IEEE 745 type
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
  static constexpr int digits = 10;

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
  static constexpr int min_exponent = -14;

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
  static cassian::Half epsilon() {
    // cassian::Half::encode(0x3c01) - cassian::Half::encode(0x3c00);
    return cassian::Half::encode(0x1400);
  }
  /**
   * maximum rounding error
   */
  static cassian::Half round_error() { return cassian::Half(0.5f); }
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
