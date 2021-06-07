/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_RUNTIME_OPENCLC_UTILS_HPP
#define CASSIAN_RUNTIME_OPENCLC_UTILS_HPP

#include <cassian/runtime/runtime.hpp>
#include <string>

/**
 * Cassian namespace.
 */
namespace cassian {
/**
 * Checks if OpenCL C 3.0 optional feature is supported
 *
 * @param[in] runtime
 * @param[in] program_type
 * @param[in] feature_macro name of feature
 * @throws cassian::RuntimeException Thrown if layout is unsupported for the
 * number of components
 */
bool check_optional_feature_support(Runtime *runtime,
                                    const std::string &program_type,
                                    const std::string &feature_macro);
} // namespace cassian

#endif