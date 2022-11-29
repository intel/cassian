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

TestConfig::TestConfig(const ca::CommandLineParser &parser)
    : TestConfigBase(parser) {
  const auto work_size = parser.get<std::string>("--work-size");
  work_size_ = suggest_work_size(work_size);
  work_size_2d_ = suggest_2d_work_size(work_size);
  work_size_3d_ = suggest_3d_work_size(work_size);
}

size_t TestConfig::work_size() const { return work_size_; }
size_t TestConfig::work_size_2d() const { return work_size_2d_; }
size_t TestConfig::work_size_3d() const { return work_size_3d_; }

const TestConfig *config = nullptr;
const TestConfig &get_test_config() { return *config; }
void set_test_config(const TestConfig &c) { config = &c; }

void add_test_arguments(cassian::CommandLineParser *parser) {
  parser->add_argument("--work-size", "");
}
