/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#if defined(TWO_ARGS)
kernel void test(global OUTPUT_TYPE *output_value,
                 global INPUT_TYPE_1 *input_value_1,
                 global INPUT_TYPE_2 *input_value_2) {
  const size_t global_id = get_global_id(0);
  output_value[global_id] = FUNCTION(input_value_1[0], input_value_2[0]);
#elif defined(THREE_ARGS)
kernel void test(global OUTPUT_TYPE *output_value,
                 global INPUT_TYPE_1 *input_value_1,
                 global INPUT_TYPE_2 *input_value_2,
                 global INPUT_TYPE_3 *input_value_3) {
  const size_t global_id = get_global_id(0);
  output_value[global_id] =
      FUNCTION(input_value_1[0], input_value_2[0], input_value_3[0]);
#else
kernel void test(global OUTPUT_TYPE *output_value,
                 global INPUT_TYPE_1 *input_value) {
  const size_t global_id = get_global_id(0);
  output_value[global_id] = FUNCTION(input_value[0]);
#endif
}
