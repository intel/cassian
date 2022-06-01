/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

#ifndef SAT_FLAG
#define SAT_FLAG 0
#endif

using out_t = OUT_TYPE;
using in_t = IN_TYPE;

// extern "C" _GENX_MAIN_ void kernel(
extern "C" _GENX_MAIN_ _GENX_FLOAT_CONTROL_(CM_DENORM_ALLOW) void kernel(
#if defined(INPUT_CONST)
#elif defined(INPUT_SCALAR)
    in_t in,
#else
    svmptr_t in_buf [[type("svmptr_t")]],
#endif
    svmptr_t out_buf [[type("svmptr_t")]]) {

#if defined(INPUT_MATRIX)
  constexpr int simd_u = SIMD_U;
  constexpr int simd_v = SIMD_V;

  matrix<in_t, simd_u, simd_v> in;
  cm_svm_block_read(in_buf, in.template format<in_t>());

  matrix<out_t, simd_u, simd_v> out = TEST_FUNC(in, SAT_FLAG);
#elif defined(INPUT_VECTOR)
  constexpr int simd = SIMD;

  vector<in_t, simd> in;
  cm_svm_block_read(in_buf, in);

  vector<out_t, simd> out = TEST_FUNC(in, SAT_FLAG);
#elif defined(INPUT_CONST)
  constexpr in_t in = INPUT;

  vector<out_t, 8> out = TEST_FUNC(in, SAT_FLAG);
#elif defined(INPUT_SCALAR)
  vector<out_t, 8> out = TEST_FUNC(in, SAT_FLAG);
#endif
  cm_svm_block_write(out_buf, out.template format<out_t>());
}
