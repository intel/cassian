/*
 * Copyright (C) 2024 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

kernel void test_kernel(const global DATA_TYPE_IN *input,
                        global DATA_TYPE_OUT *output) {
  DATA_TYPE_OUT v = *((global DATA_TYPE_OUT *)input);
  *output = v;
};
