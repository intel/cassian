/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

// Enable math defines from cmath module
#define _USE_MATH_DEFINES

#include <cassian/catch2_utils/catch2_utils.hpp>
#include <cassian/logging/logging.hpp>
#include <cassian/random/random.hpp>
#include <cassian/runtime/openclc_type_tuples.hpp>
#include <cassian/runtime/openclc_types.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cassian/test_harness/test_harness.hpp>
#include <cassian/utility/metaprogramming.hpp>
#include <cassian/utility/utility.hpp>
#include <cassian/vector/type_helpers.hpp>
#include <cassian/vector/vector.hpp>
#include <catch2/catch.hpp>
#include <common.hpp>
#include <common_functions.hpp>
#include <cstddef>
#include <enum_definitions.hpp>
#include <initializer_list>
#include <map>
#include <string>
#include <test_config.hpp>
#include <type_traits>
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

template <typename T, typename INPUT_TYPE_1,
          typename INPUT_TYPE_2 = INPUT_TYPE_1,
          typename INPUT_TYPE_3 = INPUT_TYPE_2>
std::vector<T> run_gentype_kernel(const size_t &work_size,
                                  const std::vector<INPUT_TYPE_1> &input_a,
                                  const std::vector<INPUT_TYPE_2> &input_b,
                                  const std::vector<INPUT_TYPE_3> &input_c,
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
  if (!input_c.empty()) {
    create_and_set_buffer(input_c, runtime, &buffers, kernel);
  }

  runtime->run_kernel(kernel, work_size);
  auto kernel_output = runtime->read_buffer_to_vector<T>(buffer_out);
  for (const auto &buffer : buffers) {
    runtime->release_buffer(buffer);
  }
  return kernel_output;
}

template <typename GENTYPE, typename INPUT_TYPE_1,
          typename INPUT_TYPE_2 = INPUT_TYPE_1,
          typename INPUT_TYPE_3 = INPUT_TYPE_2>
std::vector<GENTYPE> test_gentype(const std::vector<INPUT_TYPE_1> &input_a,
                                  const std::vector<INPUT_TYPE_2> &input_b,
                                  const std::vector<INPUT_TYPE_3> &input_c,
                                  const std::string &build_options,
                                  const TestConfig &config) {
  auto *runtime = config.runtime();
  const std::string source = ca::load_text_file(
      ca::get_asset("kernels/oclc_common_functions/common_functions.cl"));
  ca::Kernel kernel = runtime->create_kernel("test", source, build_options,
                                             config.program_type());
  auto output = run_gentype_kernel<GENTYPE>(config.work_size(), input_a,
                                            input_b, input_c, runtime, kernel);
  runtime->release_kernel(kernel);
  return output;
}

template <typename T, typename GENTYPE, typename INPUT_TYPE_1,
          typename INPUT_TYPE_2 = INPUT_TYPE_1,
          typename INPUT_TYPE_3 = INPUT_TYPE_2>
void run_section(const T &oclc_function,
                 const std::vector<INPUT_TYPE_1> &input_a,
                 const std::vector<INPUT_TYPE_2> &input_b,
                 const std::vector<INPUT_TYPE_3> &input_c,
                 const std::string &function_string, const TestConfig &config) {
  const auto build_options = oclc_function.get_build_options(function_string);
  const auto work_size = config.work_size();

  auto reference_vector = std::vector<GENTYPE>(work_size);
  for (auto j = 0; j < work_size; j++) {
    if (oclc_function.get_arg_num() == 3) {
      reference_vector[j] = calculate_reference<GENTYPE, INPUT_TYPE_1,
                                                INPUT_TYPE_2, INPUT_TYPE_3>(
          input_a[j], input_b[j], input_c[j], oclc_function.get_function());
    } else if (oclc_function.get_arg_num() == 2) {
      reference_vector[j] = calculate_reference<GENTYPE, INPUT_TYPE_1,
                                                INPUT_TYPE_2, INPUT_TYPE_3>(
          input_a[j], input_b[j], INPUT_TYPE_3(0),
          oclc_function.get_function());
    } else {
      reference_vector[j] = calculate_reference<GENTYPE, INPUT_TYPE_1,
                                                INPUT_TYPE_2, INPUT_TYPE_3>(
          input_a[j], INPUT_TYPE_2(0), INPUT_TYPE_3(0),
          oclc_function.get_function());
    }
  }
  const auto result =
      test_gentype<GENTYPE>(input_a, input_b, input_c, build_options, config);
  REQUIRE_THAT(result, UlpComparator<GENTYPE>(reference_vector, work_size,
                                              oclc_function.get_function()));
}

