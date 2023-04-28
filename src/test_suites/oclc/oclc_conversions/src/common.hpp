/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_OCLC_CONVERSIONS_COMMON_HPP
#define CASSIAN_OCLC_CONVERSIONS_COMMON_HPP

#include <algorithm>
#include <cassert>
#include <cassian/logging/logging.hpp>
#include <cassian/random/random.hpp>
#include <cassian/runtime/openclc_types.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cassian/test_harness/test_harness.hpp>
#include <cassian/utility/metaprogramming.hpp>
#include <cassian/utility/utility.hpp>
#include <cassian/vector/vector.hpp>
#include <catch2/catch.hpp>
#include <cfenv>
#include <cmath>
#include <common.hpp>
#include <limits>
#include <stdexcept>
#include <test_config.hpp>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

namespace ca = cassian;

enum class RoundingMode {
  round_default,
  round_to_nearest_even,
  round_toward_zero,
  round_toward_positive_infinity,
  round_toward_negative_infinity
};

std::string to_string(RoundingMode rounding_mode);
std::string function_suffix(RoundingMode rounding_mode);
void set_rounding_mode(RoundingMode rounding_mode);

enum class OverflowHandling { overflow_default, overflow_saturation };

std::string to_string(OverflowHandling overflow_handling);
std::string function_suffix(OverflowHandling overflow_handling);

class UnknownRoundingModeException : public std::runtime_error {
  using std::runtime_error::runtime_error;
};

class UnknownOverflowHandlingException : public std::runtime_error {
  using std::runtime_error::runtime_error;
};

template <typename FROM, typename TO,
          RoundingMode RND = RoundingMode::round_default,
          OverflowHandling SAT = OverflowHandling::overflow_default>
struct TestCase {
  using from_type = FROM;
  using to_type = TO;
  constexpr static RoundingMode rounding_mode = RND;
  constexpr static OverflowHandling overflow_handling = SAT;
};

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
          RoundingMode RND = RoundingMode::round_default,
          OverflowHandling SAT = OverflowHandling::overflow_default,
          typename T = typename FROM::scalar_type::host_type,
          typename U = typename TO::scalar_type::host_type,
          EnableIfBothBuiltinTypes<T, U> = 0>
std::string convert_function() {
  return std::string("convert_") + TO::device_type + function_suffix(SAT) +
         function_suffix(RND);
}

template <typename FROM, typename TO, RoundingMode RND, OverflowHandling SAT>
std::string build_options(const int work_size) {
  return "-DDATA_TYPE_OUT=" + std::string(TO::device_type) +
         " -DDATA_TYPE_IN=" + std::string(FROM::device_type) +
         " -DCONVERT_FUNC=" + convert_function<FROM, TO, RND, SAT>() +
         " -DWORK_GROUP_SIZE=" + std::to_string(work_size);
}

template <typename FROM, typename TO,
          RoundingMode RND = RoundingMode::round_default,
          OverflowHandling SAT = OverflowHandling::overflow_default,
          typename T = typename FROM::scalar_type::host_type,
          typename U = typename TO::scalar_type::host_type>
void test_in_out(std::vector<T> &values, std::vector<U> &output,
                 const std::string &program, ca::Runtime *runtime,
                 const std::string &program_type) {
  const std::string source = ca::load_text_file(ca::get_asset(program));
  const int work_size = values.size();
  const std::string options = build_options<FROM, TO, RND, SAT>(work_size);

  ca::Kernel kernel =
      runtime->create_kernel("test_kernel", source, options, program_type);
  run_kernel<FROM, TO>(kernel, values, output, runtime);
  runtime->release_kernel(kernel);
}

template <typename FROM, typename TO,
          RoundingMode RND = RoundingMode::round_default,
          OverflowHandling SAT = OverflowHandling::overflow_default,
          typename T = typename FROM::scalar_type::host_type,
          typename U = typename TO::scalar_type::host_type>
