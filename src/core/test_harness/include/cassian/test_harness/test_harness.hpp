/*
 * Copyright (C) 2021-2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_TEST_HARNESS_TEST_HARNESS_HPP
#define CASSIAN_TEST_HARNESS_TEST_HARNESS_HPP

#include <cassian/runtime/openclc_types.hpp>
#include <cassian/runtime/property_checks.hpp>
#include <cassian/runtime/runtime.hpp>
#include <memory>

/**
 * Cassian namespace.
 */
namespace cassian {

enum class AtomicMemoryType { local, global, all };

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
  std::string check(Runtime &runtime) const;

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
  template <typename T> void arithmetic_type() {
    arithmetic_type_internal<T>(nullptr);
  }

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
  template <typename T>
  void atomic_add(AtomicMemoryType type = AtomicMemoryType::all) {}

  /**
   * Require atomic min_max on a type.
   *
   * @tparam T OpenCL C scalar type to be required.
   */
  template <typename T>
  void atomic_min_max(AtomicMemoryType type = AtomicMemoryType::all) {}

  /**
   * Require atomic load_store on a type.
   *
   * @tparam T OpenCL C scalar type to be required.
   */
  template <typename T>
  void atomic_load_store(AtomicMemoryType type = AtomicMemoryType::all) {}

  /**
   * Require correctly-rounded divide/sqrt.
   *
   * @tparam T OpenCl C scalar type to be required.
   */
  template <typename T> void correctly_rounded_divide_sqrt() {}

  /**
   * Require a work group size.
   *
   * @param x Work group X size.
   * @param y Work group Y size, the default is 1.
   * @param z Work group Z size, the default is 1.
   */
  void min_work_group_size(int x, int y = 1, int z = 1);

  /**
   * Require a sub group size.
   */
  template <int N> void sub_group_size() {}

  /**
   * Require OpenCL C feature support.
   */
  void openclc_feature(const std::string &feature,
                       const std::string &program_type);

private:
  std::vector<Feature> features_;
  std::vector<std::unique_ptr<PropertyCheck>> properties_;

  /**
   * Specialization for OpenCL C type wrappers.
   *
   * If scalar_type doesn't exist in T, this overload is discarded.
   */
  template <typename T>
  void arithmetic_type_internal(typename T::scalar_type *) {
    static_assert(std::is_same_v<T, typename T::scalar_type>,
                  "Must be scalar type");
  }

  /**
   * Specialization for generic case.
   *
   * ... argument makes this overload to have lowest rank, so it gets
   * picked last.
   */
  template <typename> void arithmetic_type_internal(...) {}
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
template <> void Requirements::atomic_add<clc_float_t>(AtomicMemoryType type);

/**
 * Specialization for double.
 */
template <> void Requirements::atomic_add<clc_double_t>(AtomicMemoryType type);

/**
 * Specialization for half.
 */
template <> void Requirements::atomic_add<clc_half_t>(AtomicMemoryType type);

/**
 * Specialization for float.
 */
template <>
void Requirements::atomic_min_max<clc_float_t>(AtomicMemoryType type);

/**
 * Specialization for double.
 */
template <>
void Requirements::atomic_min_max<clc_double_t>(AtomicMemoryType type);

/**
 * Specialization for half.
 */
template <>
void Requirements::atomic_min_max<clc_half_t>(AtomicMemoryType type);

/**
 * Specialization for float.
 */
template <>
void Requirements::atomic_load_store<clc_float_t>(AtomicMemoryType type);

/**
 * Specialization for double.
 */
template <>
void Requirements::atomic_load_store<clc_double_t>(AtomicMemoryType type);

/**
 * Specialization for half.
 */
template <>
void Requirements::atomic_load_store<clc_half_t>(AtomicMemoryType type);

/**
 * Specialization for float.
 */
template <> void Requirements::correctly_rounded_divide_sqrt<clc_float_t>();

/**
 * Specialization for sub group size 8.
 */
template <> void Requirements::sub_group_size<8>();

/**
 * Specialization for sub group size 16.
 */
template <> void Requirements::sub_group_size<16>();

/**
 * Specialization for sub group size 32.
 */
template <> void Requirements::sub_group_size<32>();

/**
 * Check if a test should be skipped based on given requirements and a runtime.
 * If requirements are not fulfilled then a message is logged.
 *
 * @param[in] requirements Test requirements.
 * @param[in] runtime Runtime to check against.
 * @returns true if the test should be skipped.
 */
bool should_skip_test(const Requirements &requirements, Runtime &runtime);

} // namespace cassian
#endif
