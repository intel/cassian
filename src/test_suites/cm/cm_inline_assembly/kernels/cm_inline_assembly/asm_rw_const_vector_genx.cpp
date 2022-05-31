/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

extern "C" _GENX_MAIN_ void //
test(svmptr_t test_surface [[type("svmptr_t")]],
     svmptr_t etalon_surface [[type("svmptr_t")]]) {
  vector<int, 8> test_vector(0);
  vector<int, 8> etalon_vector(0);
  vector<int, 8> data_vector(0xf);

  asm("mov (M1, 4) %0(0,0)<2> %1(0,0)<0;4,2>"
      : "+rw"(test_vector)
      : "rw"(data_vector));
  for (int i = 0; i < 8; i += 2)
    etalon_vector(i) = data_vector(i);

  cm_svm_block_write(test_surface, test_vector);
  cm_svm_block_write(etalon_surface, etalon_vector);
}
