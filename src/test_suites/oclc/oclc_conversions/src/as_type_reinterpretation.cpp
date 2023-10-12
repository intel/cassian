/*
 * Copyright (C) 2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/catch2_utils/catch2_utils.hpp>
#include <cassian/cli/cli.hpp>
#include <cassian/runtime/openclc_type_tuples.hpp>
#include <cassian/runtime/openclc_types.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cassian/test_harness/test_harness.hpp>
#include <cassian/utility/utility.hpp>
#include <catch2/catch.hpp>
#include <common.hpp>
#include <cstdint>
#include <string>
#include <test_config.hpp>
#include <utility.hpp>
#include <vector>

namespace {

const std::string program =
    "kernels/oclc_conversions/as_type_reinterpretation.cl";

template <typename TestType> auto test_name() {
  using from = typename TestType::from_type;
  using to = typename TestType::to_type;

  return std::string(from::type_name) + "->" + std::string(to::type_name);
}

using ScalarOneByteTypesToScalarOneByteTypesTestCases = test_variant_product_t<
    tuples_to_test_cases_t<ca::ScalarOneByteTypesToScalarOneByteTypes>,
    TestVariant::reinterpretation>;
TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    ScalarOneByteTypesToScalarOneByteTypesTestCases, test_name<TestType>) {

  scalar_to_scalar<TestType>(program);
}

using Vector2OneByteTypesToVector2OneByteTypesTestCases =
    test_variant_product_t<
        tuples_to_test_cases_t<ca::Vector2OneByteTypesToVector2OneByteTypes>,
        TestVariant::reinterpretation>;
TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    Vector2OneByteTypesToVector2OneByteTypesTestCases, test_name<TestType>) {

  vector_to_vector<TestType>(program);
}

using Vector3OneByteTypesToVector3OneByteTypesTestCases =
    test_variant_product_t<
        tuples_to_test_cases_t<ca::Vector3OneByteTypesToVector3OneByteTypes>,
        TestVariant::reinterpretation>;
TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    Vector3OneByteTypesToVector3OneByteTypesTestCases, test_name<TestType>) {

  vector_to_vector<TestType>(program);
}

using Vector4OneByteTypesToVector4OneByteTypesTestCases =
    test_variant_product_t<
        tuples_to_test_cases_t<ca::Vector4OneByteTypesToVector4OneByteTypes>,
        TestVariant::reinterpretation>;
TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    Vector4OneByteTypesToVector4OneByteTypesTestCases, test_name<TestType>) {

  vector_to_vector<TestType>(program);
}

using Vector4OneByteTypesToVector3OneByteTypesTestCases =
    test_variant_product_t<
        tuples_to_test_cases_t<ca::Vector4OneByteTypesToVector3OneByteTypes>,
        TestVariant::reinterpretation>;
TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    Vector4OneByteTypesToVector3OneByteTypesTestCases, test_name<TestType>) {

  vector_to_vector<TestType>(program);
}

using Vector8OneByteTypesToVector8OneByteTypesTestCases =
    test_variant_product_t<
        tuples_to_test_cases_t<ca::Vector8OneByteTypesToVector8OneByteTypes>,
        TestVariant::reinterpretation>;
TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    Vector8OneByteTypesToVector8OneByteTypesTestCases, test_name<TestType>) {

  vector_to_vector<TestType>(program);
}

using Vector16OneByteTypesToVector16OneByteTypesTestCasesTestCases =
    test_variant_product_t<
        tuples_to_test_cases_t<ca::Vector16OneByteTypesToVector16OneByteTypes>,
        TestVariant::reinterpretation>;
TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    Vector16OneByteTypesToVector16OneByteTypesTestCasesTestCases,
    test_name<TestType>) {

  vector_to_vector<TestType>(program);
}

using ScalarTwoByteTypesToScalarTwoByteTypesTestCases = test_variant_product_t<
    tuples_to_test_cases_t<ca::ScalarTwoByteTypesToScalarTwoByteTypes>,
    TestVariant::reinterpretation>;
TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    ScalarTwoByteTypesToScalarTwoByteTypesTestCases, test_name<TestType>) {

  scalar_to_scalar<TestType>(program);
}

using Vector2TwoByteTypesToVector2TwoByteTypesTestCases =
    test_variant_product_t<
        tuples_to_test_cases_t<ca::Vector2TwoByteTypesToVector2TwoByteTypes>,
        TestVariant::reinterpretation>;
TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    Vector2TwoByteTypesToVector2TwoByteTypesTestCases, test_name<TestType>) {

  vector_to_vector<TestType>(program);
}

using Vector3TwoByteTypesToVector3TwoByteTypesTestCases =
    test_variant_product_t<
        tuples_to_test_cases_t<ca::Vector3TwoByteTypesToVector3TwoByteTypes>,
        TestVariant::reinterpretation>;
TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    Vector3TwoByteTypesToVector3TwoByteTypesTestCases, test_name<TestType>) {

  vector_to_vector<TestType>(program);
}

using Vector4TwoByteTypesToVector4TwoByteTypesTestCases =
    test_variant_product_t<
        tuples_to_test_cases_t<ca::Vector4TwoByteTypesToVector4TwoByteTypes>,
        TestVariant::reinterpretation>;
TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    Vector4TwoByteTypesToVector4TwoByteTypesTestCases, test_name<TestType>) {

  vector_to_vector<TestType>(program);
}

using Vector4TwoByteTypesToVector3TwoByteTypesTestCases =
    test_variant_product_t<
        tuples_to_test_cases_t<ca::Vector4TwoByteTypesToVector3TwoByteTypes>,
        TestVariant::reinterpretation>;
TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    Vector4TwoByteTypesToVector3TwoByteTypesTestCases, test_name<TestType>) {

  vector_to_vector<TestType>(program);
}

using Vector8TwoByteTypesToVector8TwoByteTypesTestCases =
    test_variant_product_t<
        tuples_to_test_cases_t<ca::Vector8TwoByteTypesToVector8TwoByteTypes>,
        TestVariant::reinterpretation>;
TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    Vector8TwoByteTypesToVector8TwoByteTypesTestCases, test_name<TestType>) {

  vector_to_vector<TestType>(program);
}

using Vector16TwoByteTypesToVector16TwoByteTypesTestCases =
    test_variant_product_t<
        tuples_to_test_cases_t<ca::Vector16TwoByteTypesToVector16TwoByteTypes>,
        TestVariant::reinterpretation>;
TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    Vector16TwoByteTypesToVector16TwoByteTypesTestCases, test_name<TestType>) {

  vector_to_vector<TestType>(program);
}

using ScalarFourByteTypesToScalarFourByteTypesTestCases =
    test_variant_product_t<
        tuples_to_test_cases_t<ca::ScalarFourByteTypesToScalarFourByteTypes>,
        TestVariant::reinterpretation>;
TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    ScalarFourByteTypesToScalarFourByteTypesTestCases, test_name<TestType>) {

  scalar_to_scalar<TestType>(program);
}

using Vector2FourByteTypesToVector2FourByteTypesTestCases =
    test_variant_product_t<
        tuples_to_test_cases_t<ca::Vector2FourByteTypesToVector2FourByteTypes>,
        TestVariant::reinterpretation>;
TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    Vector2FourByteTypesToVector2FourByteTypesTestCases, test_name<TestType>) {

  vector_to_vector<TestType>(program);
}

using Vector3FourByteTypesToVector3FourByteTypesTestCases =
    test_variant_product_t<
        tuples_to_test_cases_t<ca::Vector3FourByteTypesToVector3FourByteTypes>,
        TestVariant::reinterpretation>;
TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    Vector3FourByteTypesToVector3FourByteTypesTestCases, test_name<TestType>) {

  vector_to_vector<TestType>(program);
}

using Vector4FourByteTypesToVector4FourByteTypesTestCases =
    test_variant_product_t<
        tuples_to_test_cases_t<ca::Vector4FourByteTypesToVector4FourByteTypes>,
        TestVariant::reinterpretation>;
TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    Vector4FourByteTypesToVector4FourByteTypesTestCases, test_name<TestType>) {

  vector_to_vector<TestType>(program);
}

using Vector4FourByteTypesToVector3FourByteTypesTestCases =
    test_variant_product_t<
        tuples_to_test_cases_t<ca::Vector4FourByteTypesToVector3FourByteTypes>,
        TestVariant::reinterpretation>;
TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    Vector4FourByteTypesToVector3FourByteTypesTestCases, test_name<TestType>) {

  vector_to_vector<TestType>(program);
}

using Vector8FourByteTypesToVector8FourByteTypesTestCases =
    test_variant_product_t<
        tuples_to_test_cases_t<ca::Vector8FourByteTypesToVector8FourByteTypes>,
        TestVariant::reinterpretation>;
TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    Vector8FourByteTypesToVector8FourByteTypesTestCases, test_name<TestType>) {

  vector_to_vector<TestType>(program);
}

using Vector16FourByteTypesToVector16FourByteTypesTestCases =
    test_variant_product_t<
        tuples_to_test_cases_t<
            ca::Vector16FourByteTypesToVector16FourByteTypes>,
        TestVariant::reinterpretation>;
TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    Vector16FourByteTypesToVector16FourByteTypesTestCases,
    test_name<TestType>) {

  vector_to_vector<TestType>(program);
}

using ScalarEightByteTypesToScalarEightByteTypesTestCases =
    test_variant_product_t<
        tuples_to_test_cases_t<ca::ScalarEightByteTypesToScalarEightByteTypes>,
        TestVariant::reinterpretation>;
TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    ScalarEightByteTypesToScalarEightByteTypesTestCases, test_name<TestType>) {

  scalar_to_scalar<TestType>(program);
}

using Vector2EightByteTypesToVector2EightByteTypesTestCases =
    test_variant_product_t<
        tuples_to_test_cases_t<
            ca::Vector2EightByteTypesToVector2EightByteTypes>,
        TestVariant::reinterpretation>;
TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    Vector2EightByteTypesToVector2EightByteTypesTestCases,
    test_name<TestType>) {

  vector_to_vector<TestType>(program);
}

using Vector3EightByteTypesToVector3EightByteTypesTestCases =
    test_variant_product_t<
        tuples_to_test_cases_t<
            ca::Vector3EightByteTypesToVector3EightByteTypes>,
        TestVariant::reinterpretation>;
TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    Vector3EightByteTypesToVector3EightByteTypesTestCases,
    test_name<TestType>) {

  vector_to_vector<TestType>(program);
}

using Vector4EightByteTypesToVector4EightByteTypesTestCases =
    test_variant_product_t<
        tuples_to_test_cases_t<
            ca::Vector4EightByteTypesToVector4EightByteTypes>,
        TestVariant::reinterpretation>;
TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    Vector4EightByteTypesToVector4EightByteTypesTestCases,
    test_name<TestType>) {

  vector_to_vector<TestType>(program);
}

using Vector4EightByteTypesToVector3EightByteTypesTestCases =
    test_variant_product_t<
        tuples_to_test_cases_t<
            ca::Vector4EightByteTypesToVector3EightByteTypes>,
        TestVariant::reinterpretation>;
TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    Vector4EightByteTypesToVector3EightByteTypesTestCases,
    test_name<TestType>) {

  vector_to_vector<TestType>(program);
}

using Vector8EightByteTypesToVector8EightByteTypesTestCases =
    test_variant_product_t<
        tuples_to_test_cases_t<
            ca::Vector8EightByteTypesToVector8EightByteTypes>,
        TestVariant::reinterpretation>;
TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    Vector8EightByteTypesToVector8EightByteTypesTestCases,
    test_name<TestType>) {

  vector_to_vector<TestType>(program);
}

using Vector16EightByteTypesToVector16EightByteTypesTestCases =
    test_variant_product_t<
        tuples_to_test_cases_t<
            ca::Vector16EightByteTypesToVector16EightByteTypes>,
        TestVariant::reinterpretation>;
TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    Vector16EightByteTypesToVector16EightByteTypesTestCases,
    test_name<TestType>) {

  vector_to_vector<TestType>(program);
}
} // namespace
