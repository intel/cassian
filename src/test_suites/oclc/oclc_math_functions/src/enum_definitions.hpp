/*
 * Copyright (C) 2021-2024 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_OCLC_MATH_FUNCTIONS_ENUM_DEFINITIONS_HPP
#define CASSIAN_OCLC_MATH_FUNCTIONS_ENUM_DEFINITIONS_HPP

#include <catch2/catch.hpp>

// Name of enums has to be exact as oclc function

enum class Function {
  acos,
  acosh,
  acospi,
  asin,
  asinh,
  asinpi,
  atan,
  atan2,
  atanh,
  atanpi,
  atan2pi,
  cbrt,
  ceil,
  copysign,
  cos,
  cosh,
  cospi,
  erfc,
  erf,
  exp,
  exp2,
  exp10,
  expm1,
  fabs,
  fdim,
  floor,
  fma,
  fmax,
  fmin,
  fmod,
  fract,
  frexp,
  hypot,
  ilogb,
  ldexp,
  lgamma,
  lgamma_r,
  log,
  log2,
  log10,
  log1p,
  logb,
  mad,
  maxmag,
  minmag,
  modf,
  nan,
  nextafter,
  pow,
  pown,
  powr,
  remainder,
  remquo,
  rint,
  rootn,
  round,
  rsqrt,
  sincos,
  sin,
  sinh,
  sinpi,
  sqrt,
  tan,
  tanh,
  tanpi,
  tgamma,
  trunc,
  half_cos,
  half_divide,
  half_exp,
  half_exp2,
  half_exp10,
  half_log,
  half_log2,
  half_log10,
  half_powr,
  half_recip,
  half_rsqrt,
  half_sin,
  half_sqrt,
  half_tan,
  native_cos,
  native_divide,
  native_exp,
  native_exp2,
  native_exp10,
  native_log,
  native_log2,
  native_log10,
  native_powr,
  native_recip,
  native_rsqrt,
  native_sin,
  native_sqrt,
  native_tan,
  correctly_rounded_sqrt
};

enum class FloatMacros {
  flt_dig,
  flt_mant_dig,
  flt_max_10_exp,
  flt_max_exp,
  flt_min_exp,
  flt_radix,
  flt_max,
  flt_min,
  flt_epsilon,
  dbl_dig,
  dbl_mant_dig,
  dbl_max_10_exp,
  dbl_max_exp,
  dbl_min_exp,
  dbl_max,
  dbl_min,
  dbl_epsilon
};

enum class FloatConstants {
  m_e_f,
  m_log2e_f,
  m_log10e_f,
  m_ln2_f,
  m_ln10_f,
  m_pi_f,
  m_pi_2_f,
  m_pi_4_f,
  m_1_pi_f,
  m_2_pi_f,
  m_2_sqrtpi_f,
  m_sqrt2_f,
  m_sqrt1_2_f,
  m_e,
  m_log2e,
  m_log10e,
  m_ln2,
  m_ln10,
  m_pi,
  m_pi_2,
  m_pi_4,
  m_1_pi,
  m_2_pi,
  m_2_sqrtpi,
  m_sqrt2,
  m_sqrt1_2,
  maxfloat,
  huge_valf,
  infinity,
  nan,
  huge_val
};

#define CASSIAN_CATCH_REGISTER_MATH_CONSTANTS                                  \
  CATCH_REGISTER_ENUM(                                                         \
      FloatConstants, FloatConstants::m_e_f, FloatConstants::m_log2e_f,        \
      FloatConstants::m_log10e_f, FloatConstants::m_ln2_f,                     \
      FloatConstants::m_ln10_f, FloatConstants::m_pi_f,                        \
      FloatConstants::m_pi_2_f, FloatConstants::m_pi_4_f,                      \
      FloatConstants::m_1_pi_f, FloatConstants::m_2_pi_f,                      \
      FloatConstants::m_2_sqrtpi_f, FloatConstants::m_sqrt2_f,                 \
      FloatConstants::m_sqrt1_2_f, FloatConstants::m_e,                        \
      FloatConstants::m_log2e, FloatConstants::m_log10e,                       \
      FloatConstants::m_ln2, FloatConstants::m_ln10, FloatConstants::m_pi,     \
      FloatConstants::m_pi_2, FloatConstants::m_pi_4, FloatConstants::m_1_pi,  \
      FloatConstants::m_2_pi, FloatConstants::m_2_sqrtpi,                      \
      FloatConstants::m_sqrt2, FloatConstants::m_sqrt1_2,                      \
      FloatConstants::maxfloat, FloatConstants::huge_valf,                     \
      FloatConstants::infinity, FloatConstants::nan, FloatConstants::huge_val)

#define CASSIAN_CATCH_REGISTER_MATH_MACROS                                     \
  CATCH_REGISTER_ENUM(                                                         \
      FloatMacros, FloatMacros::flt_dig, FloatMacros::flt_mant_dig,            \
      FloatMacros::flt_max_10_exp, FloatMacros::flt_max_exp,                   \
      FloatMacros::flt_min_exp, FloatMacros::flt_radix, FloatMacros::flt_max,  \
      FloatMacros::flt_min, FloatMacros::flt_epsilon, FloatMacros::dbl_dig,    \
      FloatMacros::dbl_mant_dig, FloatMacros::dbl_max_10_exp,                  \
      FloatMacros::dbl_max_exp, FloatMacros::dbl_min_exp,                      \
      FloatMacros::dbl_max, FloatMacros::dbl_min, FloatMacros::dbl_epsilon)

#define CASSIAN_CATCH_REGISTER_MATH_FUNCTIONS                                  \
  CATCH_REGISTER_ENUM(                                                         \
      Function, Function::acos, Function::acosh, Function::acospi,             \
      Function::asin, Function::asinh, Function::asinpi, Function::atan,       \
      Function::atan2, Function::atanh, Function::atanpi, Function::atan2pi,   \
      Function::cbrt, Function::ceil, Function::copysign, Function::cos,       \
      Function::cosh, Function::cospi, Function::erfc, Function::erf,          \
      Function::exp, Function::exp2, Function::exp10, Function::expm1,         \
      Function::fabs, Function::fdim, Function::floor, Function::fma,          \
      Function::fmax, Function::fmin, Function::fmod, Function::fract,         \
      Function::frexp, Function::hypot, Function::ilogb, Function::ldexp,      \
      Function::lgamma, Function::lgamma_r, Function::log, Function::log2,     \
      Function::log10, Function::log1p, Function::logb, Function::mad,         \
      Function::maxmag, Function::minmag, Function::modf, Function::nan,       \
      Function::nextafter, Function::pow, Function::pown, Function::powr,      \
      Function::remainder, Function::remquo, Function::rint, Function::rootn,  \
      Function::round, Function::rsqrt, Function::sincos, Function::sinh,      \
      Function::sin, Function::sinpi, Function::sqrt, Function::tan,           \
      Function::tanh, Function::tanpi, Function::tgamma, Function::trunc,      \
      Function::half_cos, Function::half_divide, Function::half_exp,           \
      Function::half_exp2, Function::half_exp10, Function::half_log,           \
      Function::half_log2, Function::half_log10, Function::half_powr,          \
      Function::half_recip, Function::half_rsqrt, Function::half_sin,          \
      Function::half_sqrt, Function::half_tan, Function::native_cos,           \
      Function::native_divide, Function::native_exp, Function::native_exp2,    \
      Function::native_exp10, Function::native_log, Function::native_log2,     \
      Function::native_log10, Function::native_powr, Function::native_recip,   \
      Function::native_rsqrt, Function::native_sin, Function::native_sqrt,     \
      Function::native_tan, Function::correctly_rounded_sqrt)

#endif