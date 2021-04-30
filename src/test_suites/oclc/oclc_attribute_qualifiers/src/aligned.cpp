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

namespace {

void test_aligned(ca::Runtime *runtime, const size_t global_work_size,
                  const std::string &program_type) {
  const std::string source = ca::load_text_file(
      ca::get_asset("kernels/oclc_attribute_qualifiers/aligned.cl"));
  ca::Kernel kernel =
      runtime->create_kernel("test_kernel", source, "", program_type);
  runtime->run_kernel(kernel, global_work_size);
  runtime->release_kernel(kernel);
}

} // namespace

TEST_CASE("aligned", "") {
  const TestConfig &config = get_test_config();
  const auto size = config.work_size();
  REQUIRE_NOTHROW(test_aligned(config.runtime(), size, config.program_type()));
}