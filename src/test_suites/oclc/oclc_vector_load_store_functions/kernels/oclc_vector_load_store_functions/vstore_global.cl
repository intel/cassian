/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

kernel void test_kernel(const global VECTOR_TYPE *input,
                        global SCALAR_TYPE *output) {
  const size_t tid = get_global_id(0);
  FUNCTION(input[tid], tid, output);
};
