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
    svmptr_t input [[type("svmptr_t")]],
#elif defined(SRC_SCALAR)
    input_t in,
#endif
    svmptr_t output [[type("svmptr_t")]]) {
  constexpr int simd = SIMD;

#if defined(SRC_MATRIX)
  constexpr int width = WIDTH;
  constexpr int height = HEIGHT;
  matrix<input_t, width, height> in;
#elif defined(SRC_VECTOR)
  vector<input_t, simd> in;
#elif defined(SRC_CONST)
  constexpr input_t in = SRC_VALUE;
#endif

#if defined(SRC_VECTOR) || defined(SRC_MATRIX)
  cm_svm_block_read(input, in.template format<input_t>());
#endif

  vector<output_t, simd> out;

  out = CONVERT<output_t>(in);

  cm_svm_block_write(output, out);
}
