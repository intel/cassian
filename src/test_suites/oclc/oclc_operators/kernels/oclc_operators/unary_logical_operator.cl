/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

kernel void test_kernel(const global INPUT_TYPE *input,
                        global OUTPUT_TYPE *output) {
  const int tid = get_global_id(0);
  output[tid] = !input[tid];
};
