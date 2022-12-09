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

void test_printf(ca::Runtime *runtime, const size_t global_work_size,
                 const std::string &program_type) {
  const std::string source =
      ca::load_text_file(ca::get_asset("kernels/oclc_printf/text.cl"));
  ca::Kernel kernel =
      runtime->create_kernel("test_printf", source, "", program_type);
  {
    CaptureOutput captured(CaptureOutput::Stdout);
    runtime->run_kernel(kernel, global_work_size);
    std::string output = captured.GetOutput();
    // FIXME: logging pollutes the stream so for now only check if there is
    // more than global_work_size of lines
    REQUIRE(std::count(output.begin(), output.end(), '\n') >= global_work_size);
  }
  runtime->release_kernel(kernel);
}

} // namespace

TEST_CASE("printf - text", "") {
  const TestConfig &config = get_test_config();
  const auto size = config.work_size();
  REQUIRE_NOTHROW(test_printf(config.runtime(), size, config.program_type()));
}