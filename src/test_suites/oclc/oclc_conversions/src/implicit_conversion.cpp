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

const std::string program = "kernels/oclc_conversions/implicit_conversion.cl";

template <typename TestType> auto test_name() {
  using from = typename TestType::from_type;
  using to = typename TestType::to_type;

  return std::string(from::type_name) + "->" + std::string(to::type_name);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("implicit conversion", "",
                                    tuples_to_test_cases_t<ca::ScalarToScalar>,
                                    test_name<TestType>) {
  scalar_to_scalar<TestType, TestVariant::conversion>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("implicit conversion", "",
                                    tuples_to_test_cases_t<ca::ScalarToVector2>,
                                    test_name<TestType>) {
  scalar_to_vector<TestType, TestVariant::conversion>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("implicit conversion", "",
                                    tuples_to_test_cases_t<ca::ScalarToVector3>,
                                    test_name<TestType>) {
  scalar_to_vector<TestType, TestVariant::conversion>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("implicit conversion", "",
                                    tuples_to_test_cases_t<ca::ScalarToVector4>,
                                    test_name<TestType>) {
  scalar_to_vector<TestType, TestVariant::conversion>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("implicit conversion", "",
                                    tuples_to_test_cases_t<ca::ScalarToVector8>,
                                    test_name<TestType>) {
  scalar_to_vector<TestType, TestVariant::conversion>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "implicit conversion", "", tuples_to_test_cases_t<ca::ScalarToVector16>,
    test_name<TestType>) {
  scalar_to_vector<TestType, TestVariant::conversion>(program);
}

} // namespace
