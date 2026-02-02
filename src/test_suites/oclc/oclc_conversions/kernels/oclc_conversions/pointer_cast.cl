/*
 * Copyright (C) 2024-2026 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#pragma OPENCL EXTENSION cl_khr_fp64 : enable
#pragma OPENCL EXTENSION cl_khr_fp16 : enable

kernel void test_kernel(const global DATA_TYPE_IN *input,
                        global DATA_TYPE_OUT *output) {
  DATA_TYPE_OUT v = *((global DATA_TYPE_OUT *)input);
  *output = v;
};
