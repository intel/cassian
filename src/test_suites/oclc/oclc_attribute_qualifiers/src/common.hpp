/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_OCLC_ATTRIBUTE_QUALIFIERS_COMMON_HPP
#define CASSIAN_OCLC_ATTRIBUTE_QUALIFIERS_COMMON_HPP

#include "test_config.hpp"
#include <cassian/logging/logging.hpp>
#include <cassian/runtime/runtime.hpp>
#include <catch2/catch.hpp>
#include <tuple>
#include <utility>
#include <vector>

namespace ca = cassian;

int suggest_work_size(const std::string &type);

#endif
