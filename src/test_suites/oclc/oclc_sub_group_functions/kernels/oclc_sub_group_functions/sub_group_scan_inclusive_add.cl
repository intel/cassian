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

kernel void
test_kernel_sub_group_scan_inclusive_add(global uint *scan_results) {
  size_t tid = get_global_linear_id();
  uint sub_group_local_id = get_sub_group_local_id();
  DATA_TYPE curr_data_set = (DATA_TYPE)(sub_group_local_id + 1);
  DATA_TYPE expected_value =
      (DATA_TYPE)(curr_data_set * (curr_data_set + 1) / 2);
  DATA_TYPE scanned = FUNC_NAME(curr_data_set);

  scan_results[tid] = (scanned == expected_value ? 1 : 0);
};