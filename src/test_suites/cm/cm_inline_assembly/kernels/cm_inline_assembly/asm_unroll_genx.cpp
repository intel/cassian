/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

extern "C" _GENX_MAIN_ void test(svmptr_t test_s [[type("svmptr_t")]],
                                 svmptr_t etalon_s [[type("svmptr_t")]]) {
  vector<int, 8> test_v;
  vector<int, 8> etalon_v;

#pragma unroll
  for (int i = 0; i < 8; i++) {
    asm("mov (M1, 1) %0 %1" : "=r"(test_v(i)) : "i"(i));
  }

#pragma unroll
  for (int i = 0; i < 8; i++) {
    etalon_v(i) = i;
  }

  cm_svm_block_write(test_s, test_v);
  cm_svm_block_write(etalon_s, etalon_v);
}
