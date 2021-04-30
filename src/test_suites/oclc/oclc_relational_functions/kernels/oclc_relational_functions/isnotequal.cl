/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

kernel void test_kernel(const global INPUT_TYPE *input_x,
                        const global INPUT_TYPE *input_y,
                        global OUTPUT_TYPE *output) {
  const int tid = get_global_id(0);
  output[tid] = isnotequal(input_x[tid], input_y[tid]);
};
