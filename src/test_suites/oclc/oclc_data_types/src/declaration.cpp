/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/catch2_utils/catch2_utils.hpp>
#include <cassian/cli/cli.hpp>
#include <cassian/runtime/openclc_type_tuples.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cassian/test_harness/test_harness.hpp>
#include <cassian/utility/metaprogramming.hpp>
#include <cassian/utility/utility.hpp>
#include <catch2/catch.hpp>
#include <common.hpp>
#include <cstdint>
#include <string>
#include <test_config.hpp>
#include <vector>

namespace ca = cassian;

namespace {
template <typename TYPE, typename T = typename TYPE::host_type>
void test(T value, std::vector<T> &output, ca::Runtime *runtime,
          const std::string &program_type) {
  const std::string source = ca::load_text_file(
      ca::get_asset("kernels/oclc_data_types/declaration.cl"));
  const std::string build_options =
      "-DDATA_TYPE=" + std::string(TYPE::device_type) +
      " -DVALUE=" + std::to_string(value);
  ca::Kernel kernel = runtime->create_kernel("test_kernel", source,
                                             build_options, program_type);
  run_kernel<T>(kernel, output, output.size(), runtime);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("types declaration", "", ca::ScalarTypes,
                                    test_name<TestType>) {
  using type = typename TestType::host_type;

  const TestConfig &config = get_test_config();

  ca::Requirements requirements;
  requirements.arithmetic_type<typename TestType::scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  SECTION("assignment") {
    type value = 1;
    std::vector<type> output(1, type(0));
    test<TestType>(value, output, config.runtime(), config.program_type());
    REQUIRE_THAT(output, Catch::Equals(std::vector<type>{1}));
  }
}

} // namespace
