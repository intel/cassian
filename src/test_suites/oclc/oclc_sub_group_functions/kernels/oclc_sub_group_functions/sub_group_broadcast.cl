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

kernel void test_kernel_sub_group_broadcast(__global uint *broadcast_results) {
  size_t tid = get_global_id(2) * get_global_size(1) * get_global_size(0) +
               get_global_id(1) * get_global_size(0) + get_global_id(0);

  const uint current_sub_group_size = get_sub_group_size();
  DATA_TYPE results[32] = {(DATA_TYPE)0xDEADBEEF};
  DATA_TYPE val_to_broadcast = (DATA_TYPE)get_sub_group_local_id();

  for (uint i = 0; i < current_sub_group_size; i++) {
    results[i] = FUNC_NAME(val_to_broadcast, i);
  }

  uint passed = 1;
  for (uint i = 0; i < current_sub_group_size; i++) {
    if (results[i] != (DATA_TYPE)i) {
      passed = 0;
      break;
    }
  }

  broadcast_results[tid] = passed;
};
