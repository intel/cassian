/*
 * Copyright (C) 2021 Intel Corporation
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

  bool is_feature_supported(Feature feature) const override;
};

} // namespace cassian

#endif