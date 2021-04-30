/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/cli/cli.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cassian/utility/utility.hpp>
#include <catch2/catch.hpp>
#include <common.hpp>
#include <cstddef>
#include <string>
#include <test_config.hpp>
#include <vector>

namespace ca = cassian;

template <typename T>
std::vector<T> test(const std::vector<T> &input, ca::Runtime *runtime,
                    const std::string &program_type) {
  const std::string source = ca::load_text_file(
      ca::get_asset("kernels/oclc_address_space_qualifiers/constant.cl"));
  ca::Kernel kernel =
      runtime->create_kernel("test_kernel", source, "", program_type);
  return run_kernel<T>(kernel, {input}, runtime);
}

template <typename T> std::vector<T> reference(const std::vector<T> &input) {
  std::vector<T> output(input.size());
  for (size_t i = 0; i < input.size(); ++i) {
    output[0] += input[i];
  }
  for (size_t i = 1; i < output.size(); i++) {
    output[i] = output[0];
  }
  return output;
}

TEST_CASE("constant argument", "") {
  const TestConfig &config = get_test_config();

  SECTION("read") {
    const int init_value = 5;
    std::vector<int> input(config.work_size(), init_value);
    REQUIRE_THAT(test(input, config.runtime(), config.program_type()),
                 Catch::Equals(reference(input)));
  }
}