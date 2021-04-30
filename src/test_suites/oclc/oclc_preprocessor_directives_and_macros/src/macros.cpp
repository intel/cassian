/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/logging/logging.hpp>
#include <cassian/runtime/openclc_types.hpp>
#include <cassian/runtime/runtime.hpp>
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

template <typename T>
std::vector<T> test_macro(const TestConfig &config, const std::vector<T> &input,
                          const std::string &build_options) {
  const auto kernel_source = ca::load_text_file(ca::get_asset(
      "kernels/oclc_preprocessor_directives_and_macros/test_macros.cl"));
  ca::Runtime *runtime = config.runtime();
  const auto program_type = config.program_type();
  const ca::Kernel kernel = runtime->create_kernel("macro_test", kernel_source,
                                                   build_options, program_type);
  auto output = run_kernel(kernel, input, runtime);
  runtime->release_kernel(kernel);
  return output;
}

TEST_CASE("Preprocessor directives and macros", "") {
  const TestConfig &config = get_test_config();
  const size_t work_size = config.work_size();
  std::stringstream build_options;
  SECTION("String literals") {
    using type = ca::clc_char_t;
    build_options << "-DTYPE=" << type::device_type;
    SECTION("__FILE__") {
      const std::string file_name = ca::convert_to_forward_slashes(__FILE__);
      build_options << " -DFILE_MACRO -DFILE_NAME=" + file_name;
      std::vector<type::host_type> input(work_size * (file_name.length() + 1));
      const auto output = test_macro(config, input, build_options.str());
      REQUIRE_THAT(output,
                   FileMacroMatcher<type::host_type>(file_name, work_size));
    }
  }
  SECTION("Integers") {
    using type = ca::clc_int_t;
    build_options << "-DTYPE=" << type::device_type;
    std::vector<type::host_type> input(work_size);
    SECTION("__OPENCL_C_VERSION__") {
      const std::initializer_list<std::tuple<std::string, int>>
          oclc_version_std_list = {{"1.2", 120}, {"2.0", 200}, {"3.0", 300}};
      build_options << " -DOCLC_VERSION_MACRO";
      for (const auto &[cl_std_value, reference_value] :
           oclc_version_std_list) {
        DYNAMIC_SECTION("CL" << cl_std_value) {
          build_options << " -cl-std=CL" << cl_std_value;
          const auto output = test_macro(config, input, build_options.str());
          std::vector<type::host_type> reference(work_size);
          std::fill(reference.begin(), reference.end(), reference_value);
          REQUIRE_THAT(reference, Catch::Equals(output));
        }
      }
      SECTION("CL default") {
        const auto output = test_macro(config, input, build_options.str());
        REQUIRE_THAT(output, IsBetweenMatcher(120, 300));
      }
    }
  }
}

} // namespace
