/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

extern "C" _GENX_MAIN_ void //
test(svmptr_t test_surf [[type("svmptr_t")]],
     svmptr_t etalon_surf [[type("svmptr_t")]]) {
  using data_t = DATA_TYPE;
  vector<data_t, 8> test_vec;
  vector<data_t, 8> etalon_vec;

  data_t one = 1;
  data_t value;

#pragma unroll
  for (int i = 0; i < 8; i++) {
    value = one / (i + 1);
    asm("mov (M1, 1) %0 %1" : "=r"(test_vec(i)) : "F"(value));
    etalon_vec(i) = value;
  }

  cm_svm_block_write(test_surf, test_vec);
  cm_svm_block_write(etalon_surf, etalon_vec);
}
