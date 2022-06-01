/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

using data_t = TYPE;

constexpr unsigned width = WIDTH;
constexpr unsigned height = HEIGHT;

extern "C" _GENX_MAIN_ void kernel(svmptr_t result [[type("svmptr_t")]],
                                   SurfaceIndex image
                                   [[type("image2d_media_block_t")]],
                                   unsigned x, unsigned y) {
  matrix<data_t, height, width> data;

  read(image, x * sizeof(data_t), y, data);

  constexpr unsigned grain = 16 / sizeof(data_t);
  constexpr unsigned size_owords = data.n_elems() / grain;
  constexpr unsigned size_owords_padded =
      size_owords + (data.n_elems() % grain ? 1 : 0);
  constexpr unsigned size_padded = grain * size_owords_padded;

  vector<data_t, size_padded> tmp = 0;
  tmp.template select<data.n_elems(), 1>(0) = data;

#pragma unroll
  for (unsigned offset = 0; offset < tmp.n_elems(); offset += grain) {
    cm_svm_block_write(result + offset * sizeof(data_t),
                       tmp.template select<grain, 1>(offset));
  }
}
