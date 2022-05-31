/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

template <typename T, unsigned EXEC_SIZE>
inline _GENX_ vector<T, EXEC_SIZE> madd(vector<T, EXEC_SIZE> s1,
                                        vector<T, EXEC_SIZE> s2,
                                        vector<T, EXEC_SIZE> s3) {
  vector<T, EXEC_SIZE> dst;

  asm("mad (M1, %4) %0 %1 %2 %3"
      : "=r"(dst)
      : "r"(s1), "r"(s2), "r"(s3), "n"(EXEC_SIZE));

  return dst;
}

_GENX_MAIN_ void test(svmptr_t src0 [[type("svmptr_t")]],
                      svmptr_t src1 [[type("svmptr_t")]],
                      svmptr_t src2 [[type("svmptr_t")]],
                      svmptr_t dst [[type("svmptr_t")]]) {

  vector<ushort, 8> s1, s2, s3;

  cm_svm_block_read(src0, s1);
  cm_svm_block_read(src1, s2);
  cm_svm_block_read(src2, s3);

  vector<ushort, 8> d = madd(s1, s2, s3);

  cm_svm_block_write(dst, d);
}
