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
  vector<int, 8> in1, in2, in3, tmp(0), out;

  cm_svm_block_read(in0_surface, in1);
  cm_svm_block_read(in1_surface, in2);
  cm_svm_block_read(in2_surface, in3);

  asm("add (M1, 8) %1 %2 %3\n"
      "add (M1, 8) %0 %4 %5"
      : "=r"(out), "+r"(tmp)
      : "r"(in1), "r"(in2), "r"(in3));

  cm_svm_block_write(out_surface, out);
}
