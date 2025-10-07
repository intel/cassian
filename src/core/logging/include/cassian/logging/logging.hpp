/*
 * Copyright (C) 2021-2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_LOGGING_LOGGING_HPP
#define CASSIAN_LOGGING_LOGGING_HPP

#include <iostream>

/**
 * Cassian namespace.
 */
namespace cassian {

/**
 * Namespace for logging related functions.
 */
namespace logging {

enum class LogLevel { fatal = 1, error, warning, info, debug, trace };

enum class Prefix { no_prefix = 0, with_prefix = 1 };

/**
 * Class implementing logging capabilities.
 */
class Logger {
public:
  Logger &operator()(LogLevel log_level, Prefix prefix);
  template <typename T> Logger &log(const T &message) {
    if (log_level_ <= get_threshold()) {
      const std::string prefix_str =
          prefix_ == Prefix::with_prefix ? prefix(log_level_) : "";
      if (log_level_ <= LogLevel::error) {
        std::cerr << prefix_str << message;
      } else {
        std::cout << prefix_str << message;
      }
      prefix_ = Prefix::no_prefix;
    }
    return *this;
  }
  friend void set_threshold(LogLevel threshold);
  friend bool is_debug();

private:
  LogLevel get_threshold();
  static std::string prefix(LogLevel log_level);

  LogLevel log_level_;
  Prefix prefix_;
  LogLevel threshold_ = LogLevel::info;
};

/**
 * @brief Sets the logging threshold level.
 *
 * Messages with a log level below the specified threshold will be ignored.
 *
 * @param threshold The minimum LogLevel to be logged.
 */
void set_threshold(LogLevel threshold);

/**
 * @brief Checks if debug logging is enabled.
 *
 * @returns true if log level is debug or trace, false otherwise.
 */
bool is_debug();

/**
 * Log message.
 *
 * @tparam T the type of message to log.
 * @param[in] logger logger instance.
 * @param[in] message message to log.
 * @returns logger instance.
 */
template <typename T> Logger &operator<<(Logger &logger, const T &message) {
  return logger.log(message);
}

/**
 * Log message with trace level.
 *
 * @returns logger instance.
 */
Logger &trace(Prefix prefix = Prefix::with_prefix);

/**
 * Log message with debug level.
 *
 * @returns logger instance.
 */
Logger &debug(Prefix prefix = Prefix::with_prefix);

/**
 * Log message with info level.
 *
 * @returns logger instance.
 */
Logger &info(Prefix prefix = Prefix::with_prefix);

/**
 * Log message with warning level.
 *
 * @returns logger instance.
 */
Logger &warning(Prefix prefix = Prefix::with_prefix);

/**
 * Log message with error level.
 *
 * @returns logger instance.
 */
Logger &error(Prefix prefix = Prefix::with_prefix);

/**
 * Log message with fatal level.
 *
 * @returns logger instance.
 */
Logger &fatal(Prefix prefix = Prefix::with_prefix);

} // namespace logging
} // namespace cassian
#endif
