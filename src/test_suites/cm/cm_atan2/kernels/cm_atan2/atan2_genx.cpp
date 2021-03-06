/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>
#include <cm/cmtl.h>

#ifndef SAT_FLAG
#define SAT_FLAG 0
#endif

using out_t = OUT_TYPE;
using in_t = IN_TYPE;

extern "C" _GENX_MAIN_ _GENX_FLOAT_CONTROL_(CM_DENORM_ALLOW) void kernel(
#if defined(INPUT_CONST)
#elif defined(INPUT_SCALAR)
    in_t in0, in_t in1,
#else
    svmptr_t in0_buf [[type("svmptr_t")]],
    svmptr_t in1_buf [[type("svmptr_t")]],
#endif
    svmptr_t out_buf [[type("svmptr_t")]]) {
#if defined(INPUT_MATRIX)
  constexpr int simd_u = SIMD_U;
  constexpr int simd_v = SIMD_V;

  matrix<in_t, simd_u, simd_v> in0, in1;

  cm_svm_block_read(in0_buf, in0.template format<in_t>());
  cm_svm_block_read(in1_buf, in1.template format<in_t>());

  matrix<out_t, simd_u, simd_v> out = TEST_FUNC(in0, in1, SAT_FLAG);
#elif defined(INPUT_MATRIX_REF)
  constexpr int simd_u = SIMD_U;
  constexpr int simd_v = SIMD_V;

  matrix<in_t, simd_u, simd_v> in0, in1;

  cm_svm_block_read(in0_buf, in0.template format<in_t>());
  cm_svm_block_read(in1_buf, in1.template format<in_t>());

  matrix_ref<in_t, simd_u, simd_v> in0_ref = in0;
  matrix_ref<in_t, simd_u, simd_v> in1_ref = in1;

  matrix<out_t, simd_u, simd_v> out = TEST_FUNC(in0_ref, in1_ref, SAT_FLAG);
#elif defined(INPUT_VECTOR)
  constexpr int simd = SIMD;

  vector<in_t, simd> in0, in1;
  cm_svm_block_read(in0_buf, in0);
  cm_svm_block_read(in1_buf, in1);

  vector<out_t, simd> out = TEST_FUNC(in0, in1, SAT_FLAG);
#elif defined(INPUT_VECTOR_REF)
  constexpr int simd = SIMD;

  vector<in_t, simd> in0, in1;
  cm_svm_block_read(in0_buf, in0);
  cm_svm_block_read(in1_buf, in1);

  vector_ref<in_t, simd> in0_ref = in0;
  vector_ref<in_t, simd> in1_ref = in1;

  vector<out_t, simd> out = TEST_FUNC(in0_ref, in1_ref, SAT_FLAG);
#elif defined(INPUT_CONST)
  constexpr in_t in0 = INPUT0;
  constexpr in_t in1 = INPUT1;

  vector<out_t, 8> out = TEST_FUNC(in0, in1, SAT_FLAG);
#elif defined(INPUT_SCALAR)
  vector<out_t, 8> out = TEST_FUNC(in0, in1, SAT_FLAG);
#endif
  cm_svm_block_write(out_buf, out.template format<out_t>());
}
