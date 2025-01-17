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

using Vector2IntegerToVector2IntegerTestCases = rounding_mode_product_t<
    overflow_handling_product_t<
        tuples_to_test_cases_t<ca::Vector2IntegerToVector2Integer>,
        OverflowHandling::overflow_default>,
    RoundingMode::round_default, RoundingMode::round_to_nearest_even,
    RoundingMode::round_toward_zero,
    RoundingMode::round_toward_positive_infinity,
    RoundingMode::round_toward_negative_infinity>;
using Vector2IntegerToVector2IntegerSaturationTestCases =
    rounding_mode_product_t<
        overflow_handling_product_t<
            tuples_to_test_cases_t<ca::Vector2IntegerToVector2Integer>,
            OverflowHandling::overflow_saturation>,
        RoundingMode::round_default, RoundingMode::round_to_nearest_even,
        RoundingMode::round_toward_zero,
        RoundingMode::round_toward_positive_infinity,
        RoundingMode::round_toward_negative_infinity>;
using Vector2IntegerToVector2FloatingPointTestCases = rounding_mode_product_t<
    overflow_handling_product_t<
        tuples_to_test_cases_t<ca::Vector2IntegerToVector2FloatingPoint>,
        OverflowHandling::overflow_default>,
    RoundingMode::round_default, RoundingMode::round_to_nearest_even,
    RoundingMode::round_toward_zero,
    RoundingMode::round_toward_positive_infinity,
    RoundingMode::round_toward_negative_infinity>;
using Vector2FloatingPointToVector2IntegerTestCases = rounding_mode_product_t<
    overflow_handling_product_t<
        tuples_to_test_cases_t<ca::Vector2FloatingPointToVector2Integer>,
        OverflowHandling::overflow_default,
        OverflowHandling::overflow_saturation>,
    RoundingMode::round_default, RoundingMode::round_to_nearest_even,
    RoundingMode::round_toward_zero,
    RoundingMode::round_toward_positive_infinity,
    RoundingMode::round_toward_negative_infinity>;
using Vector2FloatingPointToVector2FloatingPointTestCases =
    rounding_mode_product_t<
        overflow_handling_product_t<
            tuples_to_test_cases_t<
                ca::Vector2FloatingPointToVector2FloatingPoint>,
            OverflowHandling::overflow_default>,
        RoundingMode::round_default, RoundingMode::round_to_nearest_even,
        RoundingMode::round_toward_zero,
        RoundingMode::round_toward_positive_infinity,
        RoundingMode::round_toward_negative_infinity>;

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("explicit conversion", "",
                                    Vector2IntegerToVector2IntegerTestCases,
                                    explicit_conversion_test_name<TestType>) {
  vector_to_vector<TestType, TestVariant::conversion>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "explicit conversion", "",
    Vector2IntegerToVector2IntegerSaturationTestCases,
    explicit_conversion_test_name<TestType>) {
  vector_to_vector<TestType, TestVariant::conversion>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "explicit conversion", "", Vector2IntegerToVector2FloatingPointTestCases,
    explicit_conversion_test_name<TestType>) {
  vector_to_vector<TestType, TestVariant::conversion>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "explicit conversion", "", Vector2FloatingPointToVector2IntegerTestCases,
    explicit_conversion_test_name<TestType>) {
  vector_to_vector<TestType, TestVariant::conversion>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "explicit conversion", "",
    Vector2FloatingPointToVector2FloatingPointTestCases,
    explicit_conversion_test_name<TestType>) {
  vector_to_vector<TestType, TestVariant::conversion>(program);
}

} // namespace
