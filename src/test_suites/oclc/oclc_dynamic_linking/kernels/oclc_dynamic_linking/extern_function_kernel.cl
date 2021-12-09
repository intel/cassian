/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

extern int add(int, int);

kernel void vector_add(global int *a, global int *b, global int *c) {
  size_t id = get_global_linear_id();
  c[id] = add(a[id], b[id]);
}
