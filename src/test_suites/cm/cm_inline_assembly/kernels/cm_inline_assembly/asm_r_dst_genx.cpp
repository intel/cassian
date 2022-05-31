/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

extern "C" _GENX_MAIN_ void test(svmptr_t inp [[type("svmptr_t")]],
                                 svmptr_t out [[type("svmptr_t")]]) {
  vector<uchar, 32> v;
  cm_svm_block_read(inp, v);

  asm("mov (M1, 32) %0 (-)%1" : "=r"(v) : "r"(v));

  cm_svm_block_write(out, v);
}
