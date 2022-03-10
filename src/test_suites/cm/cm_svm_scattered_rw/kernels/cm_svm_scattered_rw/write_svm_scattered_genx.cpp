/*
 * Copyright (C) 2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

using type_t = TYPE;

extern "C" _GENX_MAIN_ void kernel(SurfaceIndex offset_buf [[type("buffer_t")]],
                                   svmptr_t out_ptr [[type("svmptr_t")]]) {
  constexpr int simd = SIMD;
  vector<type_t, simd> data = {PASSED_VECTOR};

  vector<unsigned, simd> offset;
  read(offset_buf, 0, offset);

  vector<svmptr_t, simd> ptrs = out_ptr;
  ptrs = ptrs + offset;

  cm_svm_scatter_write(ptrs, data);
}