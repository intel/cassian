/*
 * Copyright (C) 2021 Intel Corporation
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

const std::string program = "kernels/oclc_conversions/explicit_cast.cl";

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "explicit cast", "", tuples_to_test_cases_t<ca::ScalarToScalarExtended>,
    generic_test_name<TestType>) {
  scalar_to_scalar<TestType, TestVariant::conversion>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "explicit cast", "", tuples_to_test_cases_t<ca::ScalarToVector2Extended>,
    generic_test_name<TestType>) {
  scalar_to_vector<TestType, TestVariant::conversion>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "explicit cast", "", tuples_to_test_cases_t<ca::ScalarToVector3Extended>,
    generic_test_name<TestType>) {
  scalar_to_vector<TestType, TestVariant::conversion>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "explicit cast", "", tuples_to_test_cases_t<ca::ScalarToVector4Extended>,
    generic_test_name<TestType>) {
  scalar_to_vector<TestType, TestVariant::conversion>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "explicit cast", "", tuples_to_test_cases_t<ca::ScalarToVector8Extended>,
    generic_test_name<TestType>) {
  scalar_to_vector<TestType, TestVariant::conversion>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "explicit cast", "", tuples_to_test_cases_t<ca::ScalarToVector16Extended>,
    generic_test_name<TestType>) {
  scalar_to_vector<TestType, TestVariant::conversion>(program);
}

} // namespace
