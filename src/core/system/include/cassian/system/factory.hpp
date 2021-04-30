/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_SYSTEM_FACTORY_HPP
#define CASSIAN_SYSTEM_FACTORY_HPP

#include <cassian/system/library.hpp>
#include <memory>
#include <string>

/**
 * Cassian namespace.
 */
namespace cassian {

/**
 * Load shared library.
 *
 * Library name must contain all system specific parts like:
 *  - `lib` prefix;
 *  - `.dll`/`.so` suffix;
 *
 * @param[in] name library name.
 * @returns pointer to an object implementing cassian::Library interface.
 * @throws cassian::LibraryNotFoundException Thrown if `name` library is not
 * found in the system.
 * @note library is unloaded when pointer is freed.
 */
std::unique_ptr<Library> load_library(const std::string &name);

} // namespace cassian
#endif
