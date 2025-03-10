/*
 * Copyright (C) 2021-2023 Intel Corporation
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
  global_offsets,
  fp16_atomics_global_load_store,
  fp32_atomics_global_load_store,
  fp64_atomics_global_load_store,
  fp16_atomics_local_load_store,
  fp32_atomics_local_load_store,
  fp64_atomics_local_load_store,
  fp16_atomics_global_add,
  fp32_atomics_global_add,
  fp64_atomics_global_add,
  fp16_atomics_local_add,
  fp32_atomics_local_add,
  fp64_atomics_local_add,
  fp16_atomics_global_min_max,
  fp32_atomics_global_min_max,
  fp64_atomics_global_min_max,
  fp16_atomics_local_min_max,
  fp32_atomics_local_min_max,
  fp64_atomics_local_min_max,
  fp32_correctly_rounded_divide_sqrt,
  intel_required_subgroup_size,
  simd8,
  simd16,
  simd32,
  integer_dp4a,
  integer_dp4a_packed,
  non_uniform_work_group
};

/**
 * Convert Feature to string representation.
 *
 * @param[in] feature object to convert.
 */
std::string to_string(Feature feature);
} // namespace cassian

#endif
