/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/system/factory.hpp>
#include <cassian/system/library.hpp>
#include <memory>
#include <string>

#if defined(_WIN32)
#include <library_windows.hpp>
#elif defined(__linux__)
#include <library_linux.hpp>
#endif

namespace cassian {

std::unique_ptr<Library> load_library(const std::string &name) {
#if defined(_WIN32)
  return std::make_unique<LibraryWindows>(name);
#elif defined(__linux__)
  return std::make_unique<LibraryLinux>(name);
#endif
}

} // namespace cassian
