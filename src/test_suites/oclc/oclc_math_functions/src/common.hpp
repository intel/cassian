/*
 * Copyright (C) 2021-2024 Intel Corporation
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

template <typename T, typename Enable = void>
struct ReplaceFloatingPointWithDouble {
  using type = T;
};

template <typename T>
using replace_fp_with_double_t =
    typename ReplaceFloatingPointWithDouble<T>::type;

template <typename T>
struct ReplaceFloatingPointWithDouble<
    T, std::enable_if_t<ca::is_floating_point_v<T>>> {
  using type = double;
};

template <typename T>
struct ReplaceFloatingPointWithDouble<
    T, std::enable_if_t<ca::is_vector_v<T> &&
                        ca::is_floating_point_v<typename T::value_type>>> {
  using type = typename ca::Vector<double, T::vector_size, T::size_in_memory>;
};

template <typename T> bool powr_special_case(T input_a, T input_b) {
  return (input_a == 0 && input_b == 0) ||
         (ca::isinf(input_a) && input_b == 0) ||
         (input_a == 1 && ca::isinf(input_b)) ||
         (input_a == -1 && ca::isinf(input_b));
}

template <typename T>
replace_fp_with_double_t<T> calculate_acos(const T &input) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = ca::acos(static_cast<double>(input[i]));
    }
    return result;
  } else {
    return ca::acos(static_cast<double>(input));
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_acosh(const T &input) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = ca::acosh(static_cast<double>(input[i]));
    }
    return result;
  } else {
    return ca::acosh(static_cast<double>(input));
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_asin(const T &input) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = ca::asin(static_cast<double>(input[i]));
    }
    return result;
  } else {
    return ca::asin(static_cast<double>(input));
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_asinh(const T &input) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = ca::asinh(static_cast<double>(input[i]));
    }
    return result;
  } else {
    return ca::asinh(static_cast<double>(input));
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_acospi(const T &input) {
  double pi_value = static_cast<double>(M_PI);
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = ca::acos(static_cast<double>(input[i])) / pi_value;
    }
    return result;
  } else {
    return ca::acos(static_cast<double>(input)) / pi_value;
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_asinpi(const T &input) {
  double pi_value = static_cast<double>(M_PI);
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = ca::asin(static_cast<double>(input[i])) / pi_value;
    }
    return result;
  } else {
    return ca::asin(static_cast<double>(input)) / pi_value;
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_atan(const T &input) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = ca::atan(static_cast<double>(input[i]));
    }
    return result;
  } else {
    return ca::atan(static_cast<double>(input));
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_atan2(const T &input_a,
                                            const T &input_b) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = ca::atan2(static_cast<double>(input_a[i]),
                            static_cast<double>(input_b[i]));
    }
    return result;
  } else {
    return ca::atan2(static_cast<double>(input_a),
                     static_cast<double>(input_b));
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_atanh(const T &input_a) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = ca::atanh(static_cast<double>(input_a[i]));
    }
    return result;
  } else {
    return ca::atanh(static_cast<double>(input_a));
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_atanpi(const T &input_a) {
  double pi_value = static_cast<double>(M_PI);
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = ca::atan(static_cast<double>(input_a[i])) / pi_value;
    }
    return result;
  } else {
    return ca::atan(static_cast<double>(input_a)) / pi_value;
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_atan2pi(const T &input_a,
                                              const T &input_b) {
  double pi_value = static_cast<double>(M_PI);
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = ca::atan2(static_cast<double>(input_a[i]),
                            static_cast<double>(input_b[i])) /
                  pi_value;
    }
    return result;
  } else {
    return ca::atan2(static_cast<double>(input_a),
                     static_cast<double>(input_b)) /
           pi_value;
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_cbrt(const T &input_a) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = ca::cbrt(static_cast<double>(input_a[i]));
    }
    return result;
  } else {
    return ca::cbrt(static_cast<double>(input_a));
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_ceil(const T &input_a) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = static_cast<double>(ca::ceil(input_a[i]));
    }
    return result;
  } else {
    return static_cast<double>(ca::ceil(input_a));
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_copysign(const T &input_a,
                                               const T &input_b) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = static_cast<double>(ca::copysign(input_a[i], input_b[i]));
    }
    return result;
  } else {
    return static_cast<double>(ca::copysign(input_a, input_b));
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_sqrt(const T &input_a) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = ca::sqrt(static_cast<double>(input_a[i]));
    }
    return result;
  } else {
    return ca::sqrt(static_cast<double>(input_a));
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_cos(const T &input_a) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = ca::cos(static_cast<double>(input_a[i]));
    }
    return result;
  } else {
    return ca::cos(static_cast<double>(input_a));
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_cosh(const T &input_a) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = ca::cosh(static_cast<double>(input_a[i]));
    }
    return result;
  } else {
    return ca::cosh(static_cast<double>(input_a));
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_cospi(const T &input) {
  double pi_value = static_cast<double>(M_PI);
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      if (ca::floor(input[i]) - input[i] == 0 &&
          input[i] != std::numeric_limits<typename T::value_type>::infinity()) {
        result[i] = 1.0;
      } else {
        result[i] = ca::cos(pi_value * static_cast<double>(input[i]));
      }
    }
    return result;
  } else {
    if (ca::floor(input) - input == 0 &&
        input != std::numeric_limits<T>::infinity()) {
      return 1.0;
    } else {
      return ca::cos(pi_value * static_cast<double>(input));
    }
  }
}

template <typename OUT, typename IN>
replace_fp_with_double_t<OUT> calculate_ilogb(const IN &input_a) {
  if constexpr (ca::is_vector_v<IN>) {
    replace_fp_with_double_t<OUT> result{};
    for (auto i = 0; i < IN::vector_size; i++) {
      result[i] = ca::ilogb<typename OUT::value_type>(input_a[i]);
    }
    return result;
  } else {
    return OUT(ca::ilogb<OUT>(input_a));
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_erfc(const T &input_a) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = ca::erfc(static_cast<double>(input_a[i]));
    }
    return result;
  } else {
    return ca::erfc(static_cast<double>(input_a));
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_erf(const T &input_a) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = ca::erf(static_cast<double>(input_a[i]));
    }
    return result;
  } else {
    return ca::erf(static_cast<double>(input_a));
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_exp(const T &input_a) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = ca::exp(static_cast<double>(input_a[i]));
    }
    return result;
  } else {
    return ca::exp(static_cast<double>(input_a));
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_exp2(const T &input_a) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = ca::exp2(static_cast<double>(input_a[i]));
    }
    return result;
  } else {
    return ca::exp2(static_cast<double>(input_a));
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_exp10(const T &input_a) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = ca::pow(10.0, static_cast<double>(input_a[i]));
    }
    return result;
  } else {
    return ca::pow(10.0, static_cast<double>(input_a));
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_expm1(const T &input_a) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = ca::expm1(static_cast<double>(input_a[i]));
    }
    return result;
  } else {
    return ca::expm1(static_cast<double>(input_a));
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_fabs(const T &input_a) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = static_cast<double>(ca::fabs(input_a[i]));
    }
    return result;
  } else {
    return static_cast<double>(ca::fabs(input_a));
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_fdim(const T &input_a, const T &input_b) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = ca::fdim(static_cast<double>(input_a[i]),
                           static_cast<double>(input_b[i]));
    }
    return result;
  } else {
    return ca::fdim(static_cast<double>(input_a), static_cast<double>(input_b));
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_floor(const T &input_a) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = ca::floor(static_cast<double>(input_a[i]));
    }
    return result;
  } else {
    return ca::floor(static_cast<double>(input_a));
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_mad(const T &input_a, const T &input_b,
                                          const T &input_c) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] =
          static_cast<double>(input_a[i]) * static_cast<double>(input_b[i]) +
          static_cast<double>(input_c[i]);
    }
    return result;
  } else {
    return static_cast<double>(input_a) * static_cast<double>(input_b) +
           static_cast<double>(input_c);
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_maxmag(const T &input_a,
                                             const T &input_b) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      if (ca::abs(input_a[i]) > ca::abs(input_b[i]))
        result[i] = static_cast<double>(input_a[i]);
      else if (ca::abs(input_a[i]) < ca::abs(input_b[i]))
        result[i] = static_cast<double>(input_b[i]);
      else
        result[i] = ca::fmax(static_cast<double>(input_a[i]),
                             static_cast<double>(input_b[i]));
    }
    return result;
  } else {
    if (ca::abs(input_a) > ca::abs(input_b))
      return static_cast<double>(input_a);
    else if (ca::abs(input_a) < ca::abs(input_b))
      return static_cast<double>(input_b);
    else
      return ca::fmax(static_cast<double>(input_a),
                      static_cast<double>(input_b));
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_minmag(const T &input_a,
                                             const T &input_b) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      if (ca::abs(input_a[i]) < ca::abs(input_b[i]))
        result[i] = static_cast<double>(input_a[i]);
      else if (ca::abs(input_a[i]) > ca::abs(input_b[i]))
        result[i] = static_cast<double>(input_b[i]);
      else
        result[i] = ca::fmin(static_cast<double>(input_a[i]),
                             static_cast<double>(input_b[i]));
    }
    return result;
  } else {
    if (ca::abs(input_a) < ca::abs(input_b))
      return static_cast<double>(input_a);
    else if (ca::abs(input_a) > ca::abs(input_b))
      return static_cast<double>(input_b);
    else
      return static_cast<double>(ca::fmin(input_a, input_b));
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_nextafter(const T &input_a,
                                                const T &input_b) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = static_cast<double>(ca::nextafter(input_a[i], input_b[i]));
    }
    return result;
  } else {
    return static_cast<double>(ca::nextafter(input_a, input_b));
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_pow(const T &input_a, const T &input_b) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = ca::pow(static_cast<double>(input_a[i]),
                          static_cast<double>(input_b[i]));
    }
    return result;
  } else {
    return ca::pow(static_cast<double>(input_a), static_cast<double>(input_b));
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_remainder(const T &input_a,
                                                const T &input_b) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = ca::remainder(static_cast<double>(input_a[i]),
                                static_cast<double>(input_b[i]));
    }
    return result;
  } else {
    return ca::remainder(static_cast<double>(input_a),
                         static_cast<double>(input_b));
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_rint(const T &input_a) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = static_cast<double>(ca::rint(input_a[i]));
    }
    return result;
  } else {
    return static_cast<double>(ca::rint(input_a));
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_round(const T &input_a) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = static_cast<double>(ca::round(input_a[i]));
    }
    return result;
  } else {
    return static_cast<double>(ca::round(input_a));
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_rsqrt(const T &input_a) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = 1.0 / ca::sqrt(static_cast<double>(input_a[i]));
    }
    return result;
  } else {
    return 1.0 / ca::sqrt(static_cast<double>(input_a));
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_sin(const T &input) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = ca::sin(static_cast<double>(input[i]));
    }
    return result;
  } else {
    return ca::sin(static_cast<double>(input));
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_sinh(const T &input) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = ca::sinh(static_cast<double>(input[i]));
    }
    return result;
  } else {
    return ca::sinh(static_cast<double>(input));
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_sinpi(const T &input) {
  double pi_value = static_cast<double>(M_PI);
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      if (input[i] == 0) {
        result[i] = 0.0;
      } else if ((ca::floor(input[i]) - input[i]) == 0 &&
                 !ca::isinf(input[i])) {
        result[i] = 0.0;
      } else {
        result[i] = ca::sin(pi_value * static_cast<double>(input[i]));
      }
    }
    return result;
  } else {
    if (input == 0) {
      return 0.0;
    } else if (ca::floor(input) - input == 0 && !ca::isinf(input)) {
      return 0.0;
    } else {
      return ca::sin(pi_value * static_cast<double>(input));
    }
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_tan(const T &input) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = ca::tan(static_cast<double>(input[i]));
    }
    return result;
  } else {
    return ca::tan(static_cast<double>(input));
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_tanh(const T &input) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = ca::tanh(static_cast<double>(input[i]));
    }
    return result;
  } else {
    return ca::tanh(static_cast<double>(input));
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_tanpi(const T &input) {
  double pi_value = static_cast<double>(M_PI);
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      if (ca::fabs(ca::fmod(static_cast<double>(input[i]), 2.0)) == 0.0) {
        result[i] = ca::copysign(0.0, static_cast<double>(input[i]));
      } else if (ca::fabs(ca::fmod(static_cast<double>(input[i]), 2.0)) ==
                 1.0) {
        result[i] = ca::copysign(0.0, -static_cast<double>(input[i]));
      } else {
        result[i] = ca::sin(pi_value * static_cast<double>(input[i])) /
                    ca::cos(pi_value * static_cast<double>(input[i]));
      }
    }
    return result;
  } else {
    if (ca::fabs(ca::fmod(input, T(2.0))) == 0.0) {
      return ca::copysign(0.0, static_cast<double>(input));
    } else if (ca::fabs(ca::fmod(input, T(2.0))) == 1.0) {
      return ca::copysign(0.0, -static_cast<double>(input));
    } else {
      return ca::sin(pi_value * static_cast<double>(input)) /
             ca::cos(pi_value * static_cast<double>(input));
    }
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_tgamma(const T &input) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = ca::tgamma(static_cast<double>(input[i]));
    }
    return result;
  } else {
    return ca::tgamma(static_cast<double>(input));
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_trunc(const T &input) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = static_cast<double>(ca::trunc(input[i]));
    }
    return result;
  } else {
    return static_cast<double>(ca::trunc(input));
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_divide(const T &input_a,
                                             const T &input_b) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] =
          static_cast<double>(input_a[i]) / static_cast<double>(input_b[i]);
    }
    return result;
  } else {
    return static_cast<double>(input_a) / static_cast<double>(input_b);
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_recip(const T &input) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = 1.0 / static_cast<double>(input[i]);
    }
    return result;
  } else {
    return 1.0 / static_cast<double>(input);
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_fma(const T &input_a, const T &input_b,
                                          const T &input_c) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = ca::fma(static_cast<double>(input_a[i]),
                          static_cast<double>(input_b[i]),
                          static_cast<double>(input_c[i]));
    }
    return result;
  } else {
    return ca::fma(static_cast<double>(input_a), static_cast<double>(input_b),
                   static_cast<double>(input_c));
  }
}

template <typename T, typename T_1 = T>
replace_fp_with_double_t<T> calculate_fmax(const T &input_a,
                                           const T_1 &input_b) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      if constexpr (ca::is_vector_v<T_1>) {
        result[i] = ca::fmax(static_cast<double>(input_a[i]),
                             static_cast<double>(input_b[i]));
      } else {
        result[i] = ca::fmax(static_cast<double>(input_a[i]),
                             static_cast<double>(input_b));
      }
    }
    return result;
  } else {
    return ca::fmax(static_cast<double>(input_a), static_cast<double>(input_b));
  }
}

template <typename T, typename T_1 = T>
replace_fp_with_double_t<T> calculate_fmin(const T &input_a,
                                           const T_1 &input_b) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      if constexpr (ca::is_vector_v<T_1>) {
        result[i] = ca::fmin(static_cast<double>(input_a[i]),
                             static_cast<double>(input_b[i]));
      } else {
        result[i] = ca::fmin(static_cast<double>(input_a[i]),
                             static_cast<double>(input_b));
      }
    }
    return result;
  } else {
    return ca::fmin(static_cast<double>(input_a), static_cast<double>(input_b));
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_fmod(const T &input_a, const T &input_b) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = ca::fmod(static_cast<double>(input_a[i]),
                           static_cast<double>(input_b[i]));
    }
    return result;
  } else {
    return ca::fmod(static_cast<double>(input_a), static_cast<double>(input_b));
  }
}

template <typename T, typename T_1 = T>
replace_fp_with_double_t<T> calculate_ldexp(const T &input_a,
                                            const T_1 &input_b) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      if constexpr (ca::is_vector_v<T_1>) {
        result[i] = ca::ldexp(static_cast<double>(input_a[i]), input_b[i]);
      } else {
        result[i] = ca::ldexp(static_cast<double>(input_a[i]), input_b);
      }
    }
    return result;
  } else {
    return ca::ldexp(static_cast<double>(input_a), input_b);
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_hypot(const T &input_a,
                                            const T &input_b) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = ca::hypot(static_cast<double>(input_a[i]),
                            static_cast<double>(input_b[i]));
    }
    return result;
  } else {
    return ca::hypot(static_cast<double>(input_a),
                     static_cast<double>(input_b));
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_lgamma(const T &input_a) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = ca::lgamma(static_cast<double>(input_a[i]));
    }
    return result;
  } else {
    return ca::lgamma(static_cast<double>(input_a));
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_log(const T &input_a) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = ca::log(static_cast<double>(input_a[i]));
    }
    return result;
  } else {
    return ca::log(static_cast<double>(input_a));
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_log2(const T &input_a) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = ca::log2(static_cast<double>(input_a[i]));
    }
    return result;
  } else {
    return ca::log2(static_cast<double>(input_a));
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_log10(const T &input_a) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = ca::log10(static_cast<double>(input_a[i]));
    }
    return result;
  } else {
    return ca::log10(static_cast<double>(input_a));
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_log1p(const T &input_a) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = ca::log1p(static_cast<double>(input_a[i]));
    }
    return result;
  } else {
    return ca::log1p(static_cast<double>(input_a));
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_logb(const T &input_a) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = ca::logb(static_cast<double>(input_a[i]));
    }
    return result;
  } else {
    return ca::logb(static_cast<double>(input_a));
  }
}

template <typename T_1, typename T_2>
replace_fp_with_double_t<T_1> calculate_nan(const T_2 &input_a) {
  if constexpr (ca::is_vector_v<T_2>) {
    replace_fp_with_double_t<T_1> result{};
    for (auto i = 0; i < T_1::vector_size; i++) {
      result[i] = static_cast<double>(
          std::numeric_limits<typename T_1::value_type>::quiet_NaN());
    }
    return result;
  } else {
    return std::numeric_limits<double>::quiet_NaN();
  }
}

template <typename T_1, typename T_2>
replace_fp_with_double_t<T_1> calculate_pown(const T_1 &input_a,
                                             const T_2 &input_b) {
  if constexpr (ca::is_vector_v<T_1>) {
    replace_fp_with_double_t<T_1> result{};
    for (auto i = 0; i < T_1::vector_size; i++) {
      result[i] = ca::pow(static_cast<double>(input_a[i]), input_b[i]);
    }
    return result;
  } else {
    return ca::pow(static_cast<double>(input_a), input_b);
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_powr(const T &input_a, const T &input_b) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      if (powr_special_case(input_a[i], input_b[i])) {
        result[i] = static_cast<double>(
            std::numeric_limits<typename T::value_type>::quiet_NaN());
      } else {
        result[i] = ca::pow(static_cast<double>(input_a[i]),
                            static_cast<double>(input_b[i]));
      }
    }
    return result;
  } else {
    if (powr_special_case(input_a, input_b)) {
      return static_cast<double>(std::numeric_limits<T>::quiet_NaN());
    } else {
      return ca::pow(static_cast<double>(input_a),
                     static_cast<double>(input_b));
    }
  }
}

template <typename T_1, typename T_2>
replace_fp_with_double_t<T_1> calculate_rootn_impl(const T_1 &rad,
                                                   const T_2 &index) {
  double radicand = static_cast<double>(rad);
  const double absRadicand = ca::abs(static_cast<double>(radicand));
  bool indexIsOdd = index % 2;

  if (ca::isnan(static_cast<double>(index)) ||
      (ca::isnan(radicand) && !ca::isinf(static_cast<double>(index)))
      // 7.5.1. Additional Requirements Beyond C99 TC2
      // rootn(x, 0) returns a NaN.
      // rootn(x, n) returns a NaN for x < 0 and n is even.
      || index == 0 || (radicand < -0.0 && !indexIsOdd)) {
    return static_cast<double>(std::numeric_limits<T_1>::quiet_NaN());
  } else if (ca::isinf(static_cast<double>(index))) {
    // no clear definition neither in OpenCL standard, nor in  IEEE 754,
    // hence deriving from std::pow(radicand, 0)
    // pow(base, +-0) returns 1 for any base, even when base is NaN.
    //
    // TODO: For the special case of rootn(0, inf) any output should be
    // accepted, but we're alligning with stdlib definition for the sake of
    // simplicity.
    return 1.0;
  } else if (ca::isinf(radicand)) {
    return ca::copysign(
        static_cast<double>(std::numeric_limits<T_1>::infinity()), radicand);
  } else if (absRadicand == 0.0 && index > 0) {
    // 7.5.1. Additional Requirements Beyond C99 TC2
    // rootn(+-0, n) is +-0 for odd n > 0.
    // rootn(+-0, n) is +0 for even n > 0.
    return ca::copysign(0.0, indexIsOdd ? radicand : 1.0);
  } else if (absRadicand == 0.0 && index < 0) {
    // 7.5.1. Additional Requirements Beyond C99 TC2
    // rootn(+-0, n) is +-inf for odd n < 0.
    // rootn(+-0, n) is +inf for even n < 0.
    return ca::copysign(
        static_cast<double>(std::numeric_limits<T_1>::infinity()),
        indexIsOdd ? radicand : 1.0);
  }
  return ca::copysign(ca::pow(absRadicand, 1.0 / static_cast<double>(index)),
                      radicand);
}

template <typename T_1, typename T_2>
replace_fp_with_double_t<T_1> calculate_rootn(const T_1 &input_a,
                                              const T_2 &input_b) {
  if constexpr (ca::is_vector_v<T_1>) {
    replace_fp_with_double_t<T_1> result{};
    for (auto i = 0; i < T_1::vector_size; i++) {
      result[i] = calculate_rootn_impl(input_a[i], input_b[i]);
    }
    return result;
  } else {
    return calculate_rootn_impl(input_a, input_b);
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_fract(const T &input_a, T &input_b) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      if (ca::isnan(input_a[i])) {
        result[i] = static_cast<double>(
            std::numeric_limits<typename T::value_type>::quiet_NaN());
      } else if (ca::isinf(input_a[i])) {
        result[i] = 0.0;
      } else {
        result[i] = std::fmin(static_cast<double>(input_a[i]) -
                                  ca::floor(static_cast<double>(input_a[i])),
                              0x1.fffffep-1f);
      }
      input_b[i] = ca::floor(static_cast<double>(input_a[i]));
    }
    return result;
  } else {
    if (ca::isnan(input_a)) {
      input_b = ca::floor(static_cast<double>(input_a));
      return std::numeric_limits<T>::quiet_NaN();
    } else if (ca::isinf(static_cast<double>(input_a))) {
      input_b = ca::floor(static_cast<double>(input_a));
      return 0;
    } else {
      input_b = ca::floor(static_cast<double>(input_a));
      return std::fmin(input_a - ca::floor(static_cast<double>(input_a)),
                       0x1.fffffep-1f);
    }
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_modf(const T &input_a, T &input_b) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = std::modf(static_cast<double>(input_a[i]), &input_b[i]);
    }
    return result;
  } else {
    return std::modf(static_cast<double>(input_a), &input_b);
  }
}

template <typename T>
replace_fp_with_double_t<T> calculate_sincos(const T &input_a, T &input_b) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = std::sin(static_cast<double>(input_a[i]));
      input_b[i] = std::cos(static_cast<double>(input_a[i]));
    }
    return result;
  } else {
    input_b = std::cos(static_cast<double>(input_a));
    return std::sin(static_cast<double>(input_a));
  }
}

template <typename T, typename T_1>
replace_fp_with_double_t<T> calculate_frexp(const T &input_a, T_1 &input_b) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = std::frexp(static_cast<double>(input_a[i]), &input_b[i]);
      if (input_b[i] == -1)
        input_b[i] = 0;
    }
    return result;
  } else {
    auto result = std::frexp(static_cast<double>(input_a), &input_b);
    if (input_b == -1)
      input_b = 0;
    return result;
  }
}

template <typename T, typename T_1>
replace_fp_with_double_t<T> calculate_remquo(const T &input_a, const T &input_b,
                                             T_1 &input_c) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
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
      result[i] = std::remquo(static_cast<double>(input_a[i]),
                              static_cast<double>(input_b[i]), &input_c[i]);
    }
    return result;
  } else {
    if ((std::fabs(input_a) == std::numeric_limits<T>::infinity()) ||
        (input_b == 0 && !ca::isnan(input_a)) ||
        ((ca::isnan(input_a)) ^ (ca::isnan(input_b)))) {
      input_c = 0;
      return std::numeric_limits<T>::quiet_NaN();
    } else {
      return std::remquo(static_cast<double>(input_a),
                         static_cast<double>(input_b), &input_c);
    }
  }
}

template <typename T, typename T_1>
replace_fp_with_double_t<T> calculate_lgamma_r(const T &input_a, T_1 &input_b) {
  if constexpr (ca::is_vector_v<T>) {
    replace_fp_with_double_t<T> result{};
    for (auto i = 0; i < T::vector_size; i++) {
      result[i] = std::lgamma(static_cast<double>(input_a[i]));
      input_b[i] = input_a[i] > 0 ? 1 : 0;
    }
    return result;
  } else {
    input_b = input_a > 0 ? 1 : 0;
    return std::lgamma(static_cast<double>(input_a));
  }
}

#endif
