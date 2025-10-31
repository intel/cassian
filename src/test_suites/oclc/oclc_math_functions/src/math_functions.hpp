/*
 * Copyright (C) 2021-2024 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_OCLC_MATH_FUNCTIONS_MATH_FUNCTIONS_HPP
#define CASSIAN_OCLC_MATH_FUNCTIONS_MATH_FUNCTIONS_HPP

#include <cassian/fp_types/math.hpp>
#include <cassian/logging/logging.hpp>
#include <cassian/utility/comparators.hpp>
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

namespace detail {

template <template <typename...> typename OCLC_FUNCTION, typename... TYPE>
struct FunctionProductRow {};

template <template <typename...> typename OCLC_FUNCTION, typename... TYPE>
struct FunctionProductRow<OCLC_FUNCTION, std::tuple<TYPE...>> {
  using type = std::tuple<OCLC_FUNCTION<TYPE>...>;
};

} // namespace detail

template <typename TYPE, template <typename...> typename... OCLC_FUNCTION>
struct FunctionProduct {
  using type = typename ca::TupleConcat<
      typename detail::FunctionProductRow<OCLC_FUNCTION, TYPE>::type...>::type;
};

template <auto FUNCTION, auto ARG_NUM, auto REFERENCE_FUNC,
          typename OUTPUT_TYPE, auto ADDRESS_SPACE = AddressSpace::clc_global,
          typename INPUT_TYPE_1 = OUTPUT_TYPE,
          typename INPUT_TYPE_2 = INPUT_TYPE_1,
          typename INPUT_TYPE_3 = INPUT_TYPE_2,
          auto REFERENCE_FUNC_DERIVED_CHECK = nullptr>
class OclcFunction {
public:
  using clc_output_type = OUTPUT_TYPE;
  using clc_input_type_1 = INPUT_TYPE_1;
  using output_type = typename OUTPUT_TYPE::host_type;
  using scalar_output_type = typename OUTPUT_TYPE::scalar_type::host_type;
  using input_type_1 = typename INPUT_TYPE_1::host_type;
  using input_type_2 = typename INPUT_TYPE_2::host_type;
  using input_type_3 = typename INPUT_TYPE_3::host_type;
  using scalar_type_1 = typename INPUT_TYPE_1::scalar_type::host_type;
  using scalar_type_2 = typename INPUT_TYPE_2::scalar_type::host_type;

  static constexpr auto function = FUNCTION;
  static constexpr auto arg_num = ARG_NUM;
  static constexpr auto calculate_reference = REFERENCE_FUNC;
  static constexpr auto calculate_reference_derived_check =
      REFERENCE_FUNC_DERIVED_CHECK;
  static constexpr auto address_space = ADDRESS_SPACE;

  using function_type = decltype(FUNCTION);

  static constexpr auto get_address_space() {
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
      throw UnknownFunctionException(get_function_string() +
                                     " address space uninitialized");
    }
  }
  static constexpr auto get_function_string() {
    return Catch::StringMaker<function_type>::convert(function);
  }
  static constexpr auto get_is_store() {
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
  static constexpr auto get_is_native() {
    switch (function) {
    case Function::native_cos:
    case Function::native_divide:
    case Function::native_exp:
    case Function::native_exp10:
    case Function::native_exp2:
    case Function::native_log:
    case Function::native_log10:
    case Function::native_log2:
    case Function::native_powr:
    case Function::native_recip:
    case Function::native_rsqrt:
    case Function::native_sin:
    case Function::native_sqrt:
    case Function::native_tan:
      return true;
    default:
      return false;
    }
  }

  static auto get_build_options() {
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
    if constexpr (address_space == AddressSpace::clc_private) {
      ss << " -DUSE_PRIVATE";
      // Private is forbidden in kernel args. Use global instead of private
      // Inside the kernel then assigne global value to private variable;
      ss << " -DADDRESS_SPACE=global";
    } else {
      ss << " -DADDRESS_SPACE=" << get_address_space();
    }

    if constexpr (std::is_same_v<function_type, Function>) {
      if constexpr (function == Function::correctly_rounded_sqrt) {
        ss << " -DFUNCTION=sqrt -cl-fp32-correctly-rounded-divide-sqrt";
      } else {
        ss << " -DFUNCTION=" << get_function_string();
      }
    } else {
      ss << " -DFUNCTION=" << get_function_string();
    }
    return ss.str();
  }

  static auto get_build_options_relaxed() {
    std::stringstream ss;
    ss << get_build_options();
    ss << " -cl-fast-relaxed-math";
    return ss.str();
  }
};

template <typename TestType> auto test_name_with_function() {
  std::stringstream ss;
  ss << TestType::get_function_string();
  ss << " - ";
  ss << TestType::clc_output_type::type_name;
  return ss.str();
}

template <typename T> constexpr auto get_gentype_values() {
  using input_type_1 = typename T::input_type_1;
  using input_type_2 = typename T::input_type_2;
  using input_type_3 = typename T::input_type_3;
  using scalar_type_1 = typename T::scalar_type_1;
  using scalar_type_2 = typename T::scalar_type_2;

  MathInputValues<T> values = {};
  if constexpr (T::function == Function::acos) {
    values.add_random_case(
        generate_value<input_type_1>(scalar_type_1(-1), scalar_type_1(1)));
  } else if constexpr (T::function == Function::asinpi) {
    const auto higher_than_one = generate_value<input_type_1>(
        scalar_type_1(1.0), std::numeric_limits<scalar_type_1>::max());
    values.add_random_case(
        generate_value<input_type_1>(scalar_type_1(-1), scalar_type_1(1)));
    values.add_edge_case(input_type_1(scalar_type_1(0.0F)));
    values.add_edge_case(input_type_1(scalar_type_1(-0.0F)));
    values.add_edge_case(higher_than_one);
  } else if constexpr (T::function == Function::acosh) {
    values.add_random_case(generate_value<input_type_1>(
        scalar_type_1(1), std::numeric_limits<scalar_type_1>::max()));
  } else if constexpr (T::function == Function::acospi) {
    const auto higher_than_one = generate_value<input_type_1>(
        scalar_type_1(1.0), std::numeric_limits<scalar_type_1>::max(),
        {scalar_type_1(1.0)});
    values.add_random_case(
        generate_value<input_type_1>(scalar_type_1(-1), scalar_type_1(1)));
    values.add_edge_case(input_type_1(scalar_type_1(1)));
    values.add_edge_case(higher_than_one);
  } else if constexpr (T::function == Function::cospi) {
    const auto infinity =
        input_type_1(std::numeric_limits<scalar_type_1>::infinity());
    // Set threshold to largest representable odd integer using mantissa digits
    const int64_t precision_threshold =
        (1LL << (std::numeric_limits<scalar_type_1>::digits - 1)) - 1LL;
    const auto even_number = input_type_1(
        scalar_type_1(generate_value<int64_t>(-precision_threshold,
                                              precision_threshold, {0}) &
                      ~1LL));
    const auto odd_number = input_type_1(
        scalar_type_1(generate_value<int64_t>(-precision_threshold,
                                              precision_threshold, {0}) |
                      1LL));
    values.add_random_case(generate_value<input_type_1>());
    values.add_edge_case(input_type_1(scalar_type_1(0.0F)));
    values.add_edge_case(input_type_1(scalar_type_1(-0.0F)));
    values.add_edge_case(even_number);
    values.add_edge_case(odd_number);
    values.add_edge_case(even_number + input_type_1(scalar_type_1(0.5F)));
    values.add_edge_case(odd_number + input_type_1(scalar_type_1(0.5F)));
    values.add_edge_case(infinity);
    values.add_edge_case(-infinity);
  } else if constexpr (T::function == Function::ceil) {
    values.add_random_case(generate_value<input_type_1>());
    values.add_edge_case(
        generate_value<input_type_1>(scalar_type_1(-1), scalar_type_1(0),
                                     {scalar_type_1(-1), scalar_type_1(0)}));
  } else if constexpr (T::function == Function::atanpi) {
    values.add_random_case(generate_value<input_type_1>());
    values.add_edge_case(input_type_1(scalar_type_1(0)));
    values.add_edge_case(input_type_1(scalar_type_1(-0)));
    values.add_edge_case(
        input_type_1(std::numeric_limits<input_type_1>::infinity()));
  } else if constexpr (T::function == Function::atan2) {
    values.add_random_case(generate_value<input_type_1>(),
                           generate_value<input_type_2>());
  } else if constexpr (T::function == Function::atan2pi) {
    static_assert(std::is_same_v<input_type_1, input_type_2>);
    const auto lower_than_zero = generate_value<input_type_1>(
        std::numeric_limits<scalar_type_1>::lowest(), scalar_type_1(0),
        {scalar_type_1(0)});
    const auto higher_than_zero = generate_value<input_type_1>(
        scalar_type_1(0), std::numeric_limits<scalar_type_1>::max(),
        {scalar_type_1(0)});
    const auto infinity =
        input_type_1(std::numeric_limits<scalar_type_1>::infinity());
    values.add_random_case(generate_value<input_type_1>(),
                           generate_value<input_type_2>());
    values.add_edge_case(input_type_1(scalar_type_1(0)),
                         input_type_2(scalar_type_1(0.0F)));
    values.add_edge_case(input_type_1(scalar_type_1(0)),
                         input_type_2(scalar_type_1(-0.0F)));
    values.add_edge_case(input_type_1(scalar_type_1(0)), lower_than_zero);
    values.add_edge_case(input_type_1(scalar_type_1(0)), higher_than_zero);
    values.add_edge_case(input_type_1(scalar_type_1(-0)),
                         input_type_2(scalar_type_1(0.0F)));
    values.add_edge_case(input_type_1(scalar_type_1(-0)),
                         input_type_2(scalar_type_1(-0.0F)));
    values.add_edge_case(input_type_1(scalar_type_1(-0)), lower_than_zero);
    values.add_edge_case(input_type_1(scalar_type_1(-0)), higher_than_zero);
    values.add_edge_case(lower_than_zero, input_type_2(scalar_type_1(0)));
    values.add_edge_case(higher_than_zero, input_type_2(scalar_type_1(0)));
    values.add_edge_case(lower_than_zero, input_type_2(scalar_type_1(-0)));
    values.add_edge_case(higher_than_zero, input_type_2(scalar_type_1(-0)));
    values.add_edge_case(higher_than_zero, -infinity);
    values.add_edge_case(higher_than_zero, infinity);
    values.add_edge_case(infinity, generate_value<input_type_2>());
    values.add_edge_case(infinity, -infinity);
    values.add_edge_case(infinity, infinity);
  } else if constexpr (T::function == Function::copysign) {
    values.add_random_case(generate_value<input_type_1>(),
                           generate_value<input_type_2>());
  } else if constexpr (T::function == Function::atanh) {
    values.add_random_case(
        generate_value<input_type_1>(scalar_type_1(-1), scalar_type_1(1),
                                     {scalar_type_1(-1), scalar_type_1(1)}));
  } else if constexpr (T::function == Function::sqrt ||
                       T::function == Function::half_sqrt) {
    values.add_random_case(generate_value<input_type_1>(
        scalar_type_1(0), std::numeric_limits<scalar_type_1>::max()));
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
        input_type_1(scalar_type_1(0.0F)),
        input_type_2(std::numeric_limits<scalar_type_2>::quiet_NaN()));
    values.add_edge_case(
        input_type_1(scalar_type_1(0.0F)),
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
    const auto lower_than_zero = generate_value<input_type_1>(
        std::numeric_limits<scalar_type_1>::lowest(), scalar_type_1(0),
        {scalar_type_1(0)});
    values.add_random_case(generate_value<input_type_1>(),
                           generate_value<input_type_2>());
    values.add_edge_case(input_type_1(scalar_type_1(0.0F)), higher_than_zero);
    values.add_edge_case(input_type_1(scalar_type_1(0.0F)), lower_than_zero);
    values.add_edge_case(input_type_1(scalar_type_1(-0.0F)), higher_than_zero);
    values.add_edge_case(input_type_1(scalar_type_1(-0.0F)), lower_than_zero);
  } else if constexpr (T::function == Function::pow) {
    const auto infinity =
        input_type_1(std::numeric_limits<scalar_type_1>::infinity());
    values.add_random_case(generate_value<input_type_1>());
    values.add_edge_case(input_type_1(scalar_type_1(0.0F)), -infinity);
    values.add_edge_case(input_type_1(scalar_type_1(-0.0F)), -infinity);
  } else if constexpr (T::function == Function::powr ||
                       T::function == Function::half_powr) {

    const auto higher_equal_zero = generate_value<input_type_1>(
        scalar_type_1(0), std::numeric_limits<scalar_type_1>::max());
    const auto higher_than_zero = generate_value<input_type_1>(
        scalar_type_1(0), std::numeric_limits<scalar_type_1>::max(),
        {scalar_type_1(0)});
    const auto lower_than_zero = generate_value<input_type_1>(
        std::numeric_limits<scalar_type_1>::lowest(), scalar_type_1(0),
        {scalar_type_1(0)});
    const auto infinity =
        input_type_1(std::numeric_limits<scalar_type_1>::infinity());
    const auto finite = generate_value<input_type_2>(
        std::numeric_limits<scalar_type_2>::lowest(),
        std::numeric_limits<scalar_type_2>::max());
    values.add_random_case(higher_equal_zero, generate_value<input_type_2>());
    values.add_edge_case(
        generate_value<input_type_1>(scalar_type_1(0),
                                     std::numeric_limits<scalar_type_1>::max(),
                                     {scalar_type_1(0)}),
        input_type_2(scalar_type_1(0.0F)));
    values.add_edge_case(
        generate_value<input_type_1>(scalar_type_1(0),
                                     std::numeric_limits<scalar_type_1>::max(),
                                     {scalar_type_1(0)}),
        input_type_2(scalar_type_1(-0.0F)));
    values.add_edge_case(
        input_type_1(scalar_type_1(0.0F)),
        generate_value<input_type_2>(-std::numeric_limits<scalar_type_2>::max(),
                                     scalar_type_2(0), {scalar_type_2(0)}));
    values.add_edge_case(
        input_type_1(scalar_type_1(-0.0F)),
        generate_value<input_type_2>(-std::numeric_limits<scalar_type_2>::max(),
                                     scalar_type_2(0), {scalar_type_2(0)}));
    values.add_edge_case(input_type_1(scalar_type_1(0.0F)), -infinity);
    values.add_edge_case(input_type_1(scalar_type_1(-0.0F)), -infinity);
    values.add_edge_case(input_type_1(scalar_type_1(0.0F)), higher_than_zero);
    values.add_edge_case(input_type_1(scalar_type_1(-0.0F)), higher_than_zero);
    values.add_edge_case(input_type_1(scalar_type_1(1.0F)), finite);
    values.add_edge_case(higher_than_zero, generate_value<input_type_2>());
    values.add_edge_case(input_type_1(scalar_type_1(-0.0F)),
                         input_type_2(scalar_type_1(0.0F)));
    values.add_edge_case(input_type_1(scalar_type_1(0.0F)),
                         input_type_2(scalar_type_1(-0.0F)));
    values.add_edge_case(input_type_1(scalar_type_1(-0.0F)),
                         input_type_2(scalar_type_1(-0.0F)));
    values.add_edge_case(input_type_1(scalar_type_1(0.0F)),
                         input_type_2(scalar_type_1(0.0F)));
    values.add_edge_case(infinity, input_type_2(scalar_type_1(0.0F)));
    values.add_edge_case(infinity, input_type_2(scalar_type_1(-0.0F)));
    values.add_edge_case(input_type_1(scalar_type_1(1.0F)), -infinity);
    values.add_edge_case(input_type_1(scalar_type_1(1.0F)), infinity);
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
    values.add_edge_case(input_type_1(scalar_type_1(-0.5F)));
    values.add_edge_case(less_than_zero);
  } else if constexpr (T::function == Function::round) {
    const auto less_than_zero =
        generate_value<input_type_1>(scalar_type_1(-0.5), scalar_type_1(0),
                                     {scalar_type_1(0), scalar_type_1(-0.5)});
    values.add_random_case(generate_value<input_type_1>());
    values.add_edge_case(less_than_zero);
  } else if constexpr (T::function == Function::sinpi) {
    using std::ceil;
    const auto infinity =
        input_type_1(std::numeric_limits<scalar_type_1>::infinity());
    const auto int_higher_than_zero =
        input_type_1(ceil(generate_value<scalar_type_1>(
            scalar_type_1(0), std::numeric_limits<scalar_type_1>::max(),
            {scalar_type_1(0)})));
    const auto int_lower_than_zero =
        input_type_1(ceil(generate_value<scalar_type_1>(
            std::numeric_limits<scalar_type_1>::lowest(), scalar_type_1(-1))));
    values.add_random_case(generate_value<input_type_1>());
    values.add_edge_case(input_type_1(scalar_type_1(0.0F)));
    values.add_edge_case(input_type_1(scalar_type_1(-0.0F)));
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
    // Set threshold to largest representable odd integer using mantissa digits
    const int64_t precision_threshold =
        (1LL << (std::numeric_limits<scalar_type_1>::digits - 1)) - 1LL;
    const auto even_number = input_type_1(
        scalar_type_1(generate_value<int64_t>(-precision_threshold,
                                              precision_threshold, {0}) &
                      ~1LL));
    const auto odd_number = input_type_1(
        scalar_type_1(generate_value<int64_t>(-precision_threshold,
                                              precision_threshold, {0}) |
                      1LL));
    values.add_random_case(generate_value<input_type_1>(
        scalar_type_1(-0.5 + 0.05), scalar_type_1(0.5 - 0.05)));
    values.add_edge_case(input_type_1(scalar_type_1(+0.0F)));
    values.add_edge_case(input_type_1(scalar_type_1(-0.0F)));
    values.add_edge_case(infinity);
    values.add_edge_case(-infinity);
    values.add_edge_case(even_number);
    values.add_edge_case(odd_number);
    values.add_edge_case(even_number + input_type_1(scalar_type_1(0.5F)));
    values.add_edge_case(odd_number + input_type_1(scalar_type_1(0.5F)));
  } else if constexpr (T::function == Function::lgamma_r) {
    using std::ceil;
    const auto int_lower_than_zero =
        input_type_1(ceil(generate_value<scalar_type_1>(
            std::numeric_limits<scalar_type_1>::lowest(), scalar_type_1(-1))));
    values.add_random_case(generate_value<input_type_1>(),
                           generate_value<input_type_2>());
    values.add_edge_case(input_type_1(0.0F), generate_value<input_type_2>());
    values.add_edge_case(input_type_1(-0.0F), generate_value<input_type_2>());
    values.add_edge_case(int_lower_than_zero, generate_value<input_type_2>());
  } else if constexpr (T::function == Function::trunc) {
    values.add_random_case(generate_value<input_type_1>());
    values.add_edge_case(input_type_1(scalar_type_1(0.5F)));
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
        std::numeric_limits<scalar_type_1>::lowest(),
        std::numeric_limits<scalar_type_1>::max(), {scalar_type_1(0)});
    values.add_random_case(generate_value<input_type_1>(), domain);
  } else if constexpr (T::function == Function::powr) {
    const auto higher_than_or_equal_zero = generate_value<input_type_1>(
        scalar_type_1(0), std::numeric_limits<scalar_type_1>::max());
    values.add_random_case(higher_than_or_equal_zero,
                           generate_value<input_type_1>());
  } else if constexpr (T::function == Function::pown) {
    const auto infinity =
        input_type_1(std::numeric_limits<scalar_type_1>::infinity());
    const auto lower_than_zero = generate_value<input_type_2>(
        std::numeric_limits<scalar_type_2>::lowest(), scalar_type_2(0),
        {scalar_type_2(0)});
    const auto higher_than_zero = generate_value<input_type_2>(
        scalar_type_2(0), std::numeric_limits<scalar_type_2>::max(),
        {scalar_type_2(0)});
    values.add_random_case(generate_value<input_type_1>(),
                           generate_value<input_type_2>());
    values.add_edge_case(generate_value<input_type_1>(), input_type_2(0.0F));
    values.add_edge_case(input_type_1(scalar_type_1(0)), input_type_2(0));
    values.add_edge_case(
        input_type_1(std::numeric_limits<scalar_type_1>::quiet_NaN()),
        input_type_2(0));
    values.add_edge_case(input_type_1(infinity), input_type_2(0));
    values.add_edge_case(input_type_1(scalar_type_1(0.0F)),
                         (lower_than_zero - (lower_than_zero % 2)) +
                             1); // odd number
    values.add_edge_case(input_type_1(scalar_type_1(0.0F)),
                         lower_than_zero -
                             (lower_than_zero % 2)); // even number
    values.add_edge_case(input_type_1(scalar_type_1(0.0F)),
                         higher_than_zero -
                             (higher_than_zero % 2)); // even number
    values.add_edge_case(input_type_1(scalar_type_1(0.0F)),
                         (higher_than_zero - (higher_than_zero % 2)) +
                             1); // odd number
    values.add_edge_case(input_type_1(scalar_type_1(-0.0F)),
                         (lower_than_zero - (lower_than_zero % 2)) +
                             1); // odd number
    values.add_edge_case(input_type_1(scalar_type_1(-0.0F)),
                         lower_than_zero -
                             (lower_than_zero % 2)); // even number
    values.add_edge_case(input_type_1(scalar_type_1(-0.0F)),
                         higher_than_zero -
                             (higher_than_zero % 2)); // even number
    values.add_edge_case(input_type_1(scalar_type_1(-0.0F)),
                         (higher_than_zero - (higher_than_zero % 2)) +
                             1); // odd number
  } else if constexpr (T::function == Function::rootn) {
    const auto lower_than_zero = generate_value<input_type_2>(
        std::numeric_limits<scalar_type_2>::lowest(), scalar_type_2(0),
        {scalar_type_2(0)});
    const auto lower_than_zero_1 = generate_value<input_type_1>(
        std::numeric_limits<scalar_type_1>::lowest(), scalar_type_1(0),
        {scalar_type_1(0)});
    const auto higher_than_zero = generate_value<input_type_2>(
        scalar_type_2(0), std::numeric_limits<scalar_type_2>::max(),
        {scalar_type_2(0)});
    const auto to_even = generate_value<input_type_2>();
    values.add_random_case(generate_value<input_type_1>(),
                           generate_value<input_type_2>());
    values.add_edge_case(input_type_1(scalar_type_1(0.0F)),
                         lower_than_zero -
                             (lower_than_zero % 2)); // even number v
    values.add_edge_case(input_type_1(scalar_type_1(-0.0F)),
                         lower_than_zero -
                             (lower_than_zero % 2)); // even number v
    values.add_edge_case(input_type_1(scalar_type_1(0.0F)),
                         (lower_than_zero - (lower_than_zero % 2)) +
                             1); // odd number v
    values.add_edge_case(input_type_1(scalar_type_1(0.0F)),
                         higher_than_zero -
                             (higher_than_zero % 2)); // even number v
    values.add_edge_case(input_type_1(scalar_type_1(-0.0F)),
                         higher_than_zero -
                             (higher_than_zero % 2)); // even number v
    values.add_edge_case(input_type_1(scalar_type_1(0.0F)),
                         (higher_than_zero - (higher_than_zero % 2)) +
                             1); // odd number v
    values.add_edge_case(input_type_1(scalar_type_1(-0.0F)),
                         (higher_than_zero - (higher_than_zero % 2)) +
                             1); // odd number v
    values.add_edge_case(lower_than_zero_1, to_even - (to_even % 2));
    values.add_edge_case(generate_value<input_type_1>(), input_type_2(0.0F));
  } else if constexpr (T::function == Function::correctly_rounded_sqrt) {
    values.add_random_case(generate_value<input_type_1>(
        scalar_type_1(0), std::numeric_limits<scalar_type_1>::max()));
    values.add_edge_case(
        input_type_1(std::numeric_limits<scalar_type_1>::lowest()));
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
std::vector<ca::scalar_type_v<T>> get_ulp_values(const Function &function,
                                                 const size_t &work_size) {
  using scalar_type = ca::scalar_type_v<T>;
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
  case Function::fract:
  case Function::frexp:
  case Function::remquo:
  case Function::modf:
    return std::vector<scalar_type>(work_size, scalar_type(0.0F));
  case Function::log1p:
  case Function::cbrt:
  case Function::rsqrt: {
    scalar_type ulp = scalar_type(2.0F);
    std::vector<scalar_type> ulp_values(work_size, ulp);
    return ulp_values;
  }
  case Function::exp:
  case Function::exp2:
  case Function::exp10:
  case Function::expm1:
  case Function::sqrt:
  case Function::correctly_rounded_sqrt:
  case Function::log10:
  case Function::lgamma:
  case Function::log:
  case Function::log2: {
    scalar_type ulp = scalar_type(3.0F);
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
  case Function::hypot:
  case Function::sinh:
  case Function::sin:
  case Function::sinpi:
  case Function::sincos: {
    scalar_type ulp = scalar_type(4.0F);
    std::vector<scalar_type> ulp_values(work_size, ulp);
    return ulp_values;
  }
  case Function::acospi:
  case Function::asinpi:
  case Function::atan:
  case Function::atanh:
  case Function::atanpi:
  case Function::tan:
  case Function::tanh: {
    scalar_type ulp = scalar_type(5.0F);
    std::vector<scalar_type> ulp_values(work_size, ulp);
    return ulp_values;
  }
  case Function::atan2:
  case Function::atan2pi:
  case Function::tanpi: {
    scalar_type ulp = scalar_type(6.0F);
    std::vector<scalar_type> ulp_values(work_size, ulp);
    return ulp_values;
  }
  case Function::erf:
  case Function::erfc:
  case Function::pow:
  case Function::rootn:
  case Function::pown:
  case Function::powr:
  case Function::tgamma: {
    scalar_type ulp = scalar_type(16.0F);
    std::vector<scalar_type> ulp_values(work_size, ulp);
    return ulp_values;
  }
  case Function::native_divide:
  case Function::native_recip:
  case Function::native_rsqrt:
  case Function::native_exp:
  case Function::native_exp2:
  case Function::native_exp10:
  case Function::native_sqrt:
  case Function::native_log:
  case Function::native_log2:
  case Function::native_log10:
  case Function::native_sin:
  case Function::native_cos:
  case Function::native_tan:
  case Function::native_powr:
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
  case Function::half_sqrt:
  case Function::lgamma_r: {
    scalar_type ulp = scalar_type(8192.0F);
    std::vector<scalar_type> ulp_values(work_size, ulp);
    return ulp_values;
  }
  default:
    throw UnknownFunctionException(
        Catch::StringMaker<Function>::convert(function) + " uninitialized");
  }
}

template <typename OUTPUT_TYPE, typename INPUT_TYPE_1, typename INPUT_TYPE_2>
std::vector<ca::PrecisionRequirement<ca::scalar_type_v<OUTPUT_TYPE>>>
requirements_function(const Function &function, const INPUT_TYPE_1 &input_a,
                      const INPUT_TYPE_2 &input_b) {
  using ca::PrecisionRequirement;
  using ca::PrecisionRequirementType;

  using scalar_type = ca::scalar_type_v<INPUT_TYPE_1>;
  using scalar_type_t2 = ca::scalar_type_v<INPUT_TYPE_2>;
  using scalar_type_output = ca::scalar_type_v<OUTPUT_TYPE>;
  constexpr auto epsilon = std::numeric_limits<scalar_type>::epsilon();
  std::vector<PrecisionRequirement<scalar_type_output>> requirements;

  switch (function) {
  case Function::asin:
  case Function::atan:
  case Function::acos: {
    PrecisionRequirement<scalar_type_output> requirement;
    requirement.type = PrecisionRequirementType::ulp_value;
    requirement.value = 4096.0F;
    requirements.emplace_back(requirement);
    if constexpr (ca::is_vector_v<INPUT_TYPE_1>) {
      for (auto k = 1; k < INPUT_TYPE_1::vector_size; k++) {
        requirements.emplace_back(requirement);
      }
    }
    return requirements;
  }

  case Function::exp2:
  case Function::exp: {
    PrecisionRequirement<scalar_type> requirement;
    requirement.type = PrecisionRequirementType::ulp_value;

    if constexpr (ca::is_vector_v<INPUT_TYPE_1>) {
      for (auto k = 0; k < INPUT_TYPE_1::vector_size; k++) {
        requirement.value = (3.0F + std::floor(std::fabs(2 * input_a[k])));
        requirements.emplace_back(requirement);
      }
    } else {
      requirement.value = (3.0F + std::floor(std::fabs(2 * input_a)));
      requirements.emplace_back(requirement);
    }
    return requirements;
  }

  case Function::log1p:
  case Function::exp10:
  case Function::expm1:
  case Function::tanh:
  case Function::tan:
  case Function::cbrt:
  case Function::atan2pi:
  case Function::atan2:
  case Function::atanpi:
  case Function::asinpi:
  case Function::asinh:
  case Function::acospi:
  case Function::acosh: {
    PrecisionRequirement<scalar_type> requirement;
    requirement.type = PrecisionRequirementType::ulp_value;
    requirement.value = 8192.0F;
    requirements.emplace_back(requirement);
    if constexpr (ca::is_vector_v<INPUT_TYPE_1>) {
      for (auto k = 1; k < INPUT_TYPE_1::vector_size; k++) {
        requirements.emplace_back(requirement);
      }
    }
    return requirements;
  }

  case Function::log2:
  case Function::log10:
  case Function::log: {
    auto give_requirement = [&requirements, epsilon](scalar_type x) {
      PrecisionRequirement<scalar_type> requirement;
      if (x >= 0.5F && x <= 2.0F) {
        requirement.type = PrecisionRequirementType::error_value;
        requirement.value = std::pow(2.0F, -21);
      } else {
        requirement.type = PrecisionRequirementType::ulp_value;
        requirement.value = 3.0F;
      }
      requirements.emplace_back(requirement);
    };

    if constexpr (ca::is_vector_v<INPUT_TYPE_1>) {
      for (auto k = 0; k < INPUT_TYPE_1::vector_size; k++) {
        give_requirement(input_a[k]);
      }
    } else {
      give_requirement(input_a);
    }
    return requirements;
  }

  case Function::tanpi:
  case Function::atanh: {
    auto give_requirement = [&requirements, epsilon](scalar_type x) {
      PrecisionRequirement<scalar_type> requirement;
      if (x >= -1 && x <= 1) {
        requirement.type = PrecisionRequirementType::ulp_value;
        requirement.value = 8192.0F;
      } else {
        requirement.type = PrecisionRequirementType::undefined;
      }
      requirements.emplace_back(requirement);
    };

    if constexpr (ca::is_vector_v<INPUT_TYPE_1>) {
      for (auto k = 0; k < INPUT_TYPE_1::vector_size; k++) {
        give_requirement(input_a[k]);
      }
    } else {
      give_requirement(input_a);
    }
    return requirements;
  }

  case Function::sincos:
  case Function::sin:
  case Function::cos: {
    auto give_requirement = [&requirements](scalar_type x) {
      PrecisionRequirement<scalar_type> requirement;
      if (x >= -M_PI && x <= M_PI) {
        requirement.type = PrecisionRequirementType::error_value;
        requirement.value = std::pow(2.0F, -11);
      } else {
        requirement.type = PrecisionRequirementType::any;
      }
      requirements.emplace_back(requirement);
    };

    if constexpr (ca::is_vector_v<INPUT_TYPE_1>) {
      for (auto k = 0; k < INPUT_TYPE_1::vector_size; k++) {
        give_requirement(input_a[k]);
      }
    } else {
      give_requirement(input_a);
    }
    return requirements;
  }

  case Function::sinpi:
  case Function::cospi: {
    auto give_requirement = [&requirements](scalar_type x) {
      PrecisionRequirement<scalar_type> requirement;
      if (x >= -1 && x <= 1) {
        requirement.type = PrecisionRequirementType::error_value;
        requirement.value = std::pow(2.0F, -11);
      } else {
        requirement.type = PrecisionRequirementType::any;
      }
      requirements.emplace_back(requirement);
    };

    if constexpr (ca::is_vector_v<INPUT_TYPE_1>) {
      for (auto k = 0; k < INPUT_TYPE_1::vector_size; k++) {
        give_requirement(input_a[k]);
      }
    } else {
      give_requirement(input_a);
    }
    return requirements;
  }

  case Function::sinh:
  case Function::cosh: {
    auto give_requirement = [&requirements, epsilon](scalar_type x) {
      PrecisionRequirement<scalar_type> requirement;
      if (x >= -88 && x <= 88) {
        requirement.type = PrecisionRequirementType::ulp_value;
        requirement.value = 8192.0F;
      } else {
        requirement.type = PrecisionRequirementType::undefined;
      }
      requirements.emplace_back(requirement);
    };

    if constexpr (ca::is_vector_v<INPUT_TYPE_1>) {
      for (auto k = 0; k < INPUT_TYPE_1::vector_size; k++) {
        give_requirement(input_a[k]);
      }
    } else {
      give_requirement(input_a);
    }
    return requirements;
  }

  case Function::pow: {
    auto give_requirement = [&requirements, epsilon](scalar_type x,
                                                     scalar_type_t2 y) {
      PrecisionRequirement<scalar_type> requirement;
      scalar_type y_range = std::pow(2.0F, 24);

      if ((x == 0 && y == 0) || (x < 0 && std::floor(y) != y) ||
          (x < 0 && (y < -y_range || y > y_range))) {
        requirement.type = PrecisionRequirementType::undefined;
      } else {
        requirement.type = PrecisionRequirementType::ulp_value;
        requirement.value = 8192.0F;
      }
      requirements.emplace_back(requirement);
    };

    if constexpr (ca::is_vector_v<INPUT_TYPE_1>) {
      for (auto k = 0; k < INPUT_TYPE_1::vector_size; k++) {
        if constexpr (ca::is_vector_v<INPUT_TYPE_2>) {
          give_requirement(input_a[k], input_b[k]);
        } else {
          give_requirement(input_a[k], input_b);
        }
      }
    } else {
      give_requirement(input_a, input_b);
    }
    return requirements;
  }

  case Function::pown: {
    auto give_requirement = [&requirements, epsilon](scalar_type x,
                                                     scalar_type_t2 y) {
      PrecisionRequirement<scalar_type> requirement;
      scalar_type y_range = std::pow(2.0F, 24);

      if (std::floor(y) != y) {
        requirement.type = PrecisionRequirementType::undefined;
      } else {
        if (x == 0 && y == 0) {
          requirement.type = PrecisionRequirementType::undefined;
        } else {
          requirement.type = PrecisionRequirementType::ulp_value;
          requirement.value = 8192.0F;
        }
      }
      requirements.emplace_back(requirement);
    };

    if constexpr (ca::is_vector_v<INPUT_TYPE_1>) {
      for (auto k = 0; k < INPUT_TYPE_1::vector_size; k++) {
        if constexpr (ca::is_vector_v<INPUT_TYPE_2>) {
          give_requirement(input_a[k], input_b[k]);
        } else {
          give_requirement(input_a[k], input_b);
        }
      }
    } else {
      give_requirement(input_a, input_b);
    }
    return requirements;
  }

  case Function::rootn: {
    auto give_requirement = [&requirements, epsilon](scalar_type x,
                                                     scalar_type_t2 y) {
      PrecisionRequirement<scalar_type> requirement;
      if ((x > 0 && y != 0) || (x < 0 && (static_cast<int>(y) % 2 == 1)) ||
          (x == 0 && y > 0)) {
        requirement.type = PrecisionRequirementType::ulp_value;
        requirement.value = 8192.0F;
      } else {
        requirement.type = PrecisionRequirementType::undefined;
      }
      requirements.emplace_back(requirement);
    };

    if constexpr (ca::is_vector_v<INPUT_TYPE_1>) {
      for (auto k = 0; k < INPUT_TYPE_1::vector_size; k++) {
        if constexpr (ca::is_vector_v<INPUT_TYPE_2>) {
          give_requirement(input_a[k], input_b[k]);
        } else {
          give_requirement(input_a[k], input_b);
        }
      }
    } else {
      give_requirement(input_a, input_b);
    }
    return requirements;
  }

  case Function::powr: {
    auto give_requirement = [&requirements, epsilon](scalar_type x,
                                                     scalar_type_t2 y) {
      PrecisionRequirement<scalar_type> requirement;
      if (x >= 0) {
        if (x == 0 && y == 0) {
          requirement.type = PrecisionRequirementType::undefined;
        } else {
          requirement.type = PrecisionRequirementType::ulp_value;
          requirement.value = 8192.0F;
        }
      } else {
        requirement.type = PrecisionRequirementType::undefined;
      }
      requirements.emplace_back(requirement);
    };

    if constexpr (ca::is_vector_v<INPUT_TYPE_1>) {
      for (auto k = 0; k < INPUT_TYPE_1::vector_size; k++) {
        if constexpr (ca::is_vector_v<INPUT_TYPE_2>) {
          give_requirement(input_a[k], input_b[k]);
        } else {
          give_requirement(input_a[k], input_b);
        }
      }
    } else {
      give_requirement(input_a, input_b);
    }
    return requirements;
  }

  default:
    throw UnknownFunctionException(
        Catch::StringMaker<Function>::convert(function) + " uninitialized");
  }
}

template <typename T_1, typename T_2, typename T_3> struct Input {
  std::vector<T_1> input_a;
  std::vector<T_2> input_b;
  std::vector<T_3> input_c;
  Input(std::vector<T_1> input_a, std::vector<T_2> input_b,
        std::vector<T_3> input_c)
      : input_a(std::move(input_a)), input_b(std::move(input_b)),
        input_c(std::move(input_c)) {}
};

template <typename T>
void create_and_set_buffer(const std::vector<T> &input, ca::Runtime *runtime,
                           std::vector<ca::Buffer> *buffers,
                           const ca::Kernel &kernel) {
  const auto buffer = runtime->create_buffer(input.size() * sizeof(T));
  runtime->write_buffer(buffer, input.data());
  buffers->push_back(buffer);
  runtime->set_kernel_argument(kernel, buffers->size() - 1, buffer);
}

template <typename T, typename INPUT, typename INPUT_B, typename INPUT_C>
std::vector<T> run_gentype_kernel(const size_t &work_size, const INPUT &input,
                                  std::vector<INPUT_B> &argument_2_output,
                                  std::vector<INPUT_C> &argument_3_output,
                                  ca::Runtime *runtime,
                                  const ca::Kernel &kernel,
                                  const bool is_store) {
  std::vector<ca::Buffer> buffers;
  const auto buffer_out = runtime->create_buffer(work_size * sizeof(T));
  buffers.push_back(buffer_out);
  runtime->set_kernel_argument(kernel, 0, buffer_out);
  create_and_set_buffer(input.input_a, runtime, &buffers, kernel);
  if (!input.input_b.empty()) {
    create_and_set_buffer(input.input_b, runtime, &buffers, kernel);
  }
  if (!input.input_c.empty()) {
    create_and_set_buffer(input.input_c, runtime, &buffers, kernel);
  }
  runtime->run_kernel(kernel, work_size);
  auto kernel_output = runtime->read_buffer_to_vector<T>(buffer_out);
  if (!input.input_b.empty() && is_store) {
    argument_2_output = runtime->read_buffer_to_vector<INPUT_B>(buffers.at(2));
  }
  if (!input.input_c.empty() && is_store) {
    argument_3_output = runtime->read_buffer_to_vector<INPUT_C>(buffers.at(3));
  }

  for (const auto &buffer : buffers) {
    runtime->release_buffer(buffer);
  }
  return kernel_output;
}

template <typename GENTYPE, typename INPUT, typename INPUT_B, typename INPUT_C>
std::vector<GENTYPE>
test_gentype(const INPUT &input, std::vector<INPUT_B> &argument_2_output,
             std::vector<INPUT_C> &argument_3_output,
             const std::string &build_options, const TestConfig &config,
             const bool is_store) {
  auto *runtime = config.runtime();
  std::string path = is_store
                         ? "kernels/oclc_math_functions/math_functions_store.cl"
                         : "kernels/oclc_math_functions/math_functions.cl";
  const std::string source = ca::load_text_file(ca::get_asset(path));
  ca::Kernel kernel = runtime->create_kernel("test", source, build_options,
                                             config.program_type());
  auto output = run_gentype_kernel<GENTYPE, INPUT, INPUT_B>(
      config.work_size(), input, argument_2_output, argument_3_output, runtime,
      kernel, is_store);
  runtime->release_kernel(kernel);
  return output;
}

template <class T, typename INPUT>
void run_section(INPUT &input, const TestConfig &config) {
  using output_type = typename T::output_type;
  using input_a_type = typename T::input_type_1;
  using input_b_type = typename T::input_type_2;
  using input_c_type = typename T::input_type_3;
  using reference_type = replace_fp_with_double_t<output_type>;
  using reference_type2 = replace_fp_with_double_t<input_b_type>;
  using reference_type3 = replace_fp_with_double_t<input_c_type>;
  using scalar_type_1 = typename T::scalar_type_1;
  const auto work_size = config.work_size();

  auto reference_vector = std::vector<reference_type>(work_size);
  auto reference_vector_2 = std::vector<reference_type2>(
      work_size); // vector for reference values returned by function argument
  auto reference_vector_3 = std::vector<reference_type3>(work_size);
  for (auto j = 0; j < work_size; j++) {
    if constexpr (T::arg_num == 3) {
      reference_vector[j] = T::calculate_reference(
          input.input_a[j], input.input_b[j], input.input_c[j]);
      reference_vector_3[j] = static_cast<reference_type3>(input.input_c[j]);
    } else if constexpr (T::arg_num == 2) {
      reference_vector[j] =
          T::calculate_reference(input.input_a[j], input.input_b[j]);
      reference_vector_2[j] = static_cast<reference_type2>(input.input_b[j]);
    } else {
      reference_vector[j] = T::calculate_reference(input.input_a[j]);
    }
  }
  std::vector<input_b_type> argument_2_output(work_size);
  std::vector<input_c_type> argument_3_output(work_size);
  const std::string build_options = T::get_build_options();
  const auto result = test_gentype<output_type, INPUT, input_b_type>(
      input, argument_2_output, argument_3_output, build_options, config,
      T::get_is_store());
  if constexpr (T::get_is_native()) {
    SUCCEED();
    return;
  }
  REQUIRE_THAT(result, ca::UlpComparator(
                           result, reference_vector,
                           get_ulp_values<output_type>(T::function, work_size),
                           input.input_a, input.input_b, input.input_c));
  if (T::get_is_store() && T::arg_num == 2) {
    REQUIRE_THAT(
        argument_2_output,
        ca::UlpComparator(argument_2_output, reference_vector_2,
                          get_ulp_values<input_b_type>(T::function, work_size),
                          input.input_a, input.input_b, input.input_c));
  }
  if (T::get_is_store() && T::arg_num == 3) {
    REQUIRE_THAT(
        argument_3_output,
        ca::UlpComparator(argument_3_output, reference_vector_3,
                          get_ulp_values<input_c_type>(T::function, work_size),
                          input.input_a, input.input_b, input.input_c));
  }
}

template <typename, typename = void>
struct has_derived_check_method : std::false_type {};

template <typename T>
struct has_derived_check_method<
    T, decltype(void(T::calculate_reference_derived_check(
           std::declval<typename T::input_type_1>())))> : std::true_type {};

template <typename T>
constexpr bool has_derived_check_method_v = has_derived_check_method<T>::value;

template <class T, typename INPUT>
void run_section_relaxed(INPUT &input, const TestConfig &config) {
  using output_type = typename T::output_type;
  using scalar_output_type = typename T::scalar_output_type;
  using input_a_type = typename T::input_type_1;
  using input_b_type = typename T::input_type_2;
  using input_c_type = typename T::input_type_3;
  using scalar_type_1 = typename T::scalar_type_1;
  using scalar_type_2 = typename T::scalar_type_2;
  const auto work_size = config.work_size();

  // Calculate references from both implementations
  auto reference_vector_standard =
      std::vector<replace_fp_with_double_t<output_type>>(work_size);
  auto reference_vector_derived_check =
      std::vector<replace_fp_with_double_t<output_type>>(work_size);

  auto reference_vector_2 = std::vector<input_b_type>(work_size);
  auto reference_vector_3 = std::vector<input_c_type>(work_size);

  for (auto j = 0; j < work_size; j++) {
    if constexpr (T::arg_num == 3) {
      // Use T::calculate_reference for standard implementation
      reference_vector_standard[j] = T::calculate_reference(
          input.input_a[j], input.input_b[j], input.input_c[j]);
      // Use impl version if it exists
      if constexpr (has_derived_check_method_v<T>) {
        reference_vector_derived_check[j] =
            T::calculate_reference_derived_check(
                input.input_a[j], input.input_b[j], input.input_c[j]);
      } else {
        // Fallback to standard if impl doesn't exist
        reference_vector_derived_check[j] = reference_vector_standard[j];
      }
      reference_vector_3[j] = input.input_c[j];
    } else if constexpr (T::arg_num == 2) {
      reference_vector_standard[j] =
          T::calculate_reference(input.input_a[j], input.input_b[j]);
      if constexpr (has_derived_check_method_v<T>) {
        reference_vector_derived_check[j] =
            T::calculate_reference_derived_check(input.input_a[j],
                                                 input.input_b[j]);
      } else {
        reference_vector_derived_check[j] = reference_vector_standard[j];
      }
      reference_vector_2[j] = input.input_b[j];
    } else {
      reference_vector_standard[j] = T::calculate_reference(input.input_a[j]);
      if constexpr (has_derived_check_method_v<T>) {
        reference_vector_derived_check[j] =
            T::calculate_reference_derived_check(input.input_a[j]);
      } else {
        reference_vector_derived_check[j] = reference_vector_standard[j];
      }
    }
  }

  std::vector<input_b_type> argument_2_output(work_size);
  std::vector<input_c_type> argument_3_output(work_size);
  const std::string build_options = T::get_build_options_relaxed();

  const auto result = test_gentype<output_type, INPUT, input_b_type>(
      input, argument_2_output, argument_3_output, build_options, config,
      T::get_is_store());

  if constexpr (T::get_is_native()) {
    return;
  }

  for (size_t i = 0; i < work_size; i++) {
    std::vector<ca::PrecisionRequirement<scalar_output_type>> req =
        requirements_function<output_type>(T::function, input.input_a[i],
                                           input.input_b[i]);

    // Try comparison with standard reference first
    bool standard_comparison_passed = false;
    try {
      ca::logging::debug() << "Trying standard comparison for index " << i
                           << '\n';
      if constexpr (T::arg_num == 1) {
        auto comparator = ca::PrecisionComparator(
            result[i], reference_vector_standard[i], req, input.input_a[i]);
        standard_comparison_passed = comparator.match(result[i]);
      } else {
        auto comparator =
            ca::PrecisionComparator(result[i], reference_vector_standard[i],
                                    req, input.input_a[i], input.input_b[i]);
        standard_comparison_passed = comparator.match(result[i]);
      }
      ca::logging::debug() << "Standard comparison result: "
                           << (standard_comparison_passed ? "PASSED" : "FAILED")
                           << '\n';
    } catch (const std::exception &e) {
      ca::logging::info() << "Exception in standard comparison: " << e.what()
                          << '\n';
      standard_comparison_passed = false;
    } catch (...) {
      ca::logging::info() << "Unknown exception in standard comparison\n";
      standard_comparison_passed = false;
    }

    if (!standard_comparison_passed) {
      if constexpr (T::arg_num == 1) {
        REQUIRE_THAT(result[i],
                     ca::PrecisionComparator(result[i],
                                             reference_vector_derived_check[i],
                                             req, input.input_a[i]));
      } else {
        REQUIRE_THAT(result[i],
                     ca::PrecisionComparator(
                         result[i], reference_vector_derived_check[i], req,
                         input.input_a[i], input.input_b[i]));
      }
    } else {
      // Standard comparison passed, explicitly succeed
      SUCCEED();
    }
  }
}

template <typename T>
std::vector<T> create_input_vector(const T &input, const uint32_t &arg_index,
                                   const uint32_t &arg_num,
                                   const size_t &work_size) {
  if (arg_num < arg_index + 1) {
    return std::vector<T>();
  }
  std::vector<T> input_vector(work_size);
  std::fill(input_vector.begin(), input_vector.end(), input);
  return input_vector;
}

template <class T, SectionType section_type>
void run_specific_section(const std::vector<Value<T>> &input_values) {
  const auto function_string = T::get_function_string();
  const auto build_options = T::get_build_options();
  const TestConfig &config = get_test_config();
  const auto work_size = config.work_size();
  using input_type_1 = typename T::input_type_1;
  using input_type_2 = typename T::input_type_2;
  using input_type_3 = typename T::input_type_3;
  for (auto i = 0U; i < input_values.size(); i++) {
    const auto section_name = create_section_name<section_type>(
        function_string, i, input_values.size());
    SECTION(section_name) {
      auto input_a = create_input_vector<input_type_1>(
          input_values[i].get_val_1(), 0, T::arg_num, work_size);
      auto input_b = create_input_vector<input_type_2>(
          input_values[i].get_val_2(), 1, T::arg_num, work_size);
      auto input_c = create_input_vector<input_type_3>(
          input_values[i].get_val_3(), 2, T::arg_num, work_size);
      if (section_type == SectionType::random) {
        input_a = randomize_input(input_a);
        input_b = randomize_input(input_b);
        input_c = randomize_input(input_c);
      }
      ca::logging::debug() << "Build options: " << build_options << '\n';
      ca::logging::debug() << "Input A: " << ca::to_string(input_a) << '\n';
      ca::logging::debug() << "Input B: " << ca::to_string(input_b) << '\n';
      ca::logging::debug() << "Input C: " << ca::to_string(input_c) << '\n';
      auto input = Input(input_a, input_b, input_c);
      run_section<T, decltype(input)>(input, config);
    }
  }
}

template <class T, SectionType section_type>
void run_specific_section_relaxed(const std::vector<Value<T>> &input_values) {
  const auto function_string = T::get_function_string();
  const auto build_options = T::get_build_options_relaxed();

  const TestConfig &config = get_test_config();
  const auto work_size = config.work_size();
  using input_type_1 = typename T::input_type_1;
  using input_type_2 = typename T::input_type_2;
  using input_type_3 = typename T::input_type_3;
  for (auto i = 0U; i < input_values.size(); i++) {
    const auto section_name = create_section_name<section_type>(
        function_string, i, input_values.size());
    SECTION(section_name) {
      auto input_a = create_input_vector<input_type_1>(
          input_values[i].get_val_1(), 0, T::arg_num, work_size);
      auto input_b = create_input_vector<input_type_2>(
          input_values[i].get_val_2(), 1, T::arg_num, work_size);
      auto input_c = create_input_vector<input_type_3>(
          input_values[i].get_val_3(), 2, T::arg_num, work_size);
      if (section_type == SectionType::random) {
        input_a = randomize_input(input_a);
        input_b = randomize_input(input_b);
        input_c = randomize_input(input_c);
      }
      ca::logging::debug() << "Build options: " << build_options << '\n';
      ca::logging::debug() << "Input A: " << ca::to_string(input_a) << '\n';
      ca::logging::debug() << "Input B: " << ca::to_string(input_b) << '\n';
      ca::logging::debug() << "Input C: " << ca::to_string(input_c) << '\n';
      auto input = Input(input_a, input_b, input_c);
      run_section_relaxed<T, decltype(input)>(input, config);
    }
  }
}

template <class T> void run_multiple_test_sections() {
  const auto input = get_gentype_values<T>();
  run_specific_section<T, SectionType::random>(input.random_values);
  run_specific_section<T, SectionType::edge>(input.edge_case_values);
}

template <class T> void run_multiple_test_sections_relaxed() {
  const auto input = get_gentype_values<T>();
  run_specific_section_relaxed<T, SectionType::random>(input.random_values);
  run_specific_section_relaxed<T, SectionType::edge>(input.edge_case_values);
}

#endif
