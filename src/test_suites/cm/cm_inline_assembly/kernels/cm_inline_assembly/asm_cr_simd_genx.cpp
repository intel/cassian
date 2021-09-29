/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "cm/cm.h"

void _GENX_MAIN_ test(SurfaceIndex in0 [[type("buffer_t")]],
                      SurfaceIndex in1 [[type("buffer_t")]],
                      SurfaceIndex out [[type("buffer_t")]]) {
  vector<ushort, 8> cond;
  vector<uint, 8> a, b;

  read(in0, 0, a);
  read(in1, 0, b);

  vector<uint, 8> c = 0;

  asm("cmp.eq (M1, 8) %0 %1 %2" : "=cr"(cond) : "r"(a), "r"(b));

  SIMD_IF_BEGIN(cond == 0) { c = a + b; }
  SIMD_IF_END

  write(out, 0, c);
}
