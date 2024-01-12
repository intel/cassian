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

template <typename T, typename SCALAR = typename T::scalar_type>
using fmax = OclcFunction<
    Function::fmax, 2,
    calculate_fmax<typename T::host_type, typename SCALAR::host_type>, T,
    AddressSpace::clc_global, T, SCALAR>;
template <typename T, typename SCALAR = typename T::scalar_type>
using fmin = OclcFunction<
    Function::fmin, 2,
    calculate_fmin<typename T::host_type, typename SCALAR::host_type>, T,
    AddressSpace::clc_global, T, SCALAR>;
template <typename T, typename INT = typename cassian::clc_int_t>
using ldexp = OclcFunction<
    Function::ldexp, 2,
    calculate_ldexp<typename T::host_type, typename INT::host_type>, T,
    AddressSpace::clc_global, T, INT>;

using Gentype = ca::TupleConcat<ca::TypesFloat, ca::TypesDouble>::type;

using GentypeFunctions = FunctionProduct<Gentype, fmax, fmin, ldexp>::type;

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("functions specializations", "",
                                    GentypeFunctions,
                                    test_name_with_function<TestType>) {
  const TestConfig &config = get_test_config();
  using scalar_type = typename TestType::clc_output_type::scalar_type;
  ca::Requirements requirements;
  requirements.arithmetic_type<scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  run_multiple_test_sections(TestType());
}

} // namespace
