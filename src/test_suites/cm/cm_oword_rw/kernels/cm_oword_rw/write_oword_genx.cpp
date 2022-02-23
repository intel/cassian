/*
 * Copyright (C) 2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

using type_t = TYPE;

extern "C" _GENX_MAIN_ void kernel(unsigned offset,
                                   SurfaceIndex buf [[type("buffer_t")]]) {
  constexpr int simd = SIMD;
  vector<type_t, simd> in = {PASSED_VECTOR};
  write(buf, offset, in);
}