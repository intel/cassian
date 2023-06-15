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
    "kernels/oclc_conversions/union_reinterpretation.cl";

template <typename TestType> auto test_name() {
  using from = typename TestType::from_type;
  using to = typename TestType::to_type;

  return std::string(from::type_name) + "->" + std::string(to::type_name);
}

using ScalarToScalarTestCases =
    test_variant_product_t<tuples_to_test_cases_t<ca::ScalarToScalar>,
                           TestVariant::reinterpretation>;
using ScalarToVector2TestCases =
    test_variant_product_t<tuples_to_test_cases_t<ca::ScalarToVector2>,
                           TestVariant::reinterpretation>;
using ScalarToVector3TestCases =
    test_variant_product_t<tuples_to_test_cases_t<ca::ScalarToVector3>,
                           TestVariant::reinterpretation>;
using ScalarToVector4TestCases =
    test_variant_product_t<tuples_to_test_cases_t<ca::ScalarToVector4>,
                           TestVariant::reinterpretation>;
using ScalarToVector8TestCases =
    test_variant_product_t<tuples_to_test_cases_t<ca::ScalarToVector8>,
                           TestVariant::reinterpretation>;
using ScalarToVector16TestCases =
    test_variant_product_t<tuples_to_test_cases_t<ca::ScalarToVector16>,
                           TestVariant::reinterpretation>;

using Vector2ToScalarTestCases =
    test_variant_product_t<tuples_to_test_cases_t<ca::Vector2ToScalar>,
                           TestVariant::reinterpretation>;
using Vector2ToVector2TestCases =
    test_variant_product_t<tuples_to_test_cases_t<ca::Vector2ToVector2>,
                           TestVariant::reinterpretation>;
using Vector2ToVector3TestCases =
    test_variant_product_t<tuples_to_test_cases_t<ca::Vector2ToVector3>,
                           TestVariant::reinterpretation>;
using Vector2ToVector4TestCases =
    test_variant_product_t<tuples_to_test_cases_t<ca::Vector2ToVector4>,
                           TestVariant::reinterpretation>;
using Vector2ToVector8TestCases =
    test_variant_product_t<tuples_to_test_cases_t<ca::Vector2ToVector8>,
                           TestVariant::reinterpretation>;
using Vector2ToVector16TestCases =
    test_variant_product_t<tuples_to_test_cases_t<ca::Vector2ToVector16>,
                           TestVariant::reinterpretation>;

using Vector3ToScalarTestCases =
    test_variant_product_t<tuples_to_test_cases_t<ca::Vector3ToScalar>,
                           TestVariant::reinterpretation>;
using Vector3ToVector2TestCases =
    test_variant_product_t<tuples_to_test_cases_t<ca::Vector3ToVector2>,
                           TestVariant::reinterpretation>;
using Vector3ToVector3TestCases =
    test_variant_product_t<tuples_to_test_cases_t<ca::Vector3ToVector3>,
                           TestVariant::reinterpretation>;
using Vector3ToVector4TestCases =
    test_variant_product_t<tuples_to_test_cases_t<ca::Vector3ToVector4>,
                           TestVariant::reinterpretation>;
using Vector3ToVector8TestCases =
    test_variant_product_t<tuples_to_test_cases_t<ca::Vector3ToVector8>,
                           TestVariant::reinterpretation>;
using Vector3ToVector16TestCases =
    test_variant_product_t<tuples_to_test_cases_t<ca::Vector3ToVector16>,
                           TestVariant::reinterpretation>;

using Vector4ToScalarTestCases =
    test_variant_product_t<tuples_to_test_cases_t<ca::Vector4ToScalar>,
                           TestVariant::reinterpretation>;
using Vector4ToVector2TestCases =
    test_variant_product_t<tuples_to_test_cases_t<ca::Vector4ToVector2>,
                           TestVariant::reinterpretation>;
using Vector4ToVector3TestCases =
    test_variant_product_t<tuples_to_test_cases_t<ca::Vector4ToVector3>,
                           TestVariant::reinterpretation>;
