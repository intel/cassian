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

using IntegerScalarToIntegerScalarTestCases = rounding_mode_product_t<
    overflow_handling_product_t<
        tuples_to_test_cases_t<ca::IntegerScalarToIntegerScalar>,
        OverflowHandling::overflow_default>,
    RoundingMode::round_default, RoundingMode::round_to_nearest_even,
    RoundingMode::round_toward_zero,
    RoundingMode::round_toward_positive_infinity,
    RoundingMode::round_toward_negative_infinity>;
using IntegerScalarToIntegerScalarSaturationTestCases = rounding_mode_product_t<
    overflow_handling_product_t<
        tuples_to_test_cases_t<ca::IntegerScalarToIntegerScalar>,
        OverflowHandling::overflow_saturation>,
    RoundingMode::round_default, RoundingMode::round_to_nearest_even,
    RoundingMode::round_toward_zero,
    RoundingMode::round_toward_positive_infinity,
    RoundingMode::round_toward_negative_infinity>;
using IntegerScalarToFloatingPointScalarTestCases = rounding_mode_product_t<
    overflow_handling_product_t<
        tuples_to_test_cases_t<ca::IntegerScalarToFloatingPointScalar>,
        OverflowHandling::overflow_default>,
    RoundingMode::round_default, RoundingMode::round_to_nearest_even,
    RoundingMode::round_toward_zero,
    RoundingMode::round_toward_positive_infinity,
    RoundingMode::round_toward_negative_infinity>;
using FloatingPointScalarToIntegerScalarTestCases = rounding_mode_product_t<
    overflow_handling_product_t<
        tuples_to_test_cases_t<ca::FloatingPointScalarToIntegerScalar>,
        OverflowHandling::overflow_default,
        OverflowHandling::overflow_saturation>,
    RoundingMode::round_default, RoundingMode::round_to_nearest_even,
    RoundingMode::round_toward_zero,
    RoundingMode::round_toward_positive_infinity,
    RoundingMode::round_toward_negative_infinity>;
using FloatingPointScalarToFloatingPointScalarTestCases =
    rounding_mode_product_t<
        overflow_handling_product_t<
            tuples_to_test_cases_t<
                ca::FloatingPointScalarToFloatingPointScalar>,
            OverflowHandling::overflow_default>,
        RoundingMode::round_default, RoundingMode::round_to_nearest_even,
        RoundingMode::round_toward_zero,
        RoundingMode::round_toward_positive_infinity,
        RoundingMode::round_toward_negative_infinity>;

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("explicit conversion", "",
                                    IntegerScalarToIntegerScalarTestCases,
                                    explicit_conversion_test_name<TestType>) {
  scalar_to_scalar<TestType, TestVariant::conversion>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "explicit conversion", "", IntegerScalarToIntegerScalarSaturationTestCases,
    explicit_conversion_test_name<TestType>) {
  scalar_to_scalar<TestType, TestVariant::conversion>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("explicit conversion", "",
                                    IntegerScalarToFloatingPointScalarTestCases,
                                    explicit_conversion_test_name<TestType>) {
  scalar_to_scalar<TestType, TestVariant::conversion>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("explicit conversion", "",
                                    FloatingPointScalarToIntegerScalarTestCases,
                                    explicit_conversion_test_name<TestType>) {
  scalar_to_scalar<TestType, TestVariant::conversion>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "explicit conversion", "",
    FloatingPointScalarToFloatingPointScalarTestCases,
    explicit_conversion_test_name<TestType>) {
  scalar_to_scalar<TestType, TestVariant::conversion>(program);
}

} // namespace
