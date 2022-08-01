/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_OCLC_WORK_ITEM_FUNCTIONS_COMMON_HPP
#define CASSIAN_OCLC_WORK_ITEM_FUNCTIONS_COMMON_HPP

#include <cassian/runtime/runtime.hpp>
#include <common.hpp>
#include <string>

namespace ca = cassian;

int suggest_work_size(const std::string &type);

template <size_t N>
std::array<size_t, N> get_max_group_size(ca::Runtime *runtime) {
  std::array<size_t, N> max_group_size = {};
  static_assert(N <= 3, "It is not supported for more than 3 dimensions");
  if constexpr (N >= 1) {
    max_group_size[0] = static_cast<size_t>(
        runtime->get_device_property(ca::DeviceProperty::max_group_size_x));
  }
  if constexpr (N >= 2) {
    max_group_size[1] = static_cast<size_t>(
        runtime->get_device_property(ca::DeviceProperty::max_group_size_y));
  }
  if constexpr (N == 3) {
    max_group_size[2] = static_cast<size_t>(
        runtime->get_device_property(ca::DeviceProperty::max_group_size_z));
  }
  return max_group_size;
}

#endif
