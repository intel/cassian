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

kernel void test_kernel_sub_group_shuffle_mixed(global uint *shuffle_results) {
  size_t tid = get_global_linear_id();
  uint sub_group_size = get_sub_group_size();
  uint sub_group_local_id = get_sub_group_local_id();
  DATA_TYPE cur = (DATA_TYPE)(sub_group_local_id + 1);
  DATA_TYPE next = cur + (DATA_TYPE)(100);
  DATA_TYPE new_value_up_down = (DATA_TYPE)0xDEADBEEF;
  DATA_TYPE new_value_down_up = (DATA_TYPE)0xDEADBEEF;
  DATA_TYPE expected_value = (DATA_TYPE)0xDEADBEEF;
  bool passed = false;
  uint delta_size_mixed = sub_group_size / 2;
  new_value_down_up = intel_sub_group_shuffle_down(cur, next, delta_size_mixed);
  new_value_up_down = intel_sub_group_shuffle_up(cur, next, delta_size_mixed);
  next += (DATA_TYPE)delta_size_mixed;
  new_value_up_down =
      intel_sub_group_shuffle_down(new_value_up_down, next, delta_size_mixed);
  new_value_down_up =
      intel_sub_group_shuffle_up(new_value_down_up, next, delta_size_mixed);

  if (USE_ALL(new_value_up_down == new_value_down_up))
    passed = true;

  shuffle_results[tid] = (passed == true ? 1 : 0);
};