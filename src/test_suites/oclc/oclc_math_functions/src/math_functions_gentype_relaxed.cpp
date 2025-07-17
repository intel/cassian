/*
 * Copyright (C) 2024 Intel Corporation
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

template <typename T>
using acos =
    OclcFunction<Function::acos, 1, calculate_acos<typename T::host_type>, T>;
template <typename T>
using acosh =
    OclcFunction<Function::acosh, 1, calculate_acosh<typename T::host_type>, T>;

template <typename T>
using acospi = OclcFunction<Function::acospi, 1,
                            calculate_acospi<typename T::host_type>, T>;
template <typename T>
using asin =
    OclcFunction<Function::asin, 1, calculate_asin<typename T::host_type>, T>;
template <typename T>
using asinh =
    OclcFunction<Function::asinh, 1, calculate_asinh<typename T::host_type>, T,
                 AddressSpace::clc_global, T, T, T,
                 calculate_asinh_derived_check<typename T::host_type>>;
template <typename T>
using asinpi = OclcFunction<Function::asinpi, 1,
                            calculate_asinpi<typename T::host_type>, T>;
template <typename T>
using atan =
    OclcFunction<Function::atan, 1, calculate_atan<typename T::host_type>, T>;
template <typename T>
using atan2 =
    OclcFunction<Function::atan2, 2, calculate_atan2<typename T::host_type>, T>;
template <typename T>
using atanh =
    OclcFunction<Function::atanh, 1, calculate_atanh<typename T::host_type>, T>;
template <typename T>
using atanpi = OclcFunction<Function::atanpi, 1,
                            calculate_atanpi<typename T::host_type>, T>;
template <typename T>
using atan2pi = OclcFunction<Function::atan2pi, 2,
                             calculate_atan2pi<typename T::host_type>, T>;
template <typename T>
using cbrt =
    OclcFunction<Function::cbrt, 1, calculate_cbrt<typename T::host_type>, T>;

template <typename T>
using cos =
    OclcFunction<Function::cos, 1, calculate_cos<typename T::host_type>, T>;
template <typename T>
using cosh =
    OclcFunction<Function::cosh, 1, calculate_cosh<typename T::host_type>, T>;
template <typename T>
using cospi =
    OclcFunction<Function::cospi, 1, calculate_cospi<typename T::host_type>, T>;
template <typename T>
using pow =
    OclcFunction<Function::pow, 2, calculate_pow<typename T::host_type>, T>;
template <typename T>
using sin =
    OclcFunction<Function::sin, 1, calculate_sin<typename T::host_type>, T>;
template <typename T>
using sinh =
    OclcFunction<Function::sinh, 1, calculate_sinh<typename T::host_type>, T>;
template <typename T>
using sinpi =
    OclcFunction<Function::sinpi, 1, calculate_sinpi<typename T::host_type>, T>;
template <typename T>
using tan =
    OclcFunction<Function::tan, 1, calculate_tan<typename T::host_type>, T>;
template <typename T>
using tanh =
    OclcFunction<Function::tanh, 1, calculate_tanh<typename T::host_type>, T>;
template <typename T>
using tanpi =
    OclcFunction<Function::tanpi, 1, calculate_tanpi<typename T::host_type>, T>;
template <typename T>
using exp =
    OclcFunction<Function::exp, 1, calculate_exp<typename T::host_type>, T>;
template <typename T>
using exp2 =
    OclcFunction<Function::exp2, 1, calculate_exp2<typename T::host_type>, T>;
template <typename T>
using exp10 =
    OclcFunction<Function::exp10, 1, calculate_exp10<typename T::host_type>, T>;
template <typename T>
using expm1 =
    OclcFunction<Function::expm1, 1, calculate_expm1<typename T::host_type>, T>;
template <typename T>
using log =
    OclcFunction<Function::log, 1, calculate_log<typename T::host_type>, T>;
template <typename T>
using log2 =
    OclcFunction<Function::log2, 1, calculate_log2<typename T::host_type>, T>;
template <typename T>
using log10 =
    OclcFunction<Function::log10, 1, calculate_log10<typename T::host_type>, T>;
template <typename T>
using log1p =
    OclcFunction<Function::log1p, 1, calculate_log1p<typename T::host_type>, T>;
template <typename T>
using powr =
    OclcFunction<Function::powr, 2, calculate_powr<typename T::host_type>, T>;
template <typename T,
          typename INT = typename cassian::detail::OpenCLCInt<T::vector_size>>
using rootn = OclcFunction<
    Function::rootn, 2,
    calculate_rootn<typename T::host_type, typename INT::host_type>, T,
    AddressSpace::clc_global, T, INT>;
template <typename T,
          typename INT = typename cassian::detail::OpenCLCInt<T::vector_size>>
using pown =
    OclcFunction<Function::pown, 2,
                 calculate_pown<typename T::host_type, typename INT::host_type>,
                 T, AddressSpace::clc_global, T, INT>;
template <typename T, AddressSpace AS>
using sincos = OclcFunction<Function::sincos, 2,
                            calculate_sincos<typename T::host_type>, T, AS>;

using Gentype = ca::TupleConcat<ca::TypesFloat>::type;

using GentypeRelaxedFunctions =
    FunctionProduct<Gentype, acos, acosh, acospi, asin, asinh, asinpi, atan,
                    atan2, atanh, atanpi, atan2pi, cbrt, cos, cosh, cospi, exp,
                    exp2, exp10, expm1, log, log2, log10, log1p, pow, pown,
                    powr, rootn, sin, sinh, sinpi, tan, tanh, tanpi>::type;

template <template <typename T, AddressSpace AS, typename...>
          typename FunctionAlias,
          typename TestType>
struct TestRelaxed {
  void operator()() {
    const TestConfig &config = get_test_config();
    using scalar_type = typename TestType::scalar_type;
    ca::Requirements requirements;
    requirements.arithmetic_type<scalar_type>();
    if (ca::should_skip_test(requirements, *config.runtime())) {
      return;
    }

    SECTION("global") {
      run_multiple_test_sections_relaxed(
          FunctionAlias<TestType, AddressSpace::clc_global>());
    }
    SECTION("local") {
      run_multiple_test_sections_relaxed(
          FunctionAlias<TestType, AddressSpace::clc_local>());
    }
  }
};

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("functions store relaxed - sincos", "",
                                    Gentype, test_name<TestType>) {
  TestRelaxed<sincos, TestType>()();
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("functions relaxed", "",
                                    GentypeRelaxedFunctions,
                                    test_name_with_function<TestType>) {
  const TestConfig &config = get_test_config();
  using scalar_type = typename TestType::clc_output_type::scalar_type;
  ca::Requirements requirements;
  requirements.arithmetic_type<scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  run_multiple_test_sections_relaxed(TestType());
}

} // namespace
