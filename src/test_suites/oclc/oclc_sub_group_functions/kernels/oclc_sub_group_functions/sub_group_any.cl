/*
 * Copyright (C) 2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */
#pragma OPENCL EXTENSION cl_intel_subgroups : enable

kernel void test_kernel_sub_group_any(__global uint *results) {
  size_t tid = get_global_linear_id();

  uint curr_sub_group_local_id = get_sub_group_local_id();
  const uint current_sub_group_size = get_sub_group_size();

  results[tid] =
      sub_group_any(curr_sub_group_local_id + 1 == current_sub_group_size) ? 1
                                                                           : 0;
};
