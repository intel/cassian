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
using sin =
    OclcFunction<Function::sin, 1, calculate_sin<typename T::host_type>, T>;
template <typename T>
using sinh =
    OclcFunction<Function::sinh, 1, calculate_sinh<typename T::host_type>, T>;
template <typename T>
using sinpi =
    OclcFunction<Function::sinpi, 1, calculate_sinpi<typename T::host_type>, T>;
template <typename T>
using tan =
    OclcFunction<Function::tan, 1, calculate_tan<typename T::host_type>, T>;
template <typename T>
using tanh =
    OclcFunction<Function::tanh, 1, calculate_tanh<typename T::host_type>, T>;
template <typename T>
using tanpi =
    OclcFunction<Function::tanpi, 1, calculate_tanpi<typename T::host_type>, T>;
template <typename T>
using tgamma = OclcFunction<Function::tgamma, 1,
                            calculate_tgamma<typename T::host_type>, T>;
template <typename T>
using trunc =
    OclcFunction<Function::trunc, 1, calculate_trunc<typename T::host_type>, T>;
template <typename T>
using erfc =
    OclcFunction<Function::erfc, 1, calculate_erfc<typename T::host_type>, T>;
template <typename T>
using erf =
    OclcFunction<Function::erf, 1, calculate_erf<typename T::host_type>, T>;
template <typename T>
using exp =
    OclcFunction<Function::exp, 1, calculate_exp<typename T::host_type>, T>;
template <typename T>
using exp2 =
    OclcFunction<Function::exp2, 1, calculate_exp2<typename T::host_type>, T>;
template <typename T>
using exp10 =
    OclcFunction<Function::exp10, 1, calculate_exp10<typename T::host_type>, T>;
template <typename T>
using expm1 =
    OclcFunction<Function::expm1, 1, calculate_expm1<typename T::host_type>, T>;
template <typename T>
using fabs =
    OclcFunction<Function::fabs, 1, calculate_fabs<typename T::host_type>, T>;
template <typename T>
using fdim =
    OclcFunction<Function::fdim, 2, calculate_fdim<typename T::host_type>, T>;
template <typename T>
using floor =
    OclcFunction<Function::floor, 1, calculate_floor<typename T::host_type>, T>;
template <typename T>
using fma =
    OclcFunction<Function::fma, 3, calculate_fma<typename T::host_type>, T>;
template <typename T>
using fmax =
    OclcFunction<Function::fmax, 2, calculate_fmax<typename T::host_type>, T>;
template <typename T>
using fmin =
    OclcFunction<Function::fmin, 2, calculate_fmin<typename T::host_type>, T>;
template <typename T>
using fmod =
    OclcFunction<Function::fmod, 2, calculate_fmod<typename T::host_type>, T>;
template <typename T>
using hypot =
    OclcFunction<Function::hypot, 2, calculate_hypot<typename T::host_type>, T>;
template <typename T>
using lgamma = OclcFunction<Function::lgamma, 1,
                            calculate_lgamma<typename T::host_type>, T>;

using Gentype =
    ca::TupleConcat<ca::TypesFloat, ca::TypesDouble, ca::TypesHalf>::type;

using GentypeFunctions =
    FunctionProduct<Gentype, sin, sinh, sinpi, tan, tanh, tanpi, tgamma, trunc,
                    erfc, erf, exp, exp2, exp10, expm1, fabs, fdim, floor, fma,
                    fmax, fmin, fmod, hypot, lgamma>::type;

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("functions", "", GentypeFunctions,
                                    test_name_with_function<TestType>) {
  const TestConfig &config = get_test_config();
  using scalar_type = typename TestType::clc_output_type::scalar_type;
  ca::Requirements requirements;
  requirements.arithmetic_type<scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  run_multiple_test_sections(TestType());
}

} // namespace
