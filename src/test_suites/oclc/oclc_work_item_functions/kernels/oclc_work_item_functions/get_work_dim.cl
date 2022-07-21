/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

uint get_work_dim_wrapper() { return get_work_dim(); };

kernel void test_kernel(global uint *output) {
  const size_t tid =
      get_global_id(2) * get_global_size(1) * get_global_size(0) +
      get_global_id(1) * get_global_size(0) + get_global_id(0);
  output[tid] = get_work_dim();
};

kernel void test_kernel_wrappers(global uint *output) {
  const size_t tid =
      get_global_id(2) * get_global_size(1) * get_global_size(0) +
      get_global_id(1) * get_global_size(0) + get_global_id(0);
  output[tid] = get_work_dim_wrapper();
};
