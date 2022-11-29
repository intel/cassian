/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_OCLC_CONVERSIONS_TEST_CONFIG_HPP
#define CASSIAN_OCLC_CONVERSIONS_TEST_CONFIG_HPP

#include <cassian/cli/cli.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cassian/test_harness/test_config.hpp>
#include <memory>
#include <string>

class TestConfig : public cassian::TestConfigBase {
public:
  explicit TestConfig(const cassian::CommandLineParser &parser);
};

const TestConfig &get_test_config();
void set_test_config(const TestConfig &config);

#endif
