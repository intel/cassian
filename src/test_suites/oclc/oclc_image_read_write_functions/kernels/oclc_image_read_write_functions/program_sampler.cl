/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifdef T_3D
#pragma OPENCL EXTENSION cl_khr_3d_image_writes : enable
#endif

const sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE |
                          CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST;

kernel void copy_image(read_only IMAGE_TYPE input,
                       write_only IMAGE_TYPE output) {
  COORD_TYPE img_coord;
#ifdef T_1D
  img_coord = (int)get_global_id(0);
#elif defined(T_2D)
  img_coord = (int2){get_global_id(0), get_global_id(1)};
#else
  img_coord = (int4){get_global_id(0), get_global_id(1), get_global_id(2), 0};
#endif
  const COLOR_TYPE color = READ_FUNC(input, sampler, img_coord);
  WRITE_FUNC(output, img_coord, color);
}