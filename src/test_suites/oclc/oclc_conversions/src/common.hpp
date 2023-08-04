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
#include <cassian/fp_types/math.hpp>
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

enum class TestVariant { conversion, reinterpretation };

enum class OverflowHandling { overflow_default, overflow_saturation };

std::string to_string(OverflowHandling overflow_handling);
std::string function_suffix(OverflowHandling overflow_handling);

class UnknownRoundingModeException : public std::runtime_error {
  using std::runtime_error::runtime_error;
};

class UnknownOverflowHandlingException : public std::runtime_error {
  using std::runtime_error::runtime_error;
};

template <typename TO> void set_rounding_mode(RoundingMode rounding_mode) {
  switch (rounding_mode) {
  case RoundingMode::round_default: {
    if (std::is_integral<TO>::value) {
      fesetround(FE_TOWARDZERO);
      return;
    } else {
      fesetround(FE_TONEAREST);
      return;
    }
  }
  case RoundingMode::round_to_nearest_even: {
    fesetround(FE_TONEAREST);
    return;
  }
  case RoundingMode::round_toward_negative_infinity: {
    fesetround(FE_DOWNWARD);
    return;
  }
  case RoundingMode::round_toward_positive_infinity: {
    fesetround(FE_UPWARD);
    return;
  }
  case RoundingMode::round_toward_zero: {
    fesetround(FE_TOWARDZERO);
    return;
  }
  default:
    throw UnknownRoundingModeException("Unknown rounding mode");
  }
}

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

template <typename FROM, typename TO,
          RoundingMode RND = RoundingMode::round_default,
          OverflowHandling SAT = OverflowHandling::overflow_default,
          typename T = typename FROM::scalar_type::host_type,
          typename U = typename TO::scalar_type::host_type>
std::string convert_function() {
  return std::string("convert_") + TO::device_type + function_suffix(SAT) +
         function_suffix(RND);
}

template <typename FROM, typename TO,
          typename T = typename FROM::scalar_type::host_type,
          typename U = typename TO::scalar_type::host_type>
std::string reinterpret_function() {
  return std::string("as_") + TO::device_type;
}

template <typename FROM, typename TO, RoundingMode RND, OverflowHandling SAT,
          TestVariant TV>
std::string build_options(const int work_size) {
  std::string options = "-DDATA_TYPE_OUT=" + std::string(TO::device_type) +
                        " -DDATA_TYPE_IN=" + std::string(FROM::device_type) +
                        " -DWORK_GROUP_SIZE=" + std::to_string(work_size);
  if constexpr (TV == TestVariant::conversion) {
    options += " -DCONVERT_FUNC=" + convert_function<FROM, TO, RND, SAT>();
  } else if constexpr (TV == TestVariant::reinterpretation) {
    options += " -DREINTERPRET_FUNC=" + reinterpret_function<FROM, TO>();
  }
  return options;
}

template <typename FROM, typename TO,
          RoundingMode RND = RoundingMode::round_default,
          OverflowHandling SAT = OverflowHandling::overflow_default,
          TestVariant TV = TestVariant::conversion,
          typename T = typename FROM::scalar_type::host_type,
          typename U = typename TO::scalar_type::host_type>
void test_in_out(std::vector<T> &values, std::vector<U> &output,
                 const std::string &program, ca::Runtime *runtime,
                 const std::string &program_type) {
  const std::string source = ca::load_text_file(ca::get_asset(program));
  const int work_size = values.size();
  const std::string options = build_options<FROM, TO, RND, SAT, TV>(work_size);

  ca::Kernel kernel =
      runtime->create_kernel("test_kernel", source, options, program_type);
  run_kernel<FROM, TO>(kernel, values, output, runtime);
  runtime->release_kernel(kernel);
}

template <typename FROM, typename TO,
          RoundingMode RND = RoundingMode::round_default,
          OverflowHandling SAT = OverflowHandling::overflow_default,
          typename T = typename FROM::scalar_type::host_type,
          typename U = typename TO::scalar_type::host_type,
          std::enable_if_t<SAT == OverflowHandling::overflow_saturation ||
                               (SAT == OverflowHandling::overflow_default &&
                                std::is_integral_v<T>),
                           int> = 0>
std::vector<T> test_values(int vector_size) {
  if constexpr (std::is_same_v<U, ca::Half>) {
    return ca::generate_vector<T>(vector_size,
                                  std::max(std::numeric_limits<T>::min(),
                                           T(std::numeric_limits<U>::min())),
                                  std::min(std::numeric_limits<T>::max(),
                                           T(std::numeric_limits<U>::max())),
                                  0);
  }
  return ca::generate_vector<T>(vector_size, 0);
}

template <typename FROM, typename TO,
          RoundingMode RND = RoundingMode::round_default,
          OverflowHandling SAT = OverflowHandling::overflow_default,
          typename T = typename FROM::scalar_type::host_type,
          typename U = typename TO::scalar_type::host_type,
          std::enable_if_t<SAT == OverflowHandling::overflow_default &&
                               ca::is_floating_point_v<T>,
                           int> = 0>
