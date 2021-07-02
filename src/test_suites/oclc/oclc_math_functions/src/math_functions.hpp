/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_OCLC_MATH_FUNCTIONS_MATH_FUNCTIONS_HPP
#define CASSIAN_OCLC_MATH_FUNCTIONS_MATH_FUNCTIONS_HPP

#include <cassian/vector/vector.hpp>
#include <catch2/catch.hpp>
#include <cmath>
#include <common.hpp>
#include <cstddef>
#include <enum_definitions.hpp>
#include <limits>
#include <math_input_values.hpp>
#include <ostream>
#include <string>
#include <vector>

CASSIAN_CATCH_REGISTER_MATH_FUNCTIONS;

template <auto FUNCTION, auto ARG_NUM, auto REFERENCE_FUNC,
          typename OUTPUT_TYPE, typename INPUT_TYPE_1 = OUTPUT_TYPE,
          typename INPUT_TYPE_2 = INPUT_TYPE_1,
          typename INPUT_TYPE_3 = INPUT_TYPE_2>
class OclcFunction {
public:
  using output_type = typename OUTPUT_TYPE::host_type;
  using input_type_1 = typename INPUT_TYPE_1::host_type;
  using input_type_2 = typename INPUT_TYPE_2::host_type;
  using input_type_3 = typename INPUT_TYPE_3::host_type;
  using scalar_type_1 = typename INPUT_TYPE_1::scalar_type::host_type;
  using scalar_type_2 = typename INPUT_TYPE_1::scalar_type::host_type;

  static constexpr auto function = FUNCTION;
  static constexpr auto arg_num = ARG_NUM;
  static constexpr auto calculate_reference = REFERENCE_FUNC;

private:
  const std::string function_string_ =
      Catch::StringMaker<Function>::convert(function);

public:
  OclcFunction() = default;
  constexpr auto get_arg_num() const { return arg_num; }
  constexpr auto get_function() const { return function; }
  auto get_function_string() const { return function_string_; }
  auto get_build_options() const {
    std::stringstream ss;
    ss << "-DOUTPUT_TYPE=" << OUTPUT_TYPE::device_type;
    ss << " -DINPUT_TYPE_1=" << INPUT_TYPE_1::device_type;
    if constexpr (arg_num == 2UL) {
      ss << " -DTWO_ARG_FUNCTION";
      ss << " -DINPUT_TYPE_2=" << INPUT_TYPE_2::device_type;
    } else if constexpr (arg_num == 3UL) {
      ss << " -DTHREE_ARG_FUNCTION";
      ss << " -DINPUT_TYPE_2=" << INPUT_TYPE_2::device_type;
      ss << " -DINPUT_TYPE_3=" << INPUT_TYPE_3::device_type;
    }
    ss << " -DFUNCTION=" << function_string_;
    return ss.str();
  }
};

