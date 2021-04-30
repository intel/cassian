/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "test_config.hpp"
#include "common.hpp"
#include <cassian/cli/cli.hpp>
#include <cassian/runtime/factory.hpp>
#include <memory>
#include <string>

namespace ca = cassian;

TestConfig::TestConfig(const ca::CommandLineParser &parser) {
  runtime_ = ca::create_runtime(parser.get<std::string>("--runtime"));
  runtime_->initialize();

  program_type_ = parser.get<std::string>("--program-type");
  const auto work_size = parser.get<std::string>("--work-size");
  work_size_ = suggest_work_size(work_size);
}

ca::Runtime *TestConfig::runtime() const { return runtime_.get(); }
std::string TestConfig::program_type() const { return program_type_; }
size_t TestConfig::work_size() const { return work_size_; }

const TestConfig *config = nullptr;
const TestConfig &get_test_config() { return *config; }
void set_test_config(const TestConfig &c) { config = &c; }

void add_test_arguments(cassian::CommandLineParser *parser) {
  parser->add_argument("--work-size", "");
}
