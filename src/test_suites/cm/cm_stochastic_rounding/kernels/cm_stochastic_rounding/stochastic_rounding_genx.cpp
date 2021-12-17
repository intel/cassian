/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

using input_t = IN_TYPE;
using output_t = OUT_TYPE;

extern "C" _GENX_MAIN_ void kernel(
#if defined(SRC_VECTOR) || defined(SRC_MATRIX)
    SurfaceIndex input [[type("buffer_t")]],
    SurfaceIndex random [[type("buffer_t")]],
#elif defined(SRC_SCALAR)
    input_t in, input_t rnd,
#endif
    SurfaceIndex output [[type("buffer_t")]]) {
  constexpr int simd = SIMD;

#if defined(SRC_MATRIX)
  constexpr int width = WIDTH;
  constexpr int height = HEIGHT;
  matrix<input_t, width, height> in;
  matrix<input_t, width, height> rnd;
#elif defined(SRC_VECTOR)
  vector<input_t, simd> in;
  vector<input_t, simd> rnd;
#endif

#if defined(SRC_VECTOR) || defined(SRC_MATRIX)
  read(input, 0, in.template format<input_t>());
  read(random, 0, rnd.template format<input_t>());
#endif

  vector<output_t, simd> out;

  out = cm_srnd<output_t>(in, rnd);

  write(output, 0, out);
}
