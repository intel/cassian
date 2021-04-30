/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_OCLC_CONVERSIONS_COMMON_HPP
#define CASSIAN_OCLC_CONVERSIONS_COMMON_HPP

#include <cassert>
#include <cassian/logging/logging.hpp>
#include <cassian/runtime/openclc_types.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cassian/test_harness/test_harness.hpp>
#include <cassian/utility/metaprogramming.hpp>
#include <cassian/utility/utility.hpp>
#include <cassian/vector/vector.hpp>
#include <catch2/catch.hpp>
#include <common.hpp>
#include <test_config.hpp>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

namespace ca = cassian;

template <typename FROM, typename TO,
          typename T = typename FROM::scalar_type::host_type,
          typename U = typename TO::scalar_type::host_type>
void run_kernel(const ca::Kernel &kernel, const std::vector<T> &input,
                std::vector<U> &output, ca::Runtime *runtime) {
  ca::Buffer input_buffer = runtime->create_buffer(
      sizeof(typename FROM::scalar_type::underlying_type) * input.size());
  ca::Buffer output_buffer = runtime->create_buffer(
      sizeof(typename TO::scalar_type::underlying_type) * output.size());

  runtime->write_buffer_from_vector<T>(input_buffer, input);

  runtime->set_kernel_argument(kernel, 0, input_buffer);
  runtime->set_kernel_argument(kernel, 1, output_buffer);

  runtime->run_kernel(kernel, 1);
  output = runtime->read_buffer_to_vector<U>(output_buffer);

  runtime->release_buffer(input_buffer);
  runtime->release_buffer(output_buffer);
}

template <typename FROM, typename TO,
          typename T = typename FROM::scalar_type::host_type,
          typename U = typename TO::scalar_type::host_type>
void run_kernel(const ca::Kernel &kernel, const std::vector<T> &input,
                const std::vector<T> &random, std::vector<U> &output,
                ca::Runtime *runtime) {
  ca::Buffer input_buffer = runtime->create_buffer(
      sizeof(typename FROM::scalar_type::underlying_type) * input.size());
  ca::Buffer random_buffer = runtime->create_buffer(
      sizeof(typename FROM::scalar_type::underlying_type) * random.size());
  ca::Buffer output_buffer = runtime->create_buffer(
      sizeof(typename TO::scalar_type::underlying_type) * output.size());

  runtime->write_buffer_from_vector<T>(input_buffer, input);
  runtime->write_buffer_from_vector<T>(random_buffer, random);

  runtime->set_kernel_argument(kernel, 0, input_buffer);
  runtime->set_kernel_argument(kernel, 1, random_buffer);
  runtime->set_kernel_argument(kernel, 2, output_buffer);

  runtime->run_kernel(kernel, 1);
  output = runtime->read_buffer_to_vector<U>(output_buffer);

  runtime->release_buffer(input_buffer);
  runtime->release_buffer(random_buffer);
  runtime->release_buffer(output_buffer);
}

template <typename T, typename U>
using EnableIfBothBuiltinTypes =
    std::enable_if_t<(std::is_scalar_v<T> || ca::is_vector_v<T>)&&(
                         std::is_scalar_v<U> || ca::is_vector_v<U>),
                     int>;

template <typename FROM, typename TO,
          typename T = typename FROM::scalar_type::host_type,
          typename U = typename TO::scalar_type::host_type,
          EnableIfBothBuiltinTypes<T, U> = 0>
std::string convert_function(const std::string &rounding_mode = "") {
  return std::string("convert_") + TO::device_type +
         (rounding_mode != "" ? "_" + rounding_mode : "");
}

template <typename FROM, typename TO,
          typename T = typename FROM::scalar_type::host_type,
          typename U = typename TO::scalar_type::host_type>
void test_in_out(std::vector<T> &values, std::vector<U> &output,
                 const std::string &program, ca::Runtime *runtime,
                 const std::string &program_type) {
  const std::string source = ca::load_text_file(ca::get_asset(program));
  const std::string build_options =
      "-DDATA_TYPE_OUT=" + std::string(TO::device_type) +
      " -DDATA_TYPE_IN=" + std::string(FROM::device_type) +
      " -DCONVERT_FUNC=" + convert_function<FROM, TO>();
  ca::Kernel kernel = runtime->create_kernel("test_kernel", source,
                                             build_options, program_type);
  run_kernel<FROM, TO>(kernel, values, output, runtime);
  runtime->release_kernel(kernel);
}

template <typename TestType> auto test_name() {
  using from = std::tuple_element_t<0, TestType>;
  using to = std::tuple_element_t<1, TestType>;
  return std::string(from::type_name) + "->" + std::string(to::type_name);
}

template <typename FROM, typename TO,
          typename T = typename FROM::scalar_type::host_type,
          typename U = typename TO::scalar_type::host_type>
