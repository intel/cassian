/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

using dst_t = DST_TYPE;
using src0_t = SRC0_TYPE;

constexpr unsigned simd = SIMD;

extern "C" _GENX_MAIN_ void test(
#if defined(SRC0_VECTOR)
    svmptr_t src0_buf [[type("svmptr_t")]],
#elif defined(SRC0_SCALAR)
    src0_t src0,
#endif
    svmptr_t dst_buf [[type("svmptr_t")]]) {
  const auto tid = cm_linear_global_id();
  const auto index = tid * simd;

#if defined(SRC0_VECTOR)
  vector<src0_t, simd> src0;
  cm_svm_block_read(src0_buf + index * sizeof(src0_t), src0);
#elif defined(SRC0_CONST)
  constexpr src0_t src0 = SRC0_VALUE;
#endif

  vector<dst_t, simd> dst = TEST_OP src0;
  cm_svm_block_write(dst_buf + index * sizeof(dst_t), dst);
}
