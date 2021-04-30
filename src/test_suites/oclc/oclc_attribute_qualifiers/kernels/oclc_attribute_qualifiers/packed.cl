/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

struct S {
  char x;
  int y __attribute__((packed));
};

struct __attribute__((packed)) S_packed {
  char x;
  int y;
};

kernel void test_kernel(global ulong *size) {
  const int gid = get_global_id(0);

  size[gid * 2] = sizeof(struct S_packed);
  size[gid * 2 + 1] = sizeof(struct S);
}
