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
    SurfaceIndex in_buf [[type("buffer_t")]],
#endif
    SurfaceIndex out_buf [[type("buffer_t")]]) {

#if defined(INPUT_MATRIX)
  constexpr int simd_u = SIMD_U;
  constexpr int simd_v = SIMD_V;

  matrix<in_t, simd_u, simd_v> in;
  read(in_buf, 0, in.template format<in_t>());

  matrix<out_t, simd_u, simd_v> out = TEST_FUNC(in, SAT_FLAG);
#elif defined(INPUT_VECTOR)
  constexpr int simd = SIMD;

  vector<in_t, simd> in;
  read(in_buf, 0, in);

  vector<out_t, simd> out = TEST_FUNC(in, SAT_FLAG);
#elif defined(INPUT_CONST)
  constexpr in_t in = INPUT;

  vector<out_t, 8> out = TEST_FUNC(in, SAT_FLAG);
#elif defined(INPUT_SCALAR)
  vector<out_t, 8> out = TEST_FUNC(in, SAT_FLAG);
#endif
  write(out_buf, 0, out.template format<out_t>());
}
