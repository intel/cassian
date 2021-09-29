/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

extern "C" _GENX_MAIN_ void test(SurfaceIndex inp [[type("buffer_t")]],
                                 SurfaceIndex out [[type("buffer_t")]]) {
  constexpr int width = 4;
  vector<int, width> arg;

  read(inp, 0, arg);

  int arg_a = arg[0];
  int arg_b = arg[1];
  int cond = arg[2];
  int res_a, res_b;

  if (cond) {
    asm("add (M1, 1) %0 %2 %3\n"
        "add (M1, 1) %1 %2 %3"
        : "=r"(res_a), "=r"(res_b)
        : "r"(arg_a), "r"(arg_b));
  } else {
    asm("add (M1, 1) %0 %2 (-)%3\n"
        "add (M1, 1) %1 %2 (-)%3"
        : "=r"(res_a), "=r"(res_b)
        : "r"(arg_a), "r"(arg_b));
  }
  vector<int, width> res = 0;
  res[0] = res_a;
  res[1] = res_b;

  write(out, 0, res);
}
