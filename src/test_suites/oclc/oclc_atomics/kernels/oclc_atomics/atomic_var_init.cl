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

global ATOMIC_TYPE global_atomic = ATOMIC_VAR_INIT(INPUT);

kernel void test_kernel(global DATA_TYPE *output) {
  *output = atomic_load(&global_atomic);
}
