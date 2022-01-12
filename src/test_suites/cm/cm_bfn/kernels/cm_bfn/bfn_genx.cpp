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
    SurfaceIndex out_buf [[type("buffer_t")]]
#else
    SurfaceIndex out_buf [[type("buffer_t")]],
#if defined(INPUT_SCALAR)
    in_t in0, in_t in1, in_t in2
#elif defined(INPUT_VECTOR)
    SurfaceIndex in0_buf [[type("buffer_t")]],
    SurfaceIndex in1_buf [[type("buffer_t")]],
    SurfaceIndex in2_buf [[type("buffer_t")]]
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
  read(in0_buf, 0, in0);
#elif defined(INPUT0_VEC_REF)
  vector<in_t, simd> in0_orig;
  read(in0_buf, 0, in0_orig);
  vector_ref<in_t, simd> in0 = in0_orig;
#endif
#if defined(INPUT1_VEC)
  vector<in_t, simd> in1;
  read(in1_buf, 0, in1);
#elif defined(INPUT1_VEC_REF)
  vector<in_t, simd> in1_orig;
  read(in1_buf, 0, in1_orig);
  vector_ref<in_t, simd> in1 = in1_orig;
#endif
#if defined(INPUT2_VEC)
  vector<in_t, simd> in2;
  read(in2_buf, 0, in2);
#elif defined(INPUT2_VEC_REF)
  vector<in_t, simd> in2_orig;
  read(in2_buf, 0, in2_orig);
  vector_ref<in_t, simd> in2 = in2_orig;
#endif
  vector<out_t, simd> out = cm_bfn<static_cast<BFNT>(OP_NUMBER)>(in0, in1, in2);
#endif
  write(out_buf, 0, out);
}