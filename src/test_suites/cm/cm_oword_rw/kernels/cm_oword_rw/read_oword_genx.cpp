/*
 * Copyright (C) 2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

using type_t = TYPE;

extern "C" _GENX_MAIN_ void kernel(unsigned offset,
                                   SurfaceIndex in_buf [[type("buffer_t")]],
                                   SurfaceIndex out_buf [[type("buffer_t")]]) {
  constexpr int simd = SIMD;
  constexpr int num_in_oword = 16 / sizeof(type_t);
  vector<type_t, simd> in;
#if defined(BUF_DWALIGNED)
  read(DWALIGNED(in_buf), offset, in);
#elif defined(BUF_MODIFIED_DWALIGNED)
  read(MODIFIED_DWALIGNED(in_buf), offset, in);
#elif defined(BUF_NOT_ALIGNED)
  read(in_buf, offset, in);
#endif
  for (unsigned global_offset = 0; global_offset < simd;
       global_offset += num_in_oword) {
    write(out_buf, global_offset * sizeof(type_t),
          in.template select<num_in_oword, 1>(global_offset));
  }
}