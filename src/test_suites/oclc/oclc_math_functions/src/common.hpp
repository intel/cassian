/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_OCLC_MATH_FUNCTIONS_COMMON_HPP
#define CASSIAN_OCLC_MATH_FUNCTIONS_COMMON_HPP

#include <algorithm>
#include <cassian/random/random.hpp>
#include <cassian/runtime/openclc_types.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cassian/test_harness/test_harness.hpp>
#include <cassian/utility/metaprogramming.hpp>
#include <cassian/utility/utility.hpp>
#include <cassian/vector/type_helpers.hpp>
#include <cassian/vector/vector.hpp>
#include <catch2/catch.hpp>
#include <common.hpp>
#include <enum_definitions.hpp>
#include <string>
#include <test_config.hpp>
#include <type_traits>
#include <unordered_map>
#include <vector>

int suggest_work_size(const std::string &type);

class UnknownFunctionException : public std::runtime_error {
  using std::runtime_error::runtime_error;
};

class UnknownTypeException : public std::runtime_error {
  using std::runtime_error::runtime_error;
};

using FloatingPointScalarTypes =
    cassian::TupleConcat<cassian::clc_float_t, cassian::clc_double_t>;

template <typename T>
T generate_value(const cassian::scalar_type_v<T> &min,
                 const cassian::scalar_type_v<T> &max,
                 const std::vector<cassian::scalar_type_v<T>> except =
                     std::vector<cassian::scalar_type_v<T>>()) {
  return cassian::generate_value<T>(min, max, 0, except);
}

template <typename T> T generate_value() {
  const auto max = std::numeric_limits<cassian::scalar_type_v<T>>::max();
  return cassian::generate_value<T>(-max, max, 0);
}

template <typename T, typename BITS_TYPE> BITS_TYPE encoding_bits(const T &x) {
  auto result = 0ULL;
  std::memcpy(&result, &x, sizeof(T));
  return static_cast<BITS_TYPE>(result);
}

template <typename T, typename BITS_TYPE>
BITS_TYPE ulp_distance(const T &a, const T &b) {
  BITS_TYPE high_bit = std::numeric_limits<BITS_TYPE>::max() ^
                       std::numeric_limits<BITS_TYPE>::max() >> 1;
  if (a == b) {
    return 0;
  }
  const auto max = std::numeric_limits<BITS_TYPE>::max();
  if (std::isnan(a) || std::isnan(b)) {
    return max;
  }
  if (std::isinf(a) || std::isinf(b)) {
    return max;
  }
  BITS_TYPE a_bits = encoding_bits<T, BITS_TYPE>(a);
  BITS_TYPE b_bits = encoding_bits<T, BITS_TYPE>(b);
  if ((high_bit & a_bits) != 0U) {
    a_bits = high_bit - a_bits;
  }
  if ((high_bit & b_bits) != 0U) {
    b_bits = high_bit - b_bits;
  }
  return (b_bits >= a_bits) ? b_bits - a_bits : a_bits - b_bits;
}

template <typename T>
std::vector<T> run_kernel_macro(const size_t &work_size,
                                cassian::Runtime *runtime,
                                const cassian::Kernel &kernel) {
  cassian::Buffer buffer_inout = runtime->create_buffer(work_size * sizeof(T));
  runtime->set_kernel_argument(kernel, static_cast<int>(0), buffer_inout);
  runtime->run_kernel(kernel, work_size);
  const auto output = runtime->read_buffer_to_vector<T>(buffer_inout);
  runtime->release_buffer(buffer_inout);
  return output;
}

template <typename T>
std::vector<T> test(const size_t &work_size, const std::string &build_options,
                    const std::string &program_type, cassian::Runtime *runtime,
                    const std::string &kernel_name) {
  const std::string source = cassian::load_text_file(
      cassian::get_asset("kernels/oclc_math_functions/" + kernel_name + ".cl"));
  cassian::Kernel kernel =
      runtime->create_kernel(kernel_name, source, build_options, program_type);
  const auto output = run_kernel_macro<T>(work_size, runtime, kernel);
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
  cassian::Requirements requirements;
  requirements.arithmetic_type<scalar_type>();
  if (cassian::should_skip_test(requirements, *config.runtime())) {
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
      const auto result =
          test<host_type>(work_size, build_options, config.program_type(),
                          config.runtime(), kernel_name);
      std::vector<host_type> reference(work_size, macro_ref_value);
      REQUIRE_THAT(reference, Catch::Equals(result));
    }
  }
}

template <typename TestType> auto test_name() {
  return std::string(TestType::type_name);
}

#endif
