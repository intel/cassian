/*
 * Copyright (C) 2025 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#define CONCAT_IMPL(A, B) A##B
#define CONCAT(A, B) CONCAT_IMPL(A, B)
#define WORK_GROUP_REDUCE CONCAT(work_group_reduce_, OPERATION)

kernel void wg_reduce(const global DATA_TYPE *input, global DATA_TYPE *output) {
  const size_t local_id = get_local_id(0);
  const size_t local_size = get_local_size(0);
  const size_t group_id = get_group_id(0);

  DATA_TYPE res = WORK_GROUP_REDUCE(input[local_id + local_size * group_id]);
  if (local_id == 0) {
    output[group_id] = res;
  }
}
