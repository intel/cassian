/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#pragma OPENCL EXTENSION cl_khr_fp64 : enable

kernel void math_macros(global TYPE *output) {
  const size_t tid = get_global_id(0);
#if defined(FLT_DIG_MACRO)
  output[tid] = FLT_DIG;
#elif defined(FLT_MANT_DIG_MACRO)
  output[tid] = FLT_MANT_DIG;
#elif defined(FLT_MAX_10_EXP_MACRO)
  output[tid] = FLT_MAX_10_EXP;
#elif defined(FLT_MAX_EXP_MACRO)
  output[tid] = FLT_MAX_EXP;
#elif defined(FLT_MIN_10_EXP_MACRO)
  output[tid] = FLT_MIN_10_EXP;
#elif defined(FLT_MIN_EXP_MACRO)
  output[tid] = FLT_MIN_EXP;
#elif defined(FLT_RADIX_MACRO)
  output[tid] = FLT_RADIX;
#elif defined(FLT_MAX_MACRO)
  output[tid] = FLT_MAX;
#elif defined(FLT_MIN_MACRO)
  output[tid] = FLT_MIN;
#elif defined(FLT_EPSILON_MACRO)
  output[tid] = FLT_EPSILON;
#elif defined(DBL_DIG_MACRO)
  output[tid] = DBL_DIG;
#elif defined(DBL_MANT_DIG_MACRO)
  output[tid] = DBL_MANT_DIG;
#elif defined(DBL_MAX_10_EXP_MACRO)
  output[tid] = DBL_MAX_10_EXP;
#elif defined(DBL_MAX_EXP_MACRO)
  output[tid] = DBL_MAX_EXP;
#elif defined(DBL_MIN_10_EXP_MACRO)
  output[tid] = DBL_MIN_10_EXP;
#elif defined(DBL_MIN_EXP_MACRO)
  output[tid] = DBL_MIN_EXP;
#elif defined(DBL_MAX_MACRO)
  output[tid] = DBL_MAX;
#elif defined(DBL_MIN_MACRO)
  output[tid] = DBL_MIN;
#elif defined(DBL_EPSILON_MACRO)
  output[tid] = DBL_EPSILON;
#else
  output[tid] = NULL;
#endif
}