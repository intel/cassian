/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

// TODO: Move it to the host
#pragma OPENCL EXTENSION cl_khr_int64_base_atomics : enable
#pragma OPENCL EXTENSION cl_khr_int64_extended_atomics : enable
#pragma OPENCL EXTENSION cl_khr_fp64 : enable

global atomic_flag global_flag = ATOMIC_FLAG_INIT;

kernel void test_kernel(global DATA_TYPE *output) {
  const size_t global_id = get_global_id(0);
  output = atomic_load(&global_flag);
}
