/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

extern "C" _GENX_MAIN_ void //
test(svmptr_t in0_surface [[type("svmptr_t")]],
     svmptr_t in1_surface [[type("svmptr_t")]],
     svmptr_t out_surface [[type("svmptr_t")]]) {
  vector<int, 8> in0_vector(0);
  vector<int, 8> in1_vector(0);
  vector<int, 8> out_vector(0);

  cm_svm_block_read(in0_surface, in0_vector);
  cm_svm_block_read(in1_surface, in1_vector);

  asm("add (M1, 8) %0 %1 %2"
      : "=r"(out_vector)
      : "r"(in0_vector), "r"(in1_vector));

  cm_svm_block_write(out_surface, out_vector);
}
