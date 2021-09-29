/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

extern "C" _GENX_MAIN_ void //
test(SurfaceIndex in0_surface [[type("buffer_t")]],
     SurfaceIndex in1_surface [[type("buffer_t")]],
     SurfaceIndex in2_surface [[type("buffer_t")]],
     SurfaceIndex out_surface [[type("buffer_t")]]) {
  vector<int, 4> in0(0);
  vector<int, 4> in1(0);
  vector<int, 4> in2(0);
  vector<int, 4> out(0);

  read(in0_surface, 0, in0);
  read(in1_surface, 0, in1);
  read(in2_surface, 0, in2);

  asm("mad (M1, 4) %0 %1 %2 %3" : "=r"(out) : "r"(in0), "r"(in1), "r"(in2));

  write(out_surface, 0, out);
}
