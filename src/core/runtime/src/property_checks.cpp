/*
 * Copyright (C) 2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/runtime/property_checks.hpp>
#include <sstream>

/**
 * Cassian namespace.
 */
namespace cassian {

std::string to_string(PropertyCheck &property_check) {
  return property_check.to_string();
}

MinWorkGroupSize::MinWorkGroupSize(int x, int y, int z) : x(x), y(y), z(z){};

bool MinWorkGroupSize::check(const Runtime &runtime) {
  return (
      (runtime.get_device_property(DeviceProperty::max_group_size_x) >= x) &&
      (runtime.get_device_property(DeviceProperty::max_group_size_y) >= y) &&
      (runtime.get_device_property(DeviceProperty::max_group_size_z) >= z));
};

std::string MinWorkGroupSize::to_string() {
  std::ostringstream oss;
  oss << "minimum workgroup size (" << x << ", " << y << ", " << z << ")";
  return oss.str();
};
} // namespace cassian
