/*
 * Copyright (C) 2021-2026 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

// TODO: Move it to the host
#pragma OPENCL EXTENSION cl_khr_int64_base_atomics : enable
#pragma OPENCL EXTENSION cl_khr_int64_extended_atomics : enable
#pragma OPENCL EXTENSION cl_khr_fp64 : enable
#pragma OPENCL EXTENSION cl_khr_fp16 : enable

kernel void test_kernel(global ATOMIC_TYPE *value,
                        const global DATA_TYPE *operand,
                        global DATA_TYPE *fetched) {
  const size_t global_id = get_global_id(0);

#if defined(MEMORY_SCOPE) && defined(MEMORY_ORDER)
  fetched[global_id] =
      atomic_exchange_explicit(ATOMIC_ADDRESS_SPACE_CAST(&value[global_id]),
                               operand[global_id], MEMORY_ORDER, MEMORY_SCOPE);
#elif defined(MEMORY_ORDER)
  fetched[global_id] =
      atomic_exchange_explicit(ATOMIC_ADDRESS_SPACE_CAST(&value[global_id]),
                               operand[global_id], MEMORY_ORDER);
#else
  fetched[global_id] = atomic_exchange(
      ATOMIC_ADDRESS_SPACE_CAST(&value[global_id]), operand[global_id]);
#endif
}