/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <array>
#include <cassian/catch2_utils/catch2_utils.hpp>
#include <cassian/cli/cli.hpp>
#include <cassian/logging/logging.hpp>
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
#include <utility>
#include <vector>

namespace ca = cassian;

namespace {

enum class RelationalOperator {
  greater_than,
  less_than,
  greater_than_or_equal,
  less_than_or_equal
};

std::string to_clc_operator(const RelationalOperator op) {
  switch (op) {
  case RelationalOperator::greater_than:
    return ">";
  case RelationalOperator::less_than:
    return "<";
  case RelationalOperator::greater_than_or_equal:
    return ">=";
  case RelationalOperator::less_than_or_equal:
    return "<=";
  default:
    throw UnknownOperatorException("Unknown operator: " +
                                   std::to_string(static_cast<int>(op)));
  }
}

template <typename OUTPUT_TYPE, typename A_TYPE, typename B_TYPE,
          typename ca::EnableIfIsIntegral<OUTPUT_TYPE> = 0>
std::function<OUTPUT_TYPE(A_TYPE, B_TYPE)>
to_function(const RelationalOperator op) {
  switch (op) {
  case RelationalOperator::greater_than:
    return [](A_TYPE a, B_TYPE b) {
      return a > b ? OUTPUT_TYPE(1) : OUTPUT_TYPE(0);
    };
    break;
  case RelationalOperator::less_than:
    return [](A_TYPE a, B_TYPE b) {
      return a < b ? OUTPUT_TYPE(1) : OUTPUT_TYPE(0);
    };
    break;
  case RelationalOperator::greater_than_or_equal:
    return [](A_TYPE a, B_TYPE b) {
      return a >= b ? OUTPUT_TYPE(1) : OUTPUT_TYPE(0);
    };
    break;
  case RelationalOperator::less_than_or_equal:
    return [](A_TYPE a, B_TYPE b) {
      return a <= b ? OUTPUT_TYPE(1) : OUTPUT_TYPE(0);
    };
    break;
  default:
    throw UnknownOperatorException("Unknown operator: " +
                                   std::to_string(static_cast<int>(op)));
  }
}

template <typename OUTPUT_TYPE, typename A_TYPE, typename B_TYPE,
          typename ca::EnableIfIsFloatingPoint<OUTPUT_TYPE> = 0>
std::function<OUTPUT_TYPE(A_TYPE, B_TYPE)>
to_function(const RelationalOperator op) {
  switch (op) {
  case RelationalOperator::greater_than:
    return [](A_TYPE a, B_TYPE b) {
      if (std::isnan(a) || std::isnan(b)) {
        return OUTPUT_TYPE(0);
      }
      return a > b ? OUTPUT_TYPE(1) : OUTPUT_TYPE(0);
    };
    break;
  case RelationalOperator::less_than:
    return [](A_TYPE a, B_TYPE b) {
      if (std::isnan(a) || std::isnan(b)) {
        return OUTPUT_TYPE(0);
      }
      return a < b ? OUTPUT_TYPE(1) : OUTPUT_TYPE(0);
    };
    break;
  case RelationalOperator::greater_than_or_equal:
    return [](A_TYPE a, B_TYPE b) {
      if (std::isnan(a) || std::isnan(b)) {
        return OUTPUT_TYPE(0);
      }
      return a >= b ? OUTPUT_TYPE(1) : OUTPUT_TYPE(0);
    };
    break;
  case RelationalOperator::less_than_or_equal:
    return [](A_TYPE a, B_TYPE b) {
      if (std::isnan(a) || std::isnan(b)) {
        return OUTPUT_TYPE(0);
      }
      return a <= b ? OUTPUT_TYPE(1) : OUTPUT_TYPE(0);
    };
    break;
  default:
    throw UnknownOperatorException("Unknown operator: " +
                                   std::to_string(static_cast<int>(op)));
  }
}

template <typename OUTPUT_TYPE, typename A_TYPE, typename B_TYPE,
          typename ca::EnableIfIsVector<OUTPUT_TYPE> = 0>
std::function<OUTPUT_TYPE(A_TYPE, B_TYPE)>
to_function(const RelationalOperator op) {
  using output_scalar_type = ca::scalar_type_v<OUTPUT_TYPE>;
  using a_scalar_type = ca::scalar_type_v<A_TYPE>;
  using b_scalar_type = ca::scalar_type_v<B_TYPE>;
  return [op](A_TYPE a, B_TYPE b) {
    return ca::component_wise_compare<OUTPUT_TYPE>(
        to_function<output_scalar_type, a_scalar_type, b_scalar_type>(op), a,
        b);
  };
}

template <typename OUTPUT_TYPE, typename A_TYPE, typename B_TYPE>
std::string get_build_options(const RelationalOperator op) {
  const std::string clc_a_type = A_TYPE::device_type;
  const std::string clc_b_type = B_TYPE::device_type;
  const std::string clc_output_type = OUTPUT_TYPE::device_type;
  const std::string clc_operator = to_clc_operator(op);
  std::string build_options =
      " -D A_TYPE=" + clc_a_type + " -D B_TYPE=" + clc_b_type +
      " -D OUTPUT_TYPE=" + clc_output_type + " -D OPERATOR=" + clc_operator;
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
     const std::vector<B_HOST_TYPE> &input_b, const RelationalOperator op,
     ca::Runtime *runtime, const std::string &program_type) {
  const std::string build_options =
      get_build_options<OUTPUT_TYPE, A_TYPE, B_TYPE>(op);
  const ca::Kernel kernel =
      create_kernel("kernels/oclc_operators/relational_operators.cl",
                    build_options, runtime, program_type);
  std::vector<OUTPUT_HOST_TYPE> output =
      run_kernel<OUTPUT_HOST_TYPE>(kernel, input_a, input_b, runtime);
  runtime->release_kernel(kernel);
  return output;
}

template <typename OUTPUT_TYPE, typename A_TYPE, typename B_TYPE>
std::vector<OUTPUT_TYPE> get_reference(const std::vector<A_TYPE> &input_a,
                                       const std::vector<B_TYPE> &input_b,
                                       const RelationalOperator op) {
  const auto reference_function = to_function<OUTPUT_TYPE, A_TYPE, B_TYPE>(op);
  std::vector<OUTPUT_TYPE> output(input_a.size());
  for (size_t i = 0; i < output.size(); ++i) {
    output[i] = reference_function(input_a[i], input_b[i]);
  }
  return output;
}

template <typename TEST_TYPE>
void test_common(const TestConfig &config, const RelationalOperator op) {
  using host_type = typename TEST_TYPE::host_type;
  using scalar_host_type = typename TEST_TYPE::scalar_type::host_type;
  using output_type = typename TEST_TYPE::logical_type;
  using host_output_type = typename output_type::host_type;

  using pair = std::pair<scalar_host_type, scalar_host_type>;

  const std::array<pair, 3> test_values = {
      {pair{5, 5}, pair{5, 2}, pair{2, 3}}};

  SECTION("specific values - uniform buffers") {
    auto value = GENERATE_REF(from_range(test_values));
    const std::vector<host_type> input_a(config.work_size(),
                                         host_type(value.first));
    const std::vector<host_type> input_b(config.work_size(),
                                         host_type(value.second));
    const std::vector<host_output_type> output =
        test<output_type, TEST_TYPE, TEST_TYPE>(
            input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_output_type> reference =
        get_reference<host_output_type>(input_a, input_b, op);
    compare(output, reference);
  }

  SECTION("random") {
    const std::vector<host_type> input_a =
        ca::generate_vector<host_type>(config.work_size(), 0);
    const std::vector<host_type> input_b =
        ca::generate_vector<host_type>(config.work_size(), 0);
    const std::vector<host_output_type> output =
        test<output_type, TEST_TYPE, TEST_TYPE>(
            input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_output_type> reference =
        get_reference<host_output_type>(input_a, input_b, op);
    compare(output, reference);
  }
}

template <typename TEST_TYPE>
void test_float(const TestConfig &config, const RelationalOperator op) {
  using host_type = typename TEST_TYPE::host_type;
  using scalar_host_type = typename TEST_TYPE::scalar_type::host_type;
  using output_type = typename TEST_TYPE::logical_type;
  using host_output_type = typename output_type::host_type;

  using pair = std::pair<scalar_host_type, scalar_host_type>;

  const std::array<pair, 4> test_values = {
      {pair{5.0, 5.0}, pair{5.0, 5.7}, pair{2.0, 2.3},
       pair{std::numeric_limits<scalar_host_type>::quiet_NaN(), -1.0}}};

  SECTION("specific values - uniform buffers") {
    auto value = GENERATE_REF(from_range(test_values));
    const std::vector<host_type> input_a(config.work_size(),
                                         host_type(value.first));
    const std::vector<host_type> input_b(config.work_size(),
                                         host_type(value.second));
    const std::vector<host_output_type> output =
        test<output_type, TEST_TYPE, TEST_TYPE>(
            input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_output_type> reference =
        get_reference<host_output_type>(input_a, input_b, op);
    compare(output, reference);
  }

  SECTION("random") {
    const std::vector<host_type> input_a =
        ca::generate_vector<host_type>(config.work_size(), 0);
    const std::vector<host_type> input_b =
        ca::generate_vector<host_type>(config.work_size(), 0);
    const std::vector<host_output_type> output =
        test<output_type, TEST_TYPE, TEST_TYPE>(
            input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_output_type> reference =
        get_reference<host_output_type>(input_a, input_b, op);
    compare(output, reference);
  }
}

template <typename TEST_TYPE>
void test_vector(const TestConfig &config, const RelationalOperator op) {
  using host_type = typename TEST_TYPE::host_type;
  using output_type = typename TEST_TYPE::logical_type;
  using host_output_type = typename output_type::host_type;
  using scalar_type = typename TEST_TYPE::scalar_type;
  using host_scalar_type = typename scalar_type::host_type;

  SECTION("operator with scalar rhs") {
    const std::vector<host_type> input_a(config.work_size(), host_type(4));
    const std::vector<host_scalar_type> input_b(config.work_size(),
                                                host_scalar_type(4));
    const std::vector<host_output_type> output =
        test<output_type, TEST_TYPE, scalar_type>(
            input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_output_type> reference =
        get_reference<host_output_type>(input_a, input_b, op);
    compare(output, reference);
  }

  SECTION("operator with scalar lhs") {
    const std::vector<host_scalar_type> input_a(config.work_size(),
                                                host_scalar_type(4));
    const std::vector<host_type> input_b(config.work_size(), host_type(4));
    const std::vector<host_output_type> output =
        test<output_type, scalar_type, TEST_TYPE>(
            input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_output_type> reference =
        get_reference<host_output_type>(input_a, input_b, op);
    compare(output, reference);
  }

  SECTION("non-uniform vector") {
    std::vector<host_scalar_type> scalar_a(host_type::vector_size);
    std::iota(scalar_a.begin(), scalar_a.end(), host_scalar_type(1));
    const std::vector<host_type> input_a(config.work_size(),
                                         host_type(scalar_a));
    const std::vector<host_output_type> output =
        test<output_type, TEST_TYPE, TEST_TYPE>(
            input_a, input_a, op, config.runtime(), config.program_type());
    const std::vector<host_output_type> reference =
        get_reference<host_output_type>(input_a, input_a, op);
    compare(output, reference);
  }

  SECTION("operator with part of vector being the same") {
    std::vector<host_scalar_type> scalar_a(host_type::vector_size);
    std::vector<host_scalar_type> scalar_b(host_type::vector_size);
    std::iota(scalar_a.begin(), scalar_a.end(), host_scalar_type(1));
    std::iota(scalar_b.begin(), scalar_b.end(), host_scalar_type(2));
    scalar_a[0] = 2;
    scalar_b[0] = scalar_a[0];
    const std::vector<host_type> input_a(config.work_size(),
                                         host_type(scalar_a));
    const std::vector<host_type> input_b(config.work_size(),
                                         host_type(scalar_b));
    const std::vector<host_output_type> output =
        test<output_type, TEST_TYPE, TEST_TYPE>(
            input_a, input_b, op, config.runtime(), config.program_type());
    const std::vector<host_output_type> reference =
        get_reference<host_output_type>(input_a, input_b, op);
    compare(output, reference);
  }
}

template <typename TestType> auto test_name() {
  return std::string(TestType::type_name);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("relational operators - less than", "",
                                    ca::UnsignedTypes, test_name<TestType>) {
  const TestConfig &config = get_test_config();

  ca::Requirements requirements;
  requirements.arithmetic_type<typename TestType::scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  test_common<TestType>(config, RelationalOperator::less_than);
  if constexpr (ca::is_vector_v<typename TestType::host_type>) {
    test_vector<TestType>(config, RelationalOperator::less_than);
  }
}

using float_types = ca::TupleConcat<ca::TypesFloat, ca::TypesDouble>::type;

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("relational operators - less than", "",
                                    float_types, test_name<TestType>) {
  const TestConfig &config = get_test_config();

  ca::Requirements requirements;
  requirements.arithmetic_type<typename TestType::scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  test_common<TestType>(config, RelationalOperator::less_than);
  test_float<TestType>(config, RelationalOperator::less_than);
  if constexpr (ca::is_vector_v<typename TestType::host_type>) {
    test_vector<TestType>(config, RelationalOperator::less_than);
  }
}

} // namespace