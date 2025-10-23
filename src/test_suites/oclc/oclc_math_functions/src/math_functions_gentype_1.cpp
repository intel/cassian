/*
 * Copyright (C) 2024 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

// Enable math defines from cmath module
#define _USE_MATH_DEFINES

#include <algorithm>
#include <cassian/catch2_utils/catch2_utils.hpp>
#include <cassian/runtime/openclc_type_tuples.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cassian/test_harness/test_harness.hpp>
#include <cassian/utility/metaprogramming.hpp>
#include <cassian/utility/utility.hpp>
#include <catch2/catch.hpp>
#include <common.hpp>
#include <cstddef>
#include <cstdint>
#include <enum_definitions.hpp>
#include <math_functions.hpp>
#include <string>
#include <test_config.hpp>
#include <vector>
namespace ca = cassian;

namespace {

template <typename T>
using acos =
    OclcFunction<Function::acos, 1, calculate_acos<typename T::host_type>, T>;
template <typename T>
using acosh =
    OclcFunction<Function::acosh, 1, calculate_acosh<typename T::host_type>, T>;

template <typename T>
using acospi = OclcFunction<Function::acospi, 1,
                            calculate_acospi<typename T::host_type>, T>;
template <typename T>
using asin =
    OclcFunction<Function::asin, 1, calculate_asin<typename T::host_type>, T>;
template <typename T>
using asinh =
    OclcFunction<Function::asinh, 1, calculate_asinh<typename T::host_type>, T>;
template <typename T>
using asinpi = OclcFunction<Function::asinpi, 1,
                            calculate_asinpi<typename T::host_type>, T>;
template <typename T>
using atan =
    OclcFunction<Function::atan, 1, calculate_atan<typename T::host_type>, T>;
template <typename T>
using atan2 =
    OclcFunction<Function::atan2, 2, calculate_atan2<typename T::host_type>, T>;
template <typename T>
using atanh =
    OclcFunction<Function::atanh, 1, calculate_atanh<typename T::host_type>, T>;
template <typename T>
using atanpi = OclcFunction<Function::atanpi, 1,
                            calculate_atanpi<typename T::host_type>, T>;
template <typename T>
using atan2pi = OclcFunction<Function::atan2pi, 2,
                             calculate_atan2pi<typename T::host_type>, T>;
template <typename T>
using cbrt =
    OclcFunction<Function::cbrt, 1, calculate_cbrt<typename T::host_type>, T>;
namespace internal {
template <typename T>
using ceil =
    OclcFunction<Function::ceil, 1, calculate_ceil<typename T::host_type>, T>;
}
template <typename T>
using copysign = OclcFunction<Function::copysign, 2,
                              calculate_copysign<typename T::host_type>, T>;
template <typename T>
using cos =
    OclcFunction<Function::cos, 1, calculate_cos<typename T::host_type>, T>;
template <typename T>
using cosh =
    OclcFunction<Function::cosh, 1, calculate_cosh<typename T::host_type>, T>;
template <typename T>
using cospi =
    OclcFunction<Function::cospi, 1, calculate_cospi<typename T::host_type>, T>;
template <typename T>
using sqrt =
    OclcFunction<Function::sqrt, 1, calculate_sqrt<typename T::host_type>, T>;
template <typename T>
using mad =
    OclcFunction<Function::mad, 3, calculate_mad<typename T::host_type>, T>;
template <typename T>
using maxmag = OclcFunction<Function::maxmag, 2,
                            calculate_maxmag<typename T::host_type>, T>;
template <typename T>
using minmag = OclcFunction<Function::minmag, 2,
                            calculate_minmag<typename T::host_type>, T>;
template <typename T>
using nextafter = OclcFunction<Function::nextafter, 2,
                               calculate_nextafter<typename T::host_type>, T>;
template <typename T>
using pow =
    OclcFunction<Function::pow, 2, calculate_pow<typename T::host_type>, T>;
template <typename T>
using remainder = OclcFunction<Function::remainder, 2,
                               calculate_remainder<typename T::host_type>, T>;
template <typename T>
using rint =
    OclcFunction<Function::rint, 1, calculate_rint<typename T::host_type>, T>;
template <typename T>
using round =
    OclcFunction<Function::round, 1, calculate_round<typename T::host_type>, T>;
template <typename T>
using rsqrt =
    OclcFunction<Function::rsqrt, 1, calculate_rsqrt<typename T::host_type>, T>;

using Gentype =
    ca::TupleConcat<ca::TypesFloat, ca::TypesDouble, ca::TypesHalf>::type;

using GentypeFunctions =
    FunctionProduct<Gentype, acos, acosh, acospi, asin, asinh, asinpi, atan,
                    atan2, atanh, atanpi, atan2pi, cbrt, internal::ceil,
                    copysign, cos, cosh, cospi, sqrt, mad, maxmag, minmag,
                    nextafter, pow, remainder, rint, round, rsqrt>::type;

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("functions", "", GentypeFunctions,
                                    test_name_with_function<TestType>) {
  const TestConfig &config = get_test_config();
  using scalar_type = typename TestType::clc_output_type::scalar_type;
  ca::Requirements requirements;
  requirements.arithmetic_type<scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  run_multiple_test_sections<TestType>();
}

} // namespace
