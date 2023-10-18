/*
 * Copyright (C) 2021-2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "common.hpp"
#include "test_config.hpp"
#include <cassian/catch2_utils/catch2_utils.hpp>
#include <cassian/cli/cli.hpp>
#include <cassian/runtime/openclc_type_tuples.hpp>
#include <cassian/runtime/openclc_types.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cassian/test_harness/test_harness.hpp>
#include <cassian/utility/utility.hpp>
#include <catch2/catch.hpp>
#include <cstdint>
#include <numeric>
#include <string>
#include <type_traits>
#include <vector>

namespace ca = cassian;

namespace {

template <typename TYPE, typename V = typename TYPE::host_type::value_type>
void test_printf_arguments(ca::Runtime *runtime, const size_t global_work_size,
                           const std::string &format, const V value,
                           const std::string &program_type) {
  const std::string source =
      ca::load_text_file(ca::get_asset("kernels/oclc_printf/arguments.cl"));
  const std::string build_options = "-DTYPE=" + std::string(TYPE::device_type) +
                                    " -DSTRING=\"" + format + "\\n\"";
  ca::Kernel kernel = runtime->create_kernel("test_printf_arguments", source,
                                             build_options, program_type);

  ca::Buffer input_buffer =
      runtime->create_buffer(sizeof(V) * global_work_size);

  runtime->write_buffer_from_vector<V>(input_buffer,
                                       std::vector<V>(global_work_size, value));

  runtime->set_kernel_argument(kernel, 0, input_buffer);
  {
    CaptureOutput captured(CaptureOutput::Stdout);
    runtime->run_kernel(kernel, global_work_size);
    std::string output = captured.GetOutput();
    // FIXME: logging pollutes the stream so for now only check if there is
    // more than global_work_size of lines
    REQUIRE(std::count(output.begin(), output.end(), '\n') >= global_work_size);
  }

  runtime->release_buffer(input_buffer);
  runtime->release_kernel(kernel);
}

std::string get_full_format(const std::string &format,
                            const std::string &flag) {
  return "%" + flag + format;
}

template <typename TestType> auto test_name() {
  return std::string(TestType::type_name);
}

template <
    typename T,
    std::enable_if_t<std::is_integral_v<T> && std::is_signed_v<T>, bool> = true>
std::vector<std::string> formats() {
  return {"d", "i"};
}

template <typename T,
          std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T>,
                           bool> = true>
std::vector<std::string> formats() {
  return {"u", "o", "x", "X"};
}

template <typename T,
          std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
std::vector<std::string> formats() {
  return {"f", "F", "e", "E", "g", "G", "a", "A"};
}

std::vector<std::string> flags_none() { return {""}; }

std::vector<std::string> flags_single() { return {"-", "+", " ", "013"}; }

std::vector<std::string> flags_two() { return {"-07", " 04"}; }

std::vector<std::string> flags_three() { return {"+ 06", " +05"}; }

std::vector<std::string> flags_four() { return {"-+ 017"}; }

std::vector<std::string> flags_repeating() { return {"+++", " - -"}; }

std::vector<std::string> flags_precision() { return {".5", "-+ 017.3"}; }

std::vector<std::string> flags_length_modifiers() {
  return {"hh", "-+ 011.3h", "l"};
}

std::vector<std::string> flags_length_modifiers_with_vectors() {
  return {"v4hh", "-+ 012.3v4h", "v4hl", ".2v4l"};
}

std::vector<std::string> flags_hash() { return {"#", "-+ #012.3h", " +#"}; }

template <typename TestType>
void run_test(const std::function<std::vector<std::string>()> &flags) {
  using type = typename TestType::scalar_type::host_type;
  const TestConfig &config = get_test_config();

  ca::Requirements requirements;
  requirements.arithmetic_type<typename TestType::scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  const auto size = config.work_size();
  ca::logging::debug() << "Type: " << std::string(TestType::type_name) << "\n";
  for (const auto &format : formats<type>()) {
    for (const auto &flag : flags()) {
      const std::string full_format = get_full_format(format, flag);
      ca::logging::debug() << "Format: " << full_format << "\n";
      REQUIRE_NOTHROW(test_printf_arguments<TestType>(
          config.runtime(), size, full_format, 1, config.program_type()));
    }
  }
}

using scalar_and_vector_types =
    ca::TupleConcat<ca::ScalarTypes, ca::VectorTypes>::type;

using length_modifiers_with_vectors_flags_types =
    std::tuple<ca::detail::OpenCLCInt<4>, ca::detail::OpenCLCLong<4>>;

using length_modifiers_flags_types =
    std::tuple<ca::detail::OpenCLCUshort<4>, ca::detail::OpenCLCLong<4>>;

using precision_flags_types =
    std::tuple<ca::detail::OpenCLCUlong<4>, ca::detail::OpenCLCUint<3>>;

using two_and_three_flags_types =
    std::tuple<ca::detail::OpenCLCChar<1>, ca::detail::OpenCLCShort<3>>;

using repeating_flags_types =
    std::tuple<ca::detail::OpenCLCUshort<1>, ca::detail::OpenCLCDouble<3>>;

using hash_flags_types =
    std::tuple<ca::detail::OpenCLCFloat<1>, ca::detail::OpenCLCDouble<1>>;

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("printf - arguments", "",
                                    scalar_and_vector_types,
                                    test_name<TestType>) {
  run_test<TestType>(flags_none);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("printf - arguments - single flag", "",
                                    scalar_and_vector_types,
                                    test_name<TestType>) {
  run_test<TestType>(flags_single);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("printf - arguments - two flags", "",
                                    two_and_three_flags_types,
                                    test_name<TestType>) {
  run_test<TestType>(flags_two);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("printf - arguments - three flags", "",
                                    two_and_three_flags_types,
                                    test_name<TestType>) {
  run_test<TestType>(flags_three);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("printf - arguments - four flags", "",
                                    scalar_and_vector_types,
                                    test_name<TestType>) {
  run_test<TestType>(flags_four);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("printf - arguments - repeating flags", "",
                                    repeating_flags_types,
                                    test_name<TestType>) {
  run_test<TestType>(flags_repeating);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("printf - arguments - precision", "",
                                    precision_flags_types,
                                    test_name<TestType>) {
  run_test<TestType>(flags_precision);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("printf - arguments - length modifiers", "",
                                    length_modifiers_flags_types,
                                    test_name<TestType>) {
  run_test<TestType>(flags_length_modifiers);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(
    "printf - arguments - length modifiers with vectors", "",
    length_modifiers_with_vectors_flags_types, test_name<TestType>) {
  run_test<TestType>(flags_length_modifiers_with_vectors);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("printf - arguments - hash flag", "",
                                    hash_flags_types, test_name<TestType>) {
  run_test<TestType>(flags_hash);
}
} // namespace
