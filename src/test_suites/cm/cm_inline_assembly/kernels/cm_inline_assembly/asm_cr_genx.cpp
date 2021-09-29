/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

extern "C" _GENX_MAIN_ void test(SurfaceIndex input_s [[type("buffer_t")]],
                                 SurfaceIndex test_s [[type("buffer_t")]],
                                 SurfaceIndex etalon_s [[type("buffer_t")]]) {
  vector<int, 8> input_v(0);
  vector<int, 8> test_v(0);
  vector<int, 8> etalon_v(0);

  read(input_s, 0, input_v);

  for (int i = 0; i < 8; i++) {
    bool pred = false;
    int first_bit = input_v(i) & 1;

    asm("cmp.ne (M1, 1) %0 %1 %2" : "=cr"(pred) : "r"(first_bit), "i"(0));
    asm("(%2) not (M1, 1) %0 %1"
        : "=a"(test_v(i))
        : "a"(input_v(i)), "cr"(pred));

    if (first_bit)
      etalon_v(i) = ~input_v(i);
  }

  write(test_s, 0, test_v);
  write(etalon_s, 0, etalon_v);
}
