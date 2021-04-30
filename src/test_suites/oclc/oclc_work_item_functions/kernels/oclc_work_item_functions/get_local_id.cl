/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

kernel void test_kernel_1(global uint *output) {
  const size_t tid = get_global_id(0);
  output[tid] = (uint)get_local_id(0);
};

kernel void test_kernel_2(global uint *output_x, global uint *output_y) {
  const size_t tid_x = get_global_id(0);
  const size_t tid_y = get_global_id(1);
  output_x[tid_x] = (uint)get_local_id(0);
  output_y[tid_y] = (uint)get_local_id(1);
};

kernel void test_kernel_3(global uint *output_x, global uint *output_y,
                          global uint *output_z) {
  const size_t tid_x = get_global_id(0);
  const size_t tid_y = get_global_id(1);
  const size_t tid_z = get_global_id(2);
  output_x[tid_x] = (uint)get_local_id(0);
  output_y[tid_y] = (uint)get_local_id(1);
  output_z[tid_z] = (uint)get_local_id(2);
};
