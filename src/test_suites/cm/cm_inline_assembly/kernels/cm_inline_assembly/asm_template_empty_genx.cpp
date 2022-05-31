/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

extern "C" _GENX_MAIN_ void //
test(svmptr_t in_surface [[type("svmptr_t")]],
     svmptr_t out_surface [[type("svmptr_t")]]) {
  vector<int, 8> in_vector;
  vector<int, 8> out_vector;

  cm_svm_block_read(in_surface, in_vector);

  for (int i = 0; i < 8; i++) {
    asm("");
    out_vector(i) = in_vector(i) - 1;
  }

  cm_svm_block_write(out_surface, out_vector);
}
