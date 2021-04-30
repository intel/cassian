/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_OCLC_RELATIONAL_FUNCTIONS_COMMON_HPP
#define CASSIAN_OCLC_RELATIONAL_FUNCTIONS_COMMON_HPP

#include <cassian/runtime/runtime.hpp>
#include <string>

int get_local_work_size(const int global_work_size,
                        const cassian::Runtime &runtime);

int suggest_work_size(const std::string &type);

#endif
