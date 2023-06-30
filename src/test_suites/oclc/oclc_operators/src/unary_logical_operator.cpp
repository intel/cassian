/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

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
#include <numeric>
#include <string>
#include <test_config.hpp>
#include <vector>

namespace ca = cassian;

namespace {

template <typename OUTPUT_TYPE, typename INPUT_TYPE,
          typename ca::EnableIfIsScalar<OUTPUT_TYPE> = 0>
std::function<OUTPUT_TYPE(INPUT_TYPE)> to_function() {
  return
      [](INPUT_TYPE input) { return !input ? OUTPUT_TYPE(1) : OUTPUT_TYPE(0); };
}

template <typename OUTPUT_TYPE, typename INPUT_TYPE,
          typename ca::EnableIfIsVector<OUTPUT_TYPE> = 0>
std::function<OUTPUT_TYPE(INPUT_TYPE)> to_function() {
  return [](INPUT_TYPE input) {
    return ca::not_component_wise<OUTPUT_TYPE>(input);
  };
}

template <typename OUTPUT_TYPE, typename INPUT_TYPE>
std::string get_build_options() {
  const std::string clc_input_type = INPUT_TYPE::device_type;
  const std::string clc_output_type = OUTPUT_TYPE::device_type;
  std::string build_options =
      " -D INPUT_TYPE=" + clc_input_type + " -D OUTPUT_TYPE=" + clc_output_type;
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

template <typename OUTPUT_TYPE, typename INPUT_TYPE,
          typename OUTPUT_HOST_TYPE = typename OUTPUT_TYPE::host_type,
          typename INPUT_HOST_TYPE = typename INPUT_TYPE::host_type>
std::vector<OUTPUT_HOST_TYPE> test(const std::vector<INPUT_HOST_TYPE> &input,
                                   ca::Runtime *runtime,
                                   const std::string &program_type) {
  const std::string build_options =
      get_build_options<OUTPUT_TYPE, INPUT_TYPE>();
  const ca::Kernel kernel =
      create_kernel("kernels/oclc_operators/unary_logical_operator.cl",
                    build_options, runtime, program_type);
  std::vector<OUTPUT_HOST_TYPE> output =
      run_kernel<OUTPUT_HOST_TYPE>(kernel, input, runtime);
  runtime->release_kernel(kernel);
  return output;
}

template <typename OUTPUT_TYPE, typename INPUT_TYPE>
std::vector<OUTPUT_TYPE> get_reference(const std::vector<INPUT_TYPE> &input) {
  const auto reference_function = to_function<OUTPUT_TYPE, INPUT_TYPE>();
  std::vector<OUTPUT_TYPE> output(input.size());
  for (size_t i = 0; i < output.size(); ++i) {
    output[i] = reference_function(input[i]);
  }
  return output;
}

template <typename TEST_TYPE> void test_not_common(const TestConfig &config) {
  using host_type = typename TEST_TYPE::host_type;
  using output_type = typename TEST_TYPE::logical_type;
  using host_output_type = typename output_type::host_type;

  SECTION("not with zero") {
    const std::vector<host_type> input(config.work_size(), host_type(0));
    const std::vector<host_output_type> output = test<output_type, TEST_TYPE>(
        input, config.runtime(), config.program_type());
    const std::vector<host_output_type> reference =
        get_reference<host_output_type>(input);
    compare(output, reference);
  }

  SECTION("not with positive value") {
    const std::vector<host_type> input(config.work_size(), host_type(5));
    const std::vector<host_output_type> output = test<output_type, TEST_TYPE>(
        input, config.runtime(), config.program_type());
    const std::vector<host_output_type> reference =
        get_reference<host_output_type>(input);
    compare(output, reference);
  }

  SECTION("non-uniform buffer") {
    std::vector<host_type> input(config.work_size());
    std::iota(input.begin(), input.end(), host_type(0));
    const std::vector<host_output_type> output = test<output_type, TEST_TYPE>(
        input, config.runtime(), config.program_type());
    const std::vector<host_output_type> reference =
        get_reference<host_output_type>(input);
    compare(output, reference);
  }

  SECTION("random") {
    const std::vector<host_type> input =
        ca::generate_vector<host_type>(config.work_size(), 0);
    const std::vector<host_output_type> output = test<output_type, TEST_TYPE>(
        input, config.runtime(), config.program_type());
    const std::vector<host_output_type> reference =
        get_reference<host_output_type>(input);
    compare(output, reference);
  }
}

template <typename TEST_TYPE> void test_not_signed(const TestConfig &config) {
  using host_type = typename TEST_TYPE::host_type;
  using output_type = typename TEST_TYPE::logical_type;
  using host_output_type = typename output_type::host_type;

  SECTION("not with negative value") {
    const std::vector<host_type> input(config.work_size(), host_type(-1));
    const std::vector<host_output_type> output = test<output_type, TEST_TYPE>(
        input, config.runtime(), config.program_type());
    const std::vector<host_output_type> reference =
        get_reference<host_output_type>(input);
    compare(output, reference);
  }
}

template <typename TEST_TYPE> void test_not_float(const TestConfig &config) {
  using host_type = typename TEST_TYPE::host_type;
  using output_type = typename TEST_TYPE::logical_type;
  using host_output_type = typename output_type::host_type;

  SECTION("not with float") {
    const std::vector<host_type> input(config.work_size(), host_type(3.3));
    const std::vector<host_output_type> output = test<output_type, TEST_TYPE>(
        input, config.runtime(), config.program_type());
    const std::vector<host_output_type> reference =
        get_reference<host_output_type>(input);
    compare(output, reference);
  }

  // TODO: Handle ULP comparisons
  // TODO: Handle NaN comparisons
}

template <typename TEST_TYPE> void test_not_vector(const TestConfig &config) {
  using host_type = typename TEST_TYPE::host_type;
  using output_type = typename TEST_TYPE::logical_type;
  using host_output_type = typename output_type::host_type;
  using scalar_type = typename TEST_TYPE::scalar_type;
  using host_scalar_type = typename scalar_type::host_type;

  SECTION("non-uniform vector") {
    std::vector<host_scalar_type> scalar(host_type::vector_size);
    std::iota(scalar.begin(), scalar.end(), host_scalar_type(1));
    const std::vector<host_type> input(config.work_size(), host_type(scalar));
    const std::vector<host_output_type> output = test<output_type, TEST_TYPE>(
        input, config.runtime(), config.program_type());
    const std::vector<host_output_type> reference =
        get_reference<host_output_type>(input);
    compare(output, reference);
  }

  SECTION("not part of vector") {
    std::vector<host_scalar_type> scalar(host_type::vector_size);
    std::iota(scalar.begin(), scalar.end(), host_scalar_type(0));
    const std::vector<host_type> input(config.work_size(), host_type(scalar));
    const std::vector<host_output_type> output = test<output_type, TEST_TYPE>(
        input, config.runtime(), config.program_type());
    const std::vector<host_output_type> reference =
        get_reference<host_output_type>(input);
    compare(output, reference);
  }
}

template <typename TestType> auto test_name() {
  return std::string(TestType::type_name);
}

using float_types = ca::TupleConcat<ca::TypesFloat, ca::TypesDouble>::type;

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("unary logical operator", "",
                                    ca::UnsignedTypes, test_name<TestType>) {
  const TestConfig &config = get_test_config();

  ca::Requirements requirements;
  requirements.arithmetic_type<typename TestType::scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  test_not_common<TestType>(config);
  if constexpr (ca::is_vector_v<typename TestType::host_type>) {
    test_not_vector<TestType>(config);
  }
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("unary logical operator", "",
                                    ca::SignedTypes, test_name<TestType>) {
  const TestConfig &config = get_test_config();

  ca::Requirements requirements;
  requirements.arithmetic_type<typename TestType::scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  test_not_common<TestType>(config);
  test_not_signed<TestType>(config);
  if constexpr (ca::is_vector_v<typename TestType::host_type>) {
    test_not_vector<TestType>(config);
  }
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("unary logical operator", "", float_types,
                                    test_name<TestType>) {
  const TestConfig &config = get_test_config();

  ca::Requirements requirements;
  requirements.arithmetic_type<typename TestType::scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  test_not_common<TestType>(config);
  test_not_signed<TestType>(config);
  test_not_float<TestType>(config);
  if constexpr (ca::is_vector_v<typename TestType::host_type>) {
    test_not_vector<TestType>(config);
  }
}

} // namespace
