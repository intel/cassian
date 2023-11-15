/*
 * Copyright (C) 2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/cli/cli.hpp>
#include <cassian/logging/logging.hpp>
#include <cassian/runtime/factory.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cassian/test_harness/test_config.hpp>
#include <cstddef>
#include <memory>
#include <string>

namespace cassian {

namespace {

class LogLevelConverter {
public:
  explicit LogLevelConverter(const std::string &log_level) {
    if (log_level == "fatal") {
      log_level_ = logging::LogLevel::fatal;
    } else if (log_level == "error") {
      log_level_ = logging::LogLevel::error;
    } else if (log_level == "warning") {
      log_level_ = logging::LogLevel::warning;
    } else if (log_level == "info") {
      log_level_ = logging::LogLevel::info;
    } else if (log_level == "debug") {
      log_level_ = logging::LogLevel::debug;
    } else if (log_level == "trace") {
      log_level_ = logging::LogLevel::trace;
    } else {
      throw RuntimeException("Unknown logging level: " + log_level);
    }
  }
  explicit operator logging::LogLevel() const { return log_level_; }

  logging::LogLevel log_level_;
};

} // namespace

TestConfigBase::TestConfigBase(const CommandLineParser &parser) {
  runtime_ = create_runtime(parser.get<std::string>("--runtime"));
  if (!parser.list_requested()) {
    runtime_->initialize();
  }

  auto log_level =
      logging::LogLevel(parser.get<LogLevelConverter>("--logging-level"));
  logging::Logger::set_threshold(log_level);

  program_type_ = parser.get<std::string>("--program-type");
}

Runtime *TestConfigBase::runtime() const { return runtime_.get(); }
std::string TestConfigBase::program_type() const { return program_type_; }

void add_harness_arguments(CommandLineParser *parser) {
  add_runtime_arguments(parser);

  parser->add_argument("--logging-level", "info");
}

} // namespace cassian