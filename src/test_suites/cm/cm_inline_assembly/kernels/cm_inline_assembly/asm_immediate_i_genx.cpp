/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

extern "C" _GENX_MAIN_ void //
test(SurfaceIndex test_surface [[type("buffer_t")]],
     SurfaceIndex etalon_surface [[type("buffer_t")]]) {
  vector<long long, 8> test_vector;
  vector<long long, 8> etalon_vector;

  const long long one = 1;

  const char char_const = one;
  char v_char;
  asm("mov (M1, 1) %0 %1" : "=r"(v_char) : "i"(char_const));
  etalon_vector(0) = char_const;
  test_vector(0) = v_char;

  const unsigned char uchar_const = one;
  unsigned char v_uchar;
  asm("mov (M1, 1) %0 %1" : "=r"(v_uchar) : "i"(uchar_const));
  etalon_vector(1) = uchar_const;
  test_vector(1) = v_uchar;

  const short short_const = one << (sizeof(char) * 8);
  short v_short;
  asm("mov (M1, 1) %0 %1" : "=r"(v_short) : "i"(short_const));
  etalon_vector(2) = short_const;
  test_vector(2) = v_short;

  const unsigned short ushort_const = one << (sizeof(char) * 8);
  unsigned short v_ushort;
  asm("mov (M1, 1) %0 %1" : "=r"(v_ushort) : "i"(ushort_const));
  etalon_vector(3) = ushort_const;
  test_vector(3) = v_ushort;

  const int int_const = one << (sizeof(short) * 8);
  int v_int;
  asm("mov (M1, 1) %0 %1" : "=r"(v_int) : "i"(int_const));
  etalon_vector(4) = int_const;
  test_vector(4) = v_int;

  const unsigned int uint_const = one << (sizeof(short) * 8);
  unsigned int v_uint;
  asm("mov (M1, 1) %0 %1" : "=r"(v_uint) : "i"(uint_const));
  etalon_vector(5) = uint_const;
  test_vector(5) = v_uint;

  const long long long_const = one << (sizeof(int) * 8);
  long long v_long;
  asm("mov (M1, 1) %0 %1" : "=r"(v_long) : "i"(long_const));
  etalon_vector(6) = long_const;
  test_vector(6) = v_long;

  const unsigned long long ulong_const = one << (sizeof(int) * 8);
  unsigned long long v_ulong;
  asm("mov (M1, 1) %0 %1" : "=r"(v_ulong) : "i"(ulong_const));
  etalon_vector(7) = ulong_const;
  test_vector(7) = v_ulong;

  write(etalon_surface, 0, etalon_vector);
  write(test_surface, 0, test_vector);
}