std::vector<T> test_values(int vector_size) {
  return ca::generate_vector<T>(
      vector_size,
      std::max(std::numeric_limits<T>::min(), T(std::numeric_limits<U>::min())),
      std::min(std::numeric_limits<T>::max(), T(std::numeric_limits<U>::max())),
      0);
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

template <
    typename FROM, typename TO, RoundingMode RND = RoundingMode::round_default,
    OverflowHandling SAT = OverflowHandling::overflow_default, TestVariant TV,
    typename T = typename FROM::scalar_type::host_type,
    typename U = typename TO::scalar_type::host_type,
    std::enable_if_t<TV == TestVariant::conversion &&
                         SAT == OverflowHandling::overflow_default &&
                         (std::is_integral_v<T> || ca::is_floating_point_v<U>),
                     int> = 0>
std::vector<U> test_references(std::vector<T> values, size_t output_size) {
  set_rounding_mode<U>(RND);
  std::vector<U> ret(values.size());
  auto cast_to_output_type = [](const T val) {
    if (std::is_same<U, ca::Half>::value)
      return U(static_cast<U>(float(val)));
    return U(static_cast<U>(val));
  };
  std::transform(std::begin(values), std::end(values), begin(ret),
                 cast_to_output_type);

  return ret;
}

template <
    typename FROM, typename TO, RoundingMode RND = RoundingMode::round_default,
    OverflowHandling SAT = OverflowHandling::overflow_default, TestVariant TV,
    typename T = typename FROM::scalar_type::host_type,
    typename U = typename TO::scalar_type::host_type,
    std::enable_if_t<TV == TestVariant::conversion &&
                         SAT == OverflowHandling::overflow_default &&
                         ca::is_floating_point_v<T> && std::is_integral_v<U>,
                     int> = 0>
std::vector<U> test_references(std::vector<T> values, size_t output_size) {
  set_rounding_mode<U>(RND);
  std::vector<U> ret(values.size());
  for (size_t i = 0; i < values.size(); i++) {
    ret[i] =
        U(static_cast<typename TO::underlying_type>(ca::nearbyint(values[i])));
  }
  return ret;
}

template <
    typename FROM, typename TO, RoundingMode RND = RoundingMode::round_default,
    OverflowHandling SAT = OverflowHandling::overflow_saturation,
    TestVariant TV, typename T = typename FROM::scalar_type::host_type,
    typename U = typename TO::scalar_type::host_type,
    std::enable_if_t<TV == TestVariant::conversion &&
                         SAT == OverflowHandling::overflow_saturation &&
                         ca::is_floating_point_v<T> && std::is_integral_v<U>,
                     int> = 0>
std::vector<U> test_references(std::vector<T> values, size_t output_size) {
  set_rounding_mode<U>(RND);
  std::vector<U> ret(values.size());
  for (size_t i = 0; i < values.size(); i++) {
    if (ca::isnan(values[i])) {
      ret[i] = U(0);
      continue;
    }
    if (values[i] > T(std::numeric_limits<U>::max())) {
      ret[i] = std::numeric_limits<U>::max();
      continue;
    }
    if (values[i] < T(std::numeric_limits<U>::lowest())) {
      ret[i] = std::numeric_limits<U>::lowest();
      continue;
    }
    ret[i] =
        U(static_cast<typename TO::underlying_type>(ca::nearbyint(values[i])));
  }
  return ret;
}

template <typename FROM, typename TO,
          RoundingMode RND = RoundingMode::round_default,
          OverflowHandling SAT = OverflowHandling::overflow_saturation,
          TestVariant TV, typename T = typename FROM::scalar_type::host_type,
          typename U = typename TO::scalar_type::host_type,
          std::enable_if_t<TV == TestVariant::conversion &&
                               SAT == OverflowHandling::overflow_saturation &&
                               std::is_integral_v<T>,
                           int> = 0>
std::vector<U> test_references(std::vector<T> values, size_t output_size) {
  set_rounding_mode<U>(RND);
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
          OverflowHandling SAT = OverflowHandling::overflow_saturation,
          TestVariant TV, typename T = typename FROM::scalar_type::host_type,
          typename U = typename TO::scalar_type::host_type,
          std::enable_if_t<TV == TestVariant::reinterpretation, int> = 0>
std::vector<U> test_references(std::vector<T> values, size_t output_size) {
  const size_t input_size_in_bytes = sizeof(T) * values.size();
  const size_t output_type_size_in_bytes = sizeof(U);
  int byte_idx = 0;
  unsigned char *buffer = reinterpret_cast<unsigned char *>(values.data());
  std::vector<U> vec;
  while (byte_idx < input_size_in_bytes && byte_idx < sizeof(U) * output_size) {
    U val = U(0);
    unsigned char *c = reinterpret_cast<unsigned char *>(&val);
    for (auto i = 0; i < output_type_size_in_bytes; i++) {
      if (byte_idx >= input_size_in_bytes) {
        break;
      }
      c[i] = buffer[byte_idx];
      byte_idx++;
    }
    vec.push_back(val);
  }
  return vec;
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

template <typename T>
void compare_bytewise(const std::vector<T> &output,
                      const std::vector<T> &reference,
                      size_t number_of_bytes_to_compare) {
  const uint8_t *output_data = reinterpret_cast<const uint8_t *>(output.data());
  const uint8_t *reference_data =
      reinterpret_cast<const uint8_t *>(reference.data());
  CHECKED_IF(std::memcmp(output_data, reference_data,
                         number_of_bytes_to_compare) == 0) {
    return;
  }
  for (auto i = 0; i < number_of_bytes_to_compare; i++) {
    REQUIRE(static_cast<int>(output_data[i]) ==
            static_cast<int>(reference_data[i]));
  }
}

template <typename TestType, TestVariant TV>
void scalar_to_scalar(const std::string &program) {
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

  auto values = test_values<from, to, rounding_mode, overflow_handling>(1);
  std::vector<type_to> output(
      1, type_to(static_cast<typename to::underlying_type>(0)));

  std::vector<type_to> references =
      test_references<from, to, rounding_mode, overflow_handling, TV>(values,
                                                                      1);
  references.resize(output.size(), references[0]);
  assert(output.size() == references.size());

  test_in_out<from, to, rounding_mode, overflow_handling, TV>(
      values, output, program, config.runtime(), config.program_type());

  if constexpr (TV == TestVariant::conversion) {
    compare(output, references);
  }
  if constexpr (TV == TestVariant::reinterpretation) {
    compare_bytewise(output, references,
                     std::min(static_cast<unsigned int>(sizeof(type_from)),
                              static_cast<unsigned int>(sizeof(type_to))));
  }
}

template <typename TestType, TestVariant TV>
void scalar_to_vector(const std::string &program) {
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

  auto values = test_values<from, typename to::scalar_type, rounding_mode,
                            overflow_handling>(1);

  std::vector<value_type_to> output(
      type_to::vector_size,
      value_type_to(static_cast<typename to::underlying_type>(0)));

  std::vector<value_type_to> references =
      test_references<from, to, rounding_mode, overflow_handling, TV>(
          values, type_to::vector_size);
  references.resize(output.size(), references[0]);
  assert(output.size() == references.size());

  test_in_out<from, to, rounding_mode, overflow_handling, TV>(
      values, output, program, config.runtime(), config.program_type());

  if constexpr (TV == TestVariant::conversion) {
    compare(output, references);
  }
  if constexpr (TV == TestVariant::reinterpretation) {
    compare_bytewise(
        output, references,
        std::min(static_cast<unsigned int>(sizeof(type_from)),
                 static_cast<unsigned int>(sizeof(underlying_type_to) *
                                           type_to::vector_size)));
  }
}

template <typename TestType, TestVariant TV>
void vector_to_vector(const std::string &program) {
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
      test_values<typename from::scalar_type, typename to::scalar_type,
                  rounding_mode, overflow_handling>(type_from::vector_size);

  std::vector<value_type_to> output(
      type_to::vector_size,
      value_type_to(static_cast<typename to::underlying_type>(0)));

  std::vector<value_type_to> references =
      test_references<from, to, rounding_mode, overflow_handling, TV>(
          values, type_to::vector_size);
  references.resize(output.size(), references[0]);
  assert(output.size() == references.size());
  test_in_out<from, to, rounding_mode, overflow_handling, TV>(
      values, output, program, config.runtime(), config.program_type());

  if constexpr (TV == TestVariant::conversion) {
    compare(output, references);
  }
  if constexpr (TV == TestVariant::reinterpretation) {
    compare_bytewise(
        output, references,
        std::min(static_cast<unsigned int>(sizeof(underlying_type_from) *
                                           type_from::vector_size),
                 static_cast<unsigned int>(sizeof(underlying_type_to) *
                                           type_to::vector_size)));
  }
}

template <typename TestType, TestVariant TV>
void vector_to_scalar(const std::string &program) {
  using from = typename TestType::from_type;
  using to = typename TestType::to_type;
  using type_from = typename from::host_type;
  using value_type_from = typename type_from::value_type;
  using type_to = typename to::host_type;
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
      test_values<typename from::scalar_type, to>(type_from::vector_size);

  std::vector<type_to> output(
      1, type_to(static_cast<typename to::underlying_type>(0)));
  std::vector<type_to> references =
      test_references<from, to, rounding_mode, overflow_handling, TV>(values,
                                                                      1);
  references.resize(output.size(), references[0]);
  assert(output.size() == references.size());

  test_in_out<from, to, rounding_mode, overflow_handling, TV>(
      values, output, program, config.runtime(), config.program_type());

  if constexpr (TV == TestVariant::conversion) {
    compare(output, references);
  }
  if constexpr (TV == TestVariant::reinterpretation) {
    compare_bytewise(
        output, references,
        std::min(static_cast<unsigned int>(sizeof(underlying_type_from) *
                                           type_from::vector_size),
                 static_cast<unsigned int>(sizeof(type_to))));
  }
}

#endif
