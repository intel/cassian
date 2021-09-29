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
  vector<int, 8> input_vector;
  vector<int, 8> test_vector(0);
  vector<int, 8> etalon_vector(0);

  const int exec_size = 4;

  read(input_surface, 0, input_vector);

  asm("mov (M1, %2) %0 %1"
      : "+r"(test_vector)
      : "r"(input_vector), "n"(exec_size));

  write(test_surface, 0, test_vector);

  for (int i = 0; i < exec_size; i++)
    etalon_vector(i) = input_vector(i);

  write(etalon_surface, 0, etalon_vector);
}
