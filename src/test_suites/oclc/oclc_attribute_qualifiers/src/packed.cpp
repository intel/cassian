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

std::vector<uint64_t> run_kernel(const cassian::Kernel &kernel,
                                 const size_t global_work_size,
                                 cassian::Runtime *runtime) {
  ca::Buffer buffer =
      runtime->create_buffer(global_work_size * 2 * sizeof(uint64_t));
  runtime->set_kernel_argument(kernel, 0, buffer);
  runtime->run_kernel(kernel, global_work_size);
  std::vector<uint64_t> output =
      runtime->read_buffer_to_vector<uint64_t>(buffer);
  runtime->release_buffer(buffer);
  return output;
}

std::vector<uint64_t> test_packed(ca::Runtime *runtime,
                                  const size_t global_work_size,
                                  const std::string &program_type) {
  const std::string source = ca::load_text_file(
      ca::get_asset("kernels/oclc_attribute_qualifiers/packed.cl"));
  ca::Kernel kernel =
      runtime->create_kernel("test_kernel", source, "", program_type);
  auto output_vector = run_kernel(kernel, global_work_size, runtime);
  runtime->release_kernel(kernel);
  return output_vector;
}

} // namespace

TEST_CASE("packed", "") {
  const TestConfig &config = get_test_config();
  const auto size = config.work_size();
  const auto output =
      test_packed(config.runtime(), size, config.program_type());
  std::vector<uint64_t> reference(size * 2, 5);
  REQUIRE_THAT(output, Catch::Equals(reference));
}