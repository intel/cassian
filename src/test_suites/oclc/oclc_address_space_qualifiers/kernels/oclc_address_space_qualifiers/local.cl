/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

kernel void test_kernel(const global int *input, local int *temp,
                        const int elements_per_work_item, global int *output) {
  const size_t global_id = get_global_id(0);
  const size_t local_id = get_local_id(0);

  for (int i = 0; i < elements_per_work_item; ++i) {
    temp[local_id * elements_per_work_item + i] =
        input[global_id * elements_per_work_item + i];
  }
  barrier(CLK_LOCAL_MEM_FENCE);
  for (int i = 0; i < elements_per_work_item; ++i) {
    output[global_id * elements_per_work_item + i] =
        temp[local_id * elements_per_work_item + i];
  }
};
