/*
 * Copyright (C) 2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>
#include <cm/cmtl.h>

using type_t = TYPE;

extern "C" _GENX_MAIN_ void kernel(unsigned global_offset,
                                   SurfaceIndex offset_buf [[type("buffer_t")]],
                                   SurfaceIndex in_buf [[type("buffer_t")]],
                                   SurfaceIndex out_offset_buf
                                   [[type("buffer_t")]],
                                   SurfaceIndex out_buf [[type("buffer_t")]]) {
  constexpr int simd = SIMD;

  vector<unsigned, simd> offset;
  vector<unsigned, simd> out_offset;
  read(offset_buf, 0, offset);
  read(out_offset_buf, 0, out_offset);

  vector<type_t, simd> in;
#if defined(READ)
  read(in_buf, global_offset, offset, in);
#elif defined(SCALED_READ)
  read_scaled(in_buf, global_offset, offset, in);
#endif
  write(out_buf, 0, out_offset, in);
}