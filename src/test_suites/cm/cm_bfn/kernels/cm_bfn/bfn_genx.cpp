/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */
#include <cm/cm.h>

using out_t = OUT_TYPE;
using in_t = IN_TYPE;

extern "C" _GENX_MAIN_ void kernel(
#if defined(INPUT_CONST)
    svmptr_t out_buf [[type("svmptr_t")]]
#else
    svmptr_t out_buf [[type("svmptr_t")]],
#if defined(INPUT_SCALAR)
    in_t in0, in_t in1, in_t in2
#elif defined(INPUT_VECTOR)
    svmptr_t in0_buf [[type("svmptr_t")]],
    svmptr_t in1_buf [[type("svmptr_t")]], svmptr_t in2_buf [[type("svmptr_t")]]
#endif
#endif
) {
#if defined(INPUT_SCALAR)
  vector<out_t, 8> out = cm_bfn<static_cast<BFNT>(OP_NUMBER)>(in0, in1, in2);
#elif defined(INPUT_CONST)
  constexpr in_t in0 = INPUT0;
  constexpr in_t in1 = INPUT1;
  constexpr in_t in2 = INPUT2;

  vector<out_t, 8> out = cm_bfn<static_cast<BFNT>(OP_NUMBER)>(in0, in1, in2);
#elif defined(INPUT_VECTOR)
  constexpr int simd = SIMD;
#if defined(INPUT0_VEC)
  vector<in_t, simd> in0;
  cm_svm_block_read(in0_buf, in0);
#elif defined(INPUT0_VEC_REF)
  vector<in_t, simd> in0_orig;
  cm_svm_block_read(in0_buf, in0_orig);
  vector_ref<in_t, simd> in0 = in0_orig;
#endif
#if defined(INPUT1_VEC)
  vector<in_t, simd> in1;
  cm_svm_block_read(in1_buf, in1);
#elif defined(INPUT1_VEC_REF)
  vector<in_t, simd> in1_orig;
  cm_svm_block_read(in1_buf, in1_orig);
  vector_ref<in_t, simd> in1 = in1_orig;
#endif
#if defined(INPUT2_VEC)
  vector<in_t, simd> in2;
  cm_svm_block_read(in2_buf, in2);
#elif defined(INPUT2_VEC_REF)
  vector<in_t, simd> in2_orig;
  cm_svm_block_read(in2_buf, in2_orig);
  vector_ref<in_t, simd> in2 = in2_orig;
#endif
  vector<out_t, simd> out = cm_bfn<static_cast<BFNT>(OP_NUMBER)>(in0, in1, in2);
#endif
  cm_svm_block_write(out_buf, out);
}