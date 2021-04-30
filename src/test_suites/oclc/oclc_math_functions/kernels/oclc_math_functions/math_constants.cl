/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#pragma OPENCL EXTENSION cl_khr_fp64 : enable

kernel void math_constants(global TYPE *output) {
  const size_t tid = get_global_id(0);
#if defined(M_E_F_MACRO)
  output[tid] = M_E_F;
#elif defined(M_LOG2E_F_MACRO)
  output[tid] = M_LOG2E_F;
#elif defined(M_LOG10E_F_MACRO)
  output[tid] = M_LOG10E_F;
#elif defined(FLT_MAX_EXP_MACRO)
  output[tid] = FLT_MAX_EXP;
#elif defined(M_LN2_F_MACRO)
  output[tid] = M_LN2_F;
#elif defined(M_LN10_F_MACRO)
  output[tid] = M_LN10_F;
#elif defined(M_PI_F_MACRO)
  output[tid] = M_PI_F;
#elif defined(M_PI_2_F_MACRO)
  output[tid] = M_PI_2_F;
#elif defined(M_PI_4_F_MACRO)
  output[tid] = M_PI_4_F;
#elif defined(M_1_PI_F_MACRO)
  output[tid] = M_1_PI_F;
#elif defined(M_2_PI_F_MACRO)
  output[tid] = M_2_PI_F;
#elif defined(M_2_SQRTPI_F_MACRO)
  output[tid] = M_2_SQRTPI_F;
#elif defined(M_SQRT2_F_MACRO)
  output[tid] = M_SQRT2_F;
#elif defined(M_SQRT1_2_F_MACRO)
  output[tid] = M_SQRT1_2_F;
#elif defined(M_E_MACRO)
  output[tid] = M_E;
#elif defined(M_LOG2E_MACRO)
  output[tid] = M_LOG2E;
#elif defined(M_LOG10E_MACRO)
  output[tid] = M_LOG10E;
#elif defined(FLT_MAX_EXP_MACRO)
  output[tid] = FLT_MAX_EXP;
#elif defined(M_LN2_MACRO)
  output[tid] = M_LN2;
#elif defined(M_LN10_MACRO)
  output[tid] = M_LN10;
#elif defined(M_PI_MACRO)
  output[tid] = M_PI;
#elif defined(M_PI_2_MACRO)
  output[tid] = M_PI_2;
#elif defined(M_PI_4_MACRO)
  output[tid] = M_PI_4;
#elif defined(M_1_PI_MACRO)
  output[tid] = M_1_PI;
#elif defined(M_2_PI_MACRO)
  output[tid] = M_2_PI;
#elif defined(M_2_SQRTPI_MACRO)
  output[tid] = M_2_SQRTPI;
#elif defined(M_SQRT2_MACRO)
  output[tid] = M_SQRT2;
#elif defined(M_SQRT1_2_MACRO)
  output[tid] = M_SQRT1_2;
#elif defined(MAXFLOAT_MACRO)
  output[tid] = MAXFLOAT;
#elif defined(HUGE_VALF_MACRO)
  output[tid] = HUGE_VALF;
#elif defined(INFINITY_MACRO)
  output[tid] = INFINITY;
#elif defined(NAN_MACRO)
  output[tid] = NAN;
#elif defined(HUGE_VAL_MACRO)
  output[tid] = HUGE_VAL;
#else
  output[tid] = NULL;
#endif
}