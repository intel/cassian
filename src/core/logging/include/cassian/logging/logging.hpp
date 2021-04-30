/*
 * Copyright (C) 2021 Intel Corporation
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

/**
 * Class implementing logging capabilities.
 */
class Logger {};

/**
 * Log message.
 *
 * @tparam T the type of message to log.
 * @param[in] logger logger instance.
 * @param[in] message message to log.
 * @returns logger instance.
 */
template <typename T> Logger &operator<<(Logger &logger, const T &message) {
  std::cout << message;
  return logger;
}

/**
 * Log message with trace level.
 *
 * @returns logger instance.
 */
Logger &trace();

/**
 * Log message with debug level.
 *
 * @returns logger instance.
 */
Logger &debug();

/**
 * Log message with info level.
 *
 * @returns logger instance.
 */
Logger &info();

/**
 * Log message with warning level.
 *
 * @returns logger instance.
 */
Logger &warning();

/**
 * Log message with error level.
 *
 * @returns logger instance.
 */
Logger &error();

/**
 * Log message with fatal level.
 *
 * @returns logger instance.
 */
Logger &fatal();

} // namespace logging
} // namespace cassian
#endif
