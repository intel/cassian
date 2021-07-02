/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/catch2_utils/catch2_utils.hpp>
#include <cassian/vector/type_helpers.hpp>
#include <catch2/catch.hpp>
#include <cfloat>
#include <common.hpp>
#include <enum_definitions.hpp>
#include <test_config.hpp>
#include <type_traits>
#include <unordered_map>

namespace {

const std::unordered_map<FloatMacros, float> reference_float_map = {
    {FloatMacros::flt_dig, FLT_DIG},
    {FloatMacros::flt_mant_dig, FLT_MANT_DIG},
    {FloatMacros::flt_max_10_exp, FLT_MAX_10_EXP},
    {FloatMacros::flt_max_exp, FLT_MAX_EXP},
    {FloatMacros::flt_min_exp, FLT_MIN_EXP},
    {FloatMacros::flt_radix, FLT_RADIX},
    {FloatMacros::flt_max, FLT_MAX},
    {FloatMacros::flt_min, FLT_MIN},
    {FloatMacros::flt_epsilon, FLT_EPSILON}};

const std::unordered_map<FloatMacros, double> reference_double_map = {
    {FloatMacros::dbl_dig, DBL_DIG},
    {FloatMacros::dbl_mant_dig, DBL_MANT_DIG},
    {FloatMacros::dbl_max_10_exp, DBL_MAX_10_EXP},
    {FloatMacros::dbl_max_exp, DBL_MAX_EXP},
    {FloatMacros::dbl_min_exp, DBL_MIN_EXP},
    {FloatMacros::dbl_max, DBL_MAX},
    {FloatMacros::dbl_min, DBL_MIN},
    {FloatMacros::dbl_epsilon, DBL_EPSILON}};

template <typename T> constexpr auto get_reference_map() {
  if constexpr (std::is_same<double, T>::value) {
    return reference_double_map;
  } else if constexpr (std::is_same<float, T>::value) {
    return reference_float_map;
  } else {
    throw UnknownTypeException("Unknown test type");
  }
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("math_macros", "", FloatingPointScalarTypes,
                                    test_name<TestType>) {
  const auto &config = get_test_config();
  const auto &reference_map = get_reference_map<typename TestType::host_type>();
  run_template_enum_test_case<TestType, FloatMacros>(config, reference_map,
                                                     "math_macros");
}

} // namespace
