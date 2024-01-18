/*
 * Copyright (C) 2024 Intel Corporation
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

const std::string program = "kernels/oclc_conversions/pointer_cast.cl";

template <typename TestType> auto test_name() {
  using from = typename TestType::from_type;
  using to = typename TestType::to_type;

  return std::string(from::type_name) + "->" + std::string(to::type_name);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("pointer cast", "",
                                    tuples_to_test_cases_t<ca::ScalarToScalar>,
                                    test_name<TestType>) {
  scalar_to_scalar<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("pointer cast", "",
                                    tuples_to_test_cases_t<ca::ScalarToVector2>,
                                    test_name<TestType>) {
  scalar_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("pointer cast", "",
                                    tuples_to_test_cases_t<ca::ScalarToVector3>,
                                    test_name<TestType>) {
  scalar_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("pointer cast", "",
                                    tuples_to_test_cases_t<ca::ScalarToVector4>,
                                    test_name<TestType>) {
  scalar_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("pointer cast", "",
                                    tuples_to_test_cases_t<ca::ScalarToVector8>,
                                    test_name<TestType>) {
  scalar_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "pointer cast", "", tuples_to_test_cases_t<ca::ScalarToVector16>,
    test_name<TestType>) {
  scalar_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("pointer cast", "",
                                    tuples_to_test_cases_t<ca::Vector2ToScalar>,
                                    test_name<TestType>) {
  vector_to_scalar<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "pointer cast", "", tuples_to_test_cases_t<ca::Vector2ToVector2>,
    test_name<TestType>) {
  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "pointer cast", "", tuples_to_test_cases_t<ca::Vector2ToVector3>,
    test_name<TestType>) {
  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "pointer cast", "", tuples_to_test_cases_t<ca::Vector2ToVector4>,
    test_name<TestType>) {
  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "pointer cast", "", tuples_to_test_cases_t<ca::Vector2ToVector8>,
    test_name<TestType>) {
  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "pointer cast", "", tuples_to_test_cases_t<ca::Vector2ToVector16>,
    test_name<TestType>) {
  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("pointer cast", "",
                                    tuples_to_test_cases_t<ca::Vector3ToScalar>,
                                    test_name<TestType>) {
  vector_to_scalar<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "pointer cast", "", tuples_to_test_cases_t<ca::Vector3ToVector2>,
    test_name<TestType>) {
  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "pointer cast", "", tuples_to_test_cases_t<ca::Vector3ToVector3>,
    test_name<TestType>) {
  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "pointer cast", "", tuples_to_test_cases_t<ca::Vector3ToVector4>,
    test_name<TestType>) {
  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "pointer cast", "", tuples_to_test_cases_t<ca::Vector3ToVector8>,
    test_name<TestType>) {
  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "pointer cast", "", tuples_to_test_cases_t<ca::Vector3ToVector16>,
    test_name<TestType>) {
  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("pointer cast", "",
                                    tuples_to_test_cases_t<ca::Vector4ToScalar>,
                                    test_name<TestType>) {
  vector_to_scalar<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "pointer cast", "", tuples_to_test_cases_t<ca::Vector4ToVector2>,
    test_name<TestType>) {
  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "pointer cast", "", tuples_to_test_cases_t<ca::Vector4ToVector3>,
    test_name<TestType>) {
  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "pointer cast", "", tuples_to_test_cases_t<ca::Vector4ToVector4>,
    test_name<TestType>) {
  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "pointer cast", "", tuples_to_test_cases_t<ca::Vector4ToVector8>,
    test_name<TestType>) {
  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "pointer cast", "", tuples_to_test_cases_t<ca::Vector4ToVector16>,
    test_name<TestType>) {
  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("pointer cast", "",
                                    tuples_to_test_cases_t<ca::Vector8ToScalar>,
                                    test_name<TestType>) {
  vector_to_scalar<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "pointer cast", "", tuples_to_test_cases_t<ca::Vector8ToVector2>,
    test_name<TestType>) {
  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "pointer cast", "", tuples_to_test_cases_t<ca::Vector8ToVector3>,
    test_name<TestType>) {
  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "pointer cast", "", tuples_to_test_cases_t<ca::Vector8ToVector4>,
    test_name<TestType>) {
  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "pointer cast", "", tuples_to_test_cases_t<ca::Vector8ToVector8>,
    test_name<TestType>) {
  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "pointer cast", "", tuples_to_test_cases_t<ca::Vector8ToVector16>,
    test_name<TestType>) {
  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "pointer cast", "", tuples_to_test_cases_t<ca::Vector16ToScalar>,
    test_name<TestType>) {
  vector_to_scalar<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "pointer cast", "", tuples_to_test_cases_t<ca::Vector16ToVector2>,
    test_name<TestType>) {
  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "pointer cast", "", tuples_to_test_cases_t<ca::Vector16ToVector3>,
    test_name<TestType>) {
  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "pointer cast", "", tuples_to_test_cases_t<ca::Vector16ToVector4>,
    test_name<TestType>) {
  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "pointer cast", "", tuples_to_test_cases_t<ca::Vector16ToVector8>,
    test_name<TestType>) {
  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "pointer cast", "", tuples_to_test_cases_t<ca::Vector16ToVector16>,
    test_name<TestType>) {
  vector_to_vector<TestType, TestVariant::reinterpretation>(program);
}

} // namespace
