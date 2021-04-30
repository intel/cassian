/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/runtime/runtime.hpp>
#include <cassian/test_harness/test_harness.hpp>
#include <cassian/utility/utility.hpp>
#include <catch2/catch.hpp>
#include <common.hpp>
#include <sstream>
#include <string>
#include <test_config.hpp>
#include <tuple>
#include <vector>

namespace ca = cassian;

namespace {

void test_extensions(const TestConfig &config, const std::string &kernel_source,
                     const std::string &build_options) {
  auto *runtime = config.runtime();
  const auto program_type = config.program_type();
  const ca::Kernel kernel = runtime->create_kernel(
      "test_extensions", kernel_source, build_options, program_type);
  runtime->release_kernel(kernel);
}
std::string
pragma_prepend(const std::string &kernel_source,
               const std::vector<std::string_view> &extension_vector) {
  std::stringstream ss;
  for (const auto &extension_name : extension_vector) {
    ss << "#pragma OPENCL EXTENSION " << extension_name << " : enable\n";
  }
  ss << kernel_source;
  return ss.str();
}

std::string to_string(const std::vector<std::string_view> &extension_vector) {
  std::stringstream ss;
  for (auto i = 0U; i < extension_vector.size(); i++) {
    ss << extension_vector[i] << (i + 1 < extension_vector.size() ? ", " : "");
  }
  return ss.str();
}

const std::initializer_list<
    std::tuple<ca::Feature, std::vector<std::string_view>>>
    extension_list = {
        {ca::Feature::int64_atomics,
         {"cl_khr_int64_base_atomics", "cl_khr_int64_extended_atomics"}}};

TEST_CASE("Pragmas", "") {
  const TestConfig &config = get_test_config();
  auto kernel_source = ca::load_text_file(ca::get_asset(
      "kernels/oclc_preprocessor_directives_and_macros/test_extensions.cl"));
  std::stringstream build_options;
  build_options << "-cl-std=CL3.0";
  for (const auto &[feature, extension_vector] : extension_list) {
    ca::Requirements requirements = {};
    requirements.feature(feature);
    DYNAMIC_SECTION("Extension: " << to_string(extension_vector)) {
      if (ca::should_skip_test(requirements, *config.runtime())) {
        return;
      }
      kernel_source = pragma_prepend(kernel_source, extension_vector);
      for (const auto &extension : extension_vector) {
        build_options << " -D" << extension << "_MACRO";
      }
      REQUIRE_NOTHROW(
          test_extensions(config, kernel_source, build_options.str()));
    }
  }
}

} // namespace
