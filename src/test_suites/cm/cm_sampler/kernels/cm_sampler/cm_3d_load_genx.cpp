/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

using data_t = READ_TYPE;

extern "C" _GENX_MAIN_ void kernel(svmptr_t out [[type("svmptr_t")]],
                                   SurfaceIndex image [[type("image2d_t")]],
                                   svmptr_t ubuf [[type("svmptr_t")]],
                                   svmptr_t vbuf [[type("svmptr_t")]]) {
  constexpr int simd = SIMD;
  constexpr int channels = CHANNELS;
  constexpr auto channel_mask = CHANNEL_MASK;

  matrix<data_t, channels, simd> result = 0;

  vector<unsigned, simd> u = 0;
  vector<unsigned, simd> v = 0;

  cm_svm_block_read(ubuf, u);
  cm_svm_block_read(vbuf, v);

  cm_3d_load<CM_3D_LOAD_LZ, channel_mask>(result.template format<data_t>(), 0,
                                          image, u, v);

#pragma unroll
  for (int i = 0; i < channels; i++) {
    cm_svm_block_write(out + i * simd * sizeof(float), result.row(i));
  }
}