std::vector<T> test_values(int vector_size) {
  return ca::generate_vector<T>(vector_size, 0);
}

template <typename FROM, typename TO,
          RoundingMode RND = RoundingMode::round_default,
          OverflowHandling SAT = OverflowHandling::overflow_default,
          typename T = typename FROM::scalar_type::host_type,
          typename U = typename TO::scalar_type::host_type>
std::vector<T> test_random_values() {
  return std::vector<T>{{T(static_cast<typename FROM::underlying_type>(1))}};
}

template <>
std::vector<float> test_random_values<ca::clc_float_t, ca::clc_half_t>();

template <typename FROM, typename TO,
          RoundingMode RND = RoundingMode::round_default,
          OverflowHandling SAT = OverflowHandling::overflow_default,
          typename T = typename FROM::scalar_type::host_type,
          typename U = typename TO::scalar_type::host_type>
std::vector<U> test_references() {
  return std::vector<U>{{U(static_cast<typename TO::underlying_type>(1))}};
}

template <typename FROM, typename TO,
          RoundingMode RND = RoundingMode::round_default,
          OverflowHandling SAT = OverflowHandling::overflow_default,
          typename T = typename FROM::scalar_type::host_type,
          typename U = typename TO::scalar_type::host_type,
          std::enable_if_t<SAT == OverflowHandling::overflow_default, int> = 0>
std::vector<U> test_references(std::vector<T> values, size_t output_size) {
  set_rounding_mode(RND);
  std::vector<U> ret(values.size());
  auto cast_to_output_type = [](const T val) {
    return U(static_cast<typename TO::underlying_type>(val));
  };
  std::transform(std::begin(values), std::end(values), begin(ret),
                 cast_to_output_type);

  return ret;
}

template <typename FROM, typename TO,
          RoundingMode RND = RoundingMode::round_default,
          OverflowHandling SAT = OverflowHandling::overflow_saturation,
          typename T = typename FROM::scalar_type::host_type,
          typename U = typename TO::scalar_type::host_type,
          std::enable_if_t<SAT == OverflowHandling::overflow_saturation &&
                               ca::is_floating_point_v<T>,
                           int> = 0>
std::vector<U> test_references(std::vector<T> values, size_t output_size) {
  set_rounding_mode(RND);
  auto tmp = values;
  std::vector<U> ret(tmp.size());
  auto convert_to_output_type = [](const T val) {
    if (std::isnan(val)) {
      return U(0);
    }
    if (val > T(std::numeric_limits<U>::max())) {
      return std::numeric_limits<U>::max();
    }
    if (val < T(std::numeric_limits<U>::lowest())) {
      return std::numeric_limits<U>::lowest();
    }
    return U(static_cast<typename TO::underlying_type>(val));
  };
  std::transform(std::begin(values), std::end(values), begin(ret),
                 convert_to_output_type);
  return ret;
}

template <typename FROM, typename TO,
          RoundingMode RND = RoundingMode::round_default,
          OverflowHandling SAT = OverflowHandling::overflow_saturation,
          typename T = typename FROM::scalar_type::host_type,
          typename U = typename TO::scalar_type::host_type,
          std::enable_if_t<SAT == OverflowHandling::overflow_saturation &&
                               std::is_integral_v<T>,
                           int> = 0>
std::vector<U> test_references(std::vector<T> values, size_t output_size) {
  set_rounding_mode(RND);
  auto tmp = values;
  std::vector<U> ret(tmp.size());
  auto convert_to_output_type = [](const T val) {
    if (val > std::numeric_limits<U>::max()) {
      return std::numeric_limits<U>::max();
    }
    if ((val < std::numeric_limits<U>::min()) &&
            (std::numeric_limits<T>::is_signed ==
             std::numeric_limits<U>::is_signed) ||
        (val < 0 && std::numeric_limits<T>::is_signed &&
         !std::numeric_limits<U>::is_signed)) {
      return std::numeric_limits<U>::min();
    }
    return U(static_cast<typename TO::underlying_type>(val));
  };
  std::transform(std::begin(values), std::end(values), begin(ret),
                 convert_to_output_type);
  return ret;
}

