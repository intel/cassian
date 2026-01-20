/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

// TODO: Move it to the host
#pragma OPENCL EXTENSION cl_khr_int64_base_atomics : enable
#pragma OPENCL EXTENSION cl_khr_int64_extended_atomics : enable
#pragma OPENCL EXTENSION cl_khr_fp64 : enable

__attribute__((reqd_work_group_size(WORK_GROUP_SIZE, 1, 1))) kernel void
test_kernel(global DATA_TYPE *value, global int *fetched) {
  const size_t global_id = get_global_id(0);
  const size_t local_id = get_local_id(0);

  local atomic_flag local_flag[WORK_GROUP_SIZE];
  atomic_flag_clear(&local_flag[local_id]);

#if defined(MEMORY_SCOPE) && defined(MEMORY_ORDER)
  fetched[global_id] = (int)atomic_flag_test_and_set_explicit(
      ATOMIC_ADDRESS_SPACE_CAST(&local_flag[local_id]), MEMORY_ORDER,
      MEMORY_SCOPE);
#elif defined(MEMORY_ORDER)
  fetched[global_id] = (int)atomic_flag_test_and_set_explicit(
      ATOMIC_ADDRESS_SPACE_CAST(&local_flag[local_id]), MEMORY_ORDER);
#else
  fetched[global_id] = (int)atomic_flag_test_and_set(
      ATOMIC_ADDRESS_SPACE_CAST(&local_flag[local_id]));
#endif

  value[global_id] = atomic_load_explicit(
      &local_flag[local_id], memory_order_relaxed, memory_scope_work_group);
}
