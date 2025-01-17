/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/catch2_utils/catch2_utils.hpp>
#include <cassian/runtime/openclc_type_tuples.hpp>
#include <catch2/catch.hpp>
#include <common.hpp>
#include <string>
#include <tuple>
#include <utility.hpp>

namespace {

const std::string program = "kernels/oclc_conversions/explicit_conversion.cl";

using Vector3IntegerToVector3IntegerTestCases = rounding_mode_product_t<
    overflow_handling_product_t<
        tuples_to_test_cases_t<ca::Vector3IntegerToVector3Integer>,
        OverflowHandling::overflow_default>,
    RoundingMode::round_default, RoundingMode::round_to_nearest_even,
    RoundingMode::round_toward_zero,
    RoundingMode::round_toward_positive_infinity,
    RoundingMode::round_toward_negative_infinity>;
using Vector3IntegerToVector3IntegerSaturationTestCases =
    rounding_mode_product_t<
        overflow_handling_product_t<
            tuples_to_test_cases_t<ca::Vector3IntegerToVector3Integer>,
            OverflowHandling::overflow_saturation>,
        RoundingMode::round_default, RoundingMode::round_to_nearest_even,
        RoundingMode::round_toward_zero,
        RoundingMode::round_toward_positive_infinity,
        RoundingMode::round_toward_negative_infinity>;
using Vector3IntegerToVector3FloatingPointTestCases = rounding_mode_product_t<
    overflow_handling_product_t<
        tuples_to_test_cases_t<ca::Vector3IntegerToVector3FloatingPoint>,
        OverflowHandling::overflow_default>,
    RoundingMode::round_default, RoundingMode::round_to_nearest_even,
    RoundingMode::round_toward_zero,
    RoundingMode::round_toward_positive_infinity,
    RoundingMode::round_toward_negative_infinity>;
using Vector3FloatingPointToVector3IntegerTestCases = rounding_mode_product_t<
    overflow_handling_product_t<
        tuples_to_test_cases_t<ca::Vector3FloatingPointToVector3Integer>,
        OverflowHandling::overflow_default,
        OverflowHandling::overflow_saturation>,
    RoundingMode::round_default, RoundingMode::round_to_nearest_even,
    RoundingMode::round_toward_zero,
    RoundingMode::round_toward_positive_infinity,
    RoundingMode::round_toward_negative_infinity>;
using Vector3FloatingPointToVector3FloatingPointTestCases =
    rounding_mode_product_t<
        overflow_handling_product_t<
            tuples_to_test_cases_t<
                ca::Vector3FloatingPointToVector3FloatingPoint>,
            OverflowHandling::overflow_default>,
        RoundingMode::round_default, RoundingMode::round_to_nearest_even,
        RoundingMode::round_toward_zero,
        RoundingMode::round_toward_positive_infinity,
        RoundingMode::round_toward_negative_infinity>;

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("explicit conversion", "",
                                    Vector3IntegerToVector3IntegerTestCases,
                                    explicit_conversion_test_name<TestType>) {
  vector_to_vector<TestType, TestVariant::conversion>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "explicit conversion", "",
    Vector3IntegerToVector3IntegerSaturationTestCases,
    explicit_conversion_test_name<TestType>) {
  vector_to_vector<TestType, TestVariant::conversion>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "explicit conversion", "", Vector3IntegerToVector3FloatingPointTestCases,
    explicit_conversion_test_name<TestType>) {
  vector_to_vector<TestType, TestVariant::conversion>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "explicit conversion", "", Vector3FloatingPointToVector3IntegerTestCases,
    explicit_conversion_test_name<TestType>) {
  vector_to_vector<TestType, TestVariant::conversion>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "explicit conversion", "",
    Vector3FloatingPointToVector3FloatingPointTestCases,
    explicit_conversion_test_name<TestType>) {
  vector_to_vector<TestType, TestVariant::conversion>(program);
}

} // namespace
