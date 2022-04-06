/*
 * Copyright (C) 2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */
#pragma OPENCL EXTENSION cl_intel_subgroups : enable
#pragma OPENCL EXTENSION cl_intel_subgroups_char : enable
#pragma OPENCL EXTENSION cl_intel_subgroups_short : enable
#pragma OPENCL EXTENSION cl_khr_fp16 : enable
#pragma OPENCL EXTENSION cl_khr_fp64 : enable

kernel void test_kernel_sub_group_shuffle_xor(global uint *shuffle_results) {
  size_t tid = get_global_linear_id();
  uint sub_group_size = get_sub_group_size();

  uint sub_group_local_id = get_sub_group_local_id();
  DATA_TYPE cur = (DATA_TYPE)(sub_group_local_id);
  uint mask = sub_group_size - 1;

  DATA_TYPE new_value = (DATA_TYPE)0xDEADBEEF;
  DATA_TYPE expected_value = (DATA_TYPE)(sub_group_local_id ^ mask);
  bool passed = false;
  new_value = intel_sub_group_shuffle_xor(cur, mask);
  if (USE_ALL(new_value == expected_value))
    passed = true;

  shuffle_results[tid] = (passed == true ? 1 : 0);
};