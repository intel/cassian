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
void test(std::vector<T> &inout, ca::Runtime *runtime,
          const std::string &program_type) {
  const std::string source = ca::load_text_file(
      ca::get_asset("kernels/oclc_address_space_qualifiers/global.cl"));
  ca::Kernel kernel =
      runtime->create_kernel("test_kernel", source, "", program_type);
  run_kernel_read_write_buffer<T>(kernel, inout, runtime);
}

template <typename T> void reference(std::vector<T> &inout) {
  for (size_t i = 0; i < inout.size(); ++i) {
    inout[i] *= inout[i];
  }
}

TEST_CASE("global", "") {
  const TestConfig &config = get_test_config();

  SECTION("read write buffer") {
    const int init_value = 5;
    std::vector<int> inout(config.work_size(), init_value);
    auto inout_ref = inout;
    test(inout, config.runtime(), config.program_type());
    reference(inout_ref);
    REQUIRE_THAT(inout, Catch::Equals(inout_ref));
  }
}