/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_OCLC_COMMON_FUNCTIONS_ENUM_DEFINITIONS_HPP
#define CASSIAN_OCLC_COMMON_FUNCTIONS_ENUM_DEFINITIONS_HPP

#include <catch2/catch.hpp>

// Name of enum has to be exact as oclc function

enum class Function {
  clamp,
  degrees,
  max,
  min,
  mix,
  radians,
  step,
  smoothstep,
  sign
};

#define CASSIAN_CATCH_REGISTER_COMMON_FUNCTIONS                                \
  CATCH_REGISTER_ENUM(Function, Function::clamp, Function::degrees,            \
                      Function::max, Function::min, Function::mix,             \
                      Function::radians, Function::step, Function::smoothstep, \
                      Function::sign)

#endif