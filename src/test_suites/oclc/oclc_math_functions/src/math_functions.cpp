/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

// Enable math defines from cmath module
#define _USE_MATH_DEFINES

#include <algorithm>
#include <cassian/catch2_utils/catch2_utils.hpp>
#include <cassian/logging/logging.hpp>
#include <cassian/random/random.hpp>
#include <cassian/runtime/openclc_type_tuples.hpp>
#include <cassian/runtime/openclc_types.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cassian/test_harness/test_harness.hpp>
#include <cassian/utility/utility.hpp>
#include <cassian/vector/type_helpers.hpp>
#include <cassian/vector/vector.hpp>
#include <catch2/catch.hpp>
#include <common.hpp>
#include <cstddef>
#include <enum_definitions.hpp>
#include <math_functions.hpp>
#include <string>
#include <test_config.hpp>
#include <vector>

namespace ca = cassian;

namespace {
constexpr std::initializer_list<Function> list_of_gentype_functions{
    Function::acos,  Function::acosh,   Function::acospi,  Function::asin,
    Function::asinh, Function::asinpi,  Function::atan,    Function::atan2,
    Function::atanh, Function::atanpi,  Function::atan2pi, Function::cbrt,
    Function::ceil,  Function::copysign};

template <typename T>
std::vector<T> run_gentype_kernel(const std::vector<T> &input,
                                  const std::vector<T> &input_values,
                                  ca::Runtime *runtime,
                                  const ca::Kernel &kernel) {
  ca::Buffer buffer_out = runtime->create_buffer(input.size() * sizeof(T));
  ca::Buffer buffer_inputs =
      runtime->create_buffer(input_values.size() * sizeof(T));
  runtime->write_buffer(buffer_inputs, input_values.data());
  runtime->set_kernel_argument(kernel, static_cast<int>(0), buffer_out);
  runtime->set_kernel_argument(kernel, static_cast<int>(1), buffer_inputs);
  runtime->run_kernel(kernel, input.size());
  auto output = runtime->read_buffer_to_vector<T>(buffer_out);
  runtime->release_buffer(buffer_out);
  runtime->release_buffer(buffer_inputs);
  return output;
}

template <typename T>
std::vector<T> test_gentype(const std::vector<T> &input_values,
                            const std::string &build_options,
                            const TestConfig &config) {
  auto *runtime = config.runtime();
  const std::vector<T> input(config.work_size());
  const std::string source = ca::load_text_file(
      ca::get_asset("kernels/oclc_math_functions/math_functions.cl"));
  ca::Kernel kernel = runtime->create_kernel("test", source, build_options,
                                             config.program_type());
  auto output = run_gentype_kernel(input, input_values, runtime, kernel);
  runtime->release_kernel(kernel);
  return output;
}

template <typename TEST_TYPE>
std::string get_build_options(const std::string &function_string,
                              const size_t &input_arg_num) {
  std::stringstream ss;
  ss << "-DTYPE=" << TEST_TYPE::device_type
     << " -DFUNCTION=" << function_string;
  if (input_arg_num == 2) {
    ss << " -DTWO_ARG";
  }
  return ss.str();
}

using GentypeTypes = ca::TupleConcat<ca::TypesFloat, ca::TypesDouble>::type;

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("gentype_math_functions", "", GentypeTypes,
                                    test_name<TestType>) {
  const TestConfig &config = get_test_config();
  const size_t work_size = config.work_size();
  using scalar_type = typename TestType::scalar_type;
  using host_type = typename TestType::host_type;
  using scalar_host_type = typename TestType::scalar_type::host_type;
  ca::Requirements requirements;
  requirements.arithmetic_type<scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }
  for (const auto &function : list_of_gentype_functions) {
    const auto function_string =
        Catch::StringMaker<Function>::convert(function);
    const auto input_values =
        get_input_values<host_type, scalar_host_type>(function);
    for (auto i = 0; i < input_values.size(); i++) {
      std::string case_name =
          i == 0 ? " Basic - random" : " Special " + std::to_string(i);
      DYNAMIC_SECTION("Function: " << function_string << case_name) {
        for (const auto &input : input_values[i]) {
          ca::logging::debug()
              << "Input value: " << ca::to_string(input) << '\n';
        }
        const std::string build_options = get_build_options<TestType>(
            function_string, input_values[i].size());
        const auto result =
            test_gentype(input_values[i], build_options, config);
        const auto reference = get_reference_value<host_type, scalar_host_type>(
            input_values[i], function);
        REQUIRE_THAT(result, UlpComparator(reference, function));
      }
    }
  }
}

} // namespace
