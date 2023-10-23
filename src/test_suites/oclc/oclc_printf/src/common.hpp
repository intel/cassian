/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_OCLC_PRINTF_COMMON_HPP
#define CASSIAN_OCLC_PRINTF_COMMON_HPP

#include "test_config.hpp"
#include <cassian/logging/logging.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cassian/utility/utility.hpp>
#include <catch2/catch.hpp>
#include <tuple>
#include <utility>
#include <vector>

#if defined(__linux__)
#include <cstdlib>
#include <sys/types.h>
#include <unistd.h>
#elif defined(_WIN32)
#include <cstdio>
#include <io.h>
#endif

namespace ca = cassian;

int suggest_work_size(const std::string &type);

class CaptureOutput {
private:
  int original_fd;
  int fd;
  int stream;
  std::string filename;

public:
  enum { Stdout = 1, Stderr = 2 };

  CaptureOutput(int stream_) : stream(stream_) {
    original_fd = dup(stream);
#if defined(__linux__)
    filename = "/tmp/cassianCOXXXXXX";
    fd = mkstemp(filename.data());
#elif defined(_WIN32)
    filename = std::tmpnam(nullptr);
    fd = creat(filename.c_str(), _S_IREAD | _S_IWRITE);
#endif
    fflush(nullptr);
    dup2(fd, stream);
    close(fd);
  }

  ~CaptureOutput() {
    if (original_fd != -1) {
      fflush(nullptr);
      dup2(original_fd, stream);
      close(original_fd);
      original_fd = -1;
    }
    if (remove(filename.c_str()) != 0) {
      ca::logging::error() << "Deleting file " << filename.c_str() << " failed";
    }
  }

  std::string GetOutput() {
    if (original_fd != -1) {
      fflush(nullptr);
      dup2(original_fd, stream);
      close(original_fd);
      original_fd = -1;
    }
    return cassian::load_text_file(filename);
  }
};

#endif
