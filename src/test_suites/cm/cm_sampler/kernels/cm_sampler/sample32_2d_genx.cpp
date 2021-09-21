/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

extern "C" _GENX_MAIN_ void kernel(SurfaceIndex out [[type("buffer_t")]],
                                   SurfaceIndex image [[type("image2d_t")]],
                                   SamplerIndex sampler [[type("sampler_t")]],
                                   float u, float v, float du, float dv) {
  constexpr int channels = CHANNELS;
  constexpr auto channel_mask = CHANNEL_MASK;
  constexpr auto ofc = FORMAT_CONTROL;

  matrix<uint16_t, channels, 32> result = 0;

  sample32(result, channel_mask, image, sampler, u, v, du, dv, ofc);

#pragma unroll
  for (int i = 0; i < channels; i++) {
    write(out, i * 32 * sizeof(uint16_t), result.row(i));
  }
}
