/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <common.hpp>

int suggest_work_size(const std::string &type) {
  const int default_size = 16;
  const int min_size = 1;
  const int max_size = 1024;
  if (type == "min") {
    return min_size;
  }
  if (type == "max") {
    return max_size;
  }
  return default_size;
}
