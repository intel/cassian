/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <memory>
#include <string>

#include <cassian/cli/cli.hpp>
#include <cassian/main/config.hpp>
#include <cassian/runtime/factory.hpp>
#include <cassian/runtime/runtime.hpp>

namespace ca = cassian;

ca::test::Config::Config(const ca::CommandLineParser &parser)
    : TestConfigBase(parser) {}

const ca::test::Config *config = nullptr;
const ca::test::Config &ca::test::get_config() { return *config; }
void ca::test::set_config(const Config &c) { config = &c; }
