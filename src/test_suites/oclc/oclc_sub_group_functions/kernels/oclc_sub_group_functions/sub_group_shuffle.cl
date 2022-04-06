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

kernel void test_kernel_sub_group_shuffle(__global uint *shuffle_results) {
  size_t tid = get_global_linear_id();
  const uint current_sub_group_size = get_sub_group_size();
  DATA_TYPE results[32] = {(DATA_TYPE)0xDEADBEEF};
  DATA_TYPE val_to_shuffle = (DATA_TYPE)get_sub_group_local_id();

  for (uint i = 0; i < current_sub_group_size; i++) {
    results[i] = intel_sub_group_shuffle(val_to_shuffle, i);
  }

  uint passed = 1;
  for (uint i = 0; i < current_sub_group_size; i++) {
    if (USE_ALL(results[i] != (DATA_TYPE)i)) {
      passed = 0;
      break;
    }
  }
  shuffle_results[tid] = passed;
};
