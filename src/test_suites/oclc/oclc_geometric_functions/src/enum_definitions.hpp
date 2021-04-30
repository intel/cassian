/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_OCLC_GEOMETRIC_FUNCTIONS_ENUM_DEFINITIONS_HPP
#define CASSIAN_OCLC_GEOMETRIC_FUNCTIONS_ENUM_DEFINITIONS_HPP

#include <catch2/catch.hpp>

// Name of enum has to be exact as oclc function

enum class Function {
  cross,
  dot,
  distance,
  length,
  normalize,
  fast_distance,
  fast_length,
  fast_normalize
};

#define CASSIAN_CATCH_REGISTER_GEOMETRIC_FUNCTIONS                             \
  CATCH_REGISTER_ENUM(Function, Function::cross, Function::dot,                \
                      Function::distance, Function::length,                    \
                      Function::normalize, Function::fast_distance,            \
                      Function::fast_length, Function::fast_normalize)

#endif