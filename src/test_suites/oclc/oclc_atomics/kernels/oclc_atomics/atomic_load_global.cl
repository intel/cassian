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

kernel void test_kernel(global ATOMIC_TYPE *input, global DATA_TYPE *output) {
  const size_t global_id = get_global_id(0);

#if defined(MEMORY_SCOPE) && defined(MEMORY_ORDER)
  output[global_id] =
      atomic_load_explicit(&input[global_id], MEMORY_ORDER, MEMORY_SCOPE);
#elif defined(MEMORY_ORDER)
  output[global_id] = atomic_load_explicit(&input[global_id], MEMORY_ORDER);
#else
  output[global_id] = atomic_load(&input[global_id]);
#endif
}
