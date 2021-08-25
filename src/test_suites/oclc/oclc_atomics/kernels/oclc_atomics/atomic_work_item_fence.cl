/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

kernel void test_kernel() {
  atomic_work_item_fence(MEMORY_FLAG, MEMORY_ORDER, MEMORY_SCOPE);
}
