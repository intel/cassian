/*
 * Copyright (C) 2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */
#pragma OPENCL EXTENSION cl_intel_subgroups : enable

kernel void test_kernel_sub_group_id(global uint *sub_group_local_ids,
                                     global uint *sub_groups_ids,
                                     global uint *sub_groups_sizes) {

  size_t tid = get_global_linear_id();
  sub_group_local_ids[tid] = get_sub_group_local_id();
  sub_groups_ids[tid] = get_sub_group_id();
  sub_groups_sizes[tid] = get_sub_group_size();
}