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
  DATA_TYPE_IN v = *input;
  DATA_TYPE_OUT i = REINTERPRET_FUNC(v);
  *output = i;
};