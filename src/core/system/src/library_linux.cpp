/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/system/library.hpp>
#include <dlfcn.h>
#include <library_linux.hpp>
#include <string>

namespace cassian {

LibraryLinux::LibraryLinux(const std::string &name) {
  handle_ = dlopen(name.c_str(), RTLD_NOW);
  if (handle_ == nullptr) {
    throw LibraryNotFoundException("Failed to load library: " + name);
  }
}

LibraryLinux::~LibraryLinux() { dlclose(handle_); }

void *LibraryLinux::get_function(const std::string &name) const {
  void *function = dlsym(handle_, name.c_str());
  if (function == nullptr) {
    throw LibraryFunctionNotFoundException("Failed to load library function: " +
                                           name);
  }
  return function;
}

} // namespace cassian
