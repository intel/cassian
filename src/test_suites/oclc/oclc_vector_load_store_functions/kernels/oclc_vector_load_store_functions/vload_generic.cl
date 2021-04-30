/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

void generic_function(const SCALAR_TYPE *input, VECTOR_TYPE *output) {
  const size_t tid = get_global_id(0);
  output[tid] = FUNCTION(tid, input);
}

kernel void test_kernel(const global SCALAR_TYPE *input,
                        global VECTOR_TYPE *output) {
  generic_function(input, output);
};
