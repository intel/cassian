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

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("sub_group_shuffle", "", GenericTestTypes,
                                    test_name<TestType>) {
  const TestConfig &config = get_test_config();

  using scalar_type = typename TestType::scalar_type;
  ca::Requirements requirements;
  requirements.arithmetic_type<scalar_type>();
  std::string test_func_name = "sub_group_shuffle";
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

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("sub_group_shuffle_common_offset", "",
                                    GenericTestTypes, test_name<TestType>) {
  const TestConfig &config = get_test_config();

  using scalar_type = typename TestType::scalar_type;
  ca::Requirements requirements;
  requirements.arithmetic_type<scalar_type>();
  std::string test_func_name = "sub_group_shuffle_common_offset";
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

const uint32_t max_delta_size = 32;
TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("sub_group_shuffle_down", "",
                                    GenericTestTypes, test_name<TestType>) {
  const TestConfig &config = get_test_config();

  using scalar_type = typename TestType::scalar_type;
  ca::Requirements requirements;
  requirements.arithmetic_type<scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }
  std::string test_func_name = "sub_group_shuffle_down";
  SECTION("1D") {
    test_subgroup_generic<TestType, 1>(get_test_config(), test_func_name,
                                       max_delta_size);
  }
  SECTION("2D") {
    test_subgroup_generic<TestType, 2>(get_test_config(), test_func_name,
                                       max_delta_size);
  }
  SECTION("3D") {
    test_subgroup_generic<TestType, 3>(get_test_config(), test_func_name,
                                       max_delta_size);
  }
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("sub_group_shuffle_up", "",
                                    GenericTestTypes, test_name<TestType>) {
  const TestConfig &config = get_test_config();

  using scalar_type = typename TestType::scalar_type;
  ca::Requirements requirements;
  requirements.arithmetic_type<scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }
  std::string test_func_name = "sub_group_shuffle_up";
  SECTION("1D") {
    test_subgroup_generic<TestType, 1>(get_test_config(), test_func_name,
                                       max_delta_size);
  }
  SECTION("2D") {
    test_subgroup_generic<TestType, 2>(get_test_config(), test_func_name,
                                       max_delta_size);
  }
  SECTION("3D") {
    test_subgroup_generic<TestType, 3>(get_test_config(), test_func_name,
                                       max_delta_size);
  }
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("sub_group_shuffle_mixed", "",
                                    GenericTestTypes, test_name<TestType>) {
  const TestConfig &config = get_test_config();

  using scalar_type = typename TestType::scalar_type;
  ca::Requirements requirements;
  requirements.arithmetic_type<scalar_type>();
  std::string test_func_name = "sub_group_shuffle_mixed";
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

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("sub_group_shuffle_xor", "",
                                    GenericTestTypes, test_name<TestType>) {
  const TestConfig &config = get_test_config();

  using scalar_type = typename TestType::scalar_type;
  ca::Requirements requirements;
  requirements.arithmetic_type<scalar_type>();
  std::string test_func_name = "sub_group_shuffle_xor";
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
