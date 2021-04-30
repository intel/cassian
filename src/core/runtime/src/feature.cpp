/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/runtime/feature.hpp>
#include <string>

namespace cassian {

std::string to_string(const Feature feature) {
  switch (feature) {
  case Feature::fp16:
    return "fp16";
  case Feature::fp64:
    return "fp64";
  case Feature::read_write_images:
    return "read_write_images";
  case Feature::image:
    return "image";
  case Feature::image2d:
    return "image2d";
  case Feature::sampling:
    return "sampling";
  case Feature::int64_atomics:
    return "int64_atomics";
  case Feature::global_offsets:
    return "global_offsets";
  default:
    return "unknown";
  }
}

} // namespace cassian
