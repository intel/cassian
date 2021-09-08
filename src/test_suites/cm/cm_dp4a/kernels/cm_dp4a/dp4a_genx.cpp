/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

using out_t = OUT_TYPE;
using acc_t = ACC_TYPE;
using a_t = A_TYPE;
using b_t = B_TYPE;

extern "C" _GENX_MAIN_ void kernel(SurfaceIndex out_buf [[type("buffer_t")]],
                                   SurfaceIndex acc_buf [[type("buffer_t")]],
                                   SurfaceIndex a_buf [[type("buffer_t")]],
                                   SurfaceIndex b_buf [[type("buffer_t")]]) {
  constexpr int simd = SIMD;

  vector<acc_t, simd> acc;
  vector<a_t, simd> a;
  vector<b_t, simd> b;

  read(acc_buf, 0, acc);
  read(a_buf, 0, a);
  read(b_buf, 0, b);

  vector<out_t, simd> out = cm_dp4a<out_t>(acc, a, b);

  write(out_buf, 0, out);
}
