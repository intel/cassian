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
test_kernel_sub_group_block_read_write_image(read_write image2d_t image) {

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
  int elements_per_pixel = ELEMENTS_PER_PIXEL;
  size_t img_pixel_width = get_image_width(image) / elements_per_pixel;
  size_t img_byte_offset = x + img_pixel_width * yc;

  DATA_TYPE read_result = FUNC_NAME1(image, pos);
  DATA_TYPE write_result = read_result;
  FUNC_NAME2(image, pos, write_result);
}
