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
  vector<int, 8> input_vector;
  vector<int, 8> test_vector(0);
  vector<int, 8> etalon_vector(0);

  const int exec_size = 4;

  cm_svm_block_read(input_surface, input_vector);

  asm("mov (M1, %2) %0 %1"
      : "+r"(test_vector)
      : "r"(input_vector), "n"(exec_size));

  cm_svm_block_write(test_surface, test_vector);

  for (int i = 0; i < exec_size; i++)
    etalon_vector(i) = input_vector(i);

  cm_svm_block_write(etalon_surface, etalon_vector);
}
