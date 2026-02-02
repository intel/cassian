/*
 * Copyright (C) 2021-2026 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

// TODO: Move it to the host
#pragma OPENCL EXTENSION cl_khr_int64_base_atomics : enable
#pragma OPENCL EXTENSION cl_khr_int64_extended_atomics : enable
#pragma OPENCL EXTENSION cl_khr_fp64 : enable
#pragma OPENCL EXTENSION cl_khr_fp16 : enable

global ATOMIC_TYPE global_atomic = ATOMIC_VAR_INIT(INPUT);

kernel void test_kernel(global DATA_TYPE *output) {
  *output = atomic_load(&global_atomic);
}
