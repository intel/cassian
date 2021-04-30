/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/vector/vector.hpp>

namespace cassian {
std::string
VectorBadNumberOfElementsException::error_message(const std::string &source,
                                                  int expected, int actual) {
  std::stringstream stream;
  stream << "cassian::Vector: " << source << " contains " << actual
         << " elements, expected " << expected;
  return stream.str();
}

std::string VectorAccessViolationException::error_message(int index, int size) {
  std::stringstream stream;
  stream << "cassian::Vector: access violation, accessed element: " << index
         << " vector size: " << size;
  return stream.str();
}

} // namespace cassian