template <typename T, typename GENTYPE>
std::vector<T> create_input_vector(
    const std::vector<std::map<uint32_t, GENTYPE>> &gentype_inputs,
    const std::vector<std::map<uint32_t, cassian::scalar_type_v<GENTYPE>>>
        &scalar_inputs,
    const uint32_t &vector_index, const uint32_t &arg_index,
    const size_t &arg_num, const size_t &work_size) {
  if (arg_num < arg_index + 1) {
    return std::vector<T>();
  }
  std::vector<T> input(work_size);
  if (gentype_inputs[vector_index].count(arg_index) != 0) {
    if constexpr (std::is_same_v<T, GENTYPE>) {
      std::fill(input.begin(), input.end(),
                gentype_inputs[vector_index].at(arg_index));
    } else {
      throw;
    }
  } else {
    if constexpr (std::is_same_v<T, cassian::scalar_type_v<GENTYPE>>) {
      std::fill(input.begin(), input.end(),
                scalar_inputs[vector_index].at(arg_index));
    } else {
      throw;
    }
  }
  return input;
}

using Gentype = ca::TupleConcat<ca::TypesFloat, ca::TypesDouble>::type;

template <class T> void run_gentype_test(const T &oclc_function) {
  using gentype = typename T::gentype;
  using input_type_1 = typename T::input_type_1;
  using input_type_2 = typename T::input_type_2;
  using input_type_3 = typename T::input_type_3;
  const TestConfig &config = get_test_config();
  const auto work_size = config.work_size();
  const auto arg_num = oclc_function.get_arg_num();
  const auto function_string =
      Catch::StringMaker<Function>::convert(oclc_function.get_function());
  const std::vector<std::map<uint32_t, cassian::scalar_type_v<gentype>>>
      list_of_scalar_inputs = oclc_function.get_gentype_scalar_values();
  const std::vector<std::map<uint32_t, gentype>> list_of_gentypef_inputs =
      oclc_function.get_gentype_values();
  for (auto i = 0; i < list_of_gentypef_inputs.size(); i++) {
    const auto section_name = create_section_name(function_string, i);
    SECTION(section_name) {
      auto input_a = create_input_vector<input_type_1, gentype>(
          list_of_gentypef_inputs, list_of_scalar_inputs, i, 0, arg_num,
          work_size);
      auto input_b = create_input_vector<input_type_2, gentype>(
          list_of_gentypef_inputs, list_of_scalar_inputs, i, 1, arg_num,
          work_size);
      auto input_c = create_input_vector<input_type_3, gentype>(
          list_of_gentypef_inputs, list_of_scalar_inputs, i, 2, arg_num,
          work_size);
      if (i == 0) {
        input_a = randomize_input(input_a);
        input_b = randomize_input(input_b);
        input_c = randomize_input(input_c);
      }
      run_section<T, gentype>(oclc_function, input_a, input_b, input_c,
                              function_string, config);
    }
  }
}
TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("common_functions_gentypex", "", Gentype,
                                    test_name<TestType>) {
  const TestConfig &config = get_test_config();
  using scalar_type = typename TestType::scalar_type;

  ca::Requirements requirements;
  requirements.arithmetic_type<typename scalar_type::host_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }
  run_gentype_test(
      GentypeXFunction<TestType, TestType, scalar_type, scalar_type>(
          Function::clamp));
  run_gentype_test(
      GentypeXFunction<TestType, TestType, scalar_type>(Function::max));
  run_gentype_test(
      GentypeXFunction<TestType, TestType, scalar_type>(Function::min));
  run_gentype_test(GentypeXFunction<TestType, TestType, TestType, scalar_type>(
      Function::mix));
  run_gentype_test(
      GentypeXFunction<TestType, scalar_type, TestType>(Function::step));
  run_gentype_test(
      GentypeXFunction<TestType, scalar_type, scalar_type, TestType>(
          Function::smoothstep));
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("common_functions_gentype", "", Gentype,
                                    test_name<TestType>) {
  const TestConfig &config = get_test_config();
  using scalar_type = typename TestType::scalar_type;

  ca::Requirements requirements;
  requirements.arithmetic_type<typename scalar_type::host_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }
  using GentypeFunc = GentypeFunction<TestType>;
  const std::initializer_list<GentypeFunc> list_of_gentype = {
      GentypeFunc(Function::clamp), GentypeFunc(Function::degrees),
      GentypeFunc(Function::max),   GentypeFunc(Function::min),
      GentypeFunc(Function::mix),   GentypeFunc(Function::radians),
      GentypeFunc(Function::step),  GentypeFunc(Function::smoothstep),
      GentypeFunc(Function::sign)};
  for (const auto &gentype_func : list_of_gentype) {
    run_gentype_test(gentype_func);
  }
}

} // namespace
