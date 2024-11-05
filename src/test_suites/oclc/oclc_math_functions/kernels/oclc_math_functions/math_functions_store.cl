/*
 * Copyright (C) 2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#pragma OPENCL EXTENSION cl_khr_fp64 : enable

#if defined(TWO_ARG_FUNCTION)
kernel void test(global OUTPUT_TYPE *output_value,
                 global INPUT_TYPE_1 *input_value_1,
                 ADDRESS_SPACE INPUT_TYPE_2 *input_value_2) {
  const size_t global_id = get_global_id(0);
#if defined(USE_PRIVATE)
private
  INPUT_TYPE_1 input1 = input_value_1[global_id];
private
  INPUT_TYPE_2 input2 = input_value_2[global_id];
  output_value[global_id] = FUNCTION(input1, &input2);
#else
  output_value[global_id] =
      FUNCTION(input_value_1[global_id], &input_value_2[global_id]);
#endif
}

#elif defined(THREE_ARG_FUNCTION)
kernel void test(global OUTPUT_TYPE *output_value,
                 global INPUT_TYPE_1 *input_value_1,
                 global INPUT_TYPE_2 *input_value_2,
                 ADDRESS_SPACE INPUT_TYPE_3 *input_value_3) {
  const size_t global_id = get_global_id(0);

#if defined(USE_PRIVATE)
private
  INPUT_TYPE_1 input1 = input_value_1[global_id];
private
  INPUT_TYPE_2 input2 = input_value_2[global_id];
private
  INPUT_TYPE_3 input3 = input_value_3[global_id];
  output_value[global_id] = FUNCTION(input1, input2, &input3);
#else
  output_value[global_id] =
      FUNCTION(input_value_1[global_id], input_value_2[global_id],
               &input_value_3[global_id]);
#endif
}
#else
kernel void test(global OUTPUT_TYPE *output_value,
                 ADDRESS_SPACE INPUT_TYPE_1 *input_value) {
  const size_t global_id = get_global_id(0);
#if defined(USE_PRIVATE)
private
  INPUT_TYPE_1 input = input_value[global_id];
  output_value[global_id] = FUNCTION(&input);
#else
  output_value[global_id] = FUNCTION(&input_value[global_id]);
#endif
}
#endif
