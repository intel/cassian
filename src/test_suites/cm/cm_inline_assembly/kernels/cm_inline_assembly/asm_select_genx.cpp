/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

_GENX_MAIN_ void test(SurfaceIndex inp [[type("buffer_t")]],
                      SurfaceIndex out [[type("buffer_t")]]) {
  matrix<float, 8, 4> m;
  vector<float, 4> dst;

  read(inp, 0, m.template format<float>());

  asm("mov (M1, 4) %0 %1" : "=r"(dst) : "r"(m.select<2, 2, 2, 2>(0, 0)));

  write(out, 0, dst);
}
