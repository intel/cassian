/*
 * Copyright (C) 2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */
#pragma OPENCL EXTENSION cl_intel_subgroups : enable

kernel void test_kernel_sub_group_size(global uint *sub_groups_sizes,
                                       global uint *sub_groups_ids) {

  size_t tid = get_global_linear_id();
  sub_groups_sizes[tid] = get_sub_group_size();
  sub_groups_ids[tid] = get_sub_group_local_id() + 1;
};

kernel void test_kernel_sub_group_size_max_value(global uint *results) {
  size_t tid = get_global_linear_id();
  uint sub_group_size = get_sub_group_size();
  results[tid] =
      ((sub_group_size > 0 && sub_group_size <= get_max_sub_group_size()) ? 1
                                                                          : 0);
};