std::vector<T> test_values() {
  return std::vector<T>{{T(static_cast<typename FROM::underlying_type>(1))}};
}

template <> std::vector<float> test_values<ca::clc_float_t, ca::clc_half_t>();

template <typename FROM, typename TO,
          typename T = typename FROM::scalar_type::host_type,
          typename U = typename TO::scalar_type::host_type>
std::vector<T> test_random_values() {
  return std::vector<T>{{T(static_cast<typename FROM::underlying_type>(1))}};
}

template <>
std::vector<float> test_random_values<ca::clc_float_t, ca::clc_half_t>();

template <typename FROM, typename TO,
          typename T = typename FROM::scalar_type::host_type,
          typename U = typename TO::scalar_type::host_type>
std::vector<U> test_references() {
  return std::vector<U>{{U(static_cast<typename TO::underlying_type>(1))}};
}

template <typename FROM, typename TO,
          typename T = typename FROM::scalar_type::host_type,
          typename U = typename TO::scalar_type::host_type>
std::vector<U> test_references(const T random) {
  return std::vector<U>{{U(static_cast<typename TO::underlying_type>(1))}};
}

template <>
std::vector<ca::Half>
test_references<ca::clc_float_t, ca::clc_half_t>(const float random);

template <typename T>
void compare(const std::vector<T> &output, const std::vector<T> &reference) {
  REQUIRE_THAT(output, Catch::Equals(reference));
}

template <>
void compare(const std::vector<float> &output,
             const std::vector<float> &reference);

template <>
void compare(const std::vector<ca::Half> &output,
             const std::vector<ca::Half> &reference);

template <typename TestType>
void scalar_to_scalar(const std::string &program,
                      const std::string &rounding_mode = "") {
  using from = std::tuple_element_t<0, TestType>;
  using to = std::tuple_element_t<1, TestType>;
  using type_from = typename from::host_type;
  using type_to = typename to::host_type;

  const TestConfig &config = get_test_config();

  ca::Requirements requirements;
  requirements.arithmetic_type<from>();
  requirements.arithmetic_type<to>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  const auto values = test_values<from, to>();
  auto index = GENERATE_COPY(range(size_t{0}, values.size()));

  std::vector<type_from> value(1, values[index]);
  std::vector<type_to> output(
      1, type_to(static_cast<typename to::underlying_type>(0)));
  const auto references = test_references<from, to>();
  assert(values.size() == references.size());

  test_in_out<from, to>(value, output, program, config.runtime(),
                        config.program_type());

  compare(output, std::vector<type_to>(1, references[index]));
}

template <typename TestType>
void scalar_to_vector(const std::string &program,
                      const std::string &rounding_mode = "") {
  using from = std::tuple_element_t<0, TestType>;
  using to = std::tuple_element_t<1, TestType>;
  using type_from = typename from::host_type;
  using type_to = typename to::host_type;
  using value_type_to = typename type_to::value_type;

  const TestConfig &config = get_test_config();

  ca::Requirements requirements;
  requirements.arithmetic_type<from>();
  requirements.arithmetic_type<typename to::scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  const auto values = test_values<from, typename to::scalar_type>();
  auto index = GENERATE_COPY(range(size_t{0}, values.size()));

  std::vector<type_from> value(1, values[index]);
  std::vector<value_type_to> output(
      type_to::vector_size,
      value_type_to(static_cast<typename to::underlying_type>(0)));
  const auto references = test_references<from, typename to::scalar_type>();
  assert(values.size() == references.size());

  test_in_out<from, to>(value, output, program, config.runtime(),
                        config.program_type());

  compare(output,
          std::vector<value_type_to>(type_to::vector_size, references[index]));
}

template <typename TestType>
void vector_to_vector(const std::string &program,
                      const std::string &rounding_mode = "") {
  using from = std::tuple_element_t<0, TestType>;
  using to = std::tuple_element_t<1, TestType>;
  using type_from = typename from::host_type;
  using value_type_from = typename type_from::value_type;
  using type_to = typename to::host_type;
  using value_type_to = typename type_to::value_type;

  const TestConfig &config = get_test_config();

  ca::Requirements requirements;
  requirements.arithmetic_type<typename from::scalar_type>();
  requirements.arithmetic_type<typename to::scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  const auto values =
      test_values<typename from::scalar_type, typename to::scalar_type>();
  auto index = GENERATE_COPY(range(size_t{0}, values.size()));

  std::vector<value_type_from> value(size_t{type_from::vector_size},
                                     values[index]);
  std::vector<value_type_to> output(
      type_to::vector_size,
      value_type_to(static_cast<typename to::underlying_type>(0)));
  const auto references =
      test_references<typename from::scalar_type, typename to::scalar_type>();
  assert(values.size() == references.size());

  test_in_out<from, to>(value, output, program, config.runtime(),
                        config.program_type());

  compare(output,
          std::vector<value_type_to>(type_to::vector_size, references[index]));
}

#endif
