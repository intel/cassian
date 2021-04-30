/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

kernel void test_kernel(const global VECTOR_TYPE *input,
                        global SCALAR_TYPE *output) {
  const size_t tid = get_global_id(0);
  const size_t padding = 2;
  const size_t padding_size = padding * VECTOR_SIZE;
  const size_t size = padding_size + VECTOR_SIZE;
  SCALAR_TYPE v[size];
  FUNCTION(input[tid], padding, v);
  for (int i = 0; i < VECTOR_SIZE; ++i) {
    output[tid * VECTOR_SIZE + i] = v[padding_size + i];
  }
};
