/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

// Enable math defines from cmath module
#define _USE_MATH_DEFINES

#include <cassian/catch2_utils/catch2_utils.hpp>
#include <catch2/catch.hpp>
#include <cmath>
#include <common.hpp>
#include <enum_definitions.hpp>
#include <limits>
#include <test_config.hpp>
#include <type_traits>
#include <unordered_map>

namespace {

const std::unordered_map<FloatConstants, float> reference_float_map = {
    {FloatConstants::m_e_f, M_E},
    {FloatConstants::m_log2e_f, M_LOG2E},
    {FloatConstants::m_log10e_f, M_LOG10E},
    {FloatConstants::m_ln2_f, M_LN2},
    {FloatConstants::m_ln10_f, M_LN10},
    {FloatConstants::m_pi_f, M_PI},
    {FloatConstants::m_pi_2_f, M_PI_2},
    {FloatConstants::m_pi_4_f, M_PI_4},
    {FloatConstants::m_1_pi_f, M_1_PI},
    {FloatConstants::m_2_pi_f, M_2_PI},
    {FloatConstants::m_2_sqrtpi_f, M_2_SQRTPI},
    {FloatConstants::m_sqrt2_f, M_SQRT2},
    {FloatConstants::m_sqrt1_2_f, M_SQRT1_2},
    {FloatConstants::maxfloat, std::numeric_limits<float>::max()},
    {FloatConstants::huge_valf, HUGE_VALF},
    {FloatConstants::infinity, INFINITY}};

const std::unordered_map<FloatConstants, double> reference_double_map = {
    {FloatConstants::m_e, M_E},
    {FloatConstants::m_log2e, M_LOG2E},
    {FloatConstants::m_log10e, M_LOG10E},
    {FloatConstants::m_ln2, M_LN2},
    {FloatConstants::m_ln10, M_LN10},
    {FloatConstants::m_pi, M_PI},
    {FloatConstants::m_pi_2, M_PI_2},
    {FloatConstants::m_pi_4, M_PI_4},
    {FloatConstants::m_1_pi, M_1_PI},
    {FloatConstants::m_2_pi, M_2_PI},
    {FloatConstants::m_2_sqrtpi, M_2_SQRTPI},
    {FloatConstants::m_sqrt2, M_SQRT2},
    {FloatConstants::m_sqrt1_2, M_SQRT1_2},
    {FloatConstants::huge_val, HUGE_VAL}};

template <typename T> constexpr auto get_reference_map() {
  if constexpr (std::is_same<double, T>::value) {
    return reference_double_map;
  } else if constexpr (std::is_same<float, T>::value) {
    return reference_float_map;
  } else {
    throw UnknownTypeException("Unknown test type");
  }
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("math_constants", "",
                                    FloatingPointScalarTypes,
                                    test_name<TestType>) {
  const auto &config = get_test_config();
  const auto &reference_map = get_reference_map<typename TestType::host_type>();
  run_template_enum_test_case<TestType, FloatConstants>(config, reference_map,
                                                        "math_constants");
}

} // namespace
