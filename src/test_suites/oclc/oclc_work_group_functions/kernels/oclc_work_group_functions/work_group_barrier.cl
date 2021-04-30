/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

kernel void test_kernel_global(const global int *input, global int *output) {
  const int gid = get_global_id(0);
  const int gsize = get_global_size(0);

  output[gid] = input[gid];

  for (int i = 0; i < 10; i++) {
    work_group_barrier(CLK_GLOBAL_MEM_FENCE, MEMORY_SCOPE);

    if (gid + 1 < gsize) {
      output[gid] += output[gid + 1];
    } else {
      output[gid] += output[0];
    }
  }
}

kernel void test_kernel_local(const global int *input, global int *output) {
  local int temp[LOCAL_WORK_SIZE];
  const int tg = get_group_id(0);
  const int tid = get_local_id(0);
  const int tsize = get_local_size(0);

  temp[tid] = input[tg * tsize + tid];

  for (int i = 0; i < 10; i++) {
    work_group_barrier(CLK_LOCAL_MEM_FENCE);

    if (tid + 1 < tsize) {
      temp[tid] += temp[tid + 1];
    } else {
      temp[tid] += temp[0];
    }
  }
  output[tg * tsize + tid] = temp[tid];
}