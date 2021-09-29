/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

extern "C" _GENX_MAIN_ void test(SurfaceIndex in0 [[type("buffer_t")]],
                                 SurfaceIndex in1 [[type("buffer_t")]],
                                 SurfaceIndex out [[type("buffer_t")]],
                                 SurfaceIndex pred [[type("buffer_t")]]) {
  vector<int, 16> vin0;
  vector<int, 16> vin1;
  vector<int, 16> vout;

  vector<uint16_t, 16> vpred;

  read(in0, 0, vin0);
  read(in1, 0, vin1);

  asm("setp (M1, 16) %0 %1" : "=cr"(vpred) : "r"(vin0));
  asm("cmp.eq (M1, 16) %0 %1 %2" : "=r"(vout) : "r"(vin0), "r"(vin1));

  write(pred, 0, vpred);
  write(out, 0, vout);
}
