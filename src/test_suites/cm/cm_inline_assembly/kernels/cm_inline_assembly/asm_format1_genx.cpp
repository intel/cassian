/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

extern "C" _GENX_MAIN_ void
test(svmptr_t s1 [[type("svmptr_t")]], svmptr_t s2 [[type("svmptr_t")]],
     svmptr_t s3 [[type("svmptr_t")]], svmptr_t s4 [[type("svmptr_t")]],
     svmptr_t s5 [[type("svmptr_t")]], svmptr_t s6 [[type("svmptr_t")]],
     svmptr_t s7 [[type("svmptr_t")]], svmptr_t s8 [[type("svmptr_t")]],
     svmptr_t s9 [[type("svmptr_t")]]) {
  vector<float, 16> A = 0;
  vector<int, 8> B, C;
  vector<float, 8> D;

  auto tid = cm_linear_global_id();

  cm_svm_block_read(s1, B);
  cm_svm_block_read(s2, C);
  cm_svm_block_read(s3, D);

  asm("(%3) xor (M1, 8) %0 %1 %2"
      : "+r"(A.template select<8, 2>().template format<int>())
      : "r"(B), "r"(C), "cr"(D.template format<unsigned>()));

  cm_svm_block_write(s5 + tid * 64, A.template format<int>());
  cm_svm_block_write(s4 + tid * 64, A);

  vector<float, 16> E = 0, F, G, H = 0;

  cm_svm_block_read(s6, F);
  cm_svm_block_read(s7, G);
  cm_svm_block_read(s9, H);
  asm("cmp.ne (M1, 32) %0 %5 0x0:d\n"
      "(%0) xor (M1, 32) %1 %5 %2"
      : "+cr"(E.template format<short>()), "+r"(H.template format<short>())
      : "r"(F.template format<short>()), "r"(G.template format<short>()));

  cm_svm_block_write(s8 + tid * 64, E);
}
