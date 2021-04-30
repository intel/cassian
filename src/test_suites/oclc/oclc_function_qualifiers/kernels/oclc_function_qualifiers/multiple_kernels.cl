/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

kernel void set_dst_value(global int *src, global int *dst);

kernel void multi_kernel(global int *src, global int *dst) {
  set_dst_value(src, dst);
};

kernel void set_dst_value(global int *src, global int *dst) {
  int tid = get_global_id(0);
  dst[tid] = src[tid];
};
