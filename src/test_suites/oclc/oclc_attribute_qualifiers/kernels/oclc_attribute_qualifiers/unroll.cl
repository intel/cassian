/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

kernel void test_kernel(global int *output) {
  const int gid = get_global_id(0);

  output[gid] = 0;

  __attribute__((opencl_unroll_hint)) for (int i = 0; i < 10; i++) {
    output[gid]++;
  }

  __attribute__((opencl_unroll_hint(5))) for (int i = 0; i < 10; i++) {
    output[gid]++;
  }
}
