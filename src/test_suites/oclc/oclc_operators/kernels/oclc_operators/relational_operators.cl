/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

kernel void test_kernel(const global A_TYPE *input_a,
                        const global B_TYPE *input_b,
                        global OUTPUT_TYPE *output) {
  const int tid = get_global_id(0);
  output[tid] = input_a[tid] OPERATOR input_b[tid];
};
