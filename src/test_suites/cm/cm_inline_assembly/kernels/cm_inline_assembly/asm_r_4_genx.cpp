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
     svmptr_t in2_surface [[type("svmptr_t")]],
     svmptr_t out_surface [[type("svmptr_t")]]) {
  vector<int, 4> in0(0);
  vector<int, 4> in1(0);
  vector<int, 4> in2(0);
  vector<int, 4> out(0);

  cm_svm_block_read(in0_surface, in0);
  cm_svm_block_read(in1_surface, in1);
  cm_svm_block_read(in2_surface, in2);

  asm("mad (M1, 4) %0 %1 %2 %3" : "=r"(out) : "r"(in0), "r"(in1), "r"(in2));

  cm_svm_block_write(out_surface, out);
}
