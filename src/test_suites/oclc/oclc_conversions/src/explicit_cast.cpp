/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/catch2_utils/catch2_utils.hpp>
#include <cassian/cli/cli.hpp>
#include <cassian/runtime/openclc_type_tuples.hpp>
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

const std::string program = "kernels/oclc_conversions/explicit_cast.cl";

template <typename TestType> auto test_name() {
  using from = typename TestType::from_type;
  using to = typename TestType::to_type;

  return std::string(from::type_name) + "->" + std::string(to::type_name);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "explicit cast", "", tuples_to_test_cases_t<ca::ScalarToScalarExtended>,
    test_name<TestType>) {
  scalar_to_scalar<TestType, TestVariant::conversion>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "explicit cast", "", tuples_to_test_cases_t<ca::ScalarToVector2Extended>,
    test_name<TestType>) {
  scalar_to_vector<TestType, TestVariant::conversion>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "explicit cast", "", tuples_to_test_cases_t<ca::ScalarToVector3Extended>,
    test_name<TestType>) {
  scalar_to_vector<TestType, TestVariant::conversion>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "explicit cast", "", tuples_to_test_cases_t<ca::ScalarToVector4Extended>,
    test_name<TestType>) {
  scalar_to_vector<TestType, TestVariant::conversion>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "explicit cast", "", tuples_to_test_cases_t<ca::ScalarToVector8Extended>,
    test_name<TestType>) {
  scalar_to_vector<TestType, TestVariant::conversion>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "explicit cast", "", tuples_to_test_cases_t<ca::ScalarToVector16Extended>,
    test_name<TestType>) {
  scalar_to_vector<TestType, TestVariant::conversion>(program);
}

} // namespace
