/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "cm/cm.h"

void _GENX_MAIN_ test(svmptr_t in0 [[type("svmptr_t")]],
                      svmptr_t in1 [[type("svmptr_t")]],
                      svmptr_t out [[type("svmptr_t")]]) {
  vector<ushort, 8> cond;
  vector<uint, 8> a, b;

  cm_svm_block_read(in0, a);
  cm_svm_block_read(in1, b);

  vector<uint, 8> c = 0;

  asm("cmp.eq (M1, 8) %0 %1 %2" : "=cr"(cond) : "r"(a), "r"(b));

  SIMD_IF_BEGIN(cond == 0) { c = a + b; }
  SIMD_IF_END

  cm_svm_block_write(out, c);
}
