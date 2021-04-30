/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

kernel void test_kernel(const global DATA_TYPE_IN *input,
                        global DATA_TYPE_OUT *output) {
  DATA_TYPE_IN v = *input;
  DATA_TYPE_OUT i = (DATA_TYPE_IN)v;
  *output = i;
};
