/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_RUNTIME_FEATURE_HPP
#define CASSIAN_RUNTIME_FEATURE_HPP

#include <string>

/**
 * Cassian namespace.
 */
namespace cassian {

/**
 * Runtime features.
 */
enum class Feature {
  fp16,
  fp64,
  read_write_images,
  image,
  image2d,
  sampling,
  int64_atomics,
  global_offsets
};

/**
 * Convert Feature to string representation.
 *
 * @param[in] feature object to convert.
 */
std::string to_string(Feature feature);
} // namespace cassian

#endif
