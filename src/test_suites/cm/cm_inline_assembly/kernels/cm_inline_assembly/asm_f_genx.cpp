/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

extern "C" _GENX_MAIN_ void //
test(SurfaceIndex test_surf [[type("buffer_t")]],
     SurfaceIndex etalon_surf [[type("buffer_t")]]) {
  using data_t = DATA_TYPE;
  vector<data_t, 8> test_vec;
  vector<data_t, 8> etalon_vec;

  data_t one = 1;
  data_t value;

#pragma unroll
  for (int i = 0; i < 8; i++) {
    value = one / (i + 1);
    asm("mov (M1, 1) %0 %1" : "=r"(test_vec(i)) : "F"(value));
    etalon_vec(i) = value;
  }

  write(test_surf, 0, test_vec);
  write(etalon_surf, 0, etalon_vec);
}
