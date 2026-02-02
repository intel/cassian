/*
 * Copyright (C) 2021-2026 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#pragma OPENCL EXTENSION cl_khr_fp64 : enable
#pragma OPENCL EXTENSION cl_khr_fp16 : enable

#if defined(TWO_ARG_FUNCTION)
kernel void test(global OUTPUT_TYPE *output_value,
                 global INPUT_TYPE_1 *input_value_1,
                 global INPUT_TYPE_2 *input_value_2) {
  const size_t global_id = get_global_id(0);
  output_value[global_id] =
      FUNCTION(input_value_1[global_id], input_value_2[global_id]);
#elif defined(THREE_ARG_FUNCTION)
kernel void test(global OUTPUT_TYPE *output_value,
                 global INPUT_TYPE_1 *input_value_1,
                 global INPUT_TYPE_2 *input_value_2,
                 global INPUT_TYPE_3 *input_value_3) {
  const size_t global_id = get_global_id(0);
  output_value[global_id] =
      FUNCTION(input_value_1[global_id], input_value_2[global_id],
               input_value_3[global_id]);
#else
kernel void test(global OUTPUT_TYPE *output_value,
                 global INPUT_TYPE_1 *input_value) {
  const size_t global_id = get_global_id(0);
  output_value[global_id] = FUNCTION(input_value[global_id]);
#endif
}