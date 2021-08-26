/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

kernel void test_kernel_global(const global int *input, global int *temp,
                               global int *output) {
  const int gid = get_global_id(0);
  const int gsize = get_global_size(0);
  const int tg = get_group_id(0);
  const int tid = get_local_id(0);
  const int tsize = get_local_size(0);

  output[gid] = 0;
  temp[gid] = input[gid];

  for (int i = 0; i < 10; i++) {
    work_group_barrier(CLK_GLOBAL_MEM_FENCE, MEMORY_SCOPE);

    if (tid + 1 < tsize) {
      output[gid] += temp[gid + 1];
    } else {
      output[gid] += temp[tg * tsize];
    }

    work_group_barrier(CLK_GLOBAL_MEM_FENCE, MEMORY_SCOPE);

    if (tid + 1 < tsize) {
      temp[gid] += output[gid + 1];
    } else {
      temp[gid] += output[tg * tsize];
    }
  }
  work_group_barrier(CLK_GLOBAL_MEM_FENCE, MEMORY_SCOPE);
  output[gid] = temp[gid];
}

kernel void test_kernel_local(const global int *input, global int *unused,
                              global int *output) {
  local int temp[LOCAL_WORK_SIZE];
  local int temp2[LOCAL_WORK_SIZE];
  const int tg = get_group_id(0);
  const int tid = get_local_id(0);
  const int tsize = get_local_size(0);

  temp[tid] = 0;
  temp2[tid] = input[tg * tsize + tid];

  for (int i = 0; i < 10; i++) {
    work_group_barrier(CLK_LOCAL_MEM_FENCE);

    if (tid + 1 < tsize) {
      temp[tid] += temp2[tid + 1];
    } else {
      temp[tid] += temp2[0];
    }

    work_group_barrier(CLK_LOCAL_MEM_FENCE);

    if (tid + 1 < tsize) {
      temp2[tid] += temp[tid + 1];
    } else {
      temp2[tid] += temp[0];
    }
  }
  work_group_barrier(CLK_LOCAL_MEM_FENCE);
  output[tg * tsize + tid] = temp2[tid];
}