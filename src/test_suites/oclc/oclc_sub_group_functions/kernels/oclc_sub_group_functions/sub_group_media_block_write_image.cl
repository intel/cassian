/*
 * Copyright (C) 2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */
#pragma OPENCL EXTENSION cl_intel_subgroups : enable
#pragma OPENCL EXTENSION cl_intel_media_block_io : enable
#pragma OPENCL EXTENSION cl_intel_required_subgroup_size : enable

__attribute__((intel_reqd_sub_group_size(SUB_GROUP_SIZE))) kernel void
test_kernel_sub_group_media_block_write_image(
    __global INPUT_DATA_TYPE *input_data, write_only image2d_t image) {

  size_t gid = get_global_id(0);
  size_t gid1 = get_global_id(1);
  size_t gid_linear = get_global_linear_id();
  size_t sub_group_size = get_sub_group_size();
  size_t sid = get_sub_group_id();
  size_t lid = get_sub_group_local_id();

  int bwidth = BLOCK_WIDTH;
  int bheight = BLOCK_HEIGHT;
  int blocks_per_row = IMAGE_WIDTH / bwidth;

  int2 offset;
  uint prod1 = gid_linear / sub_group_size;
  int xpixeloffset = bwidth * (prod1 % blocks_per_row);
  size_t type_size = sizeof(INPUT_DATA_TYPE);
  offset.x = xpixeloffset * type_size;

  uint prod2 = gid_linear / (blocks_per_row * sub_group_size);
  uint ybyteoffset = prod2 * bheight;
  offset.y = ybyteoffset;
  size_t indexes[VECTOR_SIZE];

  for (int i = 0; i < VECTOR_SIZE; i++) {
    int x1 = lid;
    int y1 = i;

    int sgoffset = y1 * sub_group_size + x1;
    int y2 = sgoffset / bwidth;
    int x2 = sgoffset % bwidth;

    indexes[i] = (y2 + offset.y) * IMAGE_WIDTH + (xpixeloffset + x2);
  }
  DATA_TYPE write_data = 0;
  for (size_t i = 0; i < VECTOR_SIZE; i++) {
#if VECTOR_SIZE > 1
    write_result[i] = input_data[indexes[i]];
#else
    write_data = input_data[indexes[i]];
#endif
  }
  FUNC_NAME1(offset, bwidth, bheight, write_data, image);
};
