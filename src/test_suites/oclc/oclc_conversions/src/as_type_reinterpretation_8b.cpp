/*
 * Copyright (C) 2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/catch2_utils/catch2_utils.hpp>
#include <catch2/catch.hpp>
#include <common.hpp>
#include <string>
#include <tuple>
#include <utility.hpp>

namespace {

const std::string program =
    "kernels/oclc_conversions/as_type_reinterpretation.cl";

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    tuples_to_test_cases_t<ca::ScalarEightByteTypesToScalarEightByteTypes>,
    generic_test_name<TestType>) {

  scalar_to_scalar<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    tuples_to_test_cases_t<ca::Vector2EightByteTypesToVector2EightByteTypes>,
    generic_test_name<TestType>) {

  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    tuples_to_test_cases_t<ca::Vector3EightByteTypesToVector3EightByteTypes>,
    generic_test_name<TestType>) {

  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    tuples_to_test_cases_t<ca::Vector4EightByteTypesToVector4EightByteTypes>,
    generic_test_name<TestType>) {

  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    tuples_to_test_cases_t<ca::Vector4EightByteTypesToVector3EightByteTypes>,
    generic_test_name<TestType>) {

  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    tuples_to_test_cases_t<ca::Vector8EightByteTypesToVector8EightByteTypes>,
    generic_test_name<TestType>) {

  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "as_type reinterpretation", "",
    tuples_to_test_cases_t<ca::Vector16EightByteTypesToVector16EightByteTypes>,
    generic_test_name<TestType>) {

  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}
} // namespace
