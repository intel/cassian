/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

kernel void test_kernel(const global DATA_TYPE *input_a,
                        const global DATA_TYPE *input_b,
                        const global SELECT_TYPE *input_c,
                        global DATA_TYPE *output) {
  const int tid = get_global_id(0);
  output[tid] = select(input_a[tid], input_b[tid], input_c[tid]);
};
