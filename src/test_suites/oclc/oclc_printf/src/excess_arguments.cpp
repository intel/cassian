/*
 * Copyright (C) 2023 Intel Corporation
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

std::vector<char> get_reference(const size_t global_work_size) {
  std::vector<char> output(global_work_size);
  for (size_t i = 0; i < global_work_size; ++i) {
    output[i] = '1';
  }
  return output;
}

void test_printf_excess_arguments_evaluation(ca::Runtime *runtime,
                                             const size_t global_work_size,
                                             const std::string &program_type) {
  size_t size = sizeof(char) * global_work_size;
  const std::string source = ca::load_text_file(
      ca::get_asset("kernels/oclc_printf/excess_arguments.cl"));
  const std::string build_options;
  ca::Kernel kernel =
      runtime->create_kernel("test_printf_excess_arguments_evaluation", source,
                             build_options, program_type);

  ca::Buffer output_buffer = runtime->create_buffer(size);

  runtime->set_kernel_argument(kernel, 0, output_buffer);
  runtime->run_kernel(kernel, global_work_size);

  std::vector<char> output_data =
      runtime->read_buffer_to_vector<char>(output_buffer);

  const std::vector<char> reference = get_reference(global_work_size);
  REQUIRE_THAT(output_data, Catch::Equals(reference));

  runtime->release_buffer(output_buffer);
  runtime->release_kernel(kernel);
}

TEST_CASE("printf - arguments - excess arguments evaluation", "") {
  const TestConfig &config = get_test_config();
  const auto size = config.work_size();
  test_printf_excess_arguments_evaluation(config.runtime(), size,
                                          config.program_type());
}
} // namespace
