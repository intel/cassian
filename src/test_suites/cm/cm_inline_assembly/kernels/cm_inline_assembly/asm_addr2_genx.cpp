/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

extern "C" _GENX_MAIN_ void test(SurfaceIndex out [[type("buffer_t")]]) {
  vector<int, 8> src = 0;
  vector<int, 8> dst = 0;

#pragma nounroll
  for (int i = 0; i < 8; i++) {
    asm("add (M1, 1) %0 %1 %2" : "=a"(src(i)) : "r"(i), "i"(10));
    // src(i) = i + 10;
  }

#pragma nounroll
  for (int i = 0; i < 8; i += 2) {
    asm("mov (M1, 1) %0 %2\n"
        "mov (M1, 1) %1 %3"
        : "=a"(dst(i)), "=a"(dst(i + 1))
        : "a"(src(i + 1)), "a"(src(i)));
  }

  write(out, 0, dst);
}
