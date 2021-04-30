/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_SYSTEM_LIBRARY_HPP
#define CASSIAN_SYSTEM_LIBRARY_HPP

#include <stdexcept>
#include <string>

/**
 * Cassian namespace.
 */
namespace cassian {

/**
 * Interface for a system specific, shared library management.
 */
class Library {
public:
  /**
   * Default constructor.
   */
  Library() = default;

  /**
   * Virtual destructor.
   */
  virtual ~Library() = default;

  /**
   * Get a pointer to a function.
   *
   * @param[in] name function name.
   * @returns pointer to a function.
   * @throws cassian::LibraryFunctionNotFoundException Thrown if `name` symbol
   * is not found in the library.
   */
  virtual void *get_function(const std::string &name) const = 0;

private:
  Library(const Library &) = default;
  Library(Library &&) = default;
  Library &operator=(const Library &) = default;
  Library &operator=(Library &&) = default;
};

/**
 * Exception class used when a given library is not found in the system.
 */
class LibraryNotFoundException : public std::runtime_error {
  using std::runtime_error::runtime_error;
};

/**
 * Exception class used when a given function is not found in the library.
 */
class LibraryFunctionNotFoundException : public std::runtime_error {
  using std::runtime_error::runtime_error;
};

} // namespace cassian
#endif
