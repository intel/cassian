/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/system/library.hpp>
#include <library_windows.hpp>
#include <string>
#include <windows.h>

namespace cassian {

LibraryWindows::LibraryWindows(const std::string &name) {
  handle_ = LoadLibraryA(name.c_str());
  if (handle_ == NULL) {
    throw LibraryNotFoundException("Failed to load library: " + name);
  }
}

LibraryWindows::~LibraryWindows() { FreeLibrary(handle_); }

void *LibraryWindows::get_function(const std::string &name) const {
  void *function = GetProcAddress(handle_, name.c_str());
  if (function == nullptr) {
    throw LibraryFunctionNotFoundException("Failed to load library function: " +
                                           name);
  }
  return function;
}

} // namespace cassian
