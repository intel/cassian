/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/catch2_utils/catch2_utils.hpp>
#include <cassian/logging/logging.hpp>
#include <cassian/random/random.hpp>
#include <cassian/runtime/openclc_type_tuples.hpp>
#include <cassian/runtime/openclc_types.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cassian/test_harness/test_harness.hpp>
#include <cassian/utility/metaprogramming.hpp>
#include <cassian/utility/utility.hpp>
#include <cassian/vector/vector.hpp>
#include <catch2/catch.hpp>
#include <common.hpp>
#include <cstddef>
#include <enum_definitions.hpp>
#include <geometric_functions.hpp>
#include <map>
#include <string>
#include <test_config.hpp>
#include <utility>
#include <vector>

namespace ca = cassian;

namespace {

template <typename T>
void create_and_set_buffer(const std::vector<T> &input, ca::Runtime *runtime,
                           std::vector<ca::Buffer> *buffers,
                           const ca::Kernel &kernel) {
  const auto buffer = runtime->create_buffer(input.size() * sizeof(T));
  runtime->write_buffer(buffer, input.data());
  buffers->push_back(buffer);
  runtime->set_kernel_argument(kernel, buffers->size() - 1, buffer);
}

template <typename T, typename INPUT_TYPE>
std::vector<T> run_gentype_kernel(const size_t &work_size,
                                  const std::vector<INPUT_TYPE> &input_a,
                                  const std::vector<INPUT_TYPE> &input_b,
                                  ca::Runtime *runtime,
                                  const ca::Kernel &kernel) {
  std::vector<ca::Buffer> buffers;
  const auto buffer_out = runtime->create_buffer(work_size * sizeof(T));
  buffers.push_back(buffer_out);
  runtime->set_kernel_argument(kernel, 0, buffer_out);
  create_and_set_buffer(input_a, runtime, &buffers, kernel);
  if (!input_b.empty()) {
    create_and_set_buffer(input_b, runtime, &buffers, kernel);
  }

  runtime->run_kernel(kernel, work_size);
  auto kernel_output = runtime->read_buffer_to_vector<T>(buffer_out);
  for (const auto &buffer : buffers) {
    runtime->release_buffer(buffer);
  }
  return kernel_output;
}

template <typename OUTPUT_TYPE, typename INPUT_TYPE>
std::vector<OUTPUT_TYPE> test_gentype(const std::vector<INPUT_TYPE> &input_a,
                                      const std::vector<INPUT_TYPE> &input_b,
                                      const std::string &build_options,
                                      const TestConfig &config) {
  auto *runtime = config.runtime();
  const std::string source = ca::load_text_file(
      ca::get_asset("kernels/oclc_geometric_functions/geometric_functions.cl"));
  ca::Kernel kernel = runtime->create_kernel("test", source, build_options,
                                             config.program_type());
  auto output = run_gentype_kernel<OUTPUT_TYPE, INPUT_TYPE>(
      config.work_size(), input_a, input_b, runtime, kernel);
  runtime->release_kernel(kernel);
  return output;
}

template <typename T, typename OUTPUT_TYPE, typename INPUT_TYPE>
void run_section(const T &oclc_function, const std::vector<INPUT_TYPE> &input_a,
                 const std::vector<INPUT_TYPE> &input_b,
                 const std::string &function_string, const TestConfig &config) {
  const auto build_options = oclc_function.get_build_options(function_string);
  const auto work_size = config.work_size();
  ca::logging::debug() << "Build options: " << build_options << '\n';
  ca::logging::debug() << "Input A: " << ca::to_string(input_a) << '\n';
  ca::logging::debug() << "Input B: " << ca::to_string(input_b) << '\n';
  auto reference_vector = std::vector<OUTPUT_TYPE>(work_size);
  for (auto j = 0; j < work_size; j++) {
    if (oclc_function.get_arg_num() == 2) {
      reference_vector[j] =
          oclc_function.calculate_reference(input_a[j], input_b[j]);
    } else {
      reference_vector[j] = oclc_function.calculate_reference(input_a[j]);
    }
  }
  const std::vector<OUTPUT_TYPE> result = test_gentype<OUTPUT_TYPE, INPUT_TYPE>(
      input_a, input_b, build_options, config);
  REQUIRE_THAT(result, (UlpComparator<OUTPUT_TYPE, INPUT_TYPE,
                                      cassian::scalar_type_v<INPUT_TYPE>>(
                           result, reference_vector, input_a, input_b,
                           oclc_function.get_function())));
}

template <typename T>
std::vector<T> create_input_vector(const std::vector<T> &list_of_inputs,
                                   const uint32_t &arg_index,
                                   const size_t &arg_num,
                                   const size_t &work_size) {
  if (arg_num < arg_index + 1) {
    return std::vector<T>();
  }
  std::vector<T> input(work_size);
  std::fill(input.begin(), input.end(), list_of_inputs[arg_index]);
  return input;
}

template <typename T, typename OUTPUT_TYPE, typename INPUT_TYPE = OUTPUT_TYPE>
void run_test(const std::vector<T> &list_of_functions,
              const TestConfig &config) {
  const auto work_size = config.work_size();
  for (const auto &oclc_function : list_of_functions) {
    const auto arg_num = oclc_function.get_arg_num();
    const auto function_string =
        Catch::StringMaker<Function>::convert(oclc_function.get_function());
    const auto list_of_inputs = oclc_function.get_input_values();
    for (auto i = 0; i < list_of_inputs.size(); i++) {
      const auto section_name = create_section_name(
          function_string, "random " + std::to_string(i + 1));
      SECTION(section_name) {
        auto input_a = create_input_vector<INPUT_TYPE>(list_of_inputs[i], 0,
                                                       arg_num, work_size);
        auto input_b = create_input_vector<INPUT_TYPE>(list_of_inputs[i], 1,
                                                       arg_num, work_size);
        input_a = randomize_input(input_a);
        input_b = randomize_input(input_b);
        run_section<T, OUTPUT_TYPE, INPUT_TYPE>(oclc_function, input_a, input_b,
                                                function_string, config);
      }
    }
    const auto special_input_vectors =
        oclc_function.get_special_values_vectors(work_size);
    std::vector<INPUT_TYPE> input_b;
    for (auto i = 0; i < special_input_vectors.size(); i++) {
      const auto section_name = create_section_name(
          function_string, "special " + std::to_string(i + 1));
      SECTION(section_name) {
        std::vector<INPUT_TYPE> input_a = special_input_vectors[i][0];
        if (arg_num == 2) {
          input_b = special_input_vectors[i][1];
        }
        run_section<T, OUTPUT_TYPE, INPUT_TYPE>(oclc_function, input_a, input_b,
                                                function_string, config);
      }
    }
  }
}

using FloatNTypes = ca::TupleConcat<ca::clc_float_t, ca::clc_float2_t,
                                    ca::clc_float3_t, ca::clc_float4_t>::type;
using DoubleNTypes =
    ca::TupleConcat<ca::clc_double_t, ca::clc_double2_t, ca::clc_double3_t,
                    ca::clc_double4_t>::type;
using FuncTypes = ca::TupleConcat<FloatNTypes, DoubleNTypes>::type;

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("geometric_functions", "", FuncTypes,
                                    test_name<TestType>) {
  const TestConfig &config = get_test_config();
  using scalar_type = typename TestType::scalar_type::host_type;

  ca::Requirements requirements;
  requirements.arithmetic_type<scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  using host_type = typename TestType::host_type;
  using VectorFunc = VectorOclcFunction<TestType, host_type>;
  using ScalarFunc = ScalarOclcFunction<TestType, host_type>;

  const std::initializer_list<VectorFunc> list_of_vector_functions{
      VectorFunc(Function::normalize)};
  const std::initializer_list<VectorFunc> cross_vector_function{
      VectorFunc(Function::cross)};
  const std::initializer_list<ScalarFunc> list_of_scalar_functions{
      ScalarFunc(Function::dot), ScalarFunc(Function::distance),
      ScalarFunc(Function::length)};
  run_test<VectorFunc, host_type>(list_of_vector_functions, config);
  run_test<ScalarFunc, scalar_type, host_type>(list_of_scalar_functions,
                                               config);
  if constexpr (get_vector_width<host_type>() == 3 ||
                get_vector_width<host_type>() == 4) {
    run_test<VectorFunc, host_type>(cross_vector_function, config);
  }
  const std::initializer_list<VectorFunc> list_of_vector_fast_functions{
      VectorFunc(Function::fast_normalize)};

  const std::initializer_list<ScalarFunc> list_of_scalar_fast_functions{
      ScalarFunc(Function::fast_distance), ScalarFunc(Function::fast_length)};

  if constexpr (std::is_same_v<host_type, float>) {
    run_test<VectorFunc, host_type>(list_of_vector_fast_functions, config);
    run_test<ScalarFunc, scalar_type, host_type>(list_of_scalar_fast_functions,
                                                 config);
  }
}

} // namespace
