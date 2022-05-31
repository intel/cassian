/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

using out_t = OUT_TYPE;
using acc_t = ACC_TYPE;
using a_t = A_TYPE;
using b_t = B_TYPE;

extern "C" _GENX_MAIN_ void kernel(svmptr_t out_buf [[type("svmptr_t")]],
                                   svmptr_t acc_buf [[type("svmptr_t")]],
                                   svmptr_t a_buf [[type("svmptr_t")]],
                                   svmptr_t b_buf [[type("svmptr_t")]]) {
  constexpr int simd = SIMD;

  vector<acc_t, simd> acc;
  vector<a_t, simd> a;
  vector<b_t, simd> b;

  cm_svm_block_read(acc_buf, acc);
  cm_svm_block_read(a_buf, a);
  cm_svm_block_read(b_buf, b);

  vector<out_t, simd> out = cm_dp4a<out_t>(acc, a, b);

  cm_svm_block_write(out_buf, out);
}
