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

kernel void test_kernel_sub_group_shuffle_up(global uint *shuffle_results) {
  size_t tid = get_global_linear_id();
  uint sub_group_size = get_max_sub_group_size();
  uint sub_group_local_id = get_sub_group_local_id();
  DATA_TYPE prev = (DATA_TYPE)(sub_group_local_id + 1);
  DATA_TYPE cur = prev + (DATA_TYPE)(100);
  DATA_TYPE new_value = (DATA_TYPE)0xDEADBEEF;
  DATA_TYPE expected_value = (DATA_TYPE)0xDEADBEEF;
  bool passed = false;
  new_value = intel_sub_group_shuffle_up(prev, cur, DELTA_SIZE);

  int SHUFFLE_INDEX = (sub_group_local_id - DELTA_SIZE);
  if (SHUFFLE_INDEX >= 0 && SHUFFLE_INDEX < sub_group_size) {
    expected_value = (DATA_TYPE)(SHUFFLE_INDEX + 1) + (DATA_TYPE)(100);
  } else if (SHUFFLE_INDEX < 0 && SHUFFLE_INDEX >= -sub_group_size) {
    expected_value = (DATA_TYPE)(SHUFFLE_INDEX + sub_group_size + 1);

  } else {
    // out of range results. skip
    shuffle_results[tid] = 1;
    return;
  }

  if (USE_ALL(new_value == expected_value))
    passed = true;

  shuffle_results[tid] = (passed == true ? 1 : 0);
};