/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

_GENX_ matrix<float, 8, 4> m1;
_GENX_ vector<float, 4> v2;
_GENX_ vector<float, 4> v3;

_GENX_MAIN_ void test(SurfaceIndex out1 [[type("buffer_t")]],
                      SurfaceIndex out2 [[type("buffer_t")]],
                      SurfaceIndex s1 [[type("buffer_t")]],
                      SurfaceIndex s2 [[type("buffer_t")]],
                      SurfaceIndex s3 [[type("buffer_t")]]) {
  vector<float, 4> out;
  read(s1, 0, m1.template format<float>());
  read(s2, 0, v2);
  read(s3, 0, v3);

  asm("mov (M1, 4) %0 %3\n"
      "mov (M1, 4) %2 %3\n"
      "mov (M1, 4) %1 %4"
      : "+r"(v3), "=r"(out), "=r"(v2)
      : "r"(m1.template select<2, 2, 2, 2>(0, 0)));

  write(out1, 0, out.template format<float>());
  write(out2, 0, v3);
}
