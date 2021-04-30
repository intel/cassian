/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_UTILITY_VERSION_CONFIG_HPP
#define CASSIAN_UTILITY_VERSION_CONFIG_HPP

#include <string>

namespace cassian {
extern const int major_version;
extern const int minor_version;
extern const int patch_version;
extern const std::string git_revision;
} // namespace cassian
#endif
