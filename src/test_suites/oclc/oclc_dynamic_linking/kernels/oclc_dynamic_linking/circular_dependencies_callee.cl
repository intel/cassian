/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

extern int add_impl(int, int);

extern int add(int a, int b) { return add_impl(a, b); }
