/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cm/cm.h>

template <typename T, unsigned EXEC_SIZE>
inline _GENX_ vector<T, EXEC_SIZE> madd(vector<T, EXEC_SIZE> s1,
                                        vector<T, EXEC_SIZE> s2,
                                        vector<T, EXEC_SIZE> s3) {
  vector<T, EXEC_SIZE> dst;

  asm("mad (M1, %4) %0 %1 %2 %3"
      : "=r"(dst)
      : "r"(s1), "r"(s2), "r"(s3), "n"(EXEC_SIZE));

  return dst;
}

_GENX_MAIN_ void test(SurfaceIndex src0 [[type("buffer_t")]],
                      SurfaceIndex src1 [[type("buffer_t")]],
                      SurfaceIndex src2 [[type("buffer_t")]],
                      SurfaceIndex dst [[type("buffer_t")]]) {

  vector<ushort, 8> s1, s2, s3;

  read(src0, 0, s1);
  read(src1, 0, s2);
  read(src2, 0, s3);

  vector<ushort, 8> d = madd(s1, s2, s3);

  write(dst, 0, d);
}
