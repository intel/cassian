/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

extern "C" _GENX_MAIN_ void test_asm(svmptr_t in [[type("svmptr_t")]],
                                     svmptr_t out [[type("svmptr_t")]]) {
  vector<int, 8> v;
  cm_svm_block_read(in, v);

  asm("add (M1, 8) %0 %1 %2" : "=r"(v) : "r"(v), "i"(5));

  cm_svm_block_write(out, v);
}

extern "C" _GENX_MAIN_ void test(svmptr_t in [[type("svmptr_t")]],
                                 svmptr_t out [[type("svmptr_t")]]) {
  vector<int, 8> v;
  cm_svm_block_read(in, v);

  v = v + 5;

  cm_svm_block_write(out, v);
}
