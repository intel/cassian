/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

extern "C" _GENX_MAIN_ void test(svmptr_t out [[type("svmptr_t")]]) {
  vector<int, 8> src = 0;
  vector<int, 8> dst = 0;
#pragma nounroll
  for (int i = 0; i < 8; i++) {
    asm("add (M1, 1) %0 %1 %2" : "=a"(src(i)) : "r"(i), "i"(10));
    // src(i) = i + 10;
  }
#pragma nounroll
  for (int i = 0; i < 8; i += 2) {
    asm("mov (M1, 1) %0 %1" : "=a"(dst(i)) : "a"(src(i + 1)));
    asm("mov (M1, 1) %0 %1" : "=a"(dst(i + 1)) : "a"(src(i)));
  }

  cm_svm_block_write(out, dst);
}
