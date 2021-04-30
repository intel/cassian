/*
 * Copyright (C) 2021 Intel Corporation
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
#include <vector>

namespace {

const std::string program = "kernels/oclc_conversions/explicit_conversion.cl";

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("explicit conversion", "",
                                    ca::ScalarToScalar, test_name<TestType>) {
  scalar_to_scalar<TestType>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("explicit conversion", "",
                                    ca::Vector2ToVector2, test_name<TestType>) {
  vector_to_vector<TestType>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("explicit conversion", "",
                                    ca::Vector3ToVector3, test_name<TestType>) {
  vector_to_vector<TestType>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("explicit conversion", "",
                                    ca::Vector4ToVector4, test_name<TestType>) {
  vector_to_vector<TestType>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("explicit conversion", "",
                                    ca::Vector8ToVector8, test_name<TestType>) {
  vector_to_vector<TestType>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("explicit conversion", "",
                                    ca::Vector16ToVector16,
                                    test_name<TestType>) {
  vector_to_vector<TestType>(program);
}

} // namespace
