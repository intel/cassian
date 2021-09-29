/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

extern "C" _GENX_MAIN_ void test(SurfaceIndex a_idx [[type("buffer_t")]],
                                 SurfaceIndex b_idx [[type("buffer_t")]],
                                 SurfaceIndex res_a_idx [[type("buffer_t")]],
                                 SurfaceIndex res_b_idx [[type("buffer_t")]]) {
  constexpr int width = 16;

  vector<int, width> a;
  vector<int, width> b;

  read(a_idx, 0, a);
  read(b_idx, 0, b);

  int cond = a[0];
  vector<int, width> res_a;
  vector<int, width> res_b;

  if (cond) {
    asm("add (M1, %4) %0 %2 %3\n"
        "add (M1, %4) %1 %2 %3"
        : "=r"(res_a), "=r"(res_b)
        : "r"(a), "r"(b), "n"(width));
  } else {
    asm("add (M1, %4) %0 %2 (-)%3\n"
        "add (M1, %4) %1 %2 (-)%3"
        : "=r"(res_a), "=r"(res_b)
        : "r"(a), "r"(b), "n"(width));
  }

  write(res_a_idx, 0, res_a);
  write(res_b_idx, 0, res_b);
}
