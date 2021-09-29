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
     SurfaceIndex out_surface [[type("buffer_t")]]) {
  vector<int, 8> in0_vector(0);
  vector<int, 8> in1_vector(0);
  vector<int, 8> out_vector(0);

  read(in0_surface, 0, in0_vector);
  read(in1_surface, 0, in1_vector);

  asm("add (M1, 8) %0 %1 %2"
      : "=r"(out_vector)
      : "r"(in0_vector), "r"(in1_vector));

  write(out_surface, 0, out_vector);
}
