/*
 * Copyright (C) 2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_TEST_HARNESS_TEST_CONFIG_HPP
#define CASSIAN_TEST_HARNESS_TEST_CONFIG_HPP

#include <cassian/cli/cli.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cstddef>
#include <memory>
#include <string>

namespace cassian {

class TestConfigBase {
public:
  TestConfigBase() = default;
  explicit TestConfigBase(const cassian::CommandLineParser &parser);
  TestConfigBase(const TestConfigBase &) = delete;
  TestConfigBase(TestConfigBase &&) = delete;
  ~TestConfigBase() = default;
  TestConfigBase &operator=(const TestConfigBase &) = delete;
  TestConfigBase &operator=(TestConfigBase &&) = delete;

  cassian::Runtime *runtime() const;
  std::string program_type() const;

protected:
  std::unique_ptr<cassian::Runtime> runtime_ = nullptr;
  std::string program_type_ = "";
};

void add_harness_arguments(CommandLineParser *parser);

} // namespace cassian

#endif
