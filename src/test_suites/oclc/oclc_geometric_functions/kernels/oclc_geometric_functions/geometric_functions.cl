/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#if defined(TWO_ARGS)
kernel void test(global OUTPUT_TYPE *output_value,
                 global INPUT_TYPE *input_value_1,
                 global INPUT_TYPE *input_value_2) {
  const size_t global_id = get_global_id(0);
  output_value[global_id] =
      FUNCTION(input_value_1[global_id], input_value_2[global_id]);
#else
kernel void test(global OUTPUT_TYPE *output_value,
                 global INPUT_TYPE *input_value) {
  const size_t global_id = get_global_id(0);
  output_value[global_id] = FUNCTION(input_value[global_id]);
#endif
}
