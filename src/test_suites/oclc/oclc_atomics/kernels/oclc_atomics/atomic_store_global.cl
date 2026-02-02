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

kernel void test_kernel(const global DATA_TYPE *input,
                        global ATOMIC_TYPE *output) {
  const size_t global_id = get_global_id(0);

#if defined(MEMORY_SCOPE) && defined(MEMORY_ORDER)
  atomic_store_explicit(ATOMIC_ADDRESS_SPACE_CAST(&output[global_id]),
                        input[global_id], MEMORY_ORDER, MEMORY_SCOPE);
#elif defined(MEMORY_ORDER)
  atomic_store_explicit(ATOMIC_ADDRESS_SPACE_CAST(&output[global_id]),
                        input[global_id], MEMORY_ORDER);
#else
  atomic_store(ATOMIC_ADDRESS_SPACE_CAST(&output[global_id]), input[global_id]);
#endif
}
