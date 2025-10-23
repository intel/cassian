/*
 * Copyright (C) 2021-2024 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

// Enable math defines from cmath module
#define _USE_MATH_DEFINES

#include <algorithm>
#include <cassian/catch2_utils/catch2_utils.hpp>
#include <cassian/runtime/openclc_type_tuples.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cassian/test_harness/test_harness.hpp>
#include <cassian/utility/metaprogramming.hpp>
#include <cassian/utility/utility.hpp>
#include <catch2/catch.hpp>
#include <common.hpp>
#include <cstddef>
#include <cstdint>
#include <enum_definitions.hpp>
#include <math_functions.hpp>
#include <string>
#include <test_config.hpp>
#include <vector>
namespace ca = cassian;

namespace {

auto run_each_test = [](auto &&... oclc_function) {
  (run_multiple_test_sections<
       std::remove_reference_t<decltype(oclc_function)>>(),
   ...);
};

using Gentype = ca::TupleConcat<ca::TypesFloat, ca::TypesDouble>::type;

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("math_functions_gentype", "", Gentype,
                                    test_name<TestType>) {
  const TestConfig &config = get_test_config();
  using scalar_type = typename TestType::scalar_type;
  ca::Requirements requirements;
  requirements.arithmetic_type<scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }
  using host_type = typename TestType::host_type;
  run_each_test(
      OclcFunction<Function::acos, 1, calculate_acos<host_type>, TestType>(),
      OclcFunction<Function::acosh, 1, calculate_acosh<host_type>, TestType>(),
      OclcFunction<Function::acospi, 1, calculate_acospi<host_type>,
                   TestType>(),
      OclcFunction<Function::asin, 1, calculate_asin<host_type>, TestType>(),
      OclcFunction<Function::asinh, 1, calculate_asinh<host_type>, TestType>(),
      OclcFunction<Function::asinpi, 1, calculate_asinpi<host_type>,
                   TestType>(),
      OclcFunction<Function::atan, 1, calculate_atan<host_type>, TestType>(),
      OclcFunction<Function::atan2, 2, calculate_atan2<host_type>, TestType>(),
      OclcFunction<Function::atanh, 1, calculate_atanh<host_type>, TestType>(),
      OclcFunction<Function::atanpi, 1, calculate_atanpi<host_type>,
                   TestType>(),
      OclcFunction<Function::atan2pi, 2, calculate_atan2pi<host_type>,
                   TestType>(),
      OclcFunction<Function::cbrt, 1, calculate_cbrt<host_type>, TestType>(),
      OclcFunction<Function::ceil, 1, calculate_ceil<host_type>, TestType>(),
      OclcFunction<Function::copysign, 2, calculate_copysign<host_type>,
                   TestType>(),
      OclcFunction<Function::cos, 1, calculate_cos<host_type>, TestType>(),
      OclcFunction<Function::cosh, 1, calculate_cosh<host_type>, TestType>(),
      OclcFunction<Function::cospi, 1, calculate_cospi<host_type>, TestType>(),
      OclcFunction<Function::sqrt, 1, calculate_sqrt<host_type>, TestType>(),
      OclcFunction<Function::mad, 3, calculate_mad<host_type>, TestType>(),
      OclcFunction<Function::maxmag, 2, calculate_maxmag<host_type>,
                   TestType>(),
      OclcFunction<Function::minmag, 2, calculate_minmag<host_type>,
                   TestType>(),
      OclcFunction<Function::nextafter, 2, calculate_nextafter<host_type>,
                   TestType>(),
      OclcFunction<Function::pow, 2, calculate_pow<host_type>, TestType>(),
      OclcFunction<Function::remainder, 2, calculate_remainder<host_type>,
                   TestType>(),
      OclcFunction<Function::rint, 1, calculate_rint<host_type>, TestType>(),
      OclcFunction<Function::round, 1, calculate_round<host_type>, TestType>(),
      OclcFunction<Function::rsqrt, 1, calculate_rsqrt<host_type>, TestType>(),
      OclcFunction<Function::sin, 1, calculate_sin<host_type>, TestType>(),
      OclcFunction<Function::sinh, 1, calculate_sinh<host_type>, TestType>(),
      OclcFunction<Function::sinpi, 1, calculate_sinpi<host_type>, TestType>(),
      OclcFunction<Function::tan, 1, calculate_tan<host_type>, TestType>(),
      OclcFunction<Function::tanh, 1, calculate_tanh<host_type>, TestType>(),
      OclcFunction<Function::tanpi, 1, calculate_tanpi<host_type>, TestType>(),
      OclcFunction<Function::tgamma, 1, calculate_tgamma<host_type>,
                   TestType>(),
      OclcFunction<Function::trunc, 1, calculate_trunc<host_type>, TestType>(),
      OclcFunction<Function::erfc, 1, calculate_erfc<host_type>, TestType>(),
      OclcFunction<Function::erf, 1, calculate_erf<host_type>, TestType>(),
      OclcFunction<Function::exp, 1, calculate_exp<host_type>, TestType>(),
      OclcFunction<Function::exp2, 1, calculate_exp2<host_type>, TestType>(),
      OclcFunction<Function::exp10, 1, calculate_exp10<host_type>, TestType>(),
      OclcFunction<Function::expm1, 1, calculate_expm1<host_type>, TestType>(),
      OclcFunction<Function::fabs, 1, calculate_fabs<host_type>, TestType>(),
      OclcFunction<Function::fdim, 2, calculate_fdim<host_type>, TestType>(),
      OclcFunction<Function::floor, 1, calculate_floor<host_type>, TestType>(),
      OclcFunction<Function::fma, 3, calculate_fma<host_type>, TestType>(),
      OclcFunction<Function::fmax, 2, calculate_fmax<host_type>, TestType>(),
      OclcFunction<Function::fmin, 2, calculate_fmin<host_type>, TestType>(),
      OclcFunction<Function::fmod, 2, calculate_fmod<host_type>, TestType>(),
      OclcFunction<Function::hypot, 2, calculate_hypot<host_type>, TestType>(),
      OclcFunction<Function::lgamma, 1, calculate_lgamma<host_type>,
                   TestType>(),
      OclcFunction<Function::log, 1, calculate_log<host_type>, TestType>(),
      OclcFunction<Function::log2, 1, calculate_log2<host_type>, TestType>(),
      OclcFunction<Function::log10, 1, calculate_log10<host_type>, TestType>(),
      OclcFunction<Function::log1p, 1, calculate_log1p<host_type>, TestType>(),
      OclcFunction<Function::logb, 1, calculate_logb<host_type>, TestType>(),
      OclcFunction<Function::powr, 2, calculate_powr<host_type>, TestType>());

  if constexpr (std::is_same_v<typename scalar_type::host_type, double>) {
    using int_type = typename TestType::logical_type::regressed_type;
    using uint_type = typename TestType::logical_type::unsigned_type;
    run_each_test(
        OclcFunction<Function::ilogb, 1,
                     calculate_ilogb<typename int_type::host_type, host_type>,
                     int_type, AddressSpace::clc_global, TestType>(),
        OclcFunction<Function::ldexp, 2,
                     calculate_ldexp<host_type, typename int_type::host_type>,
                     TestType, AddressSpace::clc_global, TestType, int_type>(),
        OclcFunction<Function::nan, 1,
                     calculate_nan<host_type, typename uint_type::host_type>,
                     TestType, AddressSpace::clc_global, uint_type>(),
        OclcFunction<Function::rootn, 2,
                     calculate_rootn<host_type, typename int_type::host_type>,
                     TestType, AddressSpace::clc_global, TestType, int_type>(),
        OclcFunction<Function::pown, 2,
                     calculate_pown<host_type, typename int_type::host_type>,
                     TestType, AddressSpace::clc_global, TestType, int_type>());
  } else {
    using int_type = typename TestType::logical_type;
    using uint_type = typename TestType::logical_type::unsigned_type;
    run_each_test(
        OclcFunction<Function::ilogb, 1,
                     calculate_ilogb<typename int_type::host_type, host_type>,
                     int_type, AddressSpace::clc_global, TestType>(),
        OclcFunction<Function::ldexp, 2,
                     calculate_ldexp<host_type, typename int_type::host_type>,
                     TestType, AddressSpace::clc_global, TestType, int_type>(),
        OclcFunction<Function::nan, 1,
                     calculate_nan<host_type, typename uint_type::host_type>,
                     TestType, AddressSpace::clc_global, uint_type>(),
        OclcFunction<Function::rootn, 2,
                     calculate_rootn<host_type, typename int_type::host_type>,
                     TestType, AddressSpace::clc_global, TestType, int_type>(),
        OclcFunction<Function::pown, 2,
                     calculate_pown<host_type, typename int_type::host_type>,
                     TestType, AddressSpace::clc_global, TestType, int_type>(),
        OclcFunction<Function::native_cos, 1, calculate_cos<host_type>,
                     TestType>(),
        OclcFunction<Function::native_divide, 2, calculate_divide<host_type>,
                     TestType>(),
        OclcFunction<Function::native_exp, 1, calculate_exp<host_type>,
                     TestType>(),
        OclcFunction<Function::native_exp2, 1, calculate_exp2<host_type>,
                     TestType>(),
        OclcFunction<Function::native_exp10, 1, calculate_exp10<host_type>,
                     TestType>(),
        OclcFunction<Function::native_log, 1, calculate_log<host_type>,
                     TestType>(),
        OclcFunction<Function::native_log2, 1, calculate_log2<host_type>,
                     TestType>(),
        OclcFunction<Function::native_log10, 1, calculate_log10<host_type>,
                     TestType>(),
        OclcFunction<Function::native_powr, 2, calculate_pow<host_type>,
                     TestType>(),
        OclcFunction<Function::native_recip, 1, calculate_recip<host_type>,
                     TestType>(),
        OclcFunction<Function::native_rsqrt, 1, calculate_rsqrt<host_type>,
                     TestType>(),
        OclcFunction<Function::native_sin, 1, calculate_sin<host_type>,
                     TestType>(),
        OclcFunction<Function::native_sqrt, 1, calculate_sqrt<host_type>,
                     TestType>(),
        OclcFunction<Function::native_tan, 1, calculate_tan<host_type>,
                     TestType>(),
        OclcFunction<Function::half_cos, 1, calculate_cos<host_type>,
                     TestType>(),
        OclcFunction<Function::half_divide, 2, calculate_divide<host_type>,
                     TestType>(),
        OclcFunction<Function::half_exp, 1, calculate_exp<host_type>,
                     TestType>(),
        OclcFunction<Function::half_exp2, 1, calculate_exp2<host_type>,
                     TestType>(),
        OclcFunction<Function::half_exp10, 1, calculate_exp10<host_type>,
                     TestType>(),
        OclcFunction<Function::half_log, 1, calculate_log<host_type>,
                     TestType>(),
        OclcFunction<Function::half_log2, 1, calculate_log2<host_type>,
                     TestType>(),
        OclcFunction<Function::half_log10, 1, calculate_log10<host_type>,
                     TestType>(),
        OclcFunction<Function::half_powr, 2, calculate_powr<host_type>,
                     TestType>(),
        OclcFunction<Function::half_recip, 1, calculate_recip<host_type>,
                     TestType>(),
        OclcFunction<Function::half_rsqrt, 1, calculate_rsqrt<host_type>,
                     TestType>(),
        OclcFunction<Function::half_sin, 1, calculate_sin<host_type>,
                     TestType>(),
        OclcFunction<Function::half_sqrt, 1, calculate_sqrt<host_type>,
                     TestType>(),
        OclcFunction<Function::half_tan, 1, calculate_tan<host_type>,
                     TestType>());
  }
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("math_functions_specializations", "",
                                    Gentype, test_name<TestType>) {
  const TestConfig &config = get_test_config();
  using scalar_type = typename TestType::scalar_type;
  ca::Requirements requirements;
  requirements.arithmetic_type<scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }
  using host_type = typename TestType::host_type;
  run_each_test(
      OclcFunction<Function::fmax, 2,
                   calculate_fmax<host_type, typename scalar_type::host_type>,
                   TestType, AddressSpace::clc_global, TestType, scalar_type>(),
      OclcFunction<Function::fmin, 2,
                   calculate_fmin<host_type, typename scalar_type::host_type>,
                   TestType, AddressSpace::clc_global, TestType,
                   scalar_type>());
  if constexpr (std::is_same_v<typename scalar_type::host_type, double>) {
    using int_type =
        typename TestType::logical_type::regressed_type::scalar_type;
    run_each_test(
        OclcFunction<Function::ldexp, 2,
                     calculate_ldexp<host_type, typename int_type::host_type>,
                     TestType, AddressSpace::clc_global, TestType, int_type>());
  } else {
    using int_type = typename TestType::logical_type::scalar_type;
    run_each_test(
        OclcFunction<Function::ldexp, 2,
                     calculate_ldexp<host_type, typename int_type::host_type>,
                     TestType, AddressSpace::clc_global, TestType, int_type>());
  }
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("math_functions_gentype - store", "",
                                    Gentype, test_name<TestType>) {
  const TestConfig &config = get_test_config();
  using scalar_type = typename TestType::scalar_type;
  ca::Requirements requirements;
  requirements.arithmetic_type<scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }
  using host_type = typename TestType::host_type;
  run_each_test(
      OclcFunction<Function::fract, 2, calculate_fract<host_type>, TestType>(),
      OclcFunction<Function::sincos, 2, calculate_sincos<host_type>,
                   TestType>(),
      OclcFunction<Function::modf, 2, calculate_modf<host_type>, TestType>(),
      OclcFunction<Function::fract, 2, calculate_fract<host_type>, TestType,
                   AddressSpace::clc_local>(),
      OclcFunction<Function::sincos, 2, calculate_sincos<host_type>, TestType,
                   AddressSpace::clc_local>(),
      OclcFunction<Function::modf, 2, calculate_modf<host_type>, TestType,
                   AddressSpace::clc_local>(),
      OclcFunction<Function::fract, 2, calculate_fract<host_type>, TestType,
                   AddressSpace::clc_private>(),
      OclcFunction<Function::sincos, 2, calculate_sincos<host_type>, TestType,
                   AddressSpace::clc_private>(),
      OclcFunction<Function::modf, 2, calculate_modf<host_type>, TestType,
                   AddressSpace::clc_private>());
  if constexpr (std::is_same_v<typename scalar_type::host_type, double>) {
    using int_type = typename TestType::logical_type::regressed_type;
    run_each_test(
        OclcFunction<Function::frexp, 2,
                     calculate_frexp<host_type, typename int_type::host_type>,
                     TestType, AddressSpace::clc_global, TestType, int_type>(),
        OclcFunction<Function::remquo, 3,
                     calculate_remquo<host_type, typename int_type::host_type>,
                     TestType, AddressSpace::clc_global, TestType, TestType,
                     int_type>(),
        OclcFunction<
            Function::lgamma_r, 2,
            calculate_lgamma_r<host_type, typename int_type::host_type>,
            TestType, AddressSpace::clc_global, TestType, int_type>(),
        OclcFunction<Function::frexp, 2,
                     calculate_frexp<host_type, typename int_type::host_type>,
                     TestType, AddressSpace::clc_local, TestType, int_type>(),
        OclcFunction<Function::remquo, 3,
                     calculate_remquo<host_type, typename int_type::host_type>,
                     TestType, AddressSpace::clc_local, TestType, TestType,
                     int_type>(),
        OclcFunction<
            Function::lgamma_r, 2,
            calculate_lgamma_r<host_type, typename int_type::host_type>,
            TestType, AddressSpace::clc_local, TestType, int_type>(),
        OclcFunction<Function::frexp, 2,
                     calculate_frexp<host_type, typename int_type::host_type>,
                     TestType, AddressSpace::clc_private, TestType, int_type>(),
        OclcFunction<Function::remquo, 3,
                     calculate_remquo<host_type, typename int_type::host_type>,
                     TestType, AddressSpace::clc_private, TestType, TestType,
                     int_type>(),
        OclcFunction<
            Function::lgamma_r, 2,
            calculate_lgamma_r<host_type, typename int_type::host_type>,
            TestType, AddressSpace::clc_private, TestType, int_type>());
  } else {
    using int_type = typename TestType::logical_type;
    run_each_test(
        OclcFunction<Function::frexp, 2,
                     calculate_frexp<host_type, typename int_type::host_type>,
                     TestType, AddressSpace::clc_global, TestType, int_type>(),
        OclcFunction<Function::remquo, 3,
                     calculate_remquo<host_type, typename int_type::host_type>,
                     TestType, AddressSpace::clc_global, TestType, TestType,
                     int_type>(),
        OclcFunction<
            Function::lgamma_r, 2,
            calculate_lgamma_r<host_type, typename int_type::host_type>,
            TestType, AddressSpace::clc_global, TestType, int_type>(),
        OclcFunction<Function::frexp, 2,
                     calculate_frexp<host_type, typename int_type::host_type>,
                     TestType, AddressSpace::clc_local, TestType, int_type>(),
        OclcFunction<Function::remquo, 3,
                     calculate_remquo<host_type, typename int_type::host_type>,
                     TestType, AddressSpace::clc_local, TestType, TestType,
                     int_type>(),
        OclcFunction<
            Function::lgamma_r, 2,
            calculate_lgamma_r<host_type, typename int_type::host_type>,
            TestType, AddressSpace::clc_local, TestType, int_type>(),
        OclcFunction<Function::frexp, 2,
                     calculate_frexp<host_type, typename int_type::host_type>,
                     TestType, AddressSpace::clc_private, TestType, int_type>(),
        OclcFunction<Function::remquo, 3,
                     calculate_remquo<host_type, typename int_type::host_type>,
                     TestType, AddressSpace::clc_private, TestType, TestType,
                     int_type>(),
        OclcFunction<
            Function::lgamma_r, 2,
            calculate_lgamma_r<host_type, typename int_type::host_type>,
            TestType, AddressSpace::clc_private, TestType, int_type>());
  }
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("math_functions_correctly_rounded_flag", "",
                                    ca::TypesFloat, test_name<TestType>) {
  const TestConfig &config = get_test_config();
  using scalar_type = typename TestType::scalar_type;
  ca::Requirements requirements;
  requirements.arithmetic_type<scalar_type>();
  requirements.correctly_rounded_divide_sqrt<scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }
  using host_type = typename TestType::host_type;
  run_each_test(OclcFunction<Function::correctly_rounded_sqrt, 1,
                             calculate_sqrt<host_type>, TestType>());
}
} // namespace
