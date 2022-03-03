/*
 * Copyright (C) 2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

using type_t = TYPE;

extern "C" _GENX_MAIN_ void kernel(SurfaceIndex out_img [[type("image1d_t")]],
                                   SurfaceIndex ubuf [[type("buffer_t")]]) {
  constexpr int channels = CHANNELS;
  constexpr int simd = SIMD;
  constexpr auto channel_mask = CHANNEL_MASK;

  matrix<type_t, channels, simd> data;
  vector<type_t, channels *simd> p_vec = {PASSED_VECTOR};
  data.template format<type_t>() = p_vec;

  vector<unsigned, simd> u;

  read(ubuf, 0, u);

  write_typed(out_img, channel_mask, data, u);
}