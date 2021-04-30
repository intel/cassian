/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "common.hpp"
#include "test_config.hpp"
#include <cassian/cli/cli.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cassian/utility/utility.hpp>
#include <catch2/catch.hpp>
#include <cstdint>
#include <numeric>
#include <string>
#include <vector>

namespace ca = cassian;

std::vector<uint32_t> test_multiple_kernels(const std::vector<uint32_t> &input,
                                            ca::Runtime *runtime,
                                            const std::string &program_type) {
  const std::string source = ca::load_text_file(
      ca::get_asset("kernels/oclc_function_qualifiers/multiple_kernels.cl"));
  ca::Kernel kernel =
      runtime->create_kernel("multi_kernel", source, "", program_type);
  auto output_vector = run_kernel_multiple<uint32_t>(
      kernel, input, {input.size(), 1, 1}, runtime);
  runtime->release_kernel(kernel);
  return output_vector;
}

TEST_CASE("multiple kernels in a single program", "") {
  const TestConfig &config = get_test_config();
  SECTION("kernel can call another kernel") {
    const auto size = config.work_size();
    std::vector<uint32_t> reference(size, 0U);
    std::iota(reference.begin(), reference.end(), 0);
    const auto golden = test_multiple_kernels(reference, config.runtime(),
                                              config.program_type());
    REQUIRE_THAT(reference, Catch::Equals(golden));
  }
}