/*
 * Copyright (C) 2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_RUNTIME_PROPERTY_CHECKS_HPP
#define CASSIAN_RUNTIME_PROPERTY_CHECKS_HPP

#include <cassian/runtime/runtime.hpp>
#include <string>

/**
 * Cassian namespace.
 */
namespace cassian {

class PropertyCheck {
public:
  virtual ~PropertyCheck() = default;
  virtual bool check(const Runtime &runtime) = 0;
  virtual std::string to_string() = 0;
};

class MinWorkGroupSize : public PropertyCheck {
  int x, y, z;

public:
  MinWorkGroupSize(int x, int y, int z);
  bool check(const Runtime &runtime) override;
  std::string to_string() override;
};

/**
 * Convert PropertyCheck to string representation.
 *
 * @param[in] property_check object to convert.
 */
std::string to_string(PropertyCheck &property_check);

} // namespace cassian

#endif
