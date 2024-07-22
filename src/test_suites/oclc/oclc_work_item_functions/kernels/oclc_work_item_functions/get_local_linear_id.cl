/*
 * Copyright (C) 2022-2024 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifdef SIMD
__attribute__((intel_reqd_sub_group_size(SIMD)))
#endif // SIMD
kernel void
test_kernel(global uint *output) {
  const size_t tid =
      get_global_id(2) * get_global_size(1) * get_global_size(0) +
      get_global_id(1) * get_global_size(0) + get_global_id(0);
  output[tid] = get_local_linear_id();
};
