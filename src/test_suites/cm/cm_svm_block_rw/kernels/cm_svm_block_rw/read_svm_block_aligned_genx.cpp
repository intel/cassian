/*
 * Copyright (C) 2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

using type_t = TYPE;

extern "C" _GENX_MAIN_ void kernel(unsigned offset,
                                   svmptr_t in_ptr [[type("svmptr_t")]],
                                   SurfaceIndex out_buf [[type("buffer_t")]]) {
  constexpr int simd = SIMD;
  vector<type_t, simd> data;
#if defined(IS_UNALIGNED)
  cm_svm_block_read_unaligned(in_ptr + offset, data);
#elif defined(IS_ALIGNED)
  cm_svm_block_read(in_ptr + offset, data);
#endif
  constexpr int num_in_oword = 16 / sizeof(type_t);
  for (unsigned global_offset = 0; global_offset < simd;
       global_offset += num_in_oword) {
    write(out_buf, global_offset * sizeof(type_t),
          data.template select<num_in_oword, 1>(global_offset));
  }
}