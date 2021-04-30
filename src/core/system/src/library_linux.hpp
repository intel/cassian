/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_SYSTEM_LIBRARY_LINUX_HPP
#define CASSIAN_SYSTEM_LIBRARY_LINUX_HPP

#include <cassian/system/library.hpp>
#include <string>

namespace cassian {
class LibraryLinux : public Library {
public:
  LibraryLinux() = delete;
  LibraryLinux(const std::string &name);
  LibraryLinux(const LibraryLinux &) = delete;
  LibraryLinux(LibraryLinux &&) = delete;
  ~LibraryLinux();
  LibraryLinux &operator=(const LibraryLinux &) = delete;
  LibraryLinux &operator=(LibraryLinux &&) = delete;

  void *get_function(const std::string &name) const override;

private:
  void *handle_ = nullptr;
};

} // namespace cassian
#endif
