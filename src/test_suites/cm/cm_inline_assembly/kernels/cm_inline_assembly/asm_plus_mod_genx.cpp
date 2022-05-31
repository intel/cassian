/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

extern "C" _GENX_MAIN_ void test(svmptr_t in0 [[type("svmptr_t")]],
                                 svmptr_t in1 [[type("svmptr_t")]],
                                 svmptr_t in2 [[type("svmptr_t")]],
                                 svmptr_t out [[type("svmptr_t")]]) {
  vector<int, 8> v0(0);
  vector<int, 8> v1(0);
  vector<int, 8> v2(0);

  cm_svm_block_read(in0, v0);
  cm_svm_block_read(in1, v1);
  cm_svm_block_read(in2, v2);

  asm("add (M1, %2) %0(0,0)<1> %0(0,0)<1;1,0> %1(0,0)<1;1,0>"
      : "+rw"(v0)
      : "rw"(v1), "n"(sizeof(v0) / sizeof(int)));

  asm("add (M1, %2) %0 %3 (-)%1"
      : "+r"(v0)
      : "r"(v2), "n"(sizeof(v0) / sizeof(int)));

  cm_svm_block_write(out, v0);
}
