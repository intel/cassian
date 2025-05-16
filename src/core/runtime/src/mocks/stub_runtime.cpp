/*
 * Copyright (C) 2021-2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/runtime/feature.hpp>
#include <cassian/runtime/mocks/stub_runtime.hpp>

namespace cassian {

bool StubRuntime::is_feature_supported(const Feature feature) const {
  switch (feature) {
  case Feature::fp16:
    return is_fp16_supported_;
  case Feature::fp64:
    return is_fp64_supported_;
  case Feature::int64_atomics:
    return is_int64_atomics_supported_;
  case Feature::simd8:
    return is_simd8_supported_;
  case Feature::simd16:
    return is_simd16_supported_;
  case Feature::simd32:
    return is_simd32_supported_;
  default:
    return false;
  }
}

int StubRuntime::get_device_property(const DeviceProperty property) const {
  switch (property) {
  case DeviceProperty::max_group_size_x:
    return max_group_size_x_;
  case DeviceProperty::max_group_size_y:
    return max_group_size_y_;
  case DeviceProperty::max_group_size_z:
    return max_group_size_z_;
  default:
    throw RuntimeException("Failed to find device property");
  }
}

Kernel StubRuntime::create_kernel(
    const std::string & /*kernel_name*/, const std::string & /*source*/,
    const std::string & /*build_options*/, const std::string & /*program_type*/,
    const std::optional<std::string> & /*spirv_options*/, bool /*quiet*/) {
  if (!create_kernel_success_) {
    throw RuntimeException("Failed to create kernel");
  }
  return Kernel();
}

} // namespace cassian
