/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_RUNTIME_OPENCLC_UTILS_HPP
#define CASSIAN_RUNTIME_OPENCLC_UTILS_HPP

#include <string>

#include <cassian/runtime/runtime.hpp>

/**
 * Cassian namespace.
 */
namespace cassian {
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
