/*
 * Copyright (C) 2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

using type_t = TYPE;

extern "C" _GENX_MAIN_ void kernel(SurfaceIndex in_img [[type("image2d_t")]],
                                   SurfaceIndex ubuf [[type("buffer_t")]],
                                   SurfaceIndex vbuf [[type("buffer_t")]],
                                   SurfaceIndex wbuf [[type("buffer_t")]],
                                   SurfaceIndex out_buf [[type("buffer_t")]]) {
  constexpr int channels = CHANNELS;
  constexpr int simd = SIMD;
  constexpr auto channel_mask = CHANNEL_MASK;
  constexpr int num_in_oword = 16 / sizeof(type_t);

  matrix<type_t, channels, simd> in;

  vector<unsigned, simd> u;
  vector<unsigned, simd> v;
  vector<unsigned, simd> w;

  read(ubuf, 0, u);
  read(vbuf, 0, v);
  read(wbuf, 0, w);

  read_typed(in_img, channel_mask, in, u, v, w);

#pragma unroll
  for (int i = 0; i < channels; i++) {
    write(out_buf, i * simd * sizeof(type_t), in.row(i));
  }
}