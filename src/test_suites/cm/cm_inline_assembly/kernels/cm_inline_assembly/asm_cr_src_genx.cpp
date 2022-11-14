/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

extern "C" _GENX_MAIN_ void test(svmptr_t inp [[type("svmptr_t")]],
                                 svmptr_t out [[type("svmptr_t")]]) {
  vector<int, 8> v;
  cm_svm_block_read(inp, v);

  vector<int, 8> pred = (v % 2) != 0;

  asm("(%2) not (M1, %3) %0 %1"
      : "=r"(v)
      : "r"(v), "cr"(pred), "n"(v.n_elems()));

  cm_svm_block_write(out, v);
}
