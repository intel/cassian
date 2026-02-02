/*
 * Copyright (C) 2023-2026 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#pragma OPENCL EXTENSION cl_khr_fp64 : enable
#pragma OPENCL EXTENSION cl_khr_fp16 : enable

kernel void test_kernel(const global DATA_TYPE_IN *input,
                        global DATA_TYPE_OUT *output) {
  const size_t global_id = get_global_id(0);
  union {
    DATA_TYPE_OUT out;
    DATA_TYPE_IN in;
  } u;
  u.in = *input;
  *output = u.out;
};