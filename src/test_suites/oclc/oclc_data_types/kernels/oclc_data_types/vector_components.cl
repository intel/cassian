/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

kernel void test_kernel(global COMPONENTS_DATA_TYPE *output) {
  DATA_TYPE i = VALUE;
  *output = i.COMPONENTS;
};
