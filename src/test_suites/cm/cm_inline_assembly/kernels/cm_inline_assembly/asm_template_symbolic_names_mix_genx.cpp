/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

extern "C" _GENX_MAIN_ void //
test(svmptr_t in_surface [[type("svmptr_t")]],
     svmptr_t test_surface [[type("svmptr_t")]],
     svmptr_t etalon_surface [[type("svmptr_t")]]) {
  vector<int, 8> in0_vector;
  vector<int, 8> in1_vector;
  vector<int, 8> test_vector;
  vector<int, 8> etalon_vector;

  cm_svm_block_read(in_surface, in0_vector);

#pragma nounroll
  for (int i = 0; i < 8; i++) {
    asm("add (M1, 1) %[dst] %1 %[src2]"
        : [ dst ] "=a"(in1_vector(i))
        : [ src1 ] "r"(i), [ src2 ] "i"(10));
    asm("add (M1, 1) %0 %[src1] %[src2]"
        : [ dst ] "=a"(test_vector(i))
        : [ src1 ] "a"(in0_vector(i)), [ src2 ] "a"(in1_vector(i)));

    etalon_vector(i) = in0_vector(i) + i + 10;
  }

  cm_svm_block_write(test_surface, test_vector);
  cm_svm_block_write(etalon_surface, etalon_vector);
}
