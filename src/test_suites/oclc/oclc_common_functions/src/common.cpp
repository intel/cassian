/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <common.hpp>
#include <string>

int suggest_work_size(const std::string &type) {
  const int default_size = 16;
  const int min_size = 2;
  const int max_size = 1024;
  if (type == "min") {
    return min_size;
  }
  if (type == "max") {
    return max_size;
  }
  return default_size;
}

std::string create_section_name(const std::string &function_string,
                                const size_t &section_type) {
  std::stringstream ss;
  ss << "Function: " << function_string << ' ';
  if (section_type == 0) {
    // randomize first input across all work items
    ss << "random";
  } else {
    ss << "special " << std::to_string(section_type);
  }
  return ss.str();
}