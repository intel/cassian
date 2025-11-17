/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

extern "C" _GENX_MAIN_ void test(svmptr_t in0 [[type("svmptr_t")]],
                                 svmptr_t in1 [[type("svmptr_t")]],
                                 svmptr_t out [[type("svmptr_t")]],
                                 svmptr_t pred [[type("svmptr_t")]]) {
  vector<int, 16> vin0;
  vector<int, 16> vin1;
  vector<int, 16> vout;

  vector<uint16_t, 16> vpred;

  cm_svm_block_read(in0, vin0);
  cm_svm_block_read(in1, vin1);

  asm("setp (M1_NM, 16) %0 %1" : "=cr"(vpred) : "r"(vin0));
  asm("cmp.eq (M1, 16) %0 %1 %2" : "=r"(vout) : "r"(vin0), "r"(vin1));

  cm_svm_block_write(pred, vpred);
  cm_svm_block_write(out, vout);
}
