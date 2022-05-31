/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

#ifndef FLAGS
#define FLAGS 0
#endif

using dst_t = DST_TYPE;
using src0_t = SRC0_TYPE;
using src1_t = SRC1_TYPE;

constexpr unsigned simd = SIMD;

extern "C" _GENX_MAIN_ void test(
#if defined(SRC0_VECTOR)
    svmptr_t src0_buf [[type("svmptr_t")]],
#elif defined(SRC0_SCALAR)
    src0_t src0,
#endif
#if defined(SRC1_VECTOR)
    svmptr_t src1_buf [[type("svmptr_t")]],
#elif defined(SRC1_SCALAR)
    src1_t src1,
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

#if defined(SRC1_VECTOR)
  vector<src1_t, simd> src1;
  cm_svm_block_read(src1_buf + index * sizeof(src1_t), src1);
#elif defined(SRC1_CONST)
  constexpr src1_t src1 = SRC1_VALUE;
#endif

  vector<dst_t, simd> dst = TEST_OP<dst_t>(src0, src1, FLAGS);
  cm_svm_block_write(dst_buf + index * sizeof(dst_t), dst);
}
