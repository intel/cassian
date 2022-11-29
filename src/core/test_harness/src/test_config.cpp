/*
 * Copyright (C) 2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/cli/cli.hpp>
#include <cassian/runtime/factory.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cassian/test_harness/test_config.hpp>
#include <cstddef>
#include <memory>
#include <string>

namespace cassian {

TestConfigBase::TestConfigBase(const CommandLineParser &parser) {
  runtime_ = create_runtime(parser.get<std::string>("--runtime"));
  runtime_->initialize();

  program_type_ = parser.get<std::string>("--program-type");
}

Runtime *TestConfigBase::runtime() const { return runtime_.get(); }
std::string TestConfigBase::program_type() const { return program_type_; }

void add_harness_arguments(CommandLineParser *parser) {
  add_runtime_arguments(parser);
}

} // namespace cassian