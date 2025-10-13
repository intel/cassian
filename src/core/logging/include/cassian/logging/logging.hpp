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
      stream() << prefix_str << message;
      prefix_ = Prefix::no_prefix;
    }
    return *this;
  }

  Logger &setf(std::ostream &(*manip)(std::ostream &)) {
    if (log_level_ <= get_threshold()) {
      stream().setf(manip(stream()).flags());
    }
    return *this;
  }

  friend void set_threshold(LogLevel threshold);
  friend bool is_debug();

  friend Logger &flush(Logger &logger);

private:
  LogLevel get_threshold();
  static std::string prefix(LogLevel log_level);
  std::ostream &stream() {
    return (log_level_ <= LogLevel::error) ? std::cerr : std::cout;
  }
  template <typename T>
  friend Logger &operator<<(Logger &logger, const T &message);

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

Logger &flush(Logger &logger);

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
 * @brief Pass through logger.
 *
 * @param[in] logger logger instance.
 * @returns logger instance.
 */
Logger &operator<<(Logger &, Logger &logger);

/**
 * @brief Overloads the stream insertion operator to allow manipulators for
 * Logger.
 *
 * This operator enables the use of custom manipulators with the Logger class,
 * similar to how std::endl works with std::ostream. The manipulator is a
 * function that takes and returns a reference to a Logger object.
 *
 * @param logger Reference to the Logger object to be manipulated.
 * @param manip Pointer to a manipulator function that operates on Logger.
 * @return Reference to the Logger object after manipulation.
 */
Logger &operator<<(Logger &logger, Logger &(*manip)(Logger &));

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
