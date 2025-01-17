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

using Vector8IntegerToVector8IntegerTestCases = rounding_mode_product_t<
    overflow_handling_product_t<
        tuples_to_test_cases_t<ca::Vector8IntegerToVector8Integer>,
        OverflowHandling::overflow_default>,
    RoundingMode::round_default, RoundingMode::round_to_nearest_even,
    RoundingMode::round_toward_zero,
    RoundingMode::round_toward_positive_infinity,
    RoundingMode::round_toward_negative_infinity>;
using Vector8IntegerToVector8IntegerSaturationTestCases =
    rounding_mode_product_t<
        overflow_handling_product_t<
            tuples_to_test_cases_t<ca::Vector8IntegerToVector8Integer>,
            OverflowHandling::overflow_saturation>,
        RoundingMode::round_default, RoundingMode::round_to_nearest_even,
        RoundingMode::round_toward_zero,
        RoundingMode::round_toward_positive_infinity,
        RoundingMode::round_toward_negative_infinity>;
using Vector8IntegerToVector8FloatingPointTestCases = rounding_mode_product_t<
    overflow_handling_product_t<
        tuples_to_test_cases_t<ca::Vector8IntegerToVector8FloatingPoint>,
        OverflowHandling::overflow_default>,
    RoundingMode::round_default, RoundingMode::round_to_nearest_even,
    RoundingMode::round_toward_zero,
    RoundingMode::round_toward_positive_infinity,
    RoundingMode::round_toward_negative_infinity>;
using Vector8FloatingPointToVector8IntegerTestCases = rounding_mode_product_t<
    overflow_handling_product_t<
        tuples_to_test_cases_t<ca::Vector8FloatingPointToVector8Integer>,
        OverflowHandling::overflow_default,
        OverflowHandling::overflow_saturation>,
    RoundingMode::round_default, RoundingMode::round_to_nearest_even,
    RoundingMode::round_toward_zero,
    RoundingMode::round_toward_positive_infinity,
    RoundingMode::round_toward_negative_infinity>;
using Vector8FloatingPointToVector8FloatingPointTestCases =
    rounding_mode_product_t<
        overflow_handling_product_t<
            tuples_to_test_cases_t<
                ca::Vector8FloatingPointToVector8FloatingPoint>,
            OverflowHandling::overflow_default>,
        RoundingMode::round_default, RoundingMode::round_to_nearest_even,
        RoundingMode::round_toward_zero,
        RoundingMode::round_toward_positive_infinity,
        RoundingMode::round_toward_negative_infinity>;

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("explicit conversion", "",
                                    Vector8IntegerToVector8IntegerTestCases,
                                    explicit_conversion_test_name<TestType>) {
  vector_to_vector<TestType, TestVariant::conversion>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "explicit conversion", "",
    Vector8IntegerToVector8IntegerSaturationTestCases,
    explicit_conversion_test_name<TestType>) {
  vector_to_vector<TestType, TestVariant::conversion>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "explicit conversion", "", Vector8IntegerToVector8FloatingPointTestCases,
    explicit_conversion_test_name<TestType>) {
  vector_to_vector<TestType, TestVariant::conversion>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "explicit conversion", "", Vector8FloatingPointToVector8IntegerTestCases,
    explicit_conversion_test_name<TestType>) {
  vector_to_vector<TestType, TestVariant::conversion>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "explicit conversion", "",
    Vector8FloatingPointToVector8FloatingPointTestCases,
    explicit_conversion_test_name<TestType>) {
  vector_to_vector<TestType, TestVariant::conversion>(program);
}

} // namespace
