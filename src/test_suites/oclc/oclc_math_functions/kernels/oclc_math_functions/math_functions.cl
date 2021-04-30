/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#pragma OPENCL EXTENSION cl_khr_fp64 : enable

kernel void test(global TYPE *output_value, global TYPE *input_value) {
  const size_t global_id = get_global_id(0);
#if defined(TWO_ARG)
  output_value[global_id] = FUNCTION(input_value[0], input_value[1]);
#else
  output_value[global_id] = FUNCTION(input_value[0]);
#endif
}
