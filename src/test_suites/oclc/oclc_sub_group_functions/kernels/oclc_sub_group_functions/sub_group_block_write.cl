/*
 * Copyright (C) 2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */
#pragma OPENCL EXTENSION cl_intel_subgroups : enable
#pragma OPENCL EXTENSION cl_intel_subgroups_char : enable
#pragma OPENCL EXTENSION cl_intel_subgroups_short : enable
#pragma OPENCL EXTENSION cl_intel_subgroups_long : enable
#pragma OPENCL EXTENSION cl_khr_fp16 : enable
#pragma OPENCL EXTENSION cl_khr_fp64 : enable

kernel void
test_kernel_sub_group_block_write(__global INPUT_DATA_TYPE *input_data,
                                  __global INPUT_DATA_TYPE *output_data) {

  size_t x = get_global_id(0);
  size_t wg_size = get_local_size(0) * get_local_size(1) * get_local_size(2);
  size_t work_group_byte_offset =
      (get_group_id(0) * get_num_groups(1) * wg_size +
       get_group_id(1) * wg_size +
       get_group_id(2) * get_num_groups(1) * get_num_groups(0) * wg_size) *
      VECTOR_SIZE;
  size_t sub_group_byte_offset =
      work_group_byte_offset +
      get_sub_group_id() * get_max_sub_group_size() * VECTOR_SIZE;
  size_t tid = x * sizeof(INPUT_DATA_TYPE);
  DATA_TYPE write_data;
  for (size_t i = 0; i < VECTOR_SIZE; i++) {
    size_t input_offset =
        sub_group_byte_offset +
        (get_sub_group_local_id() + get_max_sub_group_size() * i);
#if VECTOR_SIZE > 1
    write_data[i] = input_data[input_offset];
#else
    write_data = input_data[input_offset];
#endif
  }
  FUNC_NAME1((__global INPUT_DATA_TYPE *)(output_data + sub_group_byte_offset),
             write_data);
};
