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

template <typename T, AddressSpace AS>
using fract = OclcFunction<Function::fract, 2,
                           calculate_fract<typename T::host_type>, T, AS>;
template <typename T, AddressSpace AS>
using sincos = OclcFunction<Function::sincos, 2,
                            calculate_sincos<typename T::host_type>, T, AS>;
template <typename T, AddressSpace AS>
using modf = OclcFunction<Function::modf, 2,
                          calculate_modf<typename T::host_type>, T, AS>;
template <typename T, AddressSpace AS,
          typename INT = typename cassian::detail::OpenCLCInt<T::vector_size>>
using frexp = OclcFunction<
    Function::frexp, 2,
    calculate_frexp<typename T::host_type, typename INT::host_type>, T, AS, T,
    INT>;
template <typename T, AddressSpace AS,
          typename INT = typename cassian::detail::OpenCLCInt<T::vector_size>>
using remquo = OclcFunction<
    Function::remquo, 3,
    calculate_remquo<typename T::host_type, typename INT::host_type>, T, AS, T,
    T, INT>;
template <typename T, AddressSpace AS,
          typename INT = typename cassian::detail::OpenCLCInt<T::vector_size>>
using lgamma_r = OclcFunction<
    Function::lgamma_r, 2,
    calculate_lgamma_r<typename T::host_type, typename INT::host_type>, T, AS,
    T, INT>;

template <template <typename T, AddressSpace AS, typename...>
          typename FunctionAlias,
          typename TestType>
struct Test {
  void operator()() {
    const TestConfig &config = get_test_config();
    using scalar_type = typename TestType::scalar_type;
    ca::Requirements requirements;
    requirements.arithmetic_type<scalar_type>();
    if (ca::should_skip_test(requirements, *config.runtime())) {
      return;
    }

    SECTION("global") {
      run_multiple_test_sections(
          FunctionAlias<TestType, AddressSpace::clc_global>());
    }
    SECTION("local") {
      run_multiple_test_sections(
          FunctionAlias<TestType, AddressSpace::clc_local>());
    }
    SECTION("private") {
      run_multiple_test_sections(
          FunctionAlias<TestType, AddressSpace::clc_private>());
    }
  }
};

using Gentype = ca::TupleConcat<ca::TypesFloat, ca::TypesDouble>::type;

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("functions store - fract", "", Gentype,
                                    test_name<TestType>) {
  Test<fract, TestType>()();
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("functions store - sincos", "", Gentype,
                                    test_name<TestType>) {
  Test<sincos, TestType>()();
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("functions store - modf", "", Gentype,
                                    test_name<TestType>) {
  Test<modf, TestType>()();
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("functions store - frexp", "", Gentype,
                                    test_name<TestType>) {
  Test<frexp, TestType>()();
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("functions store - remquo", "", Gentype,
                                    test_name<TestType>) {
  Test<remquo, TestType>()();
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("functions store - lgamma_r", "", Gentype,
                                    test_name<TestType>) {
  Test<lgamma_r, TestType>()();
}

} // namespace
