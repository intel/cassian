/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_RUNTIME_OPENCLC_UTILS_HPP
#define CASSIAN_RUNTIME_OPENCLC_UTILS_HPP

#include <string>
#include <vector>

#include <cassian/runtime/runtime.hpp>

/**
 * Cassian namespace.
 */
namespace cassian {
/**
 * OpenCL C memory scope used by atomic and barrier functions.
 */
enum class AtomicMemoryScope { work_item, work_group, device, all_svm_devices };

/**
 * Convert AtomicMemoryScope to the OpenCL C enumeration constant.
 *
 * @param[in] memory_scope memory scope to convert.
 */
std::string to_string(AtomicMemoryScope memory_scope);

/**
 * OpenCL C memory order used by atomic functions.
 *
 * Enumerators are declared from weakest to strongest and test suites compare
 * them with relational operators, so do not reorder them.
 */
enum class AtomicMemoryOrder { relaxed, acquire, release, acq_rel, seq_cst };

/**
 * Convert AtomicMemoryOrder to the OpenCL C enumeration constant.
 *
 * @param[in] memory_order memory order to convert.
 */
std::string to_string(AtomicMemoryOrder memory_order);

/**
 * All memory orders usable by atomic functions.
 */
inline const std::vector<AtomicMemoryOrder> atomic_memory_orders_all = {
    AtomicMemoryOrder::relaxed, AtomicMemoryOrder::acquire,
    AtomicMemoryOrder::release, AtomicMemoryOrder::acq_rel,
    AtomicMemoryOrder::seq_cst};

/**
 * Checks if OpenCL C preprocessor macro is true.
 *
 * @param[in] runtime
 * @param[in] program_type
 * @param[in] if_clause #if clause to check
 */
bool check_optional_openclc_macro(Runtime *runtime,
                                  const std::string &program_type,
                                  const std::string &if_clause);
/**
 * Checks if OpenCL C 3.0 optional feature is supported
 *
 * @param[in] runtime
 * @param[in] program_type
 * @param[in] feature_macro name of feature
 */
bool check_optional_openclc_feature_support(Runtime *runtime,
                                            const std::string &program_type,
                                            const std::string &feature_macro);
} // namespace cassian

#endif
