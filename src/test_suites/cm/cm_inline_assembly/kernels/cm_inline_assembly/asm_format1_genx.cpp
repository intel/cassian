/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

extern "C" _GENX_MAIN_ void
test(SurfaceIndex s1 [[type("buffer_t")]], SurfaceIndex s2 [[type("buffer_t")]],
     SurfaceIndex s3 [[type("buffer_t")]], SurfaceIndex s4 [[type("buffer_t")]],
     SurfaceIndex s5 [[type("buffer_t")]], SurfaceIndex s6 [[type("buffer_t")]],
     SurfaceIndex s7 [[type("buffer_t")]], SurfaceIndex s8 [[type("buffer_t")]],
     SurfaceIndex s9 [[type("buffer_t")]]) {
  vector<float, 16> A = 0;
  vector<int, 8> B, C;
  vector<float, 8> D;

  auto tid = cm_linear_global_id();

  read(s1, 0, B);
  read(s2, 0, C);
  read(s3, 0, D);

  asm("(%3) xor (M1, 8) %0 %1 %2"
      : "+r"(A.template select<8, 2>().template format<int>())
      : "r"(B), "r"(C), "cr"(D.template format<unsigned>()));

  write(s5, tid * 64, A.template format<int>());
  write(s4, tid * 64, A);

  vector<float, 16> E = 0, F, G, H = 0;

  read(s6, 0, F);
  read(s7, 0, G);
  read(s9, 0, H);
  asm("cmp.ne (M1, 32) %0 %5 0x0:d\n"
      "(%0) xor (M1, 32) %1 %5 %2"
      : "+cr"(E.template format<short>()), "+r"(H.template format<short>())
      : "r"(F.template format<short>()), "r"(G.template format<short>()));

  write(s8, tid * 64, E);
}
