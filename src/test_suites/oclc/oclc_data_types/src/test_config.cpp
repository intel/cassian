/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/cli/cli.hpp>
#include <cassian/runtime/factory.hpp>
#include <cassian/runtime/runtime.hpp>
#include <memory>
#include <string>
#include <test_config.hpp>

namespace ca = cassian;

TestConfig::TestConfig(const ca::CommandLineParser &parser) {
  runtime_ = ca::create_runtime(parser.get<std::string>("--runtime"));
  runtime_->initialize();

  program_type_ = parser.get<std::string>("--program-type");
}

ca::Runtime *TestConfig::runtime() const { return runtime_.get(); }
std::string TestConfig::program_type() const { return program_type_; }

const TestConfig *config = nullptr;
const TestConfig &get_test_config() { return *config; }
void set_test_config(const TestConfig &c) { config = &c; }
