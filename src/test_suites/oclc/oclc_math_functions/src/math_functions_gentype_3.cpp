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
using logb =
    OclcFunction<Function::logb, 1, calculate_logb<typename T::host_type>, T>;
template <typename T>
using powr =
    OclcFunction<Function::powr, 2, calculate_powr<typename T::host_type>, T>;
template <typename T,
          typename INT = typename cassian::detail::OpenCLCInt<T::vector_size>>
using ldexp = OclcFunction<
    Function::ldexp, 2,
    calculate_ldexp<typename T::host_type, typename INT::host_type>, T,
    AddressSpace::clc_global, T, INT>;
template <typename T,
          typename UNSIGNED = typename T::logical_type::unsigned_type>
using nan = OclcFunction<
    Function::nan, 1,
    calculate_nan<typename T::host_type, typename UNSIGNED::host_type>, T,
    AddressSpace::clc_global, UNSIGNED>;
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

using Gentype =
    ca::TupleConcat<ca::TypesFloat, ca::TypesDouble, ca::TypesHalf>::type;

using GentypeFunctions = FunctionProduct<Gentype, log, log2, log10, log1p, logb,
                                         powr, ldexp, nan, rootn, pown>::type;

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("functions", "", GentypeFunctions,
                                    test_name_with_function<TestType>) {
  const TestConfig &config = get_test_config();
  using scalar_type = typename TestType::clc_output_type::scalar_type;
  ca::Requirements requirements;
  requirements.arithmetic_type<scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  run_multiple_test_sections<TestType>();
}

template <typename T,
          typename INT = typename cassian::detail::OpenCLCInt<T::vector_size>>
using ilogb = OclcFunction<
    Function::ilogb, 1,
    calculate_ilogb<typename INT::host_type, typename T::host_type>, INT,
    AddressSpace::clc_global, T>;

template <typename TestType> auto test_name_ilogb() {
  std::stringstream ss;
  ss << TestType::get_function_string();
  ss << " - ";
  ss << TestType::clc_input_type_1::type_name;
  return ss.str();
}

using GentypeIlogb = FunctionProduct<Gentype, ilogb>::type;

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("functions", "", GentypeIlogb,
                                    test_name_ilogb<TestType>) {
  const TestConfig &config = get_test_config();
  using scalar_type = typename TestType::clc_output_type::scalar_type;
  ca::Requirements requirements;
  requirements.arithmetic_type<scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  run_multiple_test_sections<TestType>();
}

} // namespace
