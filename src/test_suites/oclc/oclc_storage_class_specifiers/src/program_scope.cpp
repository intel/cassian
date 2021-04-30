/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "common.hpp"
#include "test_config.hpp"
#include <cassian/cli/cli.hpp>
#include <cassian/runtime/access_qualifier.hpp>
#include <cassian/runtime/image_properties.hpp>
#include <cassian/runtime/program_descriptor.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cassian/test_harness/test_harness.hpp>
#include <cassian/utility/utility.hpp>
#include <catch2/catch.hpp>
#include <cstddef>
#include <string>
#include <vector>

namespace ca = cassian;

std::vector<uint32_t> test_program_scope(const std::vector<uint32_t> &input,
                                         const std::string &build_options,
                                         ca::Runtime *runtime,
                                         const std::string &program_type) {
  const std::string source = ca::load_text_file(
      ca::get_asset("kernels/oclc_storage_class_specifiers/program_scope.cl"));
  ca::Kernel kernel = runtime->create_kernel("test_program_scope", source,
                                             build_options, program_type);
  auto output = run_kernel<uint32_t>(input, runtime, kernel);
  runtime->release_kernel(kernel);
  return output;
}

template <typename T> std::vector<T> calculate_reference(std::vector<T> input) {
  std::vector<T> reference(input.size());
  for (auto i = 0U; i < input.size(); i++) {
    reference[i] = input[i] + 1;
  }
  return reference;
}

TEST_CASE("storage class specifiers on program scope variables", "") {
  const TestConfig &config = get_test_config();
  std::vector<uint32_t> input(config.work_size());
  std::iota(input.begin(), input.end(), 0U);
  std::vector<uint32_t> reference = calculate_reference(input);
  SECTION("typdef") {
    std::vector<uint32_t> golden = test_program_scope(
        input, "-DTYPEDEF", config.runtime(), config.program_type());
    REQUIRE_THAT(reference, Catch::Equals(golden));
  }
  SECTION("static") {
    std::vector<uint32_t> golden = test_program_scope(
        input, "-DSTATIC", config.runtime(), config.program_type());
    REQUIRE_THAT(reference, Catch::Equals(golden));
  }
  SECTION("extern") {
    std::vector<uint32_t> golden = test_program_scope(
        input, "-DEXTERN", config.runtime(), config.program_type());
    REQUIRE_THAT(reference, Catch::Equals(golden));
  }
}