/*
 * Copyright (C) 2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

using type_t = TYPE;

extern "C" _GENX_MAIN_ void kernel(SurfaceIndex out_img [[type("image3d_t")]],
                                   SurfaceIndex ubuf [[type("buffer_t")]],
                                   SurfaceIndex vbuf [[type("buffer_t")]],
                                   SurfaceIndex rbuf [[type("buffer_t")]]) {
  constexpr int channels = CHANNELS;
  constexpr int simd = SIMD;
  constexpr auto channel_mask = CHANNEL_MASK;

  matrix<type_t, channels, simd> data;
  vector<type_t, channels *simd> p_vec = {PASSED_VECTOR};
  data.template format<type_t>() = p_vec;

  vector<unsigned, simd> u;
  vector<unsigned, simd> v;
  vector<unsigned, simd> r;

  read(ubuf, 0, u);
  read(vbuf, 0, v);
  read(rbuf, 0, r);

  write_typed(out_img, channel_mask, data, u, v, r);
}