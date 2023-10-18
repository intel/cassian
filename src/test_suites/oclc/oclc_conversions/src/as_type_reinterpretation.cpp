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

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    tuples_to_test_cases_t<ca::ScalarOneByteTypesToScalarOneByteTypes>,
    test_name<TestType>) {

  scalar_to_scalar<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    tuples_to_test_cases_t<ca::Vector2OneByteTypesToVector2OneByteTypes>,
    test_name<TestType>) {

  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    tuples_to_test_cases_t<ca::Vector3OneByteTypesToVector3OneByteTypes>,
    test_name<TestType>) {

  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    tuples_to_test_cases_t<ca::Vector4OneByteTypesToVector4OneByteTypes>,
    test_name<TestType>) {

  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    tuples_to_test_cases_t<ca::Vector4OneByteTypesToVector3OneByteTypes>,
    test_name<TestType>) {

  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    tuples_to_test_cases_t<ca::Vector8OneByteTypesToVector8OneByteTypes>,
    test_name<TestType>) {

  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    tuples_to_test_cases_t<ca::Vector16OneByteTypesToVector16OneByteTypes>,
    test_name<TestType>) {

  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    tuples_to_test_cases_t<ca::ScalarTwoByteTypesToScalarTwoByteTypes>,
    test_name<TestType>) {

  scalar_to_scalar<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    tuples_to_test_cases_t<ca::Vector2TwoByteTypesToVector2TwoByteTypes>,
    test_name<TestType>) {

  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    tuples_to_test_cases_t<ca::Vector3TwoByteTypesToVector3TwoByteTypes>,
    test_name<TestType>) {

  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    tuples_to_test_cases_t<ca::Vector4TwoByteTypesToVector4TwoByteTypes>,
    test_name<TestType>) {

  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    tuples_to_test_cases_t<ca::Vector4TwoByteTypesToVector3TwoByteTypes>,
    test_name<TestType>) {

  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    tuples_to_test_cases_t<ca::Vector8TwoByteTypesToVector8TwoByteTypes>,
    test_name<TestType>) {

  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    tuples_to_test_cases_t<ca::Vector16TwoByteTypesToVector16TwoByteTypes>,
    test_name<TestType>) {

  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    tuples_to_test_cases_t<ca::ScalarFourByteTypesToScalarFourByteTypes>,
    test_name<TestType>) {

  scalar_to_scalar<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    tuples_to_test_cases_t<ca::Vector2FourByteTypesToVector2FourByteTypes>,
    test_name<TestType>) {

  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    tuples_to_test_cases_t<ca::Vector3FourByteTypesToVector3FourByteTypes>,
    test_name<TestType>) {

  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    tuples_to_test_cases_t<ca::Vector4FourByteTypesToVector4FourByteTypes>,
    test_name<TestType>) {

  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    tuples_to_test_cases_t<ca::Vector4FourByteTypesToVector3FourByteTypes>,
    test_name<TestType>) {

  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    tuples_to_test_cases_t<ca::Vector8FourByteTypesToVector8FourByteTypes>,
    test_name<TestType>) {

  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    tuples_to_test_cases_t<ca::Vector16FourByteTypesToVector16FourByteTypes>,
    test_name<TestType>) {

  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    tuples_to_test_cases_t<ca::ScalarEightByteTypesToScalarEightByteTypes>,
    test_name<TestType>) {

  scalar_to_scalar<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    tuples_to_test_cases_t<ca::Vector2EightByteTypesToVector2EightByteTypes>,
    test_name<TestType>) {

  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    tuples_to_test_cases_t<ca::Vector3EightByteTypesToVector3EightByteTypes>,
    test_name<TestType>) {

  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    tuples_to_test_cases_t<ca::Vector4EightByteTypesToVector4EightByteTypes>,
    test_name<TestType>) {

  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    tuples_to_test_cases_t<ca::Vector4EightByteTypesToVector3EightByteTypes>,
    test_name<TestType>) {

  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    tuples_to_test_cases_t<ca::Vector8EightByteTypesToVector8EightByteTypes>,
    test_name<TestType>) {

  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    tuples_to_test_cases_t<ca::Vector16EightByteTypesToVector16EightByteTypes>,
    test_name<TestType>) {

  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}
} // namespace
