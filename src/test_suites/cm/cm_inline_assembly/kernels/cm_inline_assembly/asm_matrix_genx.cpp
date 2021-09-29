/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

extern "C" _GENX_MAIN_ void test(SurfaceIndex inp [[type("buffer_t")]],
                                 SurfaceIndex out [[type("buffer_t")]]) {
  matrix<ushort, 8, 8> m;
  read(inp, 0, m.template format<ushort>());

  asm("mov (M1, %2) %0 (-)%1"
      : "=r"(m)
      : "r"(m), "n"(sizeof(m) / (2 * sizeof(ushort))));

  write(out, 0, m.template format<ushort>());
}