template <class T> constexpr auto get_gentype_values() {
  using input_type_1 = typename T::input_type_1;
  using input_type_2 = typename T::input_type_2;
  using input_type_3 = typename T::input_type_3;
  using scalar_type_1 = typename T::scalar_type_1;
  using scalar_type_2 = typename T::scalar_type_2;

  MathInputValues<T> values = {};
  if constexpr (T::function == Function::acos) {
    values.add_random_case(generate_value<input_type_1>(-1, 1));
  } else if constexpr (T::function == Function::asinpi) {
    values.add_random_case(generate_value<input_type_1>(-1, 1));
    values.add_edge_case(input_type_1(0.0F));
    values.add_edge_case(input_type_1(2.0F));
  } else if constexpr (T::function == Function::acosh) {
    values.add_random_case(generate_value<input_type_1>(
        1, std::numeric_limits<scalar_type_1>::max()));
  } else if constexpr (T::function == Function::acospi) {
    values.add_random_case(generate_value<input_type_1>(-1, 1));
    values.add_edge_case(input_type_1(1));
    values.add_edge_case(input_type_1(2));
  } else if constexpr (T::function == Function::cospi) {
    values.add_random_case(generate_value<input_type_1>());
    values.add_edge_case(input_type_1(0));
    values.add_edge_case(input_type_1(1.5F));
    values.add_edge_case(
        input_type_1(std::numeric_limits<scalar_type_1>::infinity()));
  } else if constexpr (T::function == Function::ceil) {
    values.add_random_case(generate_value<input_type_1>());
    values.add_edge_case(generate_value<input_type_1>(-1, 0, {-1, 0}));
  } else if constexpr (T::function == Function::atanpi) {
    values.add_random_case(generate_value<input_type_1>());
    values.add_edge_case(input_type_1(0));
    values.add_edge_case(
        input_type_1(std::numeric_limits<input_type_1>::infinity()));
  } else if constexpr (T::function == Function::atan2) {
    values.add_random_case(generate_value<input_type_1>(),
                           generate_value<input_type_2>());
  } else if constexpr (T::function == Function::atan2pi) {
    static_assert(std::is_same_v<input_type_1, input_type_2>);
    const auto lower_than_zero =
        generate_value<input_type_1>(-std::numeric_limits<scalar_type_1>::max(),
                                     scalar_type_1(0), {scalar_type_1(0)});
    const auto higher_than_zero = generate_value<input_type_1>(
        scalar_type_1(0), std::numeric_limits<scalar_type_1>::max(),
        {scalar_type_1(0)});
    const auto infinity =
        input_type_1(std::numeric_limits<scalar_type_1>::infinity());
    values.add_random_case(generate_value<input_type_1>(),
                           generate_value<input_type_2>());
    values.add_edge_case(input_type_1(0), input_type_2(0.0F));
    values.add_edge_case(input_type_1(0), input_type_2(-0.0F));
    values.add_edge_case(input_type_1(0), lower_than_zero);
    values.add_edge_case(input_type_1(0), higher_than_zero);
    values.add_edge_case(lower_than_zero, input_type_2(0));
    values.add_edge_case(higher_than_zero, input_type_2(0));
    values.add_edge_case(higher_than_zero, -infinity);
    values.add_edge_case(higher_than_zero, infinity);
    values.add_edge_case(infinity, generate_value<input_type_2>());
    values.add_edge_case(infinity, -infinity);
    values.add_edge_case(infinity, infinity);
  } else if constexpr (T::function == Function::copysign) {
    values.add_random_case(generate_value<input_type_1>(),
                           generate_value<input_type_2>());
  } else if constexpr (T::function == Function::atanh) {
    values.add_random_case(generate_value<input_type_1>(-1, 1, {-1, 1}));
  } else if constexpr (T::function == Function::sqrt) {
    values.add_random_case(generate_value<input_type_1>(
        0, std::numeric_limits<scalar_type_1>::max()));
  } else if constexpr (T::function == Function::fma ||
                       T::function == Function::mad) {
    values.add_random_case(generate_value<input_type_1>(),
                           generate_value<input_type_2>(),
                           generate_value<input_type_3>());
  } else if constexpr (T::function == Function::fdim) {
    values.add_random_case(generate_value<input_type_1>(),
                           generate_value<input_type_2>());
    values.add_edge_case(
        generate_value<input_type_1>(),
        input_type_2(std::numeric_limits<scalar_type_2>::quiet_NaN()));
    values.add_edge_case(
        input_type_1(std::numeric_limits<scalar_type_1>::quiet_NaN()),
        generate_value<input_type_2>());
  } else if constexpr (T::function == Function::fmod) {
    values.add_random_case(generate_value<input_type_1>(),
                           generate_value<input_type_2>());
    values.add_edge_case(
        input_type_1(0.0F),
        input_type_2(std::numeric_limits<scalar_type_2>::quiet_NaN()));
  } else {
    if constexpr (T::arg_num == 2) {
      values.add_random_case(generate_value<input_type_1>(),
                             generate_value<input_type_2>());
    } else {
      values.add_random_case(generate_value<input_type_1>());
    }
  }
  return values;
}

