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

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("sub_group_reduce_min", "",
                                    AllScalarTestTypes, test_name<TestType>) {
  const TestConfig &config = get_test_config();

  using scalar_type = typename TestType::scalar_type;
  ca::Requirements requirements;
  requirements.arithmetic_type<scalar_type>();
  std::string test_func_name = "sub_group_reduce_min";
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

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("sub_group_reduce_max", "",
                                    AllScalarTestTypes, test_name<TestType>) {
  const TestConfig &config = get_test_config();

  using scalar_type = typename TestType::scalar_type;
  ca::Requirements requirements;
  requirements.arithmetic_type<scalar_type>();
  std::string test_func_name = "sub_group_reduce_max";
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

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("sub_group_reduce_add", "",
                                    AllScalarTestTypes, test_name<TestType>) {
  const TestConfig &config = get_test_config();

  using scalar_type = typename TestType::scalar_type;
  ca::Requirements requirements;
  requirements.arithmetic_type<scalar_type>();
  std::string test_func_name = "sub_group_reduce_add";
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
