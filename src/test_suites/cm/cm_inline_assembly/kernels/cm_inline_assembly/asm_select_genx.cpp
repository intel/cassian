/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

_GENX_MAIN_ void test(svmptr_t inp [[type("svmptr_t")]],
                      svmptr_t out [[type("svmptr_t")]]) {
  matrix<float, 8, 4> m;
  vector<float, 4> dst;

  cm_svm_block_read(inp, m.template format<float>());

  asm("mov (M1, 4) %0 %1" : "=r"(dst) : "r"(m.select<2, 2, 2, 2>(0, 0)));

  cm_svm_block_write(out, dst);
}
