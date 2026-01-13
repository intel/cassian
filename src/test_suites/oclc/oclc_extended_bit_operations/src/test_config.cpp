/*
 * Copyright (C) 2026 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/cli/cli.hpp>
#include <cassian/runtime/factory.hpp>
#include <cassian/runtime/runtime.hpp>
#include <common.hpp>
#include <memory>
#include <string>
#include <test_config.hpp>

namespace ca = cassian;

TestConfig::TestConfig(const ca::CommandLineParser &parser)
    : TestConfigBase(parser) {
  work_size_ = suggest_work_size(parser.get<std::string>("--work-size"));
}

size_t TestConfig::work_size() const { return work_size_; }

const TestConfig *config = nullptr;
const TestConfig &get_test_config() { return *config; }
void set_test_config(const TestConfig &c) { config = &c; }

void add_test_arguments(cassian::CommandLineParser *parser) {
  parser->add_argument("--work-size", "");
}
