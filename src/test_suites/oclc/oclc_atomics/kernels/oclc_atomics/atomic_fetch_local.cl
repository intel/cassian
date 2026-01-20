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
test_kernel(global DATA_TYPE *value, const global OPERAND_TYPE *operand,
            global DATA_TYPE *fetched) {
  const size_t global_id = get_global_id(0);
  const size_t local_id = get_local_id(0);

  local ATOMIC_TYPE local_atomic[WORK_GROUP_SIZE];
  atomic_store_explicit(&local_atomic[local_id], value[global_id],
                        memory_order_relaxed, memory_scope_work_group);

#if defined(MEMORY_SCOPE) && defined(MEMORY_ORDER)
  fetched[global_id] =
      FUNCTION(ATOMIC_ADDRESS_SPACE_CAST(&local_atomic[local_id]),
               operand[global_id], MEMORY_ORDER, MEMORY_SCOPE);
#elif defined(MEMORY_ORDER)
  fetched[global_id] =
      FUNCTION(ATOMIC_ADDRESS_SPACE_CAST(&local_atomic[local_id]),
               operand[global_id], MEMORY_ORDER);
#else
  fetched[global_id] = FUNCTION(
      ATOMIC_ADDRESS_SPACE_CAST(&local_atomic[local_id]), operand[global_id]);
#endif

  value[global_id] = atomic_load_explicit(
      &local_atomic[local_id], memory_order_relaxed, memory_scope_work_group);
}
