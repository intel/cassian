/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

kernel void copy_buffer(global int *dst, global int *src) {
  size_t id = get_global_linear_id();
  dst[id] = src[id];
}
