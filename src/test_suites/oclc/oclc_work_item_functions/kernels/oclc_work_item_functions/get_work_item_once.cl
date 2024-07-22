/*
 * Copyright (C) 2024 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifdef SIMD
__attribute__((intel_reqd_sub_group_size(SIMD)))
#endif // SIMD
kernel void
test_kernel_global(global atomic_int *output) {
  uint global_id = 0;
  for (int dim = 0; dim < get_work_dim(); dim++) {
    uint temp = get_global_id(dim);
    for (int i = 0; i < dim; i++) {
      temp *= get_global_size(i);
    }
    global_id += temp;
  }
  atomic_fetch_add(&output[local_id], 1);
};

#ifdef SIMD
__attribute__((intel_reqd_sub_group_size(SIMD)))
#endif // SIMD
kernel void
test_kernel_local(global atomic_int *output) {
  uint local_id = 0;
  for (int dim = 0; dim < get_work_dim(); dim++) {
    uint temp = get_local_id(dim);
    for (int i = 0; i < dim; i++) {
      temp *= get_local_size(i);
    }
    local_id += temp;
  }
  atomic_fetch_add(&output[local_id], 1);
}