/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

extern "C" _GENX_MAIN_ void test(SurfaceIndex inp [[type("buffer_t")]],
                                 SurfaceIndex out [[type("buffer_t")]]) {
  vector<uchar, 32> v;
  read(inp, 0, v);

  asm("mov (M1, 32) %0 (-)%1" : "=r"(v) : "r"(v));

  write(out, 0, v);
}
