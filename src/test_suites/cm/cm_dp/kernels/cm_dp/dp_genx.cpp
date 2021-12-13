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

extern "C" _GENX_MAIN_ _GENX_FLOAT_CONTROL_(CM_DENORM_ALLOW) void kernel(
    SurfaceIndex in0_buf [[type("buffer_t")]],
#if defined(INPUT1_CONST)
#elif defined(INPUT1_SCALAR)
    in_t in1,
#else
    SurfaceIndex in1_buf [[type("buffer_t")]],
#endif
    SurfaceIndex out_buf [[type("buffer_t")]]) {
#if defined(INPUT0_MATRIX)
  constexpr int simd0_u = SIMD0_U;
  constexpr int simd0_v = SIMD0_V;
  constexpr int simd_out = simd0_u * simd0_v;
  matrix<in_t, simd0_u, simd0_v> in0;
#elif defined(INPUT0_MATRIX_REF)
  constexpr int simd0_u = SIMD0_U;
  constexpr int simd0_v = SIMD0_V;
  constexpr int simd_out = simd0_u * simd0_v;
  matrix<in_t, simd0_u, simd0_v> in0_orig;
  matrix_ref<in_t, simd0_u, simd0_v> in0 = in0_orig;
#elif defined(INPUT0_VECTOR)
  constexpr int simd0 = SIMD0;
  constexpr int simd_out = simd0;
  vector<in_t, simd0> in0;
#elif defined(INPUT0_VECTOR_REF)
  constexpr int simd0 = SIMD0;
  constexpr int simd_out = simd0;
  vector<in_t, simd0> in0_orig;
  vector_ref<in_t, simd0> in0 = in0_orig;
#endif
  read(in0_buf, 0, in0.template format<in_t>());
#if defined(INPUT1_MATRIX)
  constexpr int simd1_u = SIMD1_U;
  constexpr int simd1_v = SIMD1_V;
  matrix<in_t, simd1_u, simd1_v> in1;
  read(in1_buf, 0, in1.template format<in_t>());
#elif defined(INPUT1_MATRIX_REF)
  constexpr int simd1_u = SIMD1_U;
  constexpr int simd1_v = SIMD1_V;
  matrix<in_t, simd1_u, simd1_v> in1_orig;
  matrix_ref<in_t, simd1_u, simd1_v> in1 = in1_orig;
  read(in1_buf, 0, in1.template format<in_t>());
#elif defined(INPUT1_VECTOR)
  constexpr int simd1 = SIMD1;
  vector<in_t, simd1> in1;
  read(in1_buf, 0, in1.template format<in_t>());
#elif defined(INPUT1_VECTOR_REF)
  constexpr int simd1 = SIMD1;
  vector<in_t, simd1> in1_orig;
  vector_ref<in_t, simd1> in1 = in1_orig;
  read(in1_buf, 0, in1.template format<in_t>());
#elif defined(INPUT1_CONST)
  constexpr in_t in1 = INPUT1;
#endif
  vector<out_t, simd_out> out = TEST_FUNC<out_t>(in0, in1, SAT_FLAG);
  write(out_buf, 0, out.template format<out_t>());
}