/*
 * Copyright (C) 2023 Intel Corporation
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

const std::string program =
    "kernels/oclc_conversions/union_reinterpretation.cl";

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("union reinterpretation", "",
                                    tuples_to_test_cases_t<ca::Vector2ToScalar>,
                                    generic_test_name<TestType>) {
  vector_to_scalar<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "union reinterpretation", "", tuples_to_test_cases_t<ca::Vector2ToVector2>,
    generic_test_name<TestType>) {
  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "union reinterpretation", "", tuples_to_test_cases_t<ca::Vector2ToVector3>,
    generic_test_name<TestType>) {
  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "union reinterpretation", "", tuples_to_test_cases_t<ca::Vector2ToVector4>,
    generic_test_name<TestType>) {
  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "union reinterpretation", "", tuples_to_test_cases_t<ca::Vector2ToVector8>,
    generic_test_name<TestType>) {
  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "union reinterpretation", "", tuples_to_test_cases_t<ca::Vector2ToVector16>,
    generic_test_name<TestType>) {
  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

} // namespace
