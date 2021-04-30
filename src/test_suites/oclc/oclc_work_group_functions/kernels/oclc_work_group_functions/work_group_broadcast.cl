/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

kernel void test_kernel_1(const global DATA_TYPE *input_value,
                          const global size_t *input_id,
                          global DATA_TYPE *output) {
  const size_t tid = get_global_id(0);
  const size_t gid = get_group_id(0);
  output[tid] = work_group_broadcast(input_value[tid], input_id[gid]);
};

kernel void test_kernel_2(const global DATA_TYPE *input_value,
                          const global size_t *input_id_x,
                          const global size_t *input_id_y,
                          global DATA_TYPE *output) {
  const int tid = get_global_id(1) * get_global_size(0) + get_global_id(0);
  const int gid_x = get_group_id(0);
  const int gid_y = get_group_id(1);
  output[tid] = work_group_broadcast(input_value[tid], input_id_x[gid_x],
                                     input_id_y[gid_y]);
};

kernel void test_kernel_3(const global DATA_TYPE *input_value,
                          const global size_t *input_id_x,
                          const global size_t *input_id_y,
                          const global size_t *input_id_z,
                          global DATA_TYPE *output) {
  const size_t tid =
      get_global_id(2) * get_global_size(1) * get_global_size(0) +
      get_global_id(1) * get_global_size(0) + get_global_id(0);
  const size_t gid_x = get_group_id(0);
  const size_t gid_y = get_group_id(1);
  const size_t gid_z = get_group_id(2);
  output[tid] = work_group_broadcast(input_value[tid], input_id_x[gid_x],
                                     input_id_y[gid_y], input_id_z[gid_z]);
};
