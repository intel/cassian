/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifdef TYPEDEF
typedef int myType;
constant myType var = 1;
#elif defined(STATIC)
constant static int var = 1;
#elif defined(EXTERN)
constant extern int var;
#endif

kernel void test_program_scope(global int *src, global int *dst) {
  int tid = get_global_id(0);
  dst[tid] = src[tid] + var;
}

#ifdef EXTERN
constant int var = 1;
#endif