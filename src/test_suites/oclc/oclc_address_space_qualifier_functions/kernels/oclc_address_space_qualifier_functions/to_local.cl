/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

local DATA_TYPE *to_local_wrapper(DATA_TYPE *input) { return to_local(input); }
const local DATA_TYPE *to_local_const_wrapper(const DATA_TYPE *input) {
  return to_local(input);
}

kernel void test_non_const(global uintptr_t *output) {
  const size_t id = get_global_id(0);
  local DATA_TYPE input;
  output[id] = (uintptr_t)to_local_wrapper(&input);
}
kernel void test_const(global uintptr_t *output) {
  const size_t id = get_global_id(0);
  const local DATA_TYPE input;
  output[id] = (uintptr_t)to_local_const_wrapper(&input);
}
