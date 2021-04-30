/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

global DATA_TYPE *to_global_wrapper(DATA_TYPE *input) {
  return to_global(input);
}
const global DATA_TYPE *to_global_const_wrapper(const DATA_TYPE *input) {
  return to_global(input);
}

kernel void test_non_const(global DATA_TYPE *input, global uintptr_t *output) {
  const size_t id = get_global_id(0);
  output[id] = (uintptr_t)to_global_wrapper(input);
}
kernel void test_const(const global DATA_TYPE *input,
                       global uintptr_t *output) {
  const size_t id = get_global_id(0);
  output[id] = (uintptr_t)to_global_const_wrapper(input);
}
