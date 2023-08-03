/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

kernel void test_kernel_global(global COMPONENTS_DATA_TYPE *output) {
  DATA_TYPE i = VALUE;
  *output = i.COMPONENTS;
};

kernel void test_kernel_local(global COMPONENTS_DATA_TYPE *output,
                              local DATA_TYPE *lptr) {
  const size_t lid = get_local_id(0);
  lptr[lid] = VALUE;
  *output = lptr[lid].COMPONENTS;
};
