/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/fp_types/half.hpp>
#include <cassian/logging/logging.hpp>
#include <cassian/runtime/feature.hpp>
#include <cassian/runtime/openclc_types.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cassian/test_harness/test_harness.hpp>
#include <cstddef>
#include <string>
#include <vector>

namespace cassian {

std::string Requirements::check(const Runtime &runtime) const {
  std::string reason;
  for (const auto feature : features_) {
    if (!runtime.is_feature_supported(feature)) {
      reason = "Feature not supported: " + to_string(feature);
    }
  }
  return reason;
}

void Requirements::feature(Feature f) { features_.push_back(f); }

template <> void Requirements::arithmetic_type<clc_half_t>() {
  features_.push_back(Feature::fp16);
}

template <> void Requirements::arithmetic_type<Half>() {
  arithmetic_type<clc_half_t>();
}

template <> void Requirements::arithmetic_type<clc_double_t>() {
  features_.push_back(Feature::fp64);
}

template <> void Requirements::arithmetic_type<double>() {
  arithmetic_type<clc_double_t>();
}

template <> void Requirements::atomic_type<clc_long_t>() {
  features_.push_back(Feature::int64_atomics);
}

template <> void Requirements::atomic_type<clc_ulong_t>() {
  features_.push_back(Feature::int64_atomics);
}

template <> void Requirements::atomic_type<clc_double_t>() {
  features_.push_back(Feature::int64_atomics);
  features_.push_back(Feature::fp64);
}

template <> void Requirements::atomic_add<clc_half_t>() {
  features_.push_back(Feature::fp16);
  features_.push_back(Feature::fp16_atomics_global_add);
  features_.push_back(Feature::fp16_atomics_local_add);
}

template <> void Requirements::atomic_add<clc_float_t>() {
  features_.push_back(Feature::fp32_atomics_global_add);
  features_.push_back(Feature::fp32_atomics_local_add);
}

template <> void Requirements::atomic_add<clc_double_t>() {
  features_.push_back(Feature::fp64);
  features_.push_back(Feature::fp64_atomics_global_add);
  features_.push_back(Feature::fp64_atomics_local_add);
}

template <> void Requirements::atomic_min_max<clc_half_t>() {
  features_.push_back(Feature::fp16);
  features_.push_back(Feature::fp16_atomics_global_min_max);
  features_.push_back(Feature::fp16_atomics_local_min_max);
}

template <> void Requirements::atomic_min_max<clc_float_t>() {
  features_.push_back(Feature::fp32_atomics_global_min_max);
  features_.push_back(Feature::fp32_atomics_local_min_max);
}

template <> void Requirements::atomic_min_max<clc_double_t>() {
  features_.push_back(Feature::fp64);
  features_.push_back(Feature::fp64_atomics_global_add);
  features_.push_back(Feature::fp64_atomics_local_add);
}

template <> void Requirements::atomic_load_store<clc_half_t>() {
  features_.push_back(Feature::fp16);
  features_.push_back(Feature::fp16_atomics_global_load_store);
  features_.push_back(Feature::fp16_atomics_local_load_store);
}

bool should_skip_test(const Requirements &requirements,
                      const Runtime &runtime) {
  // Catch2 requires at least one assertion to report test case result
  // Visual Studio can't build test_harness as a shared library
  // without CATCH_CONFIG_RUNNER, because Catch2 source is not linked
  // Visual Studio can't build test_harness as a static library
  // with CATCH_CONFIG_RUNNER, because there are duplicated symbols
  // TODO: Check if Catch3 solves this problem

  const std::string reason = requirements.check(runtime);
  if (!reason.empty()) {
    logging::warning() << reason << " - SKIPPED" << '\n';
    return true;
  }
  return false;
}

} // namespace cassian
