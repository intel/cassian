/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

kernel void test_kernel(const global int *input, global int *output) {
  const int tid = get_global_id(0);
  output[tid] = work_group_all(input[tid]);
};
