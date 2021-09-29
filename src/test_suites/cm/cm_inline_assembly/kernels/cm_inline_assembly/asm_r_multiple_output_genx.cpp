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
  vector<int, 8> in1, in2, in3, tmp(0), out;

  read(in0_surface, 0, in1);
  read(in1_surface, 0, in2);
  read(in2_surface, 0, in3);

  asm("add (M1, 8) %1 %2 %3\n"
      "add (M1, 8) %0 %4 %5"
      : "=r"(out), "+r"(tmp)
      : "r"(in1), "r"(in2), "r"(in3));

  write(out_surface, 0, out);
}
