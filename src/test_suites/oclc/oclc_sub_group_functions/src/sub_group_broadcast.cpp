/*
 * Copyright (C) 2022-2024 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <catch2/catch.hpp>
#include <common.hpp>
#include <numeric>
#include <test_config.hpp>

namespace ca = cassian;

namespace {

template <typename T> std::string test_name() {
  return std::string(T::type_name);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("sub_group_broadcast", "",
                                    AllScalarTestTypes, test_name<TestType>) {
  const TestConfig &config = get_test_config();
  std::string test_func_name = "sub_group_broadcast";
  ca::Requirements requirements;
  requirements.arithmetic_type<TestType>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }
  SECTION("1D") {
    test_subgroup_generic<TestType, 1>(get_test_config(), test_func_name);
  }
  SECTION("2D") {
    test_subgroup_generic<TestType, 2>(get_test_config(), test_func_name);
  }
  SECTION("3D") {
    test_subgroup_generic<TestType, 3>(get_test_config(), test_func_name);
  }
}

} // namespace
