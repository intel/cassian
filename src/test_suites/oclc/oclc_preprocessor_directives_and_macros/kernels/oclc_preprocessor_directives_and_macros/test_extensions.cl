/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

void kernel test_extensions() {
#if defined(cl_khr_int64_extended_atomics_MACRO) &&                            \
    defined(cl_khr_int64_base_atomics_MACRO)
  atomic_long a;
#endif
}