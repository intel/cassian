/*
 * Copyright (C) 2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */
#pragma OPENCL EXTENSION cl_intel_subgroups : enable

kernel void
test_kernel_enqueued_num_sub_group(global uint *sub_groups_nums,
                                   global uint *sub_groups_equeued) {

  size_t tid = get_global_linear_id();
  sub_groups_nums[tid] = get_num_sub_groups();
  sub_groups_equeued[tid] = get_enqueued_num_sub_groups();
};
