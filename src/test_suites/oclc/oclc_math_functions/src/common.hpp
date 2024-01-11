/*
 * Copyright (C) 2021-2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_OCLC_MATH_FUNCTIONS_COMMON_HPP
#define CASSIAN_OCLC_MATH_FUNCTIONS_COMMON_HPP

#include <algorithm>
#include <cassian/fp_types/math.hpp>
#include <cassian/random/random.hpp>
#include <cassian/runtime/openclc_types.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cassian/test_harness/test_harness.hpp>
#include <cassian/utility/metaprogramming.hpp>
#include <cassian/utility/utility.hpp>
#include <cassian/vector/vector.hpp>
#include <catch2/catch.hpp>
#include <cctype>
#include <cmath>
#include <common.hpp>
#include <cstddef>
#include <cstdint>
#include <enum_definitions.hpp>
#include <limits>
#include <ostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <test_config.hpp>
#include <type_traits>
#include <unordered_map>
#include <vector>
namespace ca = cassian;

int suggest_work_size(const std::string &type);

class UnknownFunctionException : public std::runtime_error {
  using std::runtime_error::runtime_error;
};

class UnknownTypeException : public std::runtime_error {
  using std::runtime_error::runtime_error;
};

using FloatingPointScalarTypes =
    ca::TupleConcat<ca::clc_float_t, ca::clc_double_t>;

enum class AddressSpace {
  clc_global,
  clc_local,
  clc_private,
  clc_constant,
  clc_generic
};

template <typename T>
T generate_value(const ca::scalar_type_v<T> &min,
                 const ca::scalar_type_v<T> &max,
                 const std::vector<ca::scalar_type_v<T>> except =
                     std::vector<ca::scalar_type_v<T>>()) {
  return ca::generate_value<T>(min, max, 0, except);
}

template <typename T> T generate_value() {
  const auto max = std::numeric_limits<ca::scalar_type_v<T>>::max();
  return ca::generate_value<T>(-max, max, 0);
}

template <typename T> T ulp_distance(const T &a, const T &b) {
  if (a == b) {
    return static_cast<T>(0);
  }
  if constexpr (!std::is_integral_v<T>) {
    if (ca::isnan(a) || ca::isnan(b)) {
      return std::numeric_limits<T>::quiet_NaN();
    }
    if (ca::isinf(a) || ca::isinf(b)) {
      return std::numeric_limits<T>::infinity();
    }
  }
  return ca::abs(a - b);
}

template <typename T>
std::vector<T> run_kernel_enum(const size_t &work_size, ca::Runtime *runtime,
                               const ca::Kernel &kernel) {
  ca::Buffer buffer_inout = runtime->create_buffer(work_size * sizeof(T));
  runtime->set_kernel_argument(kernel, static_cast<int>(0), buffer_inout);
  runtime->run_kernel(kernel, work_size);
  const auto output = runtime->read_buffer_to_vector<T>(buffer_inout);
  runtime->release_buffer(buffer_inout);
  return output;
}

template <typename T>
std::vector<T>
run_compute_test_enum(const size_t &work_size, const std::string &build_options,
                      const std::string &program_type, ca::Runtime *runtime,
                      const std::string &kernel_name) {
  const std::string source = ca::load_text_file(
      ca::get_asset("kernels/oclc_math_functions/" + kernel_name + ".cl"));
  ca::Kernel kernel =
      runtime->create_kernel(kernel_name, source, build_options, program_type);
  const auto output = run_kernel_enum<T>(work_size, runtime, kernel);
  runtime->release_kernel(kernel);
  return output;
}

CASSIAN_CATCH_REGISTER_MATH_MACROS;    // NOLINT
CASSIAN_CATCH_REGISTER_MATH_CONSTANTS; // NOLINT

template <typename TEST_TYPE, typename FLOAT_ENUM_TYPE>
void run_template_enum_test_case(
    const TestConfig &config,
    const std::unordered_map<FLOAT_ENUM_TYPE, typename TEST_TYPE::host_type>
        &reference_map,
    const std::string &kernel_name) {
  using scalar_type = typename TEST_TYPE::scalar_type;
  using host_type = typename TEST_TYPE::host_type;
  ca::Requirements requirements;
  requirements.arithmetic_type<scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }
  const auto work_size = config.work_size();
  for (const auto &[macro_enum, macro_ref_value] : reference_map) {
    std::string macro_string =
        Catch::StringMaker<FLOAT_ENUM_TYPE>::convert(macro_enum);
    std::transform(macro_string.begin(), macro_string.end(),
                   macro_string.begin(), ::toupper);
    DYNAMIC_SECTION(macro_string) {
      const std::string build_options =
          "-DTYPE=" + std::string(TEST_TYPE::device_type) + " -D" +
          macro_string + "_MACRO";
      const auto result = run_compute_test_enum<host_type>(
          work_size, build_options, config.program_type(), config.runtime(),
          kernel_name);
      std::vector<host_type> reference(work_size, macro_ref_value);
      REQUIRE_THAT(reference, Catch::Equals(result));
    }
  }
}

template <typename TestType> auto test_name() {
  return std::string(TestType::type_name);
}
enum class SectionType : uint32_t { random, edge };

template <SectionType section_type>
std::string create_section_name(const std::string_view &function_string,
                                const size_t &current_number,
                                const size_t &max_number) {
  std::stringstream ss;
  ss << "Function: " << function_string << ' ';
  if constexpr (section_type == SectionType::random) {
    ss << "random";
  } else if constexpr (SectionType::edge == section_type) {
    ss << "edge";
  }
  if (current_number + 1 < max_number) {
    ss << " " << std::to_string(current_number + 1);
  }

  return ss.str();
}

template <typename T, ca::EnableIfIsVector<T> = 0>
auto randomize_input(const std::vector<T> &input) {
  auto randomized_input = input;
  for (auto j = 1; j < input.size(); j++) {
    for (auto k = 0; k < T::vector_size; k++) {
      randomized_input[j][k] = ca::nextafter(
          input[j - 1][k], std::numeric_limits<ca::scalar_type_v<T>>::max());
    }
  }
  return randomized_input;
}

template <typename T, ca::EnableIfIsScalar<T> = 0>
auto randomize_input(const std::vector<T> &input) {
  auto randomized_input = input;
  for (auto j = 1; j < input.size(); j++) {
    randomized_input[j] = ca::nextafter(
        input[j - 1], std::numeric_limits<ca::scalar_type_v<T>>::max());
  }
  return randomized_input;
}

template <typename T> std::string input_to_string(const std::vector<T> &input) {
  std::stringstream ss;
  ss << "{";
  for (const auto &input_val : input) {
    ss << ca::to_string<T>(input_val) << ", ";
  }
  ss.seekp(-2, std::ios_base::end);
  ss << "}";
  return ss.str();
}

template <typename T> bool powr_special_case(T input_a, T input_b) {
  return (input_a == 0 && input_b == 0) ||
         (ca::isinf(input_a) && input_b == 0) ||
         (input_a == 1 && ca::isinf(input_b)) ||
         (input_a == -1 && ca::isinf(input_b));
}

template <typename T> T calculate_acos(const T &input) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = std::acos(input[i]);
    }
    return result;
  } else {
    return std::acos(input);
  }
}

template <typename T> T calculate_acosh(const T &input) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = std::acosh(input[i]);
    }
    return result;
  } else {
    return std::acosh(input);
  }
}

template <typename T> T calculate_asin(const T &input) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = std::asin(input[i]);
    }
    return result;
  } else {
    return std::asin(input);
  }
}

template <typename T> T calculate_asinh(const T &input) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = std::asinh(input[i]);
    }
    return result;
  } else {
    return std::asinh(input);
  }
}

template <typename T> T calculate_acospi(const T &input) {
  constexpr ca::scalar_type_v<T> pi_value = M_PI;
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = std::acos(input[i]) / pi_value;
    }
    return result;
  } else {
    return std::acos(input) / pi_value;
  }
}

template <typename T> T calculate_asinpi(const T &input) {
  constexpr ca::scalar_type_v<T> pi_value = M_PI;
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = std::asin(input[i]) / pi_value;
    }
    return result;
  } else {
    return std::asin(input) / pi_value;
  }
}

template <typename T> T calculate_atan(const T &input) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = std::atan(input[i]);
    }
    return result;
  } else {
    return std::atan(input);
  }
}

template <typename T> T calculate_atan2(const T &input_a, const T &input_b) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = std::atan2(input_a[i], input_b[i]);
    }
    return result;
  } else {
    return std::atan2(input_a, input_b);
  }
}

template <typename T> T calculate_atanh(const T &input_a) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = std::atanh(input_a[i]);
    }
    return result;
  } else {
    return std::atanh(input_a);
  }
}

template <typename T> T calculate_atanpi(const T &input_a) {
  constexpr ca::scalar_type_v<T> pi_value = M_PI;
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = std::atan(input_a[i]) / pi_value;
    }
    return result;
  } else {
    return std::atan(input_a) / pi_value;
  }
}

template <typename T> T calculate_atan2pi(const T &input_a, const T &input_b) {
  constexpr ca::scalar_type_v<T> pi_value = M_PI;
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = std::atan2(input_a[i], input_b[i]) / pi_value;
    }
    return result;
  } else {
    return std::atan2(input_a, input_b) / pi_value;
  }
}

template <typename T> T calculate_cbrt(const T &input_a) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = std::cbrt(input_a[i]);
    }
    return result;
  } else {
    return std::cbrt(input_a);
  }
}

template <typename T> T calculate_ceil(const T &input_a) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = ca::ceil(input_a[i]);
    }
    return result;
  } else {
    return ca::ceil(input_a);
  }
}

template <typename T> T calculate_copysign(const T &input_a, const T &input_b) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = std::copysign(input_a[i], input_b[i]);
    }
    return result;
  } else {
    return std::copysign(input_a, input_b);
  }
}

template <typename T> T calculate_sqrt(const T &input_a) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = ca::sqrt(input_a[i]);
    }
    return result;
  } else {
    return ca::sqrt(input_a);
  }
}

template <typename T> T calculate_cos(const T &input_a) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = std::cos(input_a[i]);
    }
    return result;
  } else {
    return std::cos(input_a);
  }
}

template <typename T> T calculate_cosh(const T &input_a) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = std::cosh(input_a[i]);
    }
    return result;
  } else {
    return std::cosh(input_a);
  }
}

template <typename T> T calculate_cospi(const T &input) {
  constexpr ca::scalar_type_v<T> pi_value = M_PI;
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      if (ca::floor(input[i]) - input[i] == 0 &&
          input[i] != std::numeric_limits<typename T::value_type>::infinity()) {
        result[i] = 1;
      } else {
        result[i] = std::cos(pi_value * input[i]);
      }
    }
    return result;
  } else {
    if (ca::floor(input) - input == 0 &&
        input != std::numeric_limits<T>::infinity()) {
      return 1;
    } else {
      return std::cos(pi_value * input);
    }
  }
}

template <typename OUT, typename IN> OUT calculate_ilogb(const IN &input_a) {
  if constexpr (ca::is_vector_v<IN>) {
    OUT result(0.0F);
    for (auto i = 0; i < IN::vector_size; i++) {
      result[i] = std::ilogb(input_a[i]);
    }
    return result;
  } else {
    return std::ilogb(input_a);
  }
}

template <typename T> T calculate_erfc(const T &input_a) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = std::erfc(input_a[i]);
    }
    return result;
  } else {
    return std::erfc(input_a);
  }
}

template <typename T> T calculate_erf(const T &input_a) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = std::erf(input_a[i]);
    }
    return result;
  } else {
    return std::erf(input_a);
  }
}

template <typename T> T calculate_exp(const T &input_a) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = std::exp(input_a[i]);
    }
    return result;
  } else {
    return std::exp(input_a);
  }
}

template <typename T> T calculate_exp2(const T &input_a) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = std::exp2(input_a[i]);
    }
    return result;
  } else {
    return std::exp2(input_a);
  }
}

template <typename T> T calculate_exp10(const T &input_a) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = std::pow(10, input_a[i]);
    }
    return result;
  } else {
    return std::pow(10, input_a);
  }
}

template <typename T> T calculate_expm1(const T &input_a) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = std::expm1(input_a[i]);
    }
    return result;
  } else {
    return std::expm1(input_a);
  }
}

template <typename T> T calculate_fabs(const T &input_a) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = std::fabs(input_a[i]);
    }
    return result;
  } else {
    return std::fabs(input_a);
  }
}

template <typename T> T calculate_fdim(const T &input_a, const T &input_b) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = std::fdim(input_a[i], input_b[i]);
    }
    return result;
  } else {
    return std::fdim(input_a, input_b);
  }
}

template <typename T> T calculate_floor(const T &input_a) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = ca::floor(input_a[i]);
    }
    return result;
  } else {
    return ca::floor(input_a);
  }
}

template <typename T>
T calculate_mad(const T &input_a, const T &input_b, const T &input_c) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = input_a[i] * input_b[i] + input_c[i];
    }
    return result;
  } else {
    return input_a * input_b + input_c;
  }
}

template <typename T> T calculate_maxmag(const T &input_a, const T &input_b) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      if (ca::abs(input_a[i]) > ca::abs(input_b[i]))
        result[i] = input_a[i];
      else if (ca::abs(input_a[i]) < ca::abs(input_b[i]))
        result[i] = input_b[i];
      else
        result[i] = std::fmax(input_a[i], input_b[i]);
    }
    return result;
  } else {
    if (ca::abs(input_a) > ca::abs(input_b))
      return input_a;
    else if (ca::abs(input_a) < ca::abs(input_b))
      return input_b;
    else
      return std::fmax(input_a, input_b);
  }
}

template <typename T> T calculate_minmag(const T &input_a, const T &input_b) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      if (ca::abs(input_a[i]) < ca::abs(input_b[i]))
        result[i] = input_a[i];
      else if (ca::abs(input_a[i]) > ca::abs(input_b[i]))
        result[i] = input_b[i];
      else
        result[i] = std::fmin(input_a[i], input_b[i]);
    }
    return result;
  } else {
    if (ca::abs(input_a) < ca::abs(input_b))
      return input_a;
    else if (ca::abs(input_a) > ca::abs(input_b))
      return input_b;
    else
      return std::fmin(input_a, input_b);
  }
}

template <typename T>
T calculate_nextafter(const T &input_a, const T &input_b) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = ca::nextafter(input_a[i], input_b[i]);
    }
    return result;
  } else {
    return ca::nextafter(input_a, input_b);
  }
}

template <typename T> T calculate_pow(const T &input_a, const T &input_b) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = std::pow(input_a[i], input_b[i]);
    }
    return result;
  } else {
    return std::pow(input_a, input_b);
  }
}

template <typename T>
T calculate_remainder(const T &input_a, const T &input_b) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = std::remainder(input_a[i], input_b[i]);
    }
    return result;
  } else {
    return std::remainder(input_a, input_b);
  }
}

template <typename T> T calculate_rint(const T &input_a) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = std::rint(input_a[i]);
    }
    return result;
  } else {
    return std::rint(input_a);
  }
}

template <typename T> T calculate_round(const T &input_a) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = ca::round(input_a[i]);
    }
    return result;
  } else {
    return ca::round(input_a);
  }
}

template <typename T> T calculate_rsqrt(const T &input_a) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = 1 / ca::sqrt(input_a[i]);
    }
    return result;
  } else {
    return 1 / ca::sqrt(input_a);
  }
}

template <typename T> T calculate_sin(const T &input) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = std::sin(input[i]);
    }
    return result;
  } else {
    return std::sin(input);
  }
}

template <typename T> T calculate_sinh(const T &input) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = std::sinh(input[i]);
    }
    return result;
  } else {
    return std::sinh(input);
  }
}

template <typename T> T calculate_sinpi(const T &input) {
  constexpr ca::scalar_type_v<T> pi_value = M_PI;
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      if (input[i] == 0) {
        result[i] = 0;
      } else if (ca::floor(input[i]) - input[i] == 0 && !ca::isinf(input[i])) {
        result[i] = 0;
      } else {
        result[i] = std::sin(pi_value * input[i]);
      }
    }
    return result;
  } else {
    if (input == 0) {
      return 0;
    } else if (ca::floor(input) - input == 0 && !ca::isinf(input)) {
      return 0;
    } else {
      return std::sin(pi_value * input);
    }
  }
}

template <typename T> T calculate_tan(const T &input) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = std::tan(input[i]);
    }
    return result;
  } else {
    return std::tan(input);
  }
}

template <typename T> T calculate_tanh(const T &input) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = std::tanh(input[i]);
    }
    return result;
  } else {
    return std::tanh(input);
  }
}

template <typename T> T calculate_tanpi(const T &input) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      if (std::fabs(std::fmod(input[i], 2.0)) == 0.0) {
        result[i] = std::copysign(0.0, input[i]);
      } else if (std::fabs(std::fmod(input[i], 2.0)) == 1.0) {
        result[i] = std::copysign(0.0, -input[i]);
      } else {
        result[i] = std::sin(M_PI * input[i]) / std::cos(M_PI * input[i]);
      }
    }
    return result;
  } else {
    if (std::fabs(std::fmod(input, 2.0)) == 0.0) {
      return std::copysign(0.0, input);
    } else if (std::fabs(std::fmod(input, 2.0)) == 1.0) {
      return std::copysign(0.0, -input);
    } else {
      return std::sin(M_PI * input) / std::cos(M_PI * input);
    }
  }
}

template <typename T> T calculate_tgamma(const T &input) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = std::tgamma(input[i]);
    }
    return result;
  } else {
    return std::tgamma(input);
  }
}

template <typename T> T calculate_trunc(const T &input) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = ca::trunc(input[i]);
    }
    return result;
  } else {
    return ca::trunc(input);
  }
}

template <typename T> T calculate_divide(const T &input_a, const T &input_b) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = input_a[i] / input_b[i];
    }
    return result;
  } else {
    return input_a / input_b;
  }
}

template <typename T> T calculate_recip(const T &input) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = 1.0 / input[i];
    }
    return result;
  } else {
    return 1.0 / input;
  }
}

template <typename T>
T calculate_fma(const T &input_a, const T &input_b, const T &input_c) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = std::fma(input_a[i], input_b[i], input_c[i]);
    }
    return result;
  } else {
    return std::fma(input_a, input_b, input_c);
  }
}

template <typename T, typename T_1 = T>
T calculate_fmax(const T &input_a, const T_1 &input_b) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      if constexpr (ca::is_vector_v<T_1>) {
        result[i] = std::fmax(input_a[i], input_b[i]);
      } else {
        result[i] = std::fmax(input_a[i], input_b);
      }
    }
    return result;
  } else {
    return std::fmax(input_a, input_b);
  }
}

template <typename T, typename T_1 = T>
T calculate_fmin(const T &input_a, const T_1 &input_b) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      if constexpr (ca::is_vector_v<T_1>) {
        result[i] = std::fmin(input_a[i], input_b[i]);
      } else {
        result[i] = std::fmin(input_a[i], input_b);
      }
    }
    return result;
  } else {
    return std::fmin(input_a, input_b);
  }
}

template <typename T> T calculate_fmod(const T &input_a, const T &input_b) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = std::fmod(input_a[i], input_b[i]);
    }
    return result;
  } else {
    return std::fmod(input_a, input_b);
  }
}

template <typename T, typename T_1 = T>
T calculate_ldexp(const T &input_a, const T_1 &input_b) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      if constexpr (ca::is_vector_v<T_1>) {
        result[i] = std::ldexp(input_a[i], input_b[i]);
      } else {
        result[i] = std::ldexp(input_a[i], input_b);
      }
    }
    return result;
  } else {
    return std::ldexp(input_a, input_b);
  }
}

template <typename T> T calculate_hypot(const T &input_a, const T &input_b) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = std::hypot(input_a[i], input_b[i]);
    }
    return result;
  } else {
    return std::hypot(input_a, input_b);
  }
}

template <typename T> T calculate_lgamma(const T &input_a) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = std::lgamma(input_a[i]);
    }
    return result;
  } else {
    return std::lgamma(input_a);
  }
}

template <typename T> T calculate_log(const T &input_a) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = std::log(input_a[i]);
    }
    return result;
  } else {
    return std::log(input_a);
  }
}

template <typename T> T calculate_log2(const T &input_a) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = std::log2(input_a[i]);
    }
    return result;
  } else {
    return std::log2(input_a);
  }
}

template <typename T> T calculate_log10(const T &input_a) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = std::log10(input_a[i]);
    }
    return result;
  } else {
    return std::log10(input_a);
  }
}

template <typename T> T calculate_log1p(const T &input_a) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = std::log1p(input_a[i]);
    }
    return result;
  } else {
    return std::log1p(input_a);
  }
}

template <typename T> T calculate_logb(const T &input_a) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = std::logb(input_a[i]);
    }
    return result;
  } else {
    return std::logb(input_a);
  }
}

template <typename T_1, typename T_2> T_1 calculate_nan(const T_2 &input_a) {
  if constexpr (ca::is_vector_v<T_2>) {
    T_1 result(0.0F);
    for (auto i = 0; i < T_1::vector_size; i++) {
      result[i] = std::numeric_limits<typename T_1::value_type>::quiet_NaN();
    }
    return result;
  } else {
    return std::numeric_limits<T_1>::quiet_NaN();
  }
}

template <typename T_1, typename T_2>
T_1 calculate_pown(const T_1 &input_a, const T_2 &input_b) {
  if constexpr (ca::is_vector_v<T_1>) {
    T_1 result(0.0F);
    for (auto i = 0; i < T_1::vector_size; i++) {
      result[i] = std::pow(input_a[i], input_b[i]);
    }
    return result;
  } else {
    return std::pow(input_a, input_b);
  }
}

template <typename T> T calculate_powr(const T &input_a, const T &input_b) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      if (powr_special_case(input_a[i], input_b[i])) {
        result[i] = std::numeric_limits<typename T::value_type>::quiet_NaN();
      } else {
        result[i] = std::pow(input_a[i], input_b[i]);
      }
    }
    return result;
  } else {
    if (powr_special_case(input_a, input_b)) {
      return std::numeric_limits<T>::quiet_NaN();
    } else {
      return std::pow(input_a, input_b);
    }
  }
}

template <typename T_1, typename T_2>
T_1 calculate_rootn(const T_1 &input_a, const T_2 &input_b) {
  if constexpr (ca::is_vector_v<T_1>) {
    T_1 result(0.0F);
    for (auto i = 0; i < T_1::vector_size; i++) {
      if (input_b[i] == 0) {
        result[i] = std::numeric_limits<typename T_1::value_type>::quiet_NaN();
      } else if (input_a[i] == -0 && (input_b[i] % 2) == 1 && input_b[i] < 0) {
        result[i] = -std::numeric_limits<typename T_1::value_type>::infinity();
      } else {
        result[i] = std::pow(input_a[i], 1.0 / input_b[i]);
      }
    }
    return result;
  } else {
    if (input_b == 0) {
      return std::numeric_limits<T_1>::quiet_NaN();
    } else if (input_a == -0 && (input_b % 2) == 1 && input_b < 0) {
      return -std::numeric_limits<T_1>::infinity();
    } else {
      return std::pow(input_a, 1.0 / input_b);
    }
  }
}

template <typename T> T calculate_fract(const T &input_a, T &input_b) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      if (ca::isnan(input_a[i])) {
        result[i] = std::numeric_limits<typename T::value_type>::quiet_NaN();
      } else if (ca::isinf(input_a[i])) {
        result[i] = 0;
      } else {
        result[i] =
            std::fmin(input_a[i] - ca::floor(input_a[i]), 0x1.fffffep-1f);
      }
      input_b[i] = ca::floor(input_a[i]);
    }
    return result;
  } else {
    if (ca::isnan(input_a)) {
      input_b = ca::floor(input_a);
      return std::numeric_limits<T>::quiet_NaN();
    } else if (ca::isinf(input_a)) {
      input_b = ca::floor(input_a);
      return 0;
    } else {
      input_b = ca::floor(input_a);
      return std::fmin(input_a - ca::floor(input_a), 0x1.fffffep-1f);
    }
  }
}

template <typename T> T calculate_modf(const T &input_a, T &input_b) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = std::modf(input_a[i], &input_b[i]);
    }
    return result;
  } else {
    return std::modf(input_a, &input_b);
  }
}

template <typename T> T calculate_sincos(const T &input_a, T &input_b) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = std::sin(input_a[i]);
      input_b[i] = std::cos(input_a[i]);
    }
    return result;
  } else {
    input_b = std::cos(input_a);
    return std::sin(input_a);
  }
}

template <typename T, typename T_1>
T calculate_frexp(const T &input_a, T_1 &input_b) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = std::frexp(input_a[i], &input_b[i]);
      if (input_b[i] == -1)
        input_b[i] = 0;
    }
    return result;
  } else {
    auto result = std::frexp(input_a, &input_b);
    if (input_b == -1)
      input_b = 0;
    return result;
  }
}

template <typename T, typename T_1>
T calculate_remquo(const T &input_a, const T &input_b, T_1 &input_c) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      if (ca::isnan(input_a[i])) {
        input_c[i] = 0;
        result[i] = std::numeric_limits<typename T::value_type>::quiet_NaN();
        continue;
      }
      if ((ca::isinf(std::fabs(input_a[i]))) ||
          (input_b[i] == 0 && !ca::isnan(input_a[i])) ||
          ((ca::isnan(input_a[i])) ^ (ca::isnan(input_b[i])))) {
        result[i] = std::numeric_limits<typename T::value_type>::quiet_NaN();
        input_c[i] = 0;
        continue;
      }
      result[i] = std::remquo(input_a[i], input_b[i], &input_c[i]);
    }
    return result;
  } else {
    if ((std::fabs(input_a) == std::numeric_limits<T>::infinity()) ||
        (input_b == 0 && !ca::isnan(input_a)) ||
        ((ca::isnan(input_a)) ^ (ca::isnan(input_b)))) {
      input_c = 0;
      return std::numeric_limits<T>::quiet_NaN();
    } else {
      return std::remquo(input_a, input_b, &input_c);
    }
  }
}

template <typename T, typename T_1>
T calculate_lgamma_r(const T &input_a, T_1 &input_b) {
  if constexpr (ca::is_vector_v<T>) {
    T result(0.0F);
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = std::lgamma(input_a[i]);
      input_b[i] = input_a[i] > 0 ? 1 : 0;
    }
    return result;
  } else {
    input_b = input_a > 0 ? 1 : 0;
    return std::lgamma(input_a);
  }
}

#endif
