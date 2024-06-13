/*
 * Copyright (C) 2022-2024 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <catch2/catch.hpp>
#include <common.hpp>
namespace ca = cassian;

namespace {
using OneTestType = std::tuple<ca::clc_uint_t>;
TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("sub_group_all", "", OneTestType,
                                    test_name<TestType>) {
  const TestConfig &config = get_test_config();

  using scalar_type = typename TestType::scalar_type;
  std::string test_func_name = "sub_group_all";
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

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("sub_group_all_negative", "", OneTestType,
                                    test_name<TestType>) {
  const TestConfig &config = get_test_config();

  using scalar_type = typename TestType::scalar_type;
  std::string test_func_name = "sub_group_all_negative";
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

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("sub_group_any", "", OneTestType,
                                    test_name<TestType>) {
  const TestConfig &config = get_test_config();

  using scalar_type = typename TestType::scalar_type;
  std::string test_func_name = "sub_group_any";
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

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("sub_group_any_negative", "", OneTestType,
                                    test_name<TestType>) {
  const TestConfig &config = get_test_config();

  using scalar_type = typename TestType::scalar_type;
  std::string test_func_name = "sub_group_any_negative";
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
