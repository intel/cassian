/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#define AS_STRING(s) AS_STRING_(s)
#define AS_STRING_(s) #s

kernel void test_printf_arguments(global const TYPE *input) {
  const int gid = get_global_id(0);
  TYPE i = input[gid];
  printf(AS_STRING(STRING), i);
}
