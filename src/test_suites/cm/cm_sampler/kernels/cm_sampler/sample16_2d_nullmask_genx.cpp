/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

using data_t = READ_TYPE;

extern "C" _GENX_MAIN_ void kernel(svmptr_t out [[type("svmptr_t")]],
                                   SurfaceIndex nullmaskout
                                   [[type("buffer_t")]],
                                   SurfaceIndex image [[type("image2d_t")]],
                                   SamplerIndex sampler [[type("sampler_t")]],
                                   svmptr_t ubuf [[type("svmptr_t")]],
                                   svmptr_t vbuf [[type("svmptr_t")]]) {
  constexpr int channels = CHANNELS;
  constexpr auto channel_mask = CHANNEL_MASK;

  matrix<data_t, channels, 16> result = 0;

  vector<float, 16> u = 0;
  vector<float, 16> v = 0;

  cm_svm_block_read(ubuf, u);
  cm_svm_block_read(vbuf, v);

  vector<uint16_t, 1> nullmask;

  sample16(result, nullmask, channel_mask, image, sampler, u, v);

#pragma unroll
  for (int i = 0; i < channels; i++) {
    cm_svm_block_write(out + i * 16 * sizeof(data_t), result.row(i));
  }

  write(nullmaskout, 0, 0, nullmask[0]);
}
