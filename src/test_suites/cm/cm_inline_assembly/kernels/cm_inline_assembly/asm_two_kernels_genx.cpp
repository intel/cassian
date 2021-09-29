/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

extern "C" _GENX_MAIN_ void test_asm(SurfaceIndex in [[type("buffer_t")]],
                                     SurfaceIndex out [[type("buffer_t")]]) {
  vector<int, 8> v;
  read(in, 0, v);

  asm("add (M1, 8) %0 %1 %2" : "=r"(v) : "r"(v), "i"(5));

  write(out, 0, v);
}

extern "C" _GENX_MAIN_ void test(SurfaceIndex in [[type("buffer_t")]],
                                 SurfaceIndex out [[type("buffer_t")]]) {
  vector<int, 8> v;
  read(in, 0, v);

  v = v + 5;

  write(out, 0, v);
}
