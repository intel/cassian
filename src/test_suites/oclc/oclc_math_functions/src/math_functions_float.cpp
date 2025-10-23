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
using native_cos = OclcFunction<Function::native_cos, 1,
                                calculate_cos<typename T::host_type>, T>;
template <typename T>
using native_divide = OclcFunction<Function::native_divide, 2,
                                   calculate_divide<typename T::host_type>, T>;
template <typename T>
using native_exp = OclcFunction<Function::native_exp, 1,
                                calculate_exp<typename T::host_type>, T>;
template <typename T>
using native_exp2 = OclcFunction<Function::native_exp2, 1,
                                 calculate_exp2<typename T::host_type>, T>;
template <typename T>
using native_exp10 = OclcFunction<Function::native_exp10, 1,
                                  calculate_exp10<typename T::host_type>, T>;
template <typename T>
using native_log = OclcFunction<Function::native_log, 1,
                                calculate_log<typename T::host_type>, T>;
template <typename T>
using native_log2 = OclcFunction<Function::native_log2, 1,
                                 calculate_log2<typename T::host_type>, T>;
template <typename T>
using native_log10 = OclcFunction<Function::native_log10, 1,
                                  calculate_log10<typename T::host_type>, T>;
template <typename T>
using native_powr = OclcFunction<Function::native_powr, 2,
                                 calculate_pow<typename T::host_type>, T>;
template <typename T>
using native_recip = OclcFunction<Function::native_recip, 1,
                                  calculate_recip<typename T::host_type>, T>;
template <typename T>
using native_rsqrt = OclcFunction<Function::native_rsqrt, 1,
                                  calculate_rsqrt<typename T::host_type>, T>;
template <typename T>
using native_sin = OclcFunction<Function::native_sin, 1,
                                calculate_sin<typename T::host_type>, T>;
template <typename T>
using native_sqrt = OclcFunction<Function::native_sqrt, 1,
                                 calculate_sqrt<typename T::host_type>, T>;
template <typename T>
using native_tan = OclcFunction<Function::native_tan, 1,
                                calculate_tan<typename T::host_type>, T>;
template <typename T>
using half_cos = OclcFunction<Function::half_cos, 1,
                              calculate_cos<typename T::host_type>, T>;
template <typename T>
using half_divide = OclcFunction<Function::half_divide, 2,
                                 calculate_divide<typename T::host_type>, T>;
template <typename T>
using half_exp = OclcFunction<Function::half_exp, 1,
                              calculate_exp<typename T::host_type>, T>;
template <typename T>
using half_exp2 = OclcFunction<Function::half_exp2, 1,
                               calculate_exp2<typename T::host_type>, T>;
template <typename T>
using half_exp10 = OclcFunction<Function::half_exp10, 1,
                                calculate_exp10<typename T::host_type>, T>;
template <typename T>
using half_log = OclcFunction<Function::half_log, 1,
                              calculate_log<typename T::host_type>, T>;
template <typename T>
using half_log2 = OclcFunction<Function::half_log2, 1,
                               calculate_log2<typename T::host_type>, T>;
template <typename T>
using half_log10 = OclcFunction<Function::half_log10, 1,
                                calculate_log10<typename T::host_type>, T>;
template <typename T>
using half_powr = OclcFunction<Function::half_powr, 2,
                               calculate_powr<typename T::host_type>, T>;
template <typename T>
using half_recip = OclcFunction<Function::half_recip, 1,
                                calculate_recip<typename T::host_type>, T>;
template <typename T>
using half_rsqrt = OclcFunction<Function::half_rsqrt, 1,
                                calculate_rsqrt<typename T::host_type>, T>;
template <typename T>
using half_sin = OclcFunction<Function::half_sin, 1,
                              calculate_sin<typename T::host_type>, T>;
template <typename T>
using half_sqrt = OclcFunction<Function::half_sqrt, 1,
                               calculate_sqrt<typename T::host_type>, T>;
template <typename T>
using half_tan = OclcFunction<Function::half_tan, 1,
                              calculate_tan<typename T::host_type>, T>;

using FloatFunctions =
    FunctionProduct<ca::TypesFloat, native_cos, native_divide, native_exp,
                    native_exp2, native_exp10, native_log, native_log2,
                    native_log10, native_powr, native_recip, native_rsqrt,
                    native_sin, native_sqrt, native_tan, half_cos, half_divide,
                    half_exp, half_exp2, half_exp10, half_log, half_log2,
                    half_log10, half_powr, half_recip, half_rsqrt, half_sin,
                    half_sqrt, half_tan>::type;

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("functions", "", FloatFunctions,
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

template <typename T>
using sqrt_cr = OclcFunction<Function::correctly_rounded_sqrt, 1,
                             calculate_sqrt<typename T::host_type>, T>;

using CRFunctions = FunctionProduct<ca::TypesFloat, sqrt_cr>::type;

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("functions", "", CRFunctions,
                                    test_name_with_function<TestType>) {
  const TestConfig &config = get_test_config();
  using scalar_type = typename TestType::clc_output_type::scalar_type;
  ca::Requirements requirements;
  requirements.arithmetic_type<scalar_type>();
  requirements.correctly_rounded_divide_sqrt<scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  run_multiple_test_sections<TestType>();
}

} // namespace
