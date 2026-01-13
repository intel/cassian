/*
 * INTEL CONFIDENTIAL
 *
 * Copyright (C) 2020-2025 Intel Corporation
 *
 * This software and the related documents are Intel copyrighted materials,
 * and your use of them is governed by the express license under which they were
 * provided to you ("License"). Unless the License provides otherwise,
 * you may not use, modify, copy, publish, distribute, disclose or transmit this
 * software or the related documents without Intel's prior written permission.
 *
 * This software and the related documents are provided as is, with no express
 * or implied warranties, other than those that are expressly stated in the
 * License.
 */

kernel void test_basic_bitfield_insert(const global INPUT_TYPE *base,
                                       const global INPUT_TYPE *insert,
                                       global OUTPUT_TYPE *output,
                                       global uint *offset,
                                       global uint *count) {
  int idx = get_global_id(0);
  output[idx] =
      bitfield_insert(base[idx], insert[idx], offset[idx], count[idx]);
}
