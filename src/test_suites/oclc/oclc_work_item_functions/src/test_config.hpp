/*
 * Copyright (C) 2021-2024 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_OCLC_WORK_ITEM_FUNCTIONS_TEST_CONFIG_HPP
#define CASSIAN_OCLC_WORK_ITEM_FUNCTIONS_TEST_CONFIG_HPP

#include <cassian/cli/cli.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cassian/test_harness/test_config.hpp>
#include <cstddef>
#include <memory>
#include <string>

namespace ca = cassian;

class TestConfig : public ca::TestConfigBase {
public:
  explicit TestConfig(const ca::CommandLineParser &parser);

  size_t work_size() const;
  std::string simd() const;

private:
  size_t work_size_ = 0;
  std::string simd_;
};

const TestConfig &get_test_config();
void set_test_config(const TestConfig &config);

void add_test_arguments(ca::CommandLineParser *parser);

#endif
