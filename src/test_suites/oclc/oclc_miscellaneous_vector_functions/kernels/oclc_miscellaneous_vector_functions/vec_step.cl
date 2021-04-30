/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

kernel void test_data_type(const global DATA_TYPE *input, global int *output) {
  const size_t id = get_global_id(0);
  output[id] = vec_step(input[id]);
};

kernel void test_pure_type(global int *output) {
  const size_t id = get_global_id(0);
  output[id] = vec_step(DATA_TYPE);
};
