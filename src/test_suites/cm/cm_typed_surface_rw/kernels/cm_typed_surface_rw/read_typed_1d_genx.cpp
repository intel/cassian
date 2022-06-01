/*
 * Copyright (C) 2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

using type_t = TYPE;

extern "C" _GENX_MAIN_ void kernel(SurfaceIndex in_img [[type("image1d_t")]],
                                   svmptr_t ubuf [[type("svmptr_t")]],
                                   svmptr_t out_buf [[type("svmptr_t")]]) {
  constexpr int channels = CHANNELS;
  constexpr int simd = SIMD;
  constexpr auto channel_mask = CHANNEL_MASK;
  constexpr int num_in_oword = 16 / sizeof(type_t);

  matrix<type_t, channels, simd> in;

  vector<unsigned, simd> u;

  cm_svm_block_read(ubuf, u);

  read_typed(in_img, channel_mask, in, u);

#pragma unroll
  for (int i = 0; i < channels; i++) {
    cm_svm_block_write(out_buf + i * simd * sizeof(type_t), in.row(i));
  }
}