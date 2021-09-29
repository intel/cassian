/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

extern "C" _GENX_MAIN_ void //
test(SurfaceIndex input_surface [[type("buffer_t")]],
     SurfaceIndex test_surface [[type("buffer_t")]],
     SurfaceIndex etalon_surface [[type("buffer_t")]]) {
  vector<int, 8> input_vector(0);
  vector<int, 8> test_vector(0);
  vector<int, 8> etalon_vector(0);

  read(input_surface, 0, input_vector);

  asm("mov (M1, 4) %0(0,0)<2> %1(0,0)<0;4,2>"
      : "+rw"(test_vector)
      : "rw"(input_vector));
  for (int i = 0; i < 8; i += 2)
    etalon_vector(i) = input_vector(i);

  write(test_surface, 0, test_vector);
  write(etalon_surface, 0, etalon_vector);
}
