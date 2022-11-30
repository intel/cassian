/*
 * Copyright (C) 2021-2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/logging/logging.hpp>

namespace cassian::logging {
static Logger logger;
LogLevel Logger::threshold_ = LogLevel::info;

Logger &Logger::operator()(LogLevel log_level, Prefix prefix) {
  log_level_ = log_level;
  prefix_ = prefix;
  return *this;
}

void Logger::set_threshold(LogLevel threshold) { threshold_ = threshold; }

LogLevel Logger::get_threshold() { return threshold_; }

std::string Logger::prefix(LogLevel log_level) {
  switch (log_level) {
  case LogLevel::fatal:
    return "Fatal error: ";
  case LogLevel::error:
    return "Error: ";
  case LogLevel::warning:
    return "Warning: ";
  default:
  case LogLevel::info:
    return "";
  case LogLevel::debug:
    return "Debug: ";
  case LogLevel::trace:
    return "Trace: ";
  }
}

Logger &trace(Prefix prefix) { return logger(LogLevel::trace, prefix); }
Logger &debug(Prefix prefix) { return logger(LogLevel::debug, prefix); }
Logger &info(Prefix prefix) { return logger(LogLevel::info, prefix); }
Logger &warning(Prefix prefix) { return logger(LogLevel::warning, prefix); }
Logger &error(Prefix prefix) { return logger(LogLevel::error, prefix); }
Logger &fatal(Prefix prefix) { return logger(LogLevel::fatal, prefix); }

} // namespace cassian::logging
