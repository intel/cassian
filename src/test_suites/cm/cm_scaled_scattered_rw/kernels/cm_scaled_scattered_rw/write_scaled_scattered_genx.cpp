/*
 * Copyright (C) 2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

using type_t = TYPE;

extern "C" _GENX_MAIN_ void kernel(
#if defined(VECTOR_INPUT)
    SurfaceIndex offset_buf [[type("buffer_t")]], unsigned global_offset,
#elif defined(VALUE_INPUT)
    unsigned offset, unsigned global_offset,
#endif
    SurfaceIndex out_buf [[type("buffer_t")]]) {
#if defined(VECTOR_INPUT)
  constexpr int simd = SIMD;
  vector<type_t, simd> data = {PASSED_VECTOR};
  vector<unsigned, simd> offset;
  read(offset_buf, 0, offset);
#elif defined(VALUE_INPUT)
  type_t data = PASSED_VALUE;
#endif
#if defined(WRITE)
  write(out_buf, global_offset, offset, data);
#elif defined(SCALED_WRITE)
  write_scaled(out_buf, global_offset, offset, data);
#endif
}