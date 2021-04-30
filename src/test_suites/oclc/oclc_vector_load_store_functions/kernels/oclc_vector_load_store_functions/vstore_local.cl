/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

kernel void test_kernel(const global VECTOR_TYPE *input,
                        global SCALAR_TYPE *output, local SCALAR_TYPE *share) {
  const size_t gid = get_global_id(0);
  const size_t lid = get_local_id(0);
  FUNCTION(input[gid], lid, share);
  for (int i = 0; i < VECTOR_SIZE; ++i) {
    output[gid * VECTOR_SIZE + i] = share[lid * VECTOR_SIZE + i];
  }
};
