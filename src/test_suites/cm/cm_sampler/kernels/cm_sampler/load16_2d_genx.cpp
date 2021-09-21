/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

extern "C" _GENX_MAIN_ void kernel(SurfaceIndex out [[type("buffer_t")]],
                                   SurfaceIndex image [[type("image2d_t")]],
                                   SurfaceIndex ubuf [[type("buffer_t")]],
                                   SurfaceIndex vbuf [[type("buffer_t")]]) {
  constexpr int channels = CHANNELS;
  constexpr auto channel_mask = CHANNEL_MASK;

  matrix<float, channels, 16> result = 0;

  vector<unsigned, 16> u = 0;
  vector<unsigned, 16> v = 0;

  read(ubuf, 0, u);
  read(vbuf, 0, v);

  load16(result, channel_mask, image, u, v);

#pragma unroll
  for (int i = 0; i < channels; i++) {
    write(out, i * 16 * sizeof(float), result.row(i));
  }
}
