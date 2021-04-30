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
#include <vector>

namespace {

const std::string program = "kernels/oclc_conversions/explicit_cast.cl";

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("explicit cast", "", ca::ScalarToScalar,
                                    test_name<TestType>) {
  scalar_to_scalar<TestType>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("explicit cast", "", ca::ScalarToVector2,
                                    test_name<TestType>) {
  scalar_to_vector<TestType>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("explicit cast", "", ca::ScalarToVector3,
                                    test_name<TestType>) {
  scalar_to_vector<TestType>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("explicit cast", "", ca::ScalarToVector4,
                                    test_name<TestType>) {
  scalar_to_vector<TestType>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("explicit cast", "", ca::ScalarToVector8,
                                    test_name<TestType>) {
  scalar_to_vector<TestType>(program);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("explicit cast", "", ca::ScalarToVector16,
                                    test_name<TestType>) {
  scalar_to_vector<TestType>(program);
}

} // namespace
