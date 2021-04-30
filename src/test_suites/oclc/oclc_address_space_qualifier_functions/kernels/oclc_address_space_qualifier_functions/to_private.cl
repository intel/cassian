/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

private
DATA_TYPE *to_private_wrapper(DATA_TYPE *input) { return to_private(input); }
const private DATA_TYPE *to_private_const_wrapper(const DATA_TYPE *input) {
  return to_private(input);
}

kernel void test_non_const(global uintptr_t *output) {
  const size_t id = get_global_id(0);
private
  DATA_TYPE input;
  output[id] = (uintptr_t)to_private_wrapper(&input);
}
kernel void test_const(global uintptr_t *output) {
  const size_t id = get_global_id(0);
  const private DATA_TYPE input;
  output[id] = (uintptr_t)to_private_const_wrapper(&input);
}
