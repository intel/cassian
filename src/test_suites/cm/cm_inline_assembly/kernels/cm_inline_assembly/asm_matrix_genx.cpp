/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

extern "C" _GENX_MAIN_ void test(svmptr_t inp [[type("svmptr_t")]],
                                 svmptr_t out [[type("svmptr_t")]]) {
  matrix<ushort, 8, 8> m;
  cm_svm_block_read(inp, m.template format<ushort>());

  asm("mov (M1, %2) %0 (-)%1"
      : "=r"(m)
      : "r"(m), "n"(sizeof(m) / (2 * sizeof(ushort))));

  cm_svm_block_write(out, m.template format<ushort>());
}
