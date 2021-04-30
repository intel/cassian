/*
 * Copyright (C) 2021 Intel Corporation
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
    std::string_view output = captured.GetOutput();
    // FIXME: logging pollutes the stream so for now only check if there is
    // more than global_work_size of lines
    REQUIRE(std::count(output.begin(), output.end(), '\n') >= global_work_size);
  }

  runtime->release_buffer(input_buffer);
  runtime->release_kernel(kernel);
}

template <typename TestType> auto test_name() {
  return std::string(TestType::type_name);
}

template <typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
std::vector<std::string> formats_to_test() {
  return {"%d", "%i"};
}

template <typename T,
          std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
std::vector<std::string> formats_to_test() {
  return {"%f", "%F", "%e", "%E", "%g", "%G", "%a", "%A"};
}

} // namespace

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("printf - arguments", "", ca::ScalarTypes,
                                    test_name<TestType>) {
  using type = typename TestType::scalar_type::host_type;

  const TestConfig &config = get_test_config();
  const auto size = config.work_size();

  for (const auto &format : formats_to_test<type>()) {
    REQUIRE_NOTHROW(test_printf_arguments<TestType>(
        config.runtime(), size, format, 1, config.program_type()));
  }
}