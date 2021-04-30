/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

struct S {
  int x;
  short y[3];
} __attribute__((aligned(32)));

struct S_optimal {
  int x;
  short y[3];
} __attribute__((aligned));

typedef int int_a64 __attribute__((aligned(64)));

kernel void test_kernel() {
  struct S aligned_struct;
  int_a64 aligned_int;

  __attribute__((aligned(128))) struct A { int i; } a;
}