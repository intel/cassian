/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

extern "C" _GENX_MAIN_ void test(SurfaceIndex test_s [[type("buffer_t")]],
                                 SurfaceIndex etalon_s [[type("buffer_t")]]) {
  vector<int, 8> test_v;
  vector<int, 8> etalon_v;

#pragma unroll
  for (int i = 0; i < 8; i++) {
    asm("mov (M1, 1) %0 %1" : "=r"(test_v(i)) : "i"(i));
  }

#pragma unroll
  for (int i = 0; i < 8; i++) {
    etalon_v(i) = i;
  }

  write(test_s, 0, test_v);
  write(etalon_s, 0, etalon_v);
}
