/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

extern "C" _GENX_MAIN_ void test(SurfaceIndex in0 [[type("buffer_t")]],
                                 SurfaceIndex in1 [[type("buffer_t")]],
                                 SurfaceIndex in2 [[type("buffer_t")]],
                                 SurfaceIndex out [[type("buffer_t")]]) {
  vector<int, 8> v0(0);
  vector<int, 8> v1(0);
  vector<int, 8> v2(0);

  read(in0, 0, v0);
  read(in1, 0, v1);
  read(in2, 0, v2);

  asm("add (M1, %2) %0(0,0)<1> %0(0,0)<1;1,0> %1(0,0)<1;1,0>"
      : "+rw"(v0)
      : "rw"(v1), "n"(sizeof(v0) / sizeof(int)));

  asm("add (M1, %3) %0 %1 %2"
      : "=r"(v0)
      : "r"(v0), "r"(v2), "n"(sizeof(v0) / sizeof(int)));

  write(out, 0, v0);
}
