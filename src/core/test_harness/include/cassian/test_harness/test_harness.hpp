/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_TEST_HARNESS_TEST_HARNESS_HPP
#define CASSIAN_TEST_HARNESS_TEST_HARNESS_HPP

#include <cassian/runtime/openclc_types.hpp>
#include <cassian/runtime/runtime.hpp>

/**
 * Cassian namespace.
 */
namespace cassian {

/**
 * Test requirements.
 */
class Requirements {
public:
  /**
   * Check if all previously stated requirements are fulfilled.
   *
   * @param[in] runtime Runtime to check against.
   * @returns reason why requirements are not fulfilled or empty string
   * otherwise.
   */
  std::string check(const Runtime &runtime) const;

  /**
   * Require a feature.
   *
   * @param[in] feature Feature to be required.
   */
  void feature(Feature feature);

  /**
   * Require an arithmetic type.
   *
   * @tparam T OpenCL C scalar type to be required.
   */
  template <typename T> void arithmetic_type() {}

  /**
   * Require an atomic type.
   *
   * @tparam T OpenCL C scalar type to be required.
   */
  template <typename T> void atomic_type() {}

  /**
   * Require atomic add on a type.
   *
   * @tparam T OpenCL C scalar type to be required.
   */
  template <typename T> void atomic_add() {}

private:
  std::vector<Feature> features_;
};

/**
 * Specialization for half.
 */
template <> void Requirements::arithmetic_type<clc_half_t>();

/**
 * Specialization for half.
 */
template <> void Requirements::arithmetic_type<Half>();

/**
 * Specialization for double.
 */
template <> void Requirements::arithmetic_type<clc_double_t>();

/**
 * Specialization for double.
 */
template <> void Requirements::arithmetic_type<double>();

/**
 * Specialization for long.
 */
template <> void Requirements::atomic_type<clc_long_t>();

/**
 * Specialization for ulong.
 */
template <> void Requirements::atomic_type<clc_ulong_t>();

/**
 * Specialization for double.
 */
template <> void Requirements::atomic_type<clc_double_t>();

/**
 * Specialization for float.
 */
template <> void Requirements::atomic_add<clc_float_t>();

/**
 * Specialization for double.
 */
template <> void Requirements::atomic_add<clc_double_t>();

/**
 * Check if a test should be skipped based on given requirements and a runtime.
 * If requirements are not fulfilled then a message is logged.
 *
 * @param[in] requirements Test requirements.
 * @param[in] runtime Runtime to check against.
 * @returns true if the test should be skipped.
 */
bool should_skip_test(const Requirements &requirements, const Runtime &runtime);

} // namespace cassian
#endif
