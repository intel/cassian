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
test_kernel_sub_group_media_block_read_write_image(read_write image2d_t image) {

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

  DATA_TYPE read_data;
  DATA_TYPE write_data;

  read_data = FUNC_NAME1(offset, bwidth, bheight, image);
  write_data = read_data;
  FUNC_NAME2(offset, bwidth, bheight, write_data, image);
};
