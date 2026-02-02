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

kernel void test_kernel(global atomic_flag *value) {
  const size_t global_id = get_global_id(0);

#if defined(MEMORY_SCOPE) && defined(MEMORY_ORDER)
  atomic_flag_clear_explicit(ATOMIC_ADDRESS_SPACE_CAST(&value[global_id]),
                             MEMORY_ORDER, MEMORY_SCOPE);
#elif defined(MEMORY_ORDER)
  atomic_flag_clear_explicit(ATOMIC_ADDRESS_SPACE_CAST(&value[global_id]),
                             MEMORY_ORDER);
#else
  atomic_flag_clear(ATOMIC_ADDRESS_SPACE_CAST(&value[global_id]));
#endif
}