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
test_kernel_sub_group_block_read_image(read_only image2d_t image,
                                       __global INPUT_DATA_TYPE *output_data) {

  size_t x = get_global_id(0);

  size_t bytes_per_subgroup = get_sub_group_size() * sizeof(INPUT_DATA_TYPE);
  size_t offset_for_subgroup =
      (get_global_id(0) * sizeof(INPUT_DATA_TYPE) / bytes_per_subgroup) *
      bytes_per_subgroup;
  int size_of_scalar = sizeof(INPUT_DATA_TYPE);
  size_t y = get_global_id(1);
  size_t yc = get_global_id(1) * VECTOR_SIZE;
  size_t tidg = get_global_linear_id();
  int2 pos = (int2)(offset_for_subgroup, yc);
  DATA_TYPE read_result = FUNC_NAME1(image, pos);
  int elements_per_pixel = 1;
  int ulong_size = sizeof(ulong);
  if (size_of_scalar == ulong_size)
    elements_per_pixel = 2;

  size_t img_pixel_width = get_image_width(image) / elements_per_pixel;
  size_t img_byte_offset = x + img_pixel_width * yc;
  for (size_t i = 0; i < VECTOR_SIZE; i++) {
#if VECTOR_SIZE > 1
    output_data[i * img_pixel_width + img_byte_offset] = read_result[i];
#else
    output_data[tidg] = read_result;
#endif
  }
};
