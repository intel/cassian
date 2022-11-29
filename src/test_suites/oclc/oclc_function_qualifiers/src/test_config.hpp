/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_OCLC_FUNCTION_QUALIFIERS_TEST_CONFIG_HPP
#define CASSIAN_OCLC_FUNCTION_QUALIFIERS_TEST_CONFIG_HPP

#include <cassian/cli/cli.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cassian/test_harness/test_config.hpp>
#include <cstddef>
#include <memory>
#include <string>

class TestConfig : public cassian::TestConfigBase {
public:
  explicit TestConfig(const cassian::CommandLineParser &parser);

  size_t work_size() const;
  size_t work_size_2d() const;
  size_t work_size_3d() const;

private:
  size_t work_size_ = 0;
  size_t work_size_2d_ = 0;
  size_t work_size_3d_ = 0;
};

const TestConfig &get_test_config();
void set_test_config(const TestConfig &config);

void add_test_arguments(cassian::CommandLineParser *parser);

#endif
