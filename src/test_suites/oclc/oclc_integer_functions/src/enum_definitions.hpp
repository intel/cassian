/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_OCLC_INTEGER_FUNCTIONS_ENUM_DEFINITIONS_HPP
#define CASSIAN_OCLC_INTEGER_FUNCTIONS_ENUM_DEFINITIONS_HPP

#include <catch2/catch.hpp>

// Name of enums has to be exact as oclc function

enum class Function {
  abs,
  abs_diff,
  add_sat,
  hadd,
  rhadd,
  clamp,
  clz,
  ctz,
  mad_hi,
  mad_sat,
  max,
  min,
  mul_hi,
  rotate,
  sub_sat,
  upsample,
  popcount,
  mad24,
  mul24
};

enum class IntegerMacros {
  char_bit,
  char_max,
  char_min,
  int_max,
  int_min,
  long_max,
  long_min,
  schar_max,
  schar_min,
  shrt_max,
  shrt_min,
  uchar_max,
  ushrt_max,
  uint_max,
  ulong_max
};

#define CASSIAN_CATCH_REGISTER_INTEGER_MACROS                                  \
  CATCH_REGISTER_ENUM(IntegerMacros, IntegerMacros::char_bit,                  \
                      IntegerMacros::char_max, IntegerMacros::char_min,        \
                      IntegerMacros::int_max, IntegerMacros::int_min,          \
                      IntegerMacros::long_max, IntegerMacros::long_min,        \
                      IntegerMacros::schar_max, IntegerMacros::schar_min,      \
                      IntegerMacros::shrt_max, IntegerMacros::shrt_min,        \
                      IntegerMacros::uchar_max, IntegerMacros::ushrt_max,      \
                      IntegerMacros::uint_max, IntegerMacros::ulong_max)

#define CASSIAN_CATCH_REGISTER_INTEGER_FUNCTIONS                               \
  CATCH_REGISTER_ENUM(Function, Function::abs, Function::abs_diff,             \
                      Function::add_sat, Function::hadd, Function::rhadd,      \
                      Function::clamp, Function::clz, Function::ctz,           \
                      Function::mad_hi, Function::mad_sat, Function::max,      \
                      Function::min, Function::mul_hi, Function::rotate,       \
                      Function::sub_sat, Function::upsample,                   \
                      Function::popcount, Function::mad24, Function::mul24)

#endif