template <typename FROM, typename TO,
          RoundingMode RND = RoundingMode::round_default,
          OverflowHandling SAT = OverflowHandling::overflow_default,
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

template <>
void compare(const std::vector<double> &output,
             const std::vector<double> &reference);

template <typename TestType> void scalar_to_scalar(const std::string &program) {
  using from = typename TestType::from_type;
  using to = typename TestType::to_type;
  using type_from = typename from::host_type;
  using type_to = typename to::host_type;

  const RoundingMode rounding_mode = TestType::rounding_mode;
  const OverflowHandling overflow_handling = TestType::overflow_handling;

  const TestConfig &config = get_test_config();

  ca::Requirements requirements;
  requirements.arithmetic_type<from>();
  requirements.arithmetic_type<to>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  auto values = test_values<from, to>(1);
  std::vector<type_to> output(
      1, type_to(static_cast<typename to::underlying_type>(0)));
  const auto references =
      test_references<from, to, rounding_mode, overflow_handling>(values, 1);
  assert(output.size() == references.size());

  test_in_out<from, to, rounding_mode, overflow_handling>(
      values, output, program, config.runtime(), config.program_type());

  compare(output, references);
}

template <typename TestType> void scalar_to_vector(const std::string &program) {
  using from = typename TestType::from_type;
  using to = typename TestType::to_type;
  using type_from = typename from::host_type;
  using type_to = typename to::host_type;
  using value_type_to = typename type_to::value_type;
  using underlying_type_to = typename to::underlying_type;

  const RoundingMode rounding_mode = TestType::rounding_mode;
  const OverflowHandling overflow_handling = TestType::overflow_handling;

  const TestConfig &config = get_test_config();

  ca::Requirements requirements;
  requirements.arithmetic_type<from>();
  requirements.arithmetic_type<typename to::scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  auto values = test_values<from, typename to::scalar_type>(1);

  std::vector<value_type_to> output(
      type_to::vector_size,
      value_type_to(static_cast<typename to::underlying_type>(0)));
  auto references =
      test_references<from, typename to::scalar_type, rounding_mode,
                      overflow_handling>(values, type_to::vector_size);
  references.resize(output.size(), references[0]);
  assert(output.size() == references.size());

  test_in_out<from, to>(values, output, program, config.runtime(),
                        config.program_type());

  compare(output, references);
}

template <typename TestType> void vector_to_vector(const std::string &program) {
  using from = typename TestType::from_type;
  using to = typename TestType::to_type;
  using type_from = typename from::host_type;
  using value_type_from = typename type_from::value_type;
  using type_to = typename to::host_type;
  using value_type_to = typename type_to::value_type;
  using underlying_type_from = typename from::underlying_type;
  using underlying_type_to = typename to::underlying_type;

  const RoundingMode rounding_mode = TestType::rounding_mode;
  const OverflowHandling overflow_handling = TestType::overflow_handling;
  const TestConfig &config = get_test_config();

  ca::Requirements requirements;
  requirements.arithmetic_type<typename from::scalar_type>();
  requirements.arithmetic_type<typename to::scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  auto values =
      test_values<typename from::scalar_type, typename to::scalar_type>(
          type_from::vector_size);

  std::vector<value_type_to> output(
      type_to::vector_size,
      value_type_to(static_cast<typename to::underlying_type>(0)));
  auto references =
      test_references<typename from::scalar_type, typename to::scalar_type,
                      rounding_mode, overflow_handling>(values,
                                                        type_to::vector_size);

  test_in_out<from, to, rounding_mode, overflow_handling>(
      values, output, program, config.runtime(), config.program_type());

  compare(output, references);
}

#endif
