/*
 * Copyright (C) 2021-2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_RUNTIME_MOCKS_STUB_RUNTIME_HPP
#define CASSIAN_RUNTIME_MOCKS_STUB_RUNTIME_HPP

#include <cassian/runtime/feature.hpp>
#include <cassian/runtime/mocks/dummy_runtime.hpp>

namespace cassian {

/**
 * Configurable implementation of Runtime interface.
 */
class StubRuntime : public DummyRuntime {
public:
  /**
   * Specify if fp16 feature should be reported as supported.
   */
  bool is_fp16_supported_ = true;

  /**
   * Specify if fp64 feature should be reported as supported.
   */
  bool is_fp64_supported_ = true;

  /**
   * Specify if int64_atomics feature should be reported as supported.
   */
  bool is_int64_atomics_supported_ = true;

  /**
   * Specify if simd8 feature should be reported as supported.
   */
  bool is_simd8_supported_ = true;

  /**
   * Specify if simd16 feature should be reported as supported.
   */
  bool is_simd16_supported_ = true;

  /**
   * Specify if simd32 feature should be reported as supported.
   */
  bool is_simd32_supported_ = true;

  /**
   * Specify max group size X.
   */
  int max_group_size_x_ = 1024;

  /**
   * Specify max group size Y.
   */
  int max_group_size_y_ = 1024;

  /**
   * Specify max group size Z.
   */
  int max_group_size_z_ = 1024;

  bool is_feature_supported(Feature feature) const override;

  int get_device_property(const DeviceProperty property) const override;
};

} // namespace cassian

#endif