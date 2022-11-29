/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_MAIN_CONFIG_HPP
#define CASSIAN_MAIN_CONFIG_HPP

#include <memory>
#include <string>

#include <cassian/cli/cli.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cassian/test_harness/test_config.hpp>

namespace cassian {
namespace test {

class Config : public cassian::TestConfigBase {
public:
  explicit Config(const cassian::CommandLineParser &parser);
};

const Config &get_config();
void set_config(const Config &config);

} // namespace test
} // namespace cassian

#endif
