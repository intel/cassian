/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

kernel void test_kernel(global int *inout) {
  int global_id = get_global_id(0);
  inout[global_id] *= inout[global_id];
};
