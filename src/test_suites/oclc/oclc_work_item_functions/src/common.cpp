/*
 * Copyright (C) 2021-2024 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <common.hpp>
#include <sstream>
#include <string>

namespace ca = cassian;

int suggest_work_size(const std::string &type) {
  const int default_size = 64;
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

ca::Feature parse_simd(const std::string &simd) {
  if (simd == "8") {
    return ca::Feature::simd8;
  }
  if (simd == "16") {
    return ca::Feature::simd16;
  }
  if (simd == "32") {
    return ca::Feature::simd32;
  }
  return ca::Feature{};
}

std::string get_build_options(const std::string &simd) {
  std::string build_options = " -cl-std=CL3.0";
  if (simd == "8") {
    build_options += " -D SIMD=8";
  }
  if (simd == "16") {
    build_options += " -D SIMD=16";
  }
  if (simd == "32") {
    build_options += " -D SIMD=32";
  }
  return build_options;
}

bool should_skip(const TestConfig &config) {
  ca::Requirements requirements;
  requirements.feature(parse_simd(config.simd()));
  return ca::should_skip_test(requirements, *config.runtime());
}