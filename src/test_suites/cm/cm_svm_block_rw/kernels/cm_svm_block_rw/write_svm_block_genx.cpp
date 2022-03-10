/*
 * Copyright (C) 2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

using type_t = TYPE;

extern "C" _GENX_MAIN_ void kernel(unsigned offset,
                                   svmptr_t out_ptr [[type("svmptr_t")]]) {
  constexpr int simd = SIMD;
  vector<type_t, simd> data = {PASSED_VECTOR};
  cm_svm_block_write(out_ptr + offset, data);
}