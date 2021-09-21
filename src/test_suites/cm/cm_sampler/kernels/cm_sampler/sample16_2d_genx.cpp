/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

using data_t = READ_TYPE;

extern "C" _GENX_MAIN_ void kernel(SurfaceIndex out [[type("buffer_t")]],
                                   SurfaceIndex image [[type("image2d_t")]],
                                   SamplerIndex sampler [[type("sampler_t")]],
                                   SurfaceIndex ubuf [[type("buffer_t")]],
                                   SurfaceIndex vbuf [[type("buffer_t")]]) {
  constexpr int channels = CHANNELS;
  constexpr auto channel_mask = CHANNEL_MASK;

  matrix<data_t, channels, 16> result = 0;

  vector<float, 16> u = 0;
  vector<float, 16> v = 0;

  read(ubuf, 0, u);
  read(vbuf, 0, v);

  sample16(result, channel_mask, image, sampler, u, v);

#pragma unroll
  for (int i = 0; i < channels; i++) {
    write(out, i * 16 * sizeof(data_t), result.row(i));
  }
}
