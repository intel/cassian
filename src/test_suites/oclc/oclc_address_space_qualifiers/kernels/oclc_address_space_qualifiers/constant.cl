/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

kernel void test_kernel(constant int *input, global int *output) {
  int global_id = get_global_id(0);
  int global_size = get_global_size(0);

  output[global_id] = 0;
  for (int i = 0; i < global_size; i++) {
    output[global_id] += input[i];
  }
};
