/*
 * Copyright (C) 2021-2022 Intel Corporation
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
#include <type_traits>
#include <vector>

CASSIAN_CATCH_REGISTER_MATH_FUNCTIONS;

template <auto FUNCTION, auto ARG_NUM, auto REFERENCE_FUNC,
          typename OUTPUT_TYPE, auto ADDRESS_SPACE = AddressSpace::clc_global,
          typename INPUT_TYPE_1 = OUTPUT_TYPE,
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
  static constexpr auto address_space = ADDRESS_SPACE;

private:
  const std::string function_string_ =
      Catch::StringMaker<Function>::convert(function);

public:
  OclcFunction() = default;
  constexpr auto get_arg_num() const { return arg_num; }
  constexpr auto get_function() const { return function; }
  constexpr auto get_address_space() const {
    switch (address_space) {
    case AddressSpace::clc_global:
      return "global";
    case AddressSpace::clc_local:
      return "local";
    case AddressSpace::clc_private:
      return "private";
    case AddressSpace::clc_constant:
      return "constant";
    case AddressSpace::clc_generic:
      return "generic";
    default:
      throw UnknownFunctionException(
          Catch::StringMaker<Function>::convert(function) +
          "address space not uninitialized");
    }
  }
  auto get_function_string() const { return function_string_; }
  auto get_is_store() const {
    switch (function) {
    case Function::fract:
    case Function::frexp:
    case Function::lgamma_r:
    case Function::remquo:
    case Function::modf:
    case Function::sincos:
      return true;
    default:
      return false;
    }
  }

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
    ss << " -DADDRESS_SPACE=" << get_address_space();
    ss << " -DFUNCTION=" << get_function_string();
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
    const auto higher_than_one = generate_value<input_type_1>(
        scalar_type_1(1.0), std::numeric_limits<scalar_type_1>::max());
    values.add_random_case(generate_value<input_type_1>(-1, 1));
    values.add_edge_case(input_type_1(0.0F));
    values.add_edge_case(input_type_1(-0.0F));
    values.add_edge_case(higher_than_one);
  } else if constexpr (T::function == Function::acosh) {
    values.add_random_case(generate_value<input_type_1>(
        1, std::numeric_limits<scalar_type_1>::max()));
  } else if constexpr (T::function == Function::acospi) {
    const auto higher_than_one = generate_value<input_type_1>(
        scalar_type_1(1.0), std::numeric_limits<scalar_type_1>::max(),
        {scalar_type_1(1.0)});
    values.add_random_case(generate_value<input_type_1>(-1, 1));
    values.add_edge_case(input_type_1(1));
    values.add_edge_case(higher_than_one);
  } else if constexpr (T::function == Function::cospi) {
    values.add_random_case(generate_value<input_type_1>());
    values.add_edge_case(input_type_1(0));
    values.add_edge_case(input_type_1(-0));
    values.add_edge_case(
        input_type_1(std::ceil(generate_value<scalar_type_1>()) + 0.5F));
    values.add_edge_case(
        input_type_1(std::numeric_limits<scalar_type_1>::infinity()));
  } else if constexpr (T::function == Function::ceil) {
    values.add_random_case(generate_value<input_type_1>());
    values.add_edge_case(generate_value<input_type_1>(-1, 0, {-1, 0}));
  } else if constexpr (T::function == Function::atanpi) {
    values.add_random_case(generate_value<input_type_1>());
    values.add_edge_case(input_type_1(0));
    values.add_edge_case(input_type_1(-0));
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
    values.add_edge_case(input_type_1(-0), input_type_2(0.0F));
    values.add_edge_case(input_type_1(-0), input_type_2(-0.0F));
    values.add_edge_case(input_type_1(-0), lower_than_zero);
    values.add_edge_case(input_type_1(-0), higher_than_zero);
    values.add_edge_case(lower_than_zero, input_type_2(0));
    values.add_edge_case(higher_than_zero, input_type_2(0));
    values.add_edge_case(lower_than_zero, input_type_2(-0));
    values.add_edge_case(higher_than_zero, input_type_2(-0));
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
  } else if constexpr (T::function == Function::sqrt ||
                       T::function == Function::half_sqrt) {
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
  } else if constexpr (T::function == Function::remquo) {
    const auto infinity =
        input_type_1(std::numeric_limits<scalar_type_1>::infinity());
    values.add_random_case(generate_value<input_type_1>(),
                           generate_value<input_type_2>(),
                           generate_value<input_type_3>());
    values.add_edge_case(-infinity, generate_value<input_type_2>(),
                         generate_value<input_type_3>());
    values.add_edge_case(generate_value<input_type_1>(), input_type_2(0),
                         generate_value<input_type_3>());
    values.add_edge_case(
        generate_value<input_type_1>(),
        input_type_2(std::numeric_limits<scalar_type_2>::quiet_NaN()),
        generate_value<input_type_3>());
    values.add_edge_case(
        input_type_1(std::numeric_limits<scalar_type_1>::quiet_NaN()),
        generate_value<input_type_2>(), generate_value<input_type_3>());
  } else if constexpr (T::function == Function::fmod) {
    values.add_random_case(generate_value<input_type_1>(),
                           generate_value<input_type_2>());
    values.add_edge_case(
        input_type_1(0.0F),
        input_type_2(std::numeric_limits<scalar_type_2>::quiet_NaN()));
    values.add_edge_case(
        input_type_1(-0.0F),
        input_type_2(std::numeric_limits<scalar_type_2>::quiet_NaN()));
  } else if constexpr (T::function == Function::exp10 ||
                       T::function == Function::half_exp10) {
    const auto infinity =
        input_type_1(std::numeric_limits<scalar_type_1>::infinity());
    values.add_random_case(generate_value<input_type_1>());
    values.add_edge_case(-infinity);
    values.add_edge_case(infinity);
  } else if constexpr (T::function == Function::nextafter) {
    const auto higher_than_zero = generate_value<input_type_1>(
        scalar_type_1(0), std::numeric_limits<scalar_type_1>::max(),
        {scalar_type_1(0)});
    const auto lower_than_zero =
        generate_value<input_type_1>(-std::numeric_limits<scalar_type_1>::max(),
                                     scalar_type_1(0), {scalar_type_1(0)});
    values.add_random_case(generate_value<input_type_1>(),
                           generate_value<input_type_2>());
    values.add_edge_case(input_type_1(0.0F), higher_than_zero);
    values.add_edge_case(input_type_1(0.0F), lower_than_zero);
    values.add_edge_case(input_type_1(-0.0F), higher_than_zero);
    values.add_edge_case(input_type_1(-0.0F), lower_than_zero);
  } else if constexpr (T::function == Function::pow) {
    const auto infinity =
        input_type_1(std::numeric_limits<scalar_type_1>::infinity());
    values.add_random_case(generate_value<input_type_1>());
    values.add_edge_case(input_type_1(0.0F), -infinity);
    values.add_edge_case(input_type_1(-0.0F), -infinity);
  } else if constexpr (T::function == Function::powr ||
                       T::function == Function::half_powr) {
    const auto higher_than_zero = generate_value<input_type_1>(
        scalar_type_1(0), std::numeric_limits<scalar_type_1>::max(),
        {scalar_type_1(0)});
    const auto lower_than_zero =
        generate_value<input_type_1>(std::numeric_limits<scalar_type_1>::min(),
                                     scalar_type_1(0), {scalar_type_1(0)});
    const auto infinity =
        input_type_1(std::numeric_limits<scalar_type_1>::infinity());
    const auto finite = generate_value<input_type_2>(
        -std::numeric_limits<scalar_type_2>::max(),
        std::numeric_limits<scalar_type_2>::max(),
        {std::numeric_limits<scalar_type_2>::infinity()});
    values.add_random_case(generate_value<input_type_1>(),
                           generate_value<input_type_2>());
    values.add_edge_case(
        generate_value<input_type_1>(
            scalar_type_1(0), std::numeric_limits<scalar_type_1>::infinity(),
            {scalar_type_1(0), std::numeric_limits<scalar_type_1>::infinity()}),
        input_type_2(0.0F));
    values.add_edge_case(
        generate_value<input_type_1>(
            scalar_type_1(0), std::numeric_limits<scalar_type_1>::infinity(),
            {scalar_type_1(0), std::numeric_limits<scalar_type_1>::infinity()}),
        input_type_2(-0.0F));
    values.add_edge_case(input_type_1(0.0F),
                         generate_value<input_type_2>(
                             -std::numeric_limits<scalar_type_2>::infinity(),
                             scalar_type_2(0),
                             {scalar_type_2(0),
                              std::numeric_limits<scalar_type_2>::infinity()}));
    values.add_edge_case(input_type_1(-0.0F),
                         generate_value<input_type_2>(
                             -std::numeric_limits<scalar_type_2>::infinity(),
                             scalar_type_2(0),
                             {scalar_type_2(0),
                              std::numeric_limits<scalar_type_2>::infinity()}));
    values.add_edge_case(input_type_1(0.0F), -infinity);
    values.add_edge_case(input_type_1(-0.0F), -infinity);
    values.add_edge_case(input_type_1(0.0F), higher_than_zero);
    values.add_edge_case(input_type_1(-0.0F), higher_than_zero);
    values.add_edge_case(input_type_1(1.0F), finite);
    values.add_edge_case(higher_than_zero, generate_value<input_type_2>());
    values.add_edge_case(input_type_1(-0.0F), input_type_2(0.0F));
    values.add_edge_case(input_type_1(0.0F), input_type_2(-0.0F));
    values.add_edge_case(input_type_1(-0.0F), input_type_2(-0.0F));
    values.add_edge_case(input_type_1(0.0F), input_type_2(0.0F));
    values.add_edge_case(infinity, input_type_2(0.0F));
    values.add_edge_case(infinity, input_type_2(-0.0F));
    values.add_edge_case(input_type_1(1.0F), infinity);
    values.add_edge_case(
        generate_value<input_type_1>(),
        input_type_2(std::numeric_limits<scalar_type_2>::quiet_NaN()));
    values.add_edge_case(
        input_type_1(std::numeric_limits<scalar_type_1>::quiet_NaN()),
        generate_value<input_type_2>());
  } else if constexpr (T::function == Function::rint) {
    const auto less_than_zero = generate_value<input_type_1>(
        scalar_type_1(-0.5), scalar_type_1(0), {scalar_type_1(0)});
    values.add_random_case(generate_value<input_type_1>());
    values.add_edge_case(input_type_1(-0.5F));
    values.add_edge_case(less_than_zero);
  } else if constexpr (T::function == Function::round) {
    const auto less_than_zero =
        generate_value<input_type_1>(scalar_type_1(-0.5), scalar_type_1(0),
                                     {scalar_type_1(0), scalar_type_1(-0.5)});
    values.add_random_case(generate_value<input_type_1>());
    values.add_edge_case(less_than_zero);
  } else if constexpr (T::function == Function::sinpi) {
    const auto infinity =
        input_type_1(std::numeric_limits<scalar_type_1>::infinity());
    const auto int_higher_than_zero =
        input_type_1(std::ceil(generate_value<scalar_type_1>(
            scalar_type_1(0), std::numeric_limits<scalar_type_1>::max(),
            {scalar_type_1(0)})));
    const auto int_lower_than_zero = input_type_1(std::ceil(
        generate_value<scalar_type_1>(std::numeric_limits<scalar_type_1>::min(),
                                      scalar_type_1(0), {scalar_type_1(0)})));

    values.add_random_case(generate_value<input_type_1>());
    values.add_edge_case(input_type_1(0.0F));
    values.add_edge_case(input_type_1(-0.0F));
    values.add_edge_case(int_lower_than_zero);
    values.add_edge_case(int_higher_than_zero);
    values.add_edge_case(infinity);
    values.add_edge_case(-infinity);
  } else if constexpr (T::function == Function::fract) {
    const auto infinity =
        input_type_1(std::numeric_limits<scalar_type_1>::infinity());
    values.add_random_case(generate_value<input_type_1>(),
                           generate_value<input_type_2>());
    values.add_edge_case(input_type_1(0.0F), generate_value<input_type_2>());
    values.add_edge_case(input_type_1(-0.0F), generate_value<input_type_2>());
    values.add_edge_case(infinity, generate_value<input_type_2>());
    values.add_edge_case(-infinity, generate_value<input_type_2>());
    values.add_edge_case(
        input_type_1(std::numeric_limits<scalar_type_1>::quiet_NaN()),
        generate_value<input_type_2>());

  } else if constexpr (T::function == Function::tanpi) {
    const auto infinity =
        input_type_1(std::numeric_limits<scalar_type_1>::infinity());
    const auto even_number =
        input_type_1(generate_value<scalar_type_1>() * scalar_type_1(2));
    const auto odd_number = input_type_1(
        generate_value<scalar_type_1>() * scalar_type_1(2) + scalar_type_1(1));
    values.add_random_case(generate_value<input_type_1>(
        -M_PI / 2.0, M_PI / 2.0, {-M_PI / 2.0, M_PI / 2.0}));
    values.add_edge_case(input_type_1(0.0F));
    values.add_edge_case(infinity);
    values.add_edge_case(even_number);
    values.add_edge_case(odd_number);
    values.add_edge_case(even_number + input_type_1(0.5F));
    values.add_edge_case(odd_number + input_type_1(0.5F));
  } else if constexpr (T::function == Function::lgamma_r) {
    const auto int_lower_than_zero = input_type_1(std::ceil(
        generate_value<scalar_type_1>(std::numeric_limits<scalar_type_1>::min(),
                                      scalar_type_1(0), {scalar_type_1(0)})));
    values.add_random_case(generate_value<input_type_1>(),
                           generate_value<input_type_2>());
    values.add_edge_case(input_type_1(0.0F), generate_value<input_type_2>());
    values.add_edge_case(input_type_1(-0.0F), generate_value<input_type_2>());
    values.add_edge_case(int_lower_than_zero, generate_value<input_type_2>());
  } else if constexpr (T::function == Function::trunc) {
    values.add_random_case(generate_value<input_type_1>());
    values.add_edge_case(input_type_1(0.5F));
  } else if constexpr (T::function == Function::sincos) {
    values.add_random_case(generate_value<input_type_1>(-1, 1),
                           generate_value<input_type_2>(-1, 1));
  } else if constexpr (T::function == Function::frexp) {
    const auto infinity =
        input_type_1(std::numeric_limits<scalar_type_1>::infinity());
    values.add_random_case(generate_value<input_type_1>(),
                           generate_value<input_type_2>());
    values.add_edge_case(
        input_type_1(std::numeric_limits<scalar_type_1>::quiet_NaN()),
        generate_value<input_type_2>());
    values.add_edge_case(infinity, generate_value<input_type_2>());
  } else if constexpr (T::function == Function::remainder) {
    const auto domain = generate_value<input_type_1>(
        -std::numeric_limits<scalar_type_1>::max(),
        std::numeric_limits<scalar_type_1>::max(), {scalar_type_1(0)});
    values.add_random_case(generate_value<input_type_1>(), domain);
  } else if constexpr (T::function == Function::powr) {
    const auto higher_than_or_equal_zero = generate_value<input_type_1>(
        scalar_type_1(0), std::numeric_limits<scalar_type_1>::max());
    values.add_random_case(higher_than_or_equal_zero,
                           generate_value<input_type_1>());
  } else if constexpr (T::function == Function::pown) {
    const auto odd_number_higher_than_zero = input_type_2(
        generate_value<scalar_type_2>(scalar_type_2(0),
                                      std::numeric_limits<scalar_type_2>::max(),
                                      {scalar_type_2(0)}) *
            scalar_type_2(2) +
        scalar_type_2(1));
    const auto even_number_higher_than_zero =
        input_type_2(generate_value<scalar_type_2>(
            scalar_type_2(0), std::numeric_limits<scalar_type_2>::max(),
            {scalar_type_2(0)})) *
        scalar_type_2(2);
    const auto odd_number_lower_than_zero =
        input_type_2(generate_value<scalar_type_2>(
                         -std::numeric_limits<scalar_type_2>::max(),
                         scalar_type_2(0), {scalar_type_2(0)}) *
                         scalar_type_2(2) +
                     scalar_type_2(1));
    const auto even_number_lower_than_zero =
        input_type_2(generate_value<scalar_type_2>(
            -std::numeric_limits<scalar_type_2>::max(), scalar_type_2(0),
            {scalar_type_2(0)})) *
        scalar_type_2(2);
    const auto infinity =
        input_type_1(std::numeric_limits<scalar_type_1>::infinity());
    values.add_random_case(generate_value<input_type_1>(),
                           generate_value<input_type_2>());
    values.add_edge_case(generate_value<input_type_1>(), input_type_2(0.0F));
    values.add_edge_case(input_type_1(0), input_type_2(0));
    values.add_edge_case(
        input_type_1(std::numeric_limits<scalar_type_1>::quiet_NaN()),
        input_type_2(0));
    values.add_edge_case(input_type_1(infinity), input_type_2(0));
    values.add_edge_case(input_type_1(0.0F), odd_number_lower_than_zero);
    values.add_edge_case(input_type_1(0.0F), even_number_lower_than_zero);
    values.add_edge_case(input_type_1(0.0F), even_number_higher_than_zero);
    values.add_edge_case(input_type_1(0.0F), odd_number_higher_than_zero);
    values.add_edge_case(input_type_1(-0.0F), odd_number_lower_than_zero);
    values.add_edge_case(input_type_1(-0.0F), even_number_lower_than_zero);
    values.add_edge_case(input_type_1(-0.0F), even_number_higher_than_zero);
    values.add_edge_case(input_type_1(-0.0F), odd_number_higher_than_zero);
  } else if constexpr (T::function == Function::rootn) {
    const auto odd_number_higher_than_zero = input_type_2(
        generate_value<scalar_type_2>(scalar_type_2(0),
                                      std::numeric_limits<scalar_type_2>::max(),
                                      {scalar_type_2(0)}) *
            scalar_type_2(2) +
        scalar_type_2(1));
    const auto even_number_higher_than_zero =
        input_type_2(generate_value<scalar_type_2>(
            scalar_type_2(0), std::numeric_limits<scalar_type_2>::max(),
            {scalar_type_2(0)})) *
        scalar_type_2(2);
    const auto odd_number_lower_than_zero =
        input_type_2(generate_value<scalar_type_2>(
                         -std::numeric_limits<scalar_type_2>::max(),
                         scalar_type_2(0), {scalar_type_2(0)}) *
                         scalar_type_2(2) +
                     scalar_type_2(1));
    const auto even_number_lower_than_zero =
        input_type_2(generate_value<scalar_type_2>(
            -std::numeric_limits<scalar_type_2>::max(), scalar_type_2(0),
            {scalar_type_2(0)})) *
        scalar_type_2(2);
    const auto even_number =
        input_type_2(generate_value<scalar_type_2>() * scalar_type_2(2));
    const auto lower_than_zero =
        generate_value<input_type_1>(-std::numeric_limits<scalar_type_1>::max(),
                                     scalar_type_1(0), {scalar_type_1(0)});

    values.add_random_case(generate_value<input_type_1>(),
                           generate_value<input_type_2>());
    values.add_edge_case(input_type_1(0.0F), odd_number_lower_than_zero);
    values.add_edge_case(input_type_1(0.0F), even_number_lower_than_zero);
    values.add_edge_case(input_type_1(0.0F), odd_number_higher_than_zero);
    values.add_edge_case(input_type_1(0.0F), even_number_higher_than_zero);
    values.add_edge_case(input_type_1(-0.0F), odd_number_lower_than_zero);
    values.add_edge_case(input_type_1(-0.0F), even_number_lower_than_zero);
    values.add_edge_case(input_type_1(-0.0F), odd_number_higher_than_zero);
    values.add_edge_case(input_type_1(-0.0F), even_number_higher_than_zero);
    values.add_edge_case(lower_than_zero, even_number);
    values.add_edge_case(generate_value<input_type_1>(), input_type_2(0.0F));
  } else {
    if constexpr (T::arg_num == 2) {
      values.add_random_case(generate_value<input_type_1>(),
                             generate_value<input_type_2>());
    } else if (T::arg_num == 3) {
      values.add_random_case(generate_value<input_type_1>(),
                             generate_value<input_type_2>(),
                             generate_value<input_type_3>());
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
  case Function::maxmag:
  case Function::minmag:
  case Function::nextafter:
  case Function::remainder:
  case Function::rint:
  case Function::round:
  case Function::trunc:
  case Function::nan:
  case Function::native_divide:
  case Function::native_recip:
  case Function::fract:
  case Function::frexp:
  case Function::remquo:
  case Function::modf:
    return std::vector<scalar_type>(work_size, 0.0F);
  case Function::log1p:
  case Function::cbrt:
  case Function::rsqrt:
  case Function::native_rsqrt: {
    constexpr scalar_type ulp = 2.0F * epsilon;
    std::vector<scalar_type> ulp_values(work_size, ulp);
    return ulp_values;
  }
  case Function::exp:
  case Function::exp2:
  case Function::exp10:
  case Function::native_exp:
  case Function::native_exp2:
  case Function::native_exp10:
  case Function::expm1:
  case Function::sqrt:
  case Function::native_sqrt:
  case Function::log10:
  case Function::lgamma:
  case Function::lgamma_r:
  case Function::log:
  case Function::log2:
  case Function::native_log:
  case Function::native_log2:
  case Function::native_log10: {
    constexpr scalar_type ulp = 3.0F * epsilon;
    std::vector<scalar_type> ulp_values(work_size, ulp);
    return ulp_values;
  }
  case Function::acos:
  case Function::acosh:
  case Function::asin:
  case Function::asinh:
  case Function::cos:
  case Function::native_cos:
  case Function::cosh:
  case Function::cospi:
  case Function::hypot:
  case Function::sinh:
  case Function::sin:
  case Function::native_sin:
  case Function::sinpi:
  case Function::sincos: {
    constexpr scalar_type ulp = 4.0F * epsilon;
    std::vector<scalar_type> ulp_values(work_size, ulp);
    return ulp_values;
  }
  case Function::acospi:
  case Function::asinpi:
  case Function::atan:
  case Function::atanh:
  case Function::atanpi:
  case Function::tan:
  case Function::native_tan:
  case Function::tanh: {
    constexpr scalar_type ulp = 5.0F * epsilon;
    std::vector<scalar_type> ulp_values(work_size, ulp);
    return ulp_values;
  }
  case Function::atan2:
  case Function::atan2pi:
  case Function::tanpi: {
    constexpr scalar_type ulp = 6.0F * epsilon;
    std::vector<scalar_type> ulp_values(work_size, ulp);
    return ulp_values;
  }
  case Function::erf:
  case Function::erfc:
  case Function::pow:
  case Function::rootn:
  case Function::pown:
  case Function::powr:
  case Function::native_powr:
  case Function::tgamma: {
    constexpr scalar_type ulp = 16.0F * epsilon;
    std::vector<scalar_type> ulp_values(work_size, ulp);
    return ulp_values;
  }
  case Function::half_cos:
  case Function::half_divide:
  case Function::half_exp:
  case Function::half_exp2:
  case Function::half_exp10:
  case Function::half_log:
  case Function::half_log2:
  case Function::half_log10:
  case Function::half_powr:
  case Function::half_recip:
  case Function::half_rsqrt:
  case Function::half_sin:
  case Function::half_tan:
  case Function::half_sqrt: {
    constexpr scalar_type ulp = 8192.0F * epsilon;
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
  if constexpr (!std::is_integral_v<T>) {
    if (std::isnan(result) && std::isnan(reference)) {
      return true;
    }
  }
  const auto ulp_dist = ulp_distance(result, reference);
  return ulp_dist <= std::fabs(result + reference) * ulp_value ||
         ulp_dist < std::numeric_limits<T>::min();
}

template <typename T, typename cassian::EnableIfIsVector<T> = 0>
bool match_results(const T &result, const T &reference,
                   const cassian::scalar_type_v<T> ulp_value) {
  for (auto i = 0UL; i < static_cast<uint32_t>(result.size()); i++) {
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