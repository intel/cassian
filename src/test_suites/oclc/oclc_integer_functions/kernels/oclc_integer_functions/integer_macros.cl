/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

kernel void integer_macros(global TYPE *output) {
  const size_t tid = get_global_id(0);
#if defined(CHAR_BIT_MACRO)
  output[tid] = CHAR_BIT;
#elif defined(CHAR_MAX_MACRO)
  output[tid] = CHAR_MAX;
#elif defined(CHAR_MIN_MACRO)
  output[tid] = CHAR_MIN;
#elif defined(INT_MAX_MACRO)
  output[tid] = INT_MAX;
#elif defined(INT_MIN_MACRO)
  output[tid] = INT_MIN;
#elif defined(LONG_MAX_MACRO)
  output[tid] = LONG_MAX;
#elif defined(LONG_MIN_MACRO)
  output[tid] = LONG_MIN;
#elif defined(SCHAR_MAX_MACRO)
  output[tid] = SCHAR_MAX;
#elif defined(SCHAR_MIN_MACRO)
  output[tid] = SCHAR_MIN;
#elif defined(SHRT_MAX_MACRO)
  output[tid] = SHRT_MAX;
#elif defined(SHRT_MIN_MACRO)
  output[tid] = SHRT_MIN;
#elif defined(UCHAR_MAX_MACRO)
  output[tid] = UCHAR_MAX;
#elif defined(USHRT_MAX_MACRO)
  output[tid] = USHRT_MAX;
#elif defined(UINT_MAX_MACRO)
  output[tid] = UINT_MAX;
#elif defined(ULONG_MAX_MACRO)
  output[tid] = ULONG_MAX;
#else
  output[tid] = NULL;
#endif
}