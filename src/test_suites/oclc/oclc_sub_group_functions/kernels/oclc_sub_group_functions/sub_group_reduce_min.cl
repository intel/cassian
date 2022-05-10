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

kernel void test_kernel_sub_group_reduce_min(global uint *reduce_results) {
  size_t tid = get_global_linear_id();
  uint sub_group_local_id = get_sub_group_local_id();
  DATA_TYPE curr_data_set = (DATA_TYPE)(sub_group_local_id + 1);
  DATA_TYPE expected_value = (DATA_TYPE)1;
  DATA_TYPE reduced = FUNC_NAME(curr_data_set);

  reduce_results[tid] = (reduced == expected_value ? 1 : 0);
};