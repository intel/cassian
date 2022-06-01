/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

using data_t = READ_TYPE;

extern "C" _GENX_MAIN_ void kernel(svmptr_t out [[type("svmptr_t")]],
                                   SurfaceIndex image [[type("image1d_t")]],
                                   svmptr_t ubuf [[type("svmptr_t")]]) {
  constexpr int channels = CHANNELS;
  constexpr auto channel_mask = CHANNEL_MASK;

  matrix<data_t, channels, 16> result = 0;

  vector<unsigned, 16> u = 0;

  cm_svm_block_read(ubuf, u);

  load16(result, channel_mask, image, u);

#pragma unroll
  for (int i = 0; i < channels; i++) {
    cm_svm_block_write(out + i * 16 * sizeof(data_t), result.row(i));
  }
}
