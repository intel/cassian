/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_UTILITY_VERSION_HPP
#define CASSIAN_UTILITY_VERSION_HPP

#include <string>

/**
 * Cassian namespace.
 */
namespace cassian {

/**
 * Get version string.
 *
 * @returns version string.
 */
std::string get_version();

/**
 * Print version string.
 */
void print_version();

} // namespace cassian
#endif
