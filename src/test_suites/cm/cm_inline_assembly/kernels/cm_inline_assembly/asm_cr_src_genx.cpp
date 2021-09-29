/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

extern "C" _GENX_MAIN_ void test(SurfaceIndex inp [[type("buffer_t")]],
                                 SurfaceIndex out [[type("buffer_t")]]) {
  vector<int, 8> v;
  read(inp, 0, v);

  auto pred = !(v % 2);

  asm("(%2) not (M1, %3) %0 %1"
      : "=r"(v)
      : "r"(v), "cr"(pred), "n"(v.n_elems()));

  write(out, 0, v);
}
