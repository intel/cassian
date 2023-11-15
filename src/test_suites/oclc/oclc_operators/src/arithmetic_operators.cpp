/*
 * Copyright (C) 2021-2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <algorithm>
#include <cassian/catch2_utils/catch2_utils.hpp>
#include <cassian/cli/cli.hpp>
#include <cassian/random/random.hpp>
#include <cassian/runtime/openclc_type_tuples.hpp>
#include <cassian/runtime/openclc_types.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cassian/test_harness/test_harness.hpp>
#include <cassian/utility/utility.hpp>
#include <cassian/vector/vector.hpp>
#include <catch2/catch.hpp>
#include <common.hpp>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <limits>
#include <numeric>
#include <string>
#include <test_config.hpp>
#include <vector>

namespace ca = cassian;

namespace {

enum class ArithmeticOperator {
  addition,
  subtraction,
  multiplication,
  division,
  remainder,
  correctly_rounded_division
};

std::string to_clc_operator(const ArithmeticOperator op) {
  switch (op) {
  case ArithmeticOperator::addition:
    return "+";
  case ArithmeticOperator::subtraction:
    return "-";
  case ArithmeticOperator::multiplication:
    return "*";
  case ArithmeticOperator::division:
  case ArithmeticOperator::correctly_rounded_division:
    return "/";
  case ArithmeticOperator::remainder:
    return "%";
  default:
    throw UnknownOperatorException("Unknown operator: " +
                                   std::to_string(static_cast<int>(op)));
  }
}

template <typename OUTPUT_TYPE, typename A_TYPE, typename B_TYPE,
          typename ca::EnableIfIsIntegral<ca::scalar_type_v<OUTPUT_TYPE>> = 0,
          typename ca::EnableIfIsIntegral<ca::scalar_type_v<A_TYPE>> = 0,
          typename ca::EnableIfIsIntegral<ca::scalar_type_v<B_TYPE>> = 0>
std::function<OUTPUT_TYPE(A_TYPE, B_TYPE)>
to_function(const ArithmeticOperator op) {
  switch (op) {
  case ArithmeticOperator::addition:
    return [](A_TYPE a, B_TYPE b) { return a + b; };
    break;
  case ArithmeticOperator::subtraction:
    return [](A_TYPE a, B_TYPE b) { return a - b; };
    break;
  case ArithmeticOperator::multiplication:
    return [](A_TYPE a, B_TYPE b) { return a * b; };
    break;
  case ArithmeticOperator::division:
  case ArithmeticOperator::correctly_rounded_division:
    return [](A_TYPE a, B_TYPE b) { return a / b; };
    break;
  case ArithmeticOperator::remainder:
    return [](A_TYPE a, B_TYPE b) { return a % b; };
    break;
  default:
    throw UnknownOperatorException("Unknown operator: " +
                                   std::to_string(static_cast<int>(op)));
  }
}

template <
    typename OUTPUT_TYPE, typename A_TYPE, typename B_TYPE,
    typename ca::EnableIfIsFloatingPoint<ca::scalar_type_v<OUTPUT_TYPE>> = 0,
    typename ca::EnableIfIsFloatingPoint<ca::scalar_type_v<A_TYPE>> = 0,
    typename ca::EnableIfIsFloatingPoint<ca::scalar_type_v<B_TYPE>> = 0>
std::function<OUTPUT_TYPE(A_TYPE, B_TYPE)>
to_function(const ArithmeticOperator op) {
  switch (op) {
  case ArithmeticOperator::addition:
    return [](A_TYPE a, B_TYPE b) { return a + b; };
    break;
  case ArithmeticOperator::subtraction:
    return [](A_TYPE a, B_TYPE b) { return a - b; };
    break;
  case ArithmeticOperator::multiplication:
    return [](A_TYPE a, B_TYPE b) { return a * b; };
    break;
  case ArithmeticOperator::division:
  case ArithmeticOperator::correctly_rounded_division:
    return [](A_TYPE a, B_TYPE b) { return a / b; };
    break;
  default:
    throw UnknownOperatorException("Unknown operator: " +
                                   std::to_string(static_cast<int>(op)));
  }
}

template <typename OUTPUT_TYPE, typename A_TYPE, typename B_TYPE>
std::string get_build_options(const ArithmeticOperator op) {
  const std::string clc_a_type = A_TYPE::device_type;
  const std::string clc_b_type = B_TYPE::device_type;
  const std::string clc_output_type = OUTPUT_TYPE::device_type;
  const std::string clc_operator = to_clc_operator(op);
  std::string build_options =
      " -D A_TYPE=" + clc_a_type + " -D B_TYPE=" + clc_b_type +
      " -D OUTPUT_TYPE=" + clc_output_type + " -D OPERATOR=" + clc_operator;
  if (op == ArithmeticOperator::correctly_rounded_division) {
    build_options = build_options + " -cl-fp32-correctly-rounded-divide-sqrt";
  }
  return build_options;
}

ca::Kernel create_kernel(const std::string &path,
                         const std::string &build_options, ca::Runtime *runtime,
                         const std::string &program_type) {
  const std::string source = ca::load_text_file(ca::get_asset(path));
  const std::string kernel_name = "test_kernel";
  return runtime->create_kernel(kernel_name, source, build_options,
                                program_type);
}

template <typename OUTPUT_TYPE, typename A_TYPE, typename B_TYPE,
          typename OUTPUT_HOST_TYPE = typename OUTPUT_TYPE::host_type,
          typename A_HOST_TYPE = typename A_TYPE::host_type,
          typename B_HOST_TYPE = typename B_TYPE::host_type>
std::vector<OUTPUT_HOST_TYPE>
test(const std::vector<A_HOST_TYPE> &input_a,
     const std::vector<B_HOST_TYPE> &input_b, const ArithmeticOperator op,
     ca::Runtime *runtime, const std::string &program_type) {
  const std::string build_options =
      get_build_options<OUTPUT_TYPE, A_TYPE, B_TYPE>(op);
  const ca::Kernel kernel =
      create_kernel("kernels/oclc_operators/arithmetic_operators.cl",
                    build_options, runtime, program_type);
  std::vector<OUTPUT_HOST_TYPE> output =
      run_kernel<OUTPUT_HOST_TYPE>(kernel, input_a, input_b, runtime);
  runtime->release_kernel(kernel);
  return output;
}

template <typename OUTPUT_TYPE, typename A_TYPE, typename B_TYPE>
std::vector<OUTPUT_TYPE> get_reference(const std::vector<A_TYPE> &input_a,
                                       const std::vector<B_TYPE> &input_b,
                                       const ArithmeticOperator op) {
  const auto reference_function = to_function<OUTPUT_TYPE, A_TYPE, B_TYPE>(op);
  std::vector<OUTPUT_TYPE> output(input_a.size());
  for (size_t i = 0; i < output.size(); ++i) {
    output[i] = reference_function(input_a[i], input_b[i]);
  }
  return output;
}

template <typename T, typename ca::EnableIfIsVector<T> = 0>
bool non_zero_value(const T &v) {
  using scalar_type = ca::scalar_type_v<T>;
  for (size_t i = 0; i < v.size(); ++i) {
    if (v[i] == scalar_type(0)) {
      return false;
    }
  }
  return true;
}

template <typename T, typename ca::EnableIfIsScalar<T> = 0>
bool non_zero_value(const T &v) {
  return v != T(0);
}

template <typename T>
std::vector<T> generate_non_zero_vector(const int size, const int seed) {
  std::vector<T> data(size);
  std::generate(data.begin(), data.end(), [&] {
    T value = T(0);
    do {
      value = ca::generate_value<T>(seed);
    } while (!non_zero_value(value));
    return value;
  });
  return data;
}

template <typename TEST_TYPE>
void test_addition_common(const TestConfig &config) {
  const ArithmeticOperator op = ArithmeticOperator::addition;
  using host_type = typename TEST_TYPE::host_type;

  SECTION("add zero") {
    const std::vector<host_type> input_a(config.work_size(), host_type(5));
    const std::vector<host_type> input_b(config.work_size(), host_type(0));
    const std::vector<host_type> output = test<TEST_TYPE, TEST_TYPE, TEST_TYPE>(
        input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }

  SECTION("add positive") {
    const std::vector<host_type> input_a(config.work_size(), host_type(5));
    const std::vector<host_type> input_b(config.work_size(), host_type(2));
    const std::vector<host_type> output = test<TEST_TYPE, TEST_TYPE, TEST_TYPE>(
        input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }

  SECTION("commutative") {
    const std::vector<host_type> input_a(config.work_size(), host_type(5));
    const std::vector<host_type> input_b(config.work_size(), host_type(2));
    const std::vector<host_type> output = test<TEST_TYPE, TEST_TYPE, TEST_TYPE>(
        input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        test<TEST_TYPE, TEST_TYPE, TEST_TYPE>(
            input_b, input_a, op, config.runtime(), config.program_type());
    compare(output, reference);
  }

  SECTION("non-uniform buffer") {
    std::vector<host_type> input_a(config.work_size());
    std::vector<host_type> input_b(config.work_size());
    std::iota(input_a.begin(), input_a.end(), host_type(1));
    std::iota(input_b.begin(), input_b.end(), host_type(2));
    const std::vector<host_type> output = test<TEST_TYPE, TEST_TYPE, TEST_TYPE>(
        input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }

  SECTION("random") {
    const std::vector<host_type> input_a =
        ca::generate_vector<host_type>(config.work_size(), 0);
    const std::vector<host_type> input_b =
        ca::generate_vector<host_type>(config.work_size(), 0);
    const std::vector<host_type> output = test<TEST_TYPE, TEST_TYPE, TEST_TYPE>(
        input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }
}

template <typename TEST_TYPE>
void test_addition_signed(const TestConfig &config) {
  const ArithmeticOperator op = ArithmeticOperator::addition;
  using host_type = typename TEST_TYPE::host_type;

  SECTION("add negative") {
    const std::vector<host_type> input_a(config.work_size(), host_type(5));
    const std::vector<host_type> input_b(config.work_size(), host_type(-2));
    const std::vector<host_type> output = test<TEST_TYPE, TEST_TYPE, TEST_TYPE>(
        input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }
}

template <typename TEST_TYPE>
void test_addition_unsigned(const TestConfig &config) {
  const ArithmeticOperator op = ArithmeticOperator::addition;
  using host_type = typename TEST_TYPE::host_type;

  SECTION("overflow") {
    const std::vector<host_type> input_a(config.work_size(),
                                         std::numeric_limits<host_type>::max());
    const std::vector<host_type> input_b(config.work_size(), host_type(2));
    const std::vector<host_type> output = test<TEST_TYPE, TEST_TYPE, TEST_TYPE>(
        input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }
}

template <typename TEST_TYPE>
void test_addition_float(const TestConfig &config) {
  const ArithmeticOperator op = ArithmeticOperator::addition;
  using host_type = typename TEST_TYPE::host_type;

  SECTION("add float") {
    const std::vector<host_type> input_a(config.work_size(), host_type(2.3));
    const std::vector<host_type> input_b(config.work_size(), host_type(1.4));
    const std::vector<host_type> output = test<TEST_TYPE, TEST_TYPE, TEST_TYPE>(
        input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }

  // TODO: Handle ULP comparisons
  // TODO: Handle NaN comparisons
}

template <typename TEST_TYPE>
void test_addition_vector(const TestConfig &config) {
  const ArithmeticOperator op = ArithmeticOperator::addition;
  using host_type = typename TEST_TYPE::host_type;
  using scalar_type = typename TEST_TYPE::scalar_type;
  using host_scalar_type = typename scalar_type::host_type;

  SECTION("add scalar rhs") {
    const std::vector<host_type> input_a(config.work_size(), host_type(4));
    const std::vector<host_scalar_type> input_b(config.work_size(),
                                                host_scalar_type(2));
    const std::vector<host_type> output =
        test<TEST_TYPE, TEST_TYPE, scalar_type>(
            input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }

  SECTION("add scalar lhs") {
    const std::vector<host_scalar_type> input_a(config.work_size(),
                                                host_scalar_type(2));
    const std::vector<host_type> input_b(config.work_size(), host_type(4));
    const std::vector<host_type> output =
        test<TEST_TYPE, scalar_type, TEST_TYPE>(
            input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }

  SECTION("non-uniform vector") {
    std::vector<host_scalar_type> scalar_a(host_type::vector_size);
    std::vector<host_scalar_type> scalar_b(host_type::vector_size);
    std::iota(scalar_a.begin(), scalar_a.end(), host_scalar_type(1));
    std::iota(scalar_b.begin(), scalar_b.end(), host_scalar_type(2));
    const std::vector<host_type> input_a(config.work_size(),
                                         host_type(scalar_a));
    const std::vector<host_type> input_b(config.work_size(),
                                         host_type(scalar_b));
    const std::vector<host_type> output = test<TEST_TYPE, TEST_TYPE, TEST_TYPE>(
        input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }
}

template <typename TestType> auto test_name() {
  return std::string(TestType::type_name);
}

using float_types = ca::TupleConcat<ca::TypesFloat, ca::TypesDouble>::type;

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("arithmetic operators - addition", "",
                                    ca::SignedTypes, test_name<TestType>) {
  const TestConfig &config = get_test_config();

  ca::Requirements requirements;
  requirements.arithmetic_type<typename TestType::scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  test_addition_common<TestType>(config);
  test_addition_signed<TestType>(config);
  if constexpr (ca::is_vector_v<typename TestType::host_type>) {
    test_addition_vector<TestType>(config);
  }
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("arithmetic operators - addition", "",
                                    ca::UnsignedTypes, test_name<TestType>) {
  const TestConfig &config = get_test_config();

  ca::Requirements requirements;
  requirements.arithmetic_type<typename TestType::scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  test_addition_common<TestType>(config);
  test_addition_unsigned<TestType>(config);
  if constexpr (ca::is_vector_v<typename TestType::host_type>) {
    test_addition_vector<TestType>(config);
  }
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("arithmetic operators - addition", "",
                                    float_types, test_name<TestType>) {
  const TestConfig &config = get_test_config();

  ca::Requirements requirements;
  requirements.arithmetic_type<typename TestType::scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  test_addition_common<TestType>(config);
  test_addition_signed<TestType>(config);
  test_addition_float<TestType>(config);
  if constexpr (ca::is_vector_v<typename TestType::host_type>) {
    test_addition_vector<TestType>(config);
  }
}

template <typename TEST_TYPE>
void test_subtraction_common(const TestConfig &config) {
  const ArithmeticOperator op = ArithmeticOperator::subtraction;
  using host_type = typename TEST_TYPE::host_type;

  SECTION("subtract zero") {
    const std::vector<host_type> input_a(config.work_size(), host_type(5));
    const std::vector<host_type> input_b(config.work_size(), host_type(0));
    const std::vector<host_type> output = test<TEST_TYPE, TEST_TYPE, TEST_TYPE>(
        input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }

  SECTION("subtract positive") {
    const std::vector<host_type> input_a(config.work_size(), host_type(5));
    const std::vector<host_type> input_b(config.work_size(), host_type(2));
    const std::vector<host_type> output = test<TEST_TYPE, TEST_TYPE, TEST_TYPE>(
        input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }

  SECTION("non-uniform buffer") {
    std::vector<host_type> input_a(config.work_size());
    std::vector<host_type> input_b(config.work_size());
    std::iota(input_a.begin(), input_a.end(), host_type(1));
    std::iota(input_b.begin(), input_b.end(), host_type(2));
    const std::vector<host_type> output = test<TEST_TYPE, TEST_TYPE, TEST_TYPE>(
        input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }

  SECTION("random") {
    const std::vector<host_type> input_a =
        ca::generate_vector<host_type>(config.work_size(), 0);
    const std::vector<host_type> input_b =
        ca::generate_vector<host_type>(config.work_size(), 0);
    const std::vector<host_type> output = test<TEST_TYPE, TEST_TYPE, TEST_TYPE>(
        input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }
}

template <typename TEST_TYPE>
void test_subtraction_signed(const TestConfig &config) {
  const ArithmeticOperator op = ArithmeticOperator::subtraction;
  using host_type = typename TEST_TYPE::host_type;

  SECTION("subtract negative") {
    const std::vector<host_type> input_a(config.work_size(), host_type(5));
    const std::vector<host_type> input_b(config.work_size(), host_type(-2));
    const std::vector<host_type> output = test<TEST_TYPE, TEST_TYPE, TEST_TYPE>(
        input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }
}

template <typename TEST_TYPE>
void test_subtraction_unsigned(const TestConfig &config) {
  const ArithmeticOperator op = ArithmeticOperator::subtraction;
  using host_type = typename TEST_TYPE::host_type;

  SECTION("overflow") {
    const std::vector<host_type> input_a(config.work_size(),
                                         std::numeric_limits<host_type>::min());
    const std::vector<host_type> input_b(config.work_size(), host_type(2));
    const std::vector<host_type> output = test<TEST_TYPE, TEST_TYPE, TEST_TYPE>(
        input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }
}

template <typename TEST_TYPE>
void test_subtraction_float(const TestConfig &config) {
  const ArithmeticOperator op = ArithmeticOperator::subtraction;
  using host_type = typename TEST_TYPE::host_type;

  SECTION("subtract float") {
    const std::vector<host_type> input_a(config.work_size(), host_type(2.3));
    const std::vector<host_type> input_b(config.work_size(), host_type(1.4));
    const std::vector<host_type> output = test<TEST_TYPE, TEST_TYPE, TEST_TYPE>(
        input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }

  // TODO: Handle ULP comparisons
  // TODO: Handle NaN comparisons
}

template <typename TEST_TYPE>
void test_subtraction_vector(const TestConfig &config) {
  const ArithmeticOperator op = ArithmeticOperator::subtraction;
  using host_type = typename TEST_TYPE::host_type;
  using scalar_type = typename TEST_TYPE::scalar_type;
  using host_scalar_type = typename scalar_type::host_type;

  SECTION("subtract scalar rhs") {
    const std::vector<host_type> input_a(config.work_size(), host_type(4));
    const std::vector<host_scalar_type> input_b(config.work_size(),
                                                host_scalar_type(2));
    const std::vector<host_type> output =
        test<TEST_TYPE, TEST_TYPE, scalar_type>(
            input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }

  SECTION("subtract scalar lhs") {
    const std::vector<host_scalar_type> input_a(config.work_size(),
                                                host_scalar_type(2));
    const std::vector<host_type> input_b(config.work_size(), host_type(4));
    const std::vector<host_type> output =
        test<TEST_TYPE, scalar_type, TEST_TYPE>(
            input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }

  SECTION("non-uniform vector") {
    std::vector<host_scalar_type> scalar_a(host_type::vector_size);
    std::vector<host_scalar_type> scalar_b(host_type::vector_size);
    std::iota(scalar_a.begin(), scalar_a.end(), host_scalar_type(4));
    std::iota(scalar_b.begin(), scalar_b.end(), host_scalar_type(2));
    const std::vector<host_type> input_a(config.work_size(),
                                         host_type(scalar_a));
    const std::vector<host_type> input_b(config.work_size(),
                                         host_type(scalar_b));
    const std::vector<host_type> output = test<TEST_TYPE, TEST_TYPE, TEST_TYPE>(
        input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("arithmetic operators - subtraction", "",
                                    ca::SignedTypes, test_name<TestType>) {
  const TestConfig &config = get_test_config();

  ca::Requirements requirements;
  requirements.arithmetic_type<typename TestType::scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  test_subtraction_common<TestType>(config);
  test_subtraction_signed<TestType>(config);
  if constexpr (ca::is_vector_v<typename TestType::host_type>) {
    test_subtraction_vector<TestType>(config);
  }
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("arithmetic operators - subtraction", "",
                                    ca::UnsignedTypes, test_name<TestType>) {
  const TestConfig &config = get_test_config();

  ca::Requirements requirements;
  requirements.arithmetic_type<typename TestType::scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  test_subtraction_common<TestType>(config);
  test_subtraction_unsigned<TestType>(config);
  if constexpr (ca::is_vector_v<typename TestType::host_type>) {
    test_subtraction_vector<TestType>(config);
  }
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("arithmetic operators - subtraction", "",
                                    float_types, test_name<TestType>) {
  const TestConfig &config = get_test_config();

  ca::Requirements requirements;
  requirements.arithmetic_type<typename TestType::scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  test_subtraction_common<TestType>(config);
  test_subtraction_signed<TestType>(config);
  test_subtraction_float<TestType>(config);
  if constexpr (ca::is_vector_v<typename TestType::host_type>) {
    test_subtraction_vector<TestType>(config);
  }
}

template <typename TEST_TYPE>
void test_multiplication_common(const TestConfig &config) {
  const ArithmeticOperator op = ArithmeticOperator::multiplication;
  using host_type = typename TEST_TYPE::host_type;

  SECTION("multiply by zero") {
    const std::vector<host_type> input_a(config.work_size(), host_type(5));
    const std::vector<host_type> input_b(config.work_size(), host_type(0));
    const std::vector<host_type> output = test<TEST_TYPE, TEST_TYPE, TEST_TYPE>(
        input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }

  SECTION("multiply by one") {
    const std::vector<host_type> input_a(config.work_size(), host_type(5));
    const std::vector<host_type> input_b(config.work_size(), host_type(1));
    const std::vector<host_type> output = test<TEST_TYPE, TEST_TYPE, TEST_TYPE>(
        input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }

  SECTION("multiply by positive") {
    const std::vector<host_type> input_a(config.work_size(), host_type(5));
    const std::vector<host_type> input_b(config.work_size(), host_type(2));
    const std::vector<host_type> output = test<TEST_TYPE, TEST_TYPE, TEST_TYPE>(
        input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }

  SECTION("commutative") {
    const std::vector<host_type> input_a(config.work_size(), host_type(5));
    const std::vector<host_type> input_b(config.work_size(), host_type(2));
    const std::vector<host_type> output = test<TEST_TYPE, TEST_TYPE, TEST_TYPE>(
        input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        test<TEST_TYPE, TEST_TYPE, TEST_TYPE>(
            input_b, input_a, op, config.runtime(), config.program_type());
    compare(output, reference);
  }

  SECTION("non-uniform buffer") {
    std::vector<host_type> input_a(config.work_size());
    std::vector<host_type> input_b(config.work_size());
    std::iota(input_a.begin(), input_a.end(), host_type(2));
    std::iota(input_b.begin(), input_b.end(), host_type(3));
    const std::vector<host_type> output = test<TEST_TYPE, TEST_TYPE, TEST_TYPE>(
        input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }

  SECTION("random") {
    const std::vector<host_type> input_a =
        ca::generate_vector<host_type>(config.work_size(), 0);
    const std::vector<host_type> input_b =
        ca::generate_vector<host_type>(config.work_size(), 0);
    const std::vector<host_type> output = test<TEST_TYPE, TEST_TYPE, TEST_TYPE>(
        input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }
}

template <typename TEST_TYPE>
void test_multiplication_signed(const TestConfig &config) {
  const ArithmeticOperator op = ArithmeticOperator::multiplication;
  using host_type = typename TEST_TYPE::host_type;

  SECTION("multiply by negative") {
    const std::vector<host_type> input_a(config.work_size(), host_type(5));
    const std::vector<host_type> input_b(config.work_size(), host_type(-2));
    const std::vector<host_type> output = test<TEST_TYPE, TEST_TYPE, TEST_TYPE>(
        input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }
}

template <typename TEST_TYPE>
void test_multiplication_unsigned(const TestConfig &config) {
  const ArithmeticOperator op = ArithmeticOperator::multiplication;
  using host_type = typename TEST_TYPE::host_type;

  SECTION("overflow") {
    const std::vector<host_type> input_a(config.work_size(),
                                         std::numeric_limits<host_type>::max());
    const std::vector<host_type> input_b(config.work_size(), host_type(2));
    const std::vector<host_type> output = test<TEST_TYPE, TEST_TYPE, TEST_TYPE>(
        input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }
}

template <typename TEST_TYPE>
void test_multiplication_float(const TestConfig &config) {
  const ArithmeticOperator op = ArithmeticOperator::multiplication;
  using host_type = typename TEST_TYPE::host_type;

  SECTION("multiply by float") {
    const std::vector<host_type> input_a(config.work_size(), host_type(2.3));
    const std::vector<host_type> input_b(config.work_size(), host_type(1.4));
    const std::vector<host_type> output = test<TEST_TYPE, TEST_TYPE, TEST_TYPE>(
        input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }

  // TODO: Handle ULP comparisons
  // TODO: Handle NaN comparisons
}

template <typename TEST_TYPE>
void test_multiplication_vector(const TestConfig &config) {
  const ArithmeticOperator op = ArithmeticOperator::multiplication;
  using host_type = typename TEST_TYPE::host_type;
  using scalar_type = typename TEST_TYPE::scalar_type;
  using host_scalar_type = typename scalar_type::host_type;

  SECTION("multiply by scalar rhs") {
    const std::vector<host_type> input_a(config.work_size(), host_type(4));
    const std::vector<host_scalar_type> input_b(config.work_size(),
                                                host_scalar_type(2));
    const std::vector<host_type> output =
        test<TEST_TYPE, TEST_TYPE, scalar_type>(
            input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }

  SECTION("multiply by scalar lhs") {
    const std::vector<host_scalar_type> input_a(config.work_size(),
                                                host_scalar_type(2));
    const std::vector<host_type> input_b(config.work_size(), host_type(4));
    const std::vector<host_type> output =
        test<TEST_TYPE, scalar_type, TEST_TYPE>(
            input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }

  SECTION("non-uniform vector") {
    std::vector<host_scalar_type> scalar_a(host_type::vector_size);
    std::vector<host_scalar_type> scalar_b(host_type::vector_size);
    std::iota(scalar_a.begin(), scalar_a.end(), host_scalar_type(4));
    std::iota(scalar_b.begin(), scalar_b.end(), host_scalar_type(2));
    const std::vector<host_type> input_a(config.work_size(),
                                         host_type(scalar_a));
    const std::vector<host_type> input_b(config.work_size(),
                                         host_type(scalar_b));
    const std::vector<host_type> output = test<TEST_TYPE, TEST_TYPE, TEST_TYPE>(
        input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("arithmetic operators - multiplication", "",
                                    ca::SignedTypes, test_name<TestType>) {
  const TestConfig &config = get_test_config();

  ca::Requirements requirements;
  requirements.arithmetic_type<typename TestType::scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }
  test_multiplication_common<TestType>(config);
  test_multiplication_signed<TestType>(config);
  if constexpr (ca::is_vector_v<typename TestType::host_type>) {
    test_multiplication_vector<TestType>(config);
  }
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("arithmetic operators - multiplication", "",
                                    ca::UnsignedTypes, test_name<TestType>) {
  const TestConfig &config = get_test_config();

  ca::Requirements requirements;
  requirements.arithmetic_type<typename TestType::scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  test_multiplication_common<TestType>(config);
  test_multiplication_unsigned<TestType>(config);
  if constexpr (ca::is_vector_v<typename TestType::host_type>) {
    test_multiplication_vector<TestType>(config);
  }
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("arithmetic operators - multiplication", "",
                                    float_types, test_name<TestType>) {
  const TestConfig &config = get_test_config();

  ca::Requirements requirements;
  requirements.arithmetic_type<typename TestType::scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  test_multiplication_common<TestType>(config);
  test_multiplication_signed<TestType>(config);
  test_multiplication_float<TestType>(config);
  if constexpr (ca::is_vector_v<typename TestType::host_type>) {
    test_multiplication_vector<TestType>(config);
  }
}

template <typename TEST_TYPE>
void test_division_common(const TestConfig &config) {
  const ArithmeticOperator op = ArithmeticOperator::division;
  using host_type = typename TEST_TYPE::host_type;

  SECTION("divide by itself") {
    const std::vector<host_type> input_a(config.work_size(), host_type(5));
    const std::vector<host_type> input_b(config.work_size(), host_type(5));
    const std::vector<host_type> output = test<TEST_TYPE, TEST_TYPE, TEST_TYPE>(
        input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }

  SECTION("divide by one") {
    const std::vector<host_type> input_a(config.work_size(), host_type(5));
    const std::vector<host_type> input_b(config.work_size(), host_type(1));
    const std::vector<host_type> output = test<TEST_TYPE, TEST_TYPE, TEST_TYPE>(
        input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }

  SECTION("divide by positive") {
    const std::vector<host_type> input_a(config.work_size(), host_type(10));
    const std::vector<host_type> input_b(config.work_size(), host_type(2));
    const std::vector<host_type> output = test<TEST_TYPE, TEST_TYPE, TEST_TYPE>(
        input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }

  SECTION("divide with remainder") {
    const std::vector<host_type> input_a(config.work_size(), host_type(5));
    const std::vector<host_type> input_b(config.work_size(), host_type(2));
    const std::vector<host_type> output = test<TEST_TYPE, TEST_TYPE, TEST_TYPE>(
        input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }

  SECTION("non-uniform buffer") {
    std::vector<host_type> input_a(config.work_size());
    std::vector<host_type> input_b(config.work_size());
    std::iota(input_a.begin(), input_a.end(), host_type(2));
    std::iota(input_b.begin(), input_b.end(), host_type(1));
    const std::vector<host_type> output = test<TEST_TYPE, TEST_TYPE, TEST_TYPE>(
        input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }

  SECTION("random") {
    const std::vector<host_type> input_a =
        generate_non_zero_vector<host_type>(config.work_size(), 0);
    const std::vector<host_type> input_b =
        generate_non_zero_vector<host_type>(config.work_size(), 0);
    const std::vector<host_type> output = test<TEST_TYPE, TEST_TYPE, TEST_TYPE>(
        input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }
}

template <typename TEST_TYPE>
void test_division_signed(const TestConfig &config) {
  const ArithmeticOperator op = ArithmeticOperator::division;
  using host_type = typename TEST_TYPE::host_type;

  SECTION("divide by negative") {
    const std::vector<host_type> input_a(config.work_size(), host_type(6));
    const std::vector<host_type> input_b(config.work_size(), host_type(-2));
    const std::vector<host_type> output = test<TEST_TYPE, TEST_TYPE, TEST_TYPE>(
        input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }
}

template <typename TEST_TYPE>
void test_division_float(const TestConfig &config,
                         const ArithmeticOperator op) {
  using host_type = typename TEST_TYPE::host_type;

  SECTION("divide by float") {
    const std::vector<host_type> input_a(config.work_size(), host_type(2.3));
    const std::vector<host_type> input_b(config.work_size(), host_type(1.4));
    const std::vector<host_type> output = test<TEST_TYPE, TEST_TYPE, TEST_TYPE>(
        input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }

  SECTION("divide one by max") {
    const std::vector<host_type> input_a(config.work_size(), host_type(1.0));
    const std::vector<host_type> input_b(
        config.work_size(), host_type(std::numeric_limits<host_type>::max()));
    const std::vector<host_type> output = test<TEST_TYPE, TEST_TYPE, TEST_TYPE>(
        input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }

  // TODO: Handle ULP comparisons
  // TODO: Handle NaN comparisons
}

template <typename TEST_TYPE>
void test_division_vector(const TestConfig &config,
                          const ArithmeticOperator op) {
  using host_type = typename TEST_TYPE::host_type;
  using scalar_type = typename TEST_TYPE::scalar_type;
  using host_scalar_type = typename scalar_type::host_type;

  SECTION("divide by scalar rhs") {
    const std::vector<host_type> input_a(config.work_size(), host_type(4));
    const std::vector<host_scalar_type> input_b(config.work_size(),
                                                host_scalar_type(2));
    const std::vector<host_type> output =
        test<TEST_TYPE, TEST_TYPE, scalar_type>(
            input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }

  SECTION("divide by scalar lhs") {
    const std::vector<host_scalar_type> input_a(config.work_size(),
                                                host_scalar_type(2));
    const std::vector<host_type> input_b(config.work_size(), host_type(4));
    const std::vector<host_type> output =
        test<TEST_TYPE, scalar_type, TEST_TYPE>(
            input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }

  SECTION("non-uniform vector") {
    std::vector<host_scalar_type> scalar_a(host_type::vector_size);
    std::vector<host_scalar_type> scalar_b(host_type::vector_size);
    std::iota(scalar_a.begin(), scalar_a.end(), host_scalar_type(4));
    std::iota(scalar_b.begin(), scalar_b.end(), host_scalar_type(2));
    const std::vector<host_type> input_a(config.work_size(),
                                         host_type(scalar_a));
    const std::vector<host_type> input_b(config.work_size(),
                                         host_type(scalar_b));
    const std::vector<host_type> output = test<TEST_TYPE, TEST_TYPE, TEST_TYPE>(
        input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("arithmetic operators - division", "",
                                    ca::SignedTypes, test_name<TestType>) {
  const TestConfig &config = get_test_config();

  ca::Requirements requirements;
  requirements.arithmetic_type<typename TestType::scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  test_division_common<TestType>(config);
  test_division_signed<TestType>(config);
  if constexpr (ca::is_vector_v<typename TestType::host_type>) {
    test_division_vector<TestType>(config, ArithmeticOperator::division);
  }
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("arithmetic operators - division", "",
                                    ca::UnsignedTypes, test_name<TestType>) {
  const TestConfig &config = get_test_config();

  ca::Requirements requirements;
  requirements.arithmetic_type<typename TestType::scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  test_division_common<TestType>(config);
  if constexpr (ca::is_vector_v<typename TestType::host_type>) {
    test_division_vector<TestType>(config, ArithmeticOperator::division);
  }
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("arithmetic operators - division", "",
                                    float_types, test_name<TestType>) {
  const TestConfig &config = get_test_config();

  ca::Requirements requirements;
  requirements.arithmetic_type<typename TestType::scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  test_division_common<TestType>(config);
  test_division_signed<TestType>(config);
  test_division_float<TestType>(config, ArithmeticOperator::division);
  if constexpr (ca::is_vector_v<typename TestType::host_type>) {
    test_division_vector<TestType>(config, ArithmeticOperator::division);
  }
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "arithmetic operators - correctly rounded division", "", ca::TypesFloat,
    test_name<TestType>) {
  const TestConfig &config = get_test_config();

  ca::Requirements requirements;
  requirements.arithmetic_type<typename TestType::scalar_type>();
  requirements.correctly_rounded_divide_sqrt<typename TestType::scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  test_division_float<TestType>(config,
                                ArithmeticOperator::correctly_rounded_division);
  if constexpr (ca::is_vector_v<typename TestType::host_type>) {
    test_division_vector<TestType>(
        config, ArithmeticOperator::correctly_rounded_division);
  }
}

template <typename TEST_TYPE>
void test_remainder_common(const TestConfig &config) {
  const ArithmeticOperator op = ArithmeticOperator::remainder;
  using host_type = typename TEST_TYPE::host_type;

  SECTION("divide by itself") {
    const std::vector<host_type> input_a(config.work_size(), host_type(5));
    const std::vector<host_type> input_b(config.work_size(), host_type(5));
    const std::vector<host_type> output = test<TEST_TYPE, TEST_TYPE, TEST_TYPE>(
        input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }

  SECTION("divide by power") {
    const std::vector<host_type> input_a(config.work_size(), host_type(5));
    const std::vector<host_type> input_b(config.work_size(), host_type(10));
    const std::vector<host_type> output = test<TEST_TYPE, TEST_TYPE, TEST_TYPE>(
        input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }

  SECTION("divide by positive") {
    const std::vector<host_type> input_a(config.work_size(), host_type(5));
    const std::vector<host_type> input_b(config.work_size(), host_type(2));
    const std::vector<host_type> output = test<TEST_TYPE, TEST_TYPE, TEST_TYPE>(
        input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }

  SECTION("non-uniform buffer") {
    std::vector<host_type> input_a(config.work_size());
    std::vector<host_type> input_b(config.work_size());
    std::iota(input_a.begin(), input_a.end(), host_type(2));
    std::iota(input_b.begin(), input_b.end(), host_type(1));
    const std::vector<host_type> output = test<TEST_TYPE, TEST_TYPE, TEST_TYPE>(
        input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }

  SECTION("random") {
    const std::vector<host_type> input_a =
        generate_non_zero_vector<host_type>(config.work_size(), 0);
    const std::vector<host_type> input_b =
        generate_non_zero_vector<host_type>(config.work_size(), 0);
    const std::vector<host_type> output = test<TEST_TYPE, TEST_TYPE, TEST_TYPE>(
        input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }
}

template <typename TEST_TYPE>
void test_remainder_signed(const TestConfig &config) {
  const ArithmeticOperator op = ArithmeticOperator::remainder;
  using host_type = typename TEST_TYPE::host_type;

  SECTION("divide by negative") {
    const std::vector<host_type> input_a(config.work_size(), host_type(6));
    const std::vector<host_type> input_b(config.work_size(), host_type(-2));
    const std::vector<host_type> output = test<TEST_TYPE, TEST_TYPE, TEST_TYPE>(
        input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }
}

template <typename TEST_TYPE>
void test_remainder_float(const TestConfig &config) {
  const ArithmeticOperator op = ArithmeticOperator::remainder;
  using host_type = typename TEST_TYPE::host_type;

  SECTION("divide by float") {
    const std::vector<host_type> input_a(config.work_size(), host_type(2.3));
    const std::vector<host_type> input_b(config.work_size(), host_type(1.4));
    const std::vector<host_type> output = test<TEST_TYPE, TEST_TYPE, TEST_TYPE>(
        input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }

  // TODO: Handle ULP comparisons
  // TODO: Handle NaN comparisons
}

template <typename TEST_TYPE>
void test_remainder_vector(const TestConfig &config) {
  const ArithmeticOperator op = ArithmeticOperator::remainder;
  using host_type = typename TEST_TYPE::host_type;
  using scalar_type = typename TEST_TYPE::scalar_type;
  using host_scalar_type = typename scalar_type::host_type;

  SECTION("divide by scalar rhs") {
    const std::vector<host_type> input_a(config.work_size(), host_type(4));
    const std::vector<host_scalar_type> input_b(config.work_size(),
                                                host_scalar_type(7));
    const std::vector<host_type> output =
        test<TEST_TYPE, TEST_TYPE, scalar_type>(
            input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }

  SECTION("divide by scalar lhs") {
    const std::vector<host_scalar_type> input_a(config.work_size(),
                                                host_scalar_type(7));
    const std::vector<host_type> input_b(config.work_size(), host_type(4));
    const std::vector<host_type> output =
        test<TEST_TYPE, scalar_type, TEST_TYPE>(
            input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }

  SECTION("non-uniform vector") {
    std::vector<host_scalar_type> scalar_a(host_type::vector_size);
    std::vector<host_scalar_type> scalar_b(host_type::vector_size);
    std::iota(scalar_a.begin(), scalar_a.end(), host_scalar_type(1));
    std::iota(scalar_b.begin(), scalar_b.end(), host_scalar_type(2));
    const std::vector<host_type> input_a(config.work_size(),
                                         host_type(scalar_a));
    const std::vector<host_type> input_b(config.work_size(),
                                         host_type(scalar_b));
    const std::vector<host_type> output = test<TEST_TYPE, TEST_TYPE, TEST_TYPE>(
        input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_type> reference =
        get_reference<host_type>(input_a, input_b, op);
    compare(output, reference);
  }
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("arithmetic operators - remainder", "",
                                    ca::SignedTypes, test_name<TestType>) {
  const TestConfig &config = get_test_config();

  ca::Requirements requirements;
  requirements.arithmetic_type<typename TestType::scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  test_remainder_common<TestType>(config);
  test_remainder_signed<TestType>(config);
  if constexpr (ca::is_vector_v<typename TestType::host_type>) {
    test_remainder_vector<TestType>(config);
  }
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("arithmetic operators - remainder", "",
                                    ca::UnsignedTypes, test_name<TestType>) {
  const TestConfig &config = get_test_config();

  ca::Requirements requirements;
  requirements.arithmetic_type<typename TestType::scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  test_remainder_common<TestType>(config);
  if constexpr (ca::is_vector_v<typename TestType::host_type>) {
    test_remainder_vector<TestType>(config);
  }
}

} // namespace
