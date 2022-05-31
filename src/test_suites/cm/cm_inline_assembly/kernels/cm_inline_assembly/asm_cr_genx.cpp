/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

extern "C" _GENX_MAIN_ void test(svmptr_t input_s [[type("svmptr_t")]],
                                 svmptr_t test_s [[type("svmptr_t")]],
                                 svmptr_t etalon_s [[type("svmptr_t")]]) {
  vector<int, 8> input_v(0);
  vector<int, 8> test_v(0);
  vector<int, 8> etalon_v(0);

  cm_svm_block_read(input_s, input_v);

  for (int i = 0; i < 8; i++) {
    bool pred = false;
    int first_bit = input_v(i) & 1;

    asm("cmp.ne (M1, 1) %0 %1 %2" : "=cr"(pred) : "r"(first_bit), "i"(0));
    asm("(%2) not (M1, 1) %0 %1"
        : "=a"(test_v(i))
        : "a"(input_v(i)), "cr"(pred));

    if (first_bit)
      etalon_v(i) = ~input_v(i);
  }

  cm_svm_block_write(test_s, test_v);
  cm_svm_block_write(etalon_s, etalon_v);
}
