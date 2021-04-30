/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/runtime/runtime.hpp>
#include <common.hpp>

int get_local_work_size(const int global_work_size,
                        const cassian::Runtime &runtime) {
  const int max_size = runtime.get_device_property(
      cassian::DeviceProperty::max_total_group_size);
  return global_work_size > max_size ? max_size : global_work_size;
}

int suggest_work_size(const std::string &type) {
  const int default_size = 16;
  const int min_size = 1;
  const int max_size = 1024;
  if (type == "min") {
    return min_size;
  }
  if (type == "max") {
    return max_size;
  }
  return default_size;
}
