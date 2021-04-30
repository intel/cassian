/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_OCLC_ACCESS_QUALIFIERS_TEST_CONFIG_HPP
#define CASSIAN_OCLC_ACCESS_QUALIFIERS_TEST_CONFIG_HPP

#include <cassian/cli/cli.hpp>
#include <cassian/runtime/runtime.hpp>
#include <memory>
#include <string>

class TestConfig {
public:
  TestConfig() = default;
  explicit TestConfig(const cassian::CommandLineParser &parser);
  TestConfig(const TestConfig &) = delete;
  TestConfig(TestConfig &&) = delete;
  ~TestConfig() = default;
  TestConfig &operator=(const TestConfig &) = delete;
  TestConfig &operator=(TestConfig &&) = delete;

  cassian::Runtime *runtime() const;
  std::string program_type() const;

private:
  std::unique_ptr<cassian::Runtime> runtime_ = nullptr;
  std::string program_type_ = "";
};

const TestConfig &get_test_config();
void set_test_config(const TestConfig &config);

#endif
