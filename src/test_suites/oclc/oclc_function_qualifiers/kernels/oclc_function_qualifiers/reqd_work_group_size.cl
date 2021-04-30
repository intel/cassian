/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

typedef struct {
  uint local_size_x;
  uint local_size_y;
  uint local_size_z;
} work_size_data;

kernel __attribute__((reqd_work_group_size(X, Y, Z))) void
attr_reqd_work_group_size(global work_size_data *data) {
  int tid = get_global_id(2) * get_global_size(0) * get_global_size(1) +
            get_global_id(1) * get_global_size(0) + get_global_id(0);
  data[tid].local_size_x = get_local_size(0);
  data[tid].local_size_y = get_local_size(1);
  data[tid].local_size_z = get_local_size(2);
};