/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

kernel void test_kernel(const global SCALAR_TYPE *input,
                        global VECTOR_TYPE *output, local SCALAR_TYPE *share) {
  const size_t gid = get_global_id(0);
  const size_t lid = get_local_id(0);
  for (int i = 0; i < VECTOR_SIZE; ++i) {
    share[lid * VECTOR_SIZE + i] = input[gid * VECTOR_SIZE + i];
  }
  output[gid] = FUNCTION(lid, share);
};
