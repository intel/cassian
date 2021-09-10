/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/catch2_utils/catch2_utils.hpp>
#include <cassian/runtime/openclc_type_tuples.hpp>
#include <cassian/runtime/openclc_types.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cassian/test_harness/test_harness.hpp>
#include <cassian/utility/utility.hpp>
#include <cassian/vector/vector.hpp>
#include <catch2/catch.hpp>
#include <cfloat>
#include <climits>
#include <common.hpp>
#include <enum_definitions.hpp>
#include <test_config.hpp>
#include <unordered_map>
#include <vector>

namespace ca = cassian;

CASSIAN_CATCH_REGISTER_INTEGER_MACROS; // NOLINT

namespace {

const std::unordered_map<IntegerMacros, int8_t> reference_char_map = {
    {IntegerMacros::char_min, CHAR_MIN},
    {IntegerMacros::char_max, CHAR_MAX},
    {IntegerMacros::char_bit, CHAR_BIT},
    {IntegerMacros::schar_max, SCHAR_MAX},
    {IntegerMacros::schar_min, SCHAR_MIN}};

const std::unordered_map<IntegerMacros, uint8_t> reference_unsigned_char_map = {
    {IntegerMacros::uchar_max, UCHAR_MAX}};

const std::unordered_map<IntegerMacros, int16_t> reference_short_map = {
    {IntegerMacros::shrt_max, SHRT_MAX}, {IntegerMacros::shrt_min, SHRT_MIN}};

const std::unordered_map<IntegerMacros, uint16_t> reference_unsigned_short_map =
    {{IntegerMacros::ushrt_max, USHRT_MAX}};

const std::unordered_map<IntegerMacros, int32_t> reference_int_map = {
    {IntegerMacros::int_max, INT_MAX}, {IntegerMacros::int_min, INT_MIN}};

const std::unordered_map<IntegerMacros, uint32_t> reference_unsigned_int_map = {
    {IntegerMacros::uint_max, UINT_MAX}};

const std::unordered_map<IntegerMacros, int64_t> reference_long_map = {
    {IntegerMacros::long_max, LLONG_MAX}, {IntegerMacros::long_min, LLONG_MIN}};

const std::unordered_map<IntegerMacros, uint64_t> reference_unsigned_long_map =
    {{IntegerMacros::ulong_max, ULLONG_MAX}};

template <typename T> auto get_reference_map() {
  if constexpr (std::is_same<int8_t, T>::value) {
    return reference_char_map;
  } else if constexpr (std::is_same<uint8_t, T>::value) {
    return reference_unsigned_char_map;
  } else if constexpr (std::is_same<int16_t, T>::value) {
    return reference_short_map;
  } else if constexpr (std::is_same<uint16_t, T>::value) {
    return reference_unsigned_short_map;
  } else if constexpr (std::is_same<int32_t, T>::value) {
    return reference_int_map;
  } else if constexpr (std::is_same<uint32_t, T>::value) {
    return reference_unsigned_int_map;
  } else if constexpr (std::is_same<int64_t, T>::value) {
    return reference_long_map;
  } else if constexpr (std::is_same<uint64_t, T>::value) {
    return reference_unsigned_long_map;
  } else {
    throw UnknownTypeException("Unknown test type");
  }
}

template <typename T>
std::vector<T> run_kernel_macro(const size_t &work_size,
                                cassian::Runtime *runtime,
                                const cassian::Kernel &kernel) {
  cassian::Buffer buffer_inout = runtime->create_buffer(work_size * sizeof(T));
  runtime->set_kernel_argument(kernel, static_cast<int>(0), buffer_inout);
  runtime->run_kernel(kernel, work_size);
  auto output = runtime->read_buffer_to_vector<T>(buffer_inout);
  runtime->release_buffer(buffer_inout);
  return output;
}

template <typename T>
std::vector<T>
test_macro(const size_t &work_size, const std::string &build_options,
           const std::string &program_type, cassian::Runtime *runtime) {
  const std::string source = cassian::load_text_file(
      cassian::get_asset("kernels/oclc_integer_functions/integer_macros.cl"));
  cassian::Kernel kernel = runtime->create_kernel("integer_macros", source,
                                                  build_options, program_type);
  auto output = run_kernel_macro<T>(work_size, runtime, kernel);
  runtime->release_kernel(kernel);
  return output;
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("integer_macros", "",
                                    ca::IntegerScalarTypes,
                                    test_name<TestType>) {
  const auto &config = get_test_config();
  using scalar_type = typename TestType::scalar_type;
  using host_type = typename TestType::host_type;
  ca::Requirements requirements;
  requirements.arithmetic_type<scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }
  const auto &reference_map = get_reference_map<host_type>();
  const auto work_size = config.work_size();
  for (const auto &[macro_enum, macro_ref_value] : reference_map) {
    std::string macro_string =
        Catch::StringMaker<IntegerMacros>::convert(macro_enum);
    std::transform(macro_string.begin(), macro_string.end(),
                   macro_string.begin(), ::toupper);
    DYNAMIC_SECTION(macro_string) {
      const std::string build_options =
          "-DTYPE=" + std::string(TestType::device_type) + " -D" +
          macro_string + "_MACRO";
      const auto result = test_macro<host_type>(
          work_size, build_options, config.program_type(), config.runtime());
      std::vector<host_type> reference(work_size, macro_ref_value);
      REQUIRE_THAT(reference, Catch::Equals(result));
    }
  }
}

} // namespace
