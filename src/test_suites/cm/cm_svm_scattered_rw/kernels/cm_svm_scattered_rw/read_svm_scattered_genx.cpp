/*
 * Copyright (C) 2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

using type_t = TYPE;

extern "C" _GENX_MAIN_ void kernel(svmptr_t offset_buf [[type("svmptr_t")]],
                                   svmptr_t in_ptr [[type("svmptr_t")]],
                                   svmptr_t out_buf [[type("svmptr_t")]]) {
  constexpr int simd = SIMD;
  vector<type_t, simd> data;

  vector<unsigned, simd> offset;
  cm_svm_block_read(offset_buf, offset);

  vector<svmptr_t, simd> ptrs = in_ptr;
  ptrs = ptrs + offset;

  cm_svm_scatter_read(ptrs, data);

  constexpr int num_in_oword = 16 / sizeof(type_t);
  for (unsigned global_offset = 0; global_offset < simd;
       global_offset += num_in_oword) {
    cm_svm_block_write(out_buf + global_offset * sizeof(type_t),
                       data.template select<num_in_oword, 1>(global_offset));
  }
}