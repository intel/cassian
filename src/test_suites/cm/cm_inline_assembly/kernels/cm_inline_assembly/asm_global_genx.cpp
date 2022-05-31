/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

_GENX_ matrix<float, 8, 4> m1;
_GENX_ vector<float, 4> v2;
_GENX_ vector<float, 4> v3;

_GENX_MAIN_ void test(svmptr_t out1 [[type("svmptr_t")]],
                      svmptr_t out2 [[type("svmptr_t")]],
                      svmptr_t s1 [[type("svmptr_t")]],
                      svmptr_t s2 [[type("svmptr_t")]],
                      svmptr_t s3 [[type("svmptr_t")]]) {
  vector<float, 4> out;
  cm_svm_block_read(s1, m1.template format<float>());
  cm_svm_block_read(s2, v2);
  cm_svm_block_read(s3, v3);

  asm("mov (M1, 4) %0 %3\n"
      "mov (M1, 4) %2 %3\n"
      "mov (M1, 4) %1 %4"
      : "+r"(v3), "=r"(out), "=r"(v2)
      : "r"(m1.template select<2, 2, 2, 2>(0, 0)));

  cm_svm_block_write(out1, out.template format<float>());
  cm_svm_block_write(out2, v3);
}