using Vector4ToVector4TestCases =
    test_variant_product_t<tuples_to_test_cases_t<ca::Vector4ToVector4>,
                           TestVariant::reinterpretation>;
using Vector4ToVector8TestCases =
    test_variant_product_t<tuples_to_test_cases_t<ca::Vector4ToVector8>,
                           TestVariant::reinterpretation>;
using Vector4ToVector16TestCases =
    test_variant_product_t<tuples_to_test_cases_t<ca::Vector4ToVector16>,
                           TestVariant::reinterpretation>;

using Vector8ToScalarTestCases =
    test_variant_product_t<tuples_to_test_cases_t<ca::Vector8ToScalar>,
                           TestVariant::reinterpretation>;
using Vector8ToVector2TestCases =
    test_variant_product_t<tuples_to_test_cases_t<ca::Vector8ToVector2>,
                           TestVariant::reinterpretation>;
using Vector8ToVector3TestCases =
    test_variant_product_t<tuples_to_test_cases_t<ca::Vector8ToVector3>,
                           TestVariant::reinterpretation>;
using Vector8ToVector4TestCases =
    test_variant_product_t<tuples_to_test_cases_t<ca::Vector8ToVector4>,
                           TestVariant::reinterpretation>;
using Vector8ToVector8TestCases =
    test_variant_product_t<tuples_to_test_cases_t<ca::Vector8ToVector8>,
                           TestVariant::reinterpretation>;
using Vector8ToVector16TestCases =
    test_variant_product_t<tuples_to_test_cases_t<ca::Vector8ToVector16>,
                           TestVariant::reinterpretation>;

using Vector16ToScalarTestCases =
    test_variant_product_t<tuples_to_test_cases_t<ca::Vector16ToScalar>,
                           TestVariant::reinterpretation>;
using Vector16ToVector2TestCases =
    test_variant_product_t<tuples_to_test_cases_t<ca::Vector16ToVector2>,
                           TestVariant::reinterpretation>;
using Vector16ToVector3TestCases =
    test_variant_product_t<tuples_to_test_cases_t<ca::Vector16ToVector3>,
                           TestVariant::reinterpretation>;
using Vector16ToVector4TestCases =
    test_variant_product_t<tuples_to_test_cases_t<ca::Vector16ToVector4>,
                           TestVariant::reinterpretation>;
using Vector16ToVector8TestCases =
    test_variant_product_t<tuples_to_test_cases_t<ca::Vector16ToVector8>,
                           TestVariant::reinterpretation>;
