/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_RUNTIME_FACTORY_HPP
#define CASSIAN_RUNTIME_FACTORY_HPP

#include <cassian/cli/cli.hpp>
#include <cassian/runtime/runtime.hpp>
#include <memory>
#include <stdexcept>
#include <string>

/**
 * Cassian namespace.
 */
namespace cassian {

/**
 * Create runtime.
 *
 * @param[in] name runtime name.
 * @returns pointer to an object implementing cassian::Runtime interface.
 * @throws cassian::UnknownRuntimeException Thrown if `name` is not a supported
 * runtime.
 */
std::unique_ptr<Runtime> create_runtime(const std::string &name);

/**
 * Override runtime creation.
 *
 * @param[in] name runtime name.
 * @returns pointer to an object implementing cassian::Runtime interface
 * or nullptr if `name` should be handled by `create_runtime`.
 */
std::unique_ptr<Runtime> create_runtime_extra(const std::string &name);

/**
 * Register runtime specific command line arguments.
 *
 * @param[in] parser parser to update.
 */
void add_runtime_arguments(CommandLineParser *parser);

/**
 * Exception class used when an unknown runtime is used.
 */
class UnknownRuntimeException : public std::runtime_error {
  using std::runtime_error::runtime_error;
};
} // namespace cassian

#endif
