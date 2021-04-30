/*
 * Copyright (C) 2021 Intel Corporation
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
  default:
    return false;
  }
}

} // namespace cassian
