/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/logging/logging.hpp>

namespace cassian::logging {
static Logger logger;

Logger &trace() { return logger; }
Logger &debug() { return logger; }
Logger &info() { return logger; }
Logger &warning() { return logger; }
Logger &error() { return logger; }
Logger &fatal() { return logger; }
} // namespace cassian::logging
