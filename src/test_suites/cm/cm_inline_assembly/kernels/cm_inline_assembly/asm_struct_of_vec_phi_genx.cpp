/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

extern "C" _GENX_MAIN_ void test(svmptr_t a_idx [[type("svmptr_t")]],
                                 svmptr_t b_idx [[type("svmptr_t")]],
                                 svmptr_t res_a_idx [[type("svmptr_t")]],
                                 svmptr_t res_b_idx [[type("svmptr_t")]]) {
  constexpr int width = 16;

  vector<int, width> a;
  vector<int, width> b;

  cm_svm_block_read(a_idx, a);
  cm_svm_block_read(b_idx, b);

  int cond = a[0];
  vector<int, width> res_a;
  vector<int, width> res_b;

  if (cond) {
    asm("add (M1, %4) %0 %2 %3\n"
        "add (M1, %4) %1 %2 %3"
        : "=r"(res_a), "=r"(res_b)
        : "r"(a), "r"(b), "n"(width));
  } else {
    asm("add (M1, %4) %0 %2 (-)%3\n"
        "add (M1, %4) %1 %2 (-)%3"
        : "=r"(res_a), "=r"(res_b)
        : "r"(a), "r"(b), "n"(width));
  }

  cm_svm_block_write(res_a_idx, res_a);
  cm_svm_block_write(res_b_idx, res_b);
}
