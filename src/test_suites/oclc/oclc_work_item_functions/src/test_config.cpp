/*
 * Copyright (C) 2021-2024 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/cli/cli.hpp>
#include <cassian/runtime/factory.hpp>
#include <cassian/runtime/runtime.hpp>
#include <common.hpp>
#include <cstddef>
#include <memory>
#include <string>
#include <test_config.hpp>

namespace ca = cassian;

TestConfig::TestConfig(const ca::CommandLineParser &parser)
    : TestConfigBase(parser) {
  work_size_ = suggest_work_size(parser.get<std::string>("--work-size"));
  simd_ = parser.get<std::string>("--simd");
}

size_t TestConfig::work_size() const { return work_size_; }
std::string TestConfig::simd() const { return simd_; }

const TestConfig *config = nullptr;
const TestConfig &get_test_config() { return *config; }
void set_test_config(const TestConfig &c) { config = &c; }

void add_test_arguments(cassian::CommandLineParser *parser) {
  parser->add_argument("--simd", "");
  parser->add_argument("--work-size", "");
}