template <typename T>
std::vector<cassian::scalar_type_v<T>> get_ulp_values(const Function &function,
                                                      const size_t &work_size) {
  using scalar_type = cassian::scalar_type_v<T>;
  constexpr auto epsilon = std::numeric_limits<scalar_type>::epsilon();
  switch (function) {
  case Function::ceil:
  case Function::copysign:
  case Function::ilogb:
  case Function::mad:
  case Function::fabs:
  case Function::fdim:
  case Function::floor:
  case Function::fma:
  case Function::fmax:
  case Function::fmin:
  case Function::fmod:
  case Function::ldexp:
  case Function::logb:
    return std::vector<scalar_type>(work_size, 0.0F);
  case Function::log1p:
  case Function::cbrt: {
    constexpr scalar_type ulp = 2.0F * epsilon;
    std::vector<scalar_type> ulp_values(work_size, ulp);
    return ulp_values;
  }
  case Function::exp:
  case Function::exp2:
  case Function::expm1:
  case Function::sqrt:
  case Function::log10:
  case Function::log:
  case Function::log2: {
    constexpr scalar_type ulp = 3.0F * epsilon;
    std::vector<scalar_type> ulp_values(work_size, ulp);
    return ulp_values;
  }
  case Function::acos:
  case Function::acosh:
  case Function::asin:
  case Function::asinh:
  case Function::cos:
  case Function::cosh:
  case Function::cospi:
  case Function::hypot: {
    constexpr scalar_type ulp = 4.0F * epsilon;
    std::vector<scalar_type> ulp_values(work_size, ulp);
    return ulp_values;
  }
  case Function::acospi:
  case Function::asinpi:
  case Function::atan:
  case Function::atanh:
  case Function::atanpi: {
    constexpr scalar_type ulp = 5.0F * epsilon;
    std::vector<scalar_type> ulp_values(work_size, ulp);
    return ulp_values;
  }
  case Function::atan2:
  case Function::atan2pi: {
    constexpr scalar_type ulp = 6.0F * epsilon;
    std::vector<scalar_type> ulp_values(work_size, ulp);
    return ulp_values;
  }
  case Function::erf:
  case Function::erfc: {
    constexpr scalar_type ulp = 16.0F * epsilon;
    std::vector<scalar_type> ulp_values(work_size, ulp);
    return ulp_values;
  }
  default:
    throw UnknownFunctionException(
        Catch::StringMaker<Function>::convert(function) + " uninitialized");
  }
}
template <typename T, typename cassian::EnableIfIsScalar<T> = 0>
bool match_results(const T &result, const T &reference, const T ulp_value) {
  if (std::isnan(result) && std::isnan(reference)) {
    return true;
  }
  const auto ulp_dist = ulp_distance(result, reference);
  return ulp_dist <= std::fabs(result + reference) * ulp_value ||
         ulp_dist < std::numeric_limits<T>::min();
}
template <typename T, typename cassian::EnableIfIsVector<T> = 0>
bool match_results(const T &result, const T &reference,
                   const cassian::scalar_type_v<T> ulp_value) {
  for (auto i = 0UL; i < result.size(); i++) {
    if (!match_results(result[i], reference[i], ulp_value)) {
      return false;
    }
  }
  return true;
}

template <class OUTPUT_TYPE>
class UlpComparator : public Catch::MatcherBase<std::vector<OUTPUT_TYPE>> {
  using scalar_type = cassian::scalar_type_v<OUTPUT_TYPE>;
  std::vector<OUTPUT_TYPE> reference;
  std::vector<scalar_type> ulp_values;

public:
  UlpComparator(const std::vector<OUTPUT_TYPE> &reference,
                const size_t &work_size, const Function &function)
      : reference(reference) {
    ulp_values = get_ulp_values<OUTPUT_TYPE>(function, work_size);
  }

  bool match(std::vector<OUTPUT_TYPE> const &result) const override {
    for (auto i = 0U; i < result.size(); i++) {
      if (!match_results(result[i], reference[i], ulp_values[i])) {
        return false;
      }
    }
    return true;
  }
  std::string describe() const override {
    return "\nreference: " + input_to_string<OUTPUT_TYPE>(reference);
  }
};

#endif