/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_OCLC_MATH_FUNCTIONS_MATH_FUNCTIONS_HPP
#define CASSIAN_OCLC_MATH_FUNCTIONS_MATH_FUNCTIONS_HPP

#include <cassian/utility/utility.hpp>
#include <catch2/catch.hpp>
#include <cmath>
#include <common.hpp>
#include <enum_definitions.hpp>
#include <vector>

CASSIAN_CATCH_REGISTER_MATH_FUNCTIONS;

template <typename T, typename INPUT_TYPE, cassian::EnableIfIsVector<T> = 0>
T get_reference_value(const std::vector<T> &input_values,
                      const Function &function) {
  T c;
  for (auto i = 0; i < c.size(); i++) {
    if (input_values.size() == 2) {
      c[i] = get_reference_value<INPUT_TYPE, INPUT_TYPE>(
          {input_values[0][i], input_values[1][i]}, function);
    } else {
      c[i] = get_reference_value<INPUT_TYPE, INPUT_TYPE>({input_values[0][i]},
                                                         function);
    }
  }
  return c;
}

template <typename T, typename INPUT_TYPE,
          cassian::EnableIfIsFloatingPoint<T> = 0>
T get_reference_value(const std::vector<T> &input_values,
                      const Function &function) {
  switch (function) {
  case Function::acos:
    return std::acos(input_values[0]);
  case Function::acosh:
    return std::acosh(input_values[0]);
  case Function::acospi:
    return std::acos(input_values[0]) / static_cast<T>(M_PI);
  case Function::asin:
    return std::asin(input_values[0]);
  case Function::asinh:
    return std::asinh(input_values[0]);
  case Function::asinpi:
    return std::asin(input_values[0]) / static_cast<T>(M_PI);
  case Function::atan:
    return std::atan(input_values[0]);
  case Function::atan2:
    return std::atan2(input_values[0], input_values[1]);
  case Function::atan2pi:
    return std::atan2(input_values[0], input_values[1]) / static_cast<T>(M_PI);
  case Function::atanh:
    return std::atanh(input_values[0]);
  case Function::atanpi:
    return std::atan(input_values[0]) / static_cast<T>(M_PI);
  case Function::cbrt:
    return std::cbrt(input_values[0]);
  case Function::ceil:
    return std::ceil(input_values[0]);
  case Function::copysign:
    return std::copysign(input_values[0], input_values[1]);
  default:
    throw UnknownFunctionException(
        "get_reference_value for Function: " +
        Catch::StringMaker<Function>::convert(function) +
        " is not implemented");
  }
};

template <typename T, typename scalar_type>
std::vector<std::vector<T>> get_input_values(Function function) {
  switch (function) {
  case Function::acos:
  case Function::asin: {
    return {{generate_value<T>(-1, 1)}};
  }
  case Function::asinpi:
    return {{generate_value<T>(-1, 1)}, {T(0)}, {T(2)}};
  case Function::acosh:
    return {{generate_value<T>(-1, std::numeric_limits<scalar_type>::max())}};
  case Function::acospi:
    return {{generate_value<T>(-1, std::numeric_limits<scalar_type>::max())},
            {T(1)},
            {T(2)}};
  case Function::asinh:
  case Function::atan:
  case Function::cbrt:
    return {{generate_value<T>()}};
  case Function::ceil:
    return {{generate_value<T>()}, {generate_value<T>(-1, 0, {-1, 0})}};
  case Function::atanpi:
    return {{generate_value<T>()},
            {T(0)},
            {T(std::numeric_limits<scalar_type>::infinity())}};
  case Function::atan2:
    return {{generate_value<T>(), generate_value<T>()}};
  case Function::atan2pi: {
    const auto lower_than_zero =
        generate_value<T>(-std::numeric_limits<scalar_type>::max(),
                          scalar_type(0), {scalar_type(0)});
    const auto higher_than_zero =
        generate_value<T>(0, std::numeric_limits<scalar_type>::max(), {0});
    const auto infinity = T(std::numeric_limits<scalar_type>::infinity());
    return {{generate_value<T>(), generate_value<T>()},
            {T(0), T(0.0F)},
            {T(0), T(-0.0F)},
            {T(0), lower_than_zero},
            {T(0), higher_than_zero},
            {lower_than_zero, T(0)},
            {higher_than_zero, T(0)},
            {higher_than_zero, -infinity},
            {higher_than_zero, infinity},
            {infinity, generate_value<T>()},
            {infinity, -infinity},
            {infinity, infinity}};
  }
  case Function::copysign:
    return {{generate_value<T>(), generate_value<T>()}, {T(-1), T(1)}};
  case Function::atanh:
    return {{generate_value<T>(-1, 1, {-1, 1})}};
  default:
    throw UnknownFunctionException(
        "get_input_values for Function: " +
        Catch::StringMaker<Function>::convert(function) +
        " is not implemented");
  }
}

template <typename T, typename cassian::EnableIfIsFloatingPoint<T> = 0>
T get_ulp_value(const Function &function) {
  switch (function) {
  case Function::acos:
  case Function::acosh:
  case Function::asin:
  case Function::asinh:
    return static_cast<T>(4);
  case Function::acospi:
  case Function::asinpi:
  case Function::atan:
  case Function::atanh:
  case Function::atanpi:
    return static_cast<T>(5);
  case Function::atan2:
  case Function::atan2pi:
    return static_cast<T>(6);
  case Function::cbrt:
    return static_cast<T>(2);
  case Function::ceil:
  case Function::copysign:
    return static_cast<T>(0);
  default:
    throw UnknownFunctionException(
        "get_ulp_value for Function: " +
        Catch::StringMaker<Function>::convert(function) +
        " is not implemented");
  }
}

template <typename T, typename cassian::EnableIfIsVector<T> = 0>
bool match_results(const T &result, const T &reference,
                   const Function &function) {
  for (auto i = 0; i < result.size(); i++) {
    if (!match_results(result[i], reference[i], function)) {
      return false;
    }
  }
  return true;
}

template <typename T, typename cassian::EnableIfIsFloatingPoint<T> = 0>
bool match_results(const T &result, const T &reference,
                   const Function &function) {
  using BITS_TYPE = typename std::conditional<
      sizeof(T) == sizeof(uint32_t), uint32_t,
      typename std::conditional<sizeof(T) == sizeof(uint64_t), uint64_t,
                                uint16_t>::type>::type;

  const auto ulp_value = get_ulp_value<T>(function);
  const auto ulp_dist = ulp_distance<T, BITS_TYPE>(result, reference);
  if (std::isnan(result) && std::isnan(reference)) {
    return true;
  }
  return ulp_dist <= ulp_value;
}

template <typename T>
class UlpComparator : public Catch::MatcherBase<std::vector<T>> {
  T reference;
  Function function;

public:
  UlpComparator(const T &reference, const Function &function)
      : reference(reference), function(function) {}

  bool match(std::vector<T> const &result) const override {
    for (const auto &res : result) {
      if (!match_results(res, reference, function)) {
        return false;
      }
    }
    return true;
  }
  std::string describe() const override {
    std::ostringstream ss;
    ss << "each value in vector should be equal to: "
       << cassian::to_string(reference);
    return ss.str();
  }
};

#endif