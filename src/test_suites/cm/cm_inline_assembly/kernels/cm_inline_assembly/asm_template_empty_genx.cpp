/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

extern "C" _GENX_MAIN_ void //
test(SurfaceIndex in_surface [[type("buffer_t")]],
     SurfaceIndex out_surface [[type("buffer_t")]]) {
  vector<int, 8> in_vector;
  vector<int, 8> out_vector;

  read(in_surface, 0, in_vector);

  for (int i = 0; i < 8; i++) {
    asm("");
    out_vector(i) = in_vector(i) - 1;
  }

  write(out_surface, 0, out_vector);
}
