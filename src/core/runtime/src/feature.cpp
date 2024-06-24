/*
 * Copyright (C) 2021-2023 Intel Corporation
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
  case Feature::fp16_atomics_global_load_store:
    return "fp16_atomics_global_load_store";
  case Feature::fp32_atomics_global_load_store:
    return "fp32_atomics_global_load_store";
  case Feature::fp64_atomics_global_load_store:
    return "fp64_atomics_global_load_store";
  case Feature::fp16_atomics_local_load_store:
    return "fp16_atomics_local_load_store";
  case Feature::fp32_atomics_local_load_store:
    return "fp32_atomics_local_load_store";
  case Feature::fp64_atomics_local_load_store:
    return "fp64_atomics_local_load_store";
  case Feature::fp16_atomics_global_add:
    return "fp16_atomics_global_add";
  case Feature::fp32_atomics_global_add:
    return "fp32_atomics_global_add";
  case Feature::fp64_atomics_global_add:
    return "fp64_atomics_global_add";
  case Feature::fp16_atomics_local_add:
    return "fp16_atomics_local_add";
  case Feature::fp32_atomics_local_add:
    return "fp32_atomics_local_add";
  case Feature::fp64_atomics_local_add:
    return "fp64_atomics_local_add";
  case Feature::fp16_atomics_global_min_max:
    return "fp16_atomics_global_min_max";
  case Feature::fp32_atomics_global_min_max:
    return "fp32_atomics_global_min_max";
  case Feature::fp64_atomics_global_min_max:
    return "fp64_atomics_global_min_max";
  case Feature::fp16_atomics_local_min_max:
    return "fp16_atomics_local_min_max";
  case Feature::fp32_atomics_local_min_max:
    return "fp32_atomics_local_min_max";
  case Feature::fp64_atomics_local_min_max:
    return "fp64_atomics_local_min_max";
  case Feature::fp32_correctly_rounded_divide_sqrt:
    return "fp32_correctly_rounded_divide_sqrt";
  case Feature::intel_required_subgroup_size:
    return "intel_required_subgroup_size";
  case Feature::simd8:
    return "simd8";
  case Feature::simd16:
    return "simd16";
  case Feature::simd32:
    return "simd32";
  case Feature::integer_dp4a:
    return "integer_dp4a";
  case Feature::integer_dp4a_packed:
    return "integer_dp4a_packed";
  default:
    return "unknown";
  }
}

} // namespace cassian
