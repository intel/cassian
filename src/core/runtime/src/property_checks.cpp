/*
 * Copyright (C) 2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/runtime/openclc_utils.hpp>
#include <cassian/runtime/property_checks.hpp>
#include <sstream>

/**
 * Cassian namespace.
 */
namespace cassian {

std::string to_string(PropertyCheck &property_check) {
  return property_check.to_string();
}

MinWorkGroupSize::MinWorkGroupSize(int x, int y, int z) : x(x), y(y), z(z) {}

bool MinWorkGroupSize::check(Runtime &runtime) {
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

OpenclcFeature::OpenclcFeature(std::string program_type, std::string feature)
    : program_type(std::move(program_type)), feature(std::move(feature)) {}

bool OpenclcFeature::check(Runtime &runtime) {
  return check_optional_openclc_feature_support(&runtime, program_type,
                                                feature);
};

std::string OpenclcFeature::to_string() {
  std::ostringstream oss;
  oss << "OpenCL C feature " << feature;
  return oss.str();
};

} // namespace cassian
