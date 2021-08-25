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
#pragma OPENCL EXTENSION cl_intel_spirv_subgroups : enable

kernel void test_kernel(global ATOMIC_TYPE *value, global DATA_TYPE *expected,
                        const global DATA_TYPE *desired, global bool *result) {
  const size_t global_id = get_global_id(0);

#if COMPARISON_TYPE == weak
#if defined(FAILURE_MEMORY_ORDER) && defined(SUCCESS_MEMORY_ORDER) &&          \
    defined(MEMORY_SCOPE)
  result[global_id] = atomic_compare_exchange_weak_explicit(
      &value[global_id], &expected[global_id], desired[global_id],
      SUCCESS_MEMORY_ORDER, FAILURE_MEMORY_ORDER, MEMORY_SCOPE);

#elif defined(FAILURE_MEMORY_ORDER) && defined(SUCCESS_MEMORY_ORDER)
  result[global_id] = atomic_compare_exchange_weak_explicit(
      &value[global_id], &expected[global_id], desired[global_id],
      SUCCESS_MEMORY_ORDER, FAILURE_MEMORY_ORDER);
#else
  result[global_id] = atomic_compare_exchange_weak(
      &value[global_id], &expected[global_id], desired[global_id]);
#endif
#elif COMPARISON_TYPE == strong
#if defined(FAILURE_MEMORY_ORDER) && defined(SUCCESS_MEMORY_ORDER) &&          \
    defined(MEMORY_SCOPE)
  result[global_id] = atomic_compare_exchange_strong_explicit(
      &value[global_id], &expected[global_id], desired[global_id],
      SUCCESS_MEMORY_ORDER, FAILURE_MEMORY_ORDER, MEMORY_SCOPE);

#elif defined(FAILURE_MEMORY_ORDER) && defined(SUCCESS_MEMORY_ORDER)
  result[global_id] = atomic_compare_exchange_strong_explicit(
      &value[global_id], &expected[global_id], desired[global_id],
      SUCCESS_MEMORY_ORDER, FAILURE_MEMORY_ORDER);
#else
  result[global_id] = atomic_compare_exchange_strong(
      &value[global_id], &expected[global_id], desired[global_id]);
#endif
#endif
}