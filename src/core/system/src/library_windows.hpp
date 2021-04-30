/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_SYSTEM_LIBRARY_WINDOWS_HPP
#define CASSIAN_SYSTEM_LIBRARY_WINDOWS_HPP

#include <cassian/system/library.hpp>
#include <string>
#include <windows.h>

namespace cassian {
class LibraryWindows : public Library {
public:
  LibraryWindows() = delete;
  LibraryWindows(const std::string &name);
  LibraryWindows(const LibraryWindows &) = delete;
  LibraryWindows(LibraryWindows &&) = delete;
  ~LibraryWindows();
  LibraryWindows &operator=(const LibraryWindows &) = delete;
  LibraryWindows &operator=(LibraryWindows &&) = delete;

  void *get_function(const std::string &name) const override;

private:
  HMODULE handle_ = NULL;
};

} // namespace cassian
#endif
