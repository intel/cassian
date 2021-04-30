/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

kernel void test_global_to_local(const global DATA_TYPE *input,
                                 const int elements_per_work_item,
                                 local DATA_TYPE *temp,
                                 global DATA_TYPE *output) {
  const size_t group_id = get_group_id(0);
  const size_t local_size = get_local_size(0);
  const size_t elements_per_work_group = local_size * elements_per_work_item;
  const size_t group_offset = group_id * elements_per_work_group;
  event_t event = async_work_group_copy(temp, input + group_offset,
                                        elements_per_work_group, 0);
  wait_group_events(1, &event);

  const size_t global_id = get_global_id(0);
  const size_t local_id = get_local_id(0);
  const size_t global_offset = global_id * elements_per_work_item;
  const size_t local_offset = local_id * elements_per_work_item;
  for (int i = 0; i < elements_per_work_item; ++i) {
    output[global_offset + i] = temp[local_offset + i];
  }
};

kernel void test_local_to_global(const global DATA_TYPE *input,
                                 const int elements_per_work_item,
                                 local DATA_TYPE *temp,
                                 global DATA_TYPE *output) {
  const size_t global_id = get_global_id(0);
  const size_t local_id = get_local_id(0);
  const size_t global_offset = global_id * elements_per_work_item;
  const size_t local_offset = local_id * elements_per_work_item;
  for (int i = 0; i < elements_per_work_item; ++i) {
    temp[local_offset + i] = input[global_offset + i];
  }

  const size_t group_id = get_group_id(0);
  const size_t local_size = get_local_size(0);
  const size_t elements_per_work_group = local_size * elements_per_work_item;
  const size_t group_offset = group_id * elements_per_work_group;
  event_t event = async_work_group_copy(output + group_offset, temp,
                                        elements_per_work_group, 0);
  wait_group_events(1, &event);
};

kernel void test_global_to_local_to_global(const global DATA_TYPE *input,
                                           const int elements_per_work_item,
                                           local DATA_TYPE *temp,
                                           global DATA_TYPE *output) {
  const size_t group_id = get_group_id(0);
  const size_t local_size = get_local_size(0);
  const size_t elements_per_work_group = local_size * elements_per_work_item;
  const size_t group_offset = group_id * elements_per_work_group;
  event_t event = 0;
  event = async_work_group_copy(temp, input + group_offset,
                                elements_per_work_group, event);
  event = async_work_group_copy(output + group_offset, temp,
                                elements_per_work_group, event);
  wait_group_events(1, &event);
};
