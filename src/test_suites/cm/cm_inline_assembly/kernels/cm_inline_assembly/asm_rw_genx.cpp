/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

extern "C" _GENX_MAIN_ void //
test(svmptr_t input_surface [[type("svmptr_t")]],
     svmptr_t test_surface [[type("svmptr_t")]],
     svmptr_t etalon_surface [[type("svmptr_t")]]) {
  vector<int, 8> input_vector(0);
  vector<int, 8> test_vector(0);
  vector<int, 8> etalon_vector(0);

  cm_svm_block_read(input_surface, input_vector);

  asm("mov (M1, 4) %0(0,0)<2> %1(0,0)<0;4,2>"
      : "+rw"(test_vector)
      : "rw"(input_vector));
  for (int i = 0; i < 8; i += 2)
    etalon_vector(i) = input_vector(i);

  cm_svm_block_write(test_surface, test_vector);
  cm_svm_block_write(etalon_surface, etalon_vector);
}