using Vector16ToVector16TestCases =
    test_variant_product_t<tuples_to_test_cases_t<ca::Vector16ToVector16>,
                           TestVariant::reinterpretation>;

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("union reinterpretation", "",
                                    ScalarToScalarTestCases,
                                    test_name<TestType>) {
  scalar_to_scalar<TestType>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("union reinterpretation", "",
                                    ScalarToVector2TestCases,
                                    test_name<TestType>) {
  scalar_to_vector<TestType>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("union reinterpretation", "",
                                    ScalarToVector3TestCases,
                                    test_name<TestType>) {
  scalar_to_vector<TestType>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("union reinterpretation", "",
                                    ScalarToVector4TestCases,
                                    test_name<TestType>) {
  scalar_to_vector<TestType>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("union reinterpretation", "",
                                    ScalarToVector8TestCases,
                                    test_name<TestType>) {
  scalar_to_vector<TestType>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("union reinterpretation", "",
                                    ScalarToVector16TestCases,
                                    test_name<TestType>) {
  scalar_to_vector<TestType>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("union reinterpretation", "",
                                    Vector2ToScalarTestCases,
                                    test_name<TestType>) {
  vector_to_scalar<TestType>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("union reinterpretation", "",
                                    Vector2ToVector2TestCases,
                                    test_name<TestType>) {
  vector_to_vector<TestType>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("union reinterpretation", "",
                                    Vector2ToVector3TestCases,
                                    test_name<TestType>) {
  vector_to_vector<TestType>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("union reinterpretation", "",
                                    Vector2ToVector4TestCases,
                                    test_name<TestType>) {
  vector_to_vector<TestType>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("union reinterpretation", "",
                                    Vector2ToVector8TestCases,
                                    test_name<TestType>) {
  vector_to_vector<TestType>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("union reinterpretation", "",
                                    Vector2ToVector16TestCases,
                                    test_name<TestType>) {
  vector_to_vector<TestType>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("union reinterpretation", "",
                                    Vector3ToScalarTestCases,
                                    test_name<TestType>) {
  vector_to_scalar<TestType>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("union reinterpretation", "",
                                    Vector3ToVector2TestCases,
                                    test_name<TestType>) {
  vector_to_vector<TestType>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("union reinterpretation", "",
                                    Vector3ToVector3TestCases,
                                    test_name<TestType>) {
  vector_to_vector<TestType>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("union reinterpretation", "",
                                    Vector3ToVector4TestCases,
                                    test_name<TestType>) {
  vector_to_vector<TestType>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("union reinterpretation", "",
                                    Vector3ToVector8TestCases,
                                    test_name<TestType>) {
  vector_to_vector<TestType>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("union reinterpretation", "",
                                    Vector3ToVector16TestCases,
                                    test_name<TestType>) {
  vector_to_vector<TestType>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("union reinterpretation", "",
                                    Vector4ToScalarTestCases,
                                    test_name<TestType>) {
  vector_to_scalar<TestType>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("union reinterpretation", "",
                                    Vector4ToVector2TestCases,
                                    test_name<TestType>) {
  vector_to_vector<TestType>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("union reinterpretation", "",
                                    Vector4ToVector3TestCases,
                                    test_name<TestType>) {
  vector_to_vector<TestType>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("union reinterpretation", "",
                                    Vector4ToVector4TestCases,
                                    test_name<TestType>) {
  vector_to_vector<TestType>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("union reinterpretation", "",
                                    Vector4ToVector8TestCases,
                                    test_name<TestType>) {
  vector_to_vector<TestType>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("union reinterpretation", "",
                                    Vector4ToVector16TestCases,
                                    test_name<TestType>) {
  vector_to_vector<TestType>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("union reinterpretation", "",
                                    Vector8ToScalarTestCases,
                                    test_name<TestType>) {
  vector_to_scalar<TestType>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("union reinterpretation", "",
                                    Vector8ToVector2TestCases,
                                    test_name<TestType>) {
  vector_to_vector<TestType>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("union reinterpretation", "",
                                    Vector8ToVector3TestCases,
                                    test_name<TestType>) {
  vector_to_vector<TestType>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("union reinterpretation", "",
                                    Vector8ToVector4TestCases,
                                    test_name<TestType>) {
  vector_to_vector<TestType>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("union reinterpretation", "",
                                    Vector8ToVector8TestCases,
                                    test_name<TestType>) {
  vector_to_vector<TestType>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("union reinterpretation", "",
                                    Vector8ToVector16TestCases,
                                    test_name<TestType>) {
  vector_to_vector<TestType>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("union reinterpretation", "",
                                    Vector16ToScalarTestCases,
                                    test_name<TestType>) {
  vector_to_scalar<TestType>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("union reinterpretation", "",
                                    Vector16ToVector2TestCases,
                                    test_name<TestType>) {
  vector_to_vector<TestType>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("union reinterpretation", "",
                                    Vector16ToVector3TestCases,
                                    test_name<TestType>) {
  vector_to_vector<TestType>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("union reinterpretation", "",
                                    Vector16ToVector4TestCases,
                                    test_name<TestType>) {
  vector_to_vector<TestType>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("union reinterpretation", "",
                                    Vector16ToVector8TestCases,
                                    test_name<TestType>) {
  vector_to_vector<TestType>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("union reinterpretation", "",
                                    Vector16ToVector16TestCases,
                                    test_name<TestType>) {
  vector_to_vector<TestType>(program);
}

} // namespace
