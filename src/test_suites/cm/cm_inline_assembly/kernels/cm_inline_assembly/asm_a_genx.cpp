/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

extern "C" _GENX_MAIN_ void //
test(SurfaceIndex in_surface [[type("buffer_t")]],
     SurfaceIndex test_surface [[type("buffer_t")]],
     SurfaceIndex etalon_surface [[type("buffer_t")]]) {
  vector<int, 8> in0_vector;
  vector<int, 8> in1_vector;
  vector<int, 8> test_vector;
  vector<int, 8> etalon_vector;

  read(in_surface, 0, in0_vector);

#pragma nounroll
  for (int i = 0; i < 8; i++) {
    asm("add (M1, 1) %0 %1 %2" : "=a"(in1_vector(i)) : "r"(i), "i"(10));
    asm("add (M1, 1) %0 %1 %2"
        : "=a"(test_vector(i))
        : "a"(in0_vector(i)), "a"(in1_vector(i)));

    etalon_vector(i) = in0_vector(i) + i + 10;
  }

  write(test_surface, 0, test_vector);
  write(etalon_surface, 0, etalon_vector);
}
