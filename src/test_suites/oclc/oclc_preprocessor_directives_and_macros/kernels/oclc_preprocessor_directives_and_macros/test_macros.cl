/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifdef FILE_MACRO
#define xstr(s) str(s)
#define str(s) #s
#line 3 xstr(FILE_NAME)
#endif

kernel void macro_test(global TYPE *output) {
  int id = get_global_id(0);
#ifdef FILE_MACRO
#ifdef __FILE__
  int i = 0;
  id = id * (sizeof(__FILE__));
  constant char *f = __FILE__;
  for (; i < sizeof(__FILE__); i++) {
    output[id + i] = f[i];
  }
#else
  output[id] = NULL;
#endif
#endif
#ifdef OCLC_VERSION_MACRO
#ifdef __OPENCL_C_VERSION__
  output[id] = __OPENCL_C_VERSION__;
#else
  output[id] = NULL;
#endif
#endif
}
