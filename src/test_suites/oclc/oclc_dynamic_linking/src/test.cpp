/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <catch2/catch.hpp>

#include <cassian/main/flags_builder.hpp>
#include <cassian/main/test_helper.hpp>
#include <cassian/random/random.hpp>
#include <cassian/reference/dp4a.hpp>
#include <cassian/utility/utility.hpp>

namespace ca = cassian;
using ca::test::FlagsBuilder;
using ca::test::Language;

TEST_CASE("oclc_dynamic_link_single_file", "[oclc][link]") {
  const std::string source = ca::load_text_file(
      ca::get_asset("kernels/oclc_dynamic_linking/single.cl"));

  constexpr auto simt = 16;

  auto input = ca::generate_vector<int>(simt, 0);
  decltype(input) output;

  ca::test::output(output, input.size());
  ca::test::input(input);

  const auto flags =
      FlagsBuilder(Language::opencl_c).append("-cl-std=CL3.0").str();
  const auto program_type = ca::test::default_program_type();

  ca::test::kernel(simt, "copy_buffer",
                   {ca::ProgramDescriptor(source, flags, program_type)});

  REQUIRE_THAT(output, Catch::Equals(input));
}

TEST_CASE("oclc_dynamic_link_external_function", "[oclc][link]") {
  const std::string kernel = ca::load_text_file(
      ca::get_asset("kernels/oclc_dynamic_linking/extern_function_kernel.cl"));
  const std::string callee = ca::load_text_file(
      ca::get_asset("kernels/oclc_dynamic_linking/extern_function_callee.cl"));

  constexpr auto simt = 16;

  const auto flags =
      FlagsBuilder(Language::opencl_c).append("-cl-std=CL3.0").str();
  const auto program_type = ca::test::default_program_type();

  const std::vector<ca::ProgramDescriptor> programs = {
      ca::ProgramDescriptor(kernel, flags, program_type),
      ca::ProgramDescriptor(callee, flags, program_type,
                            "-library-compilation"),
  };

  auto a = ca::generate_vector<int>(simt, 0);
  auto b = ca::generate_vector<int>(simt, 0);

  decltype(a) c;
  decltype(a) cref;

  std::transform(std::begin(a), std::end(a), std::begin(b),
                 std::back_inserter(cref),
                 [](auto x, auto y) { return x + y; });

  ca::test::input(a);
  ca::test::input(b);
  ca::test::output(c, cref.size());

  ca::test::kernel(simt, "vector_add", programs);

  REQUIRE_THAT(c, Catch::Equals(cref));
}

TEST_CASE("oclc_dynamic_link_circular_dependencies", "[oclc][link]") {
  const std::string kernel = ca::load_text_file(ca::get_asset(
      "kernels/oclc_dynamic_linking/circular_dependencies_kernel.cl"));
  const std::string callee = ca::load_text_file(ca::get_asset(
      "kernels/oclc_dynamic_linking/circular_dependencies_callee.cl"));

  constexpr auto simt = 16;

  const auto flags =
      FlagsBuilder(Language::opencl_c).append("-cl-std=CL3.0").str();
  const auto program_type = ca::test::default_program_type();

  const std::vector<ca::ProgramDescriptor> programs = {
      ca::ProgramDescriptor(kernel, flags, program_type,
                            "-library-compilation"),
      ca::ProgramDescriptor(callee, flags, program_type,
                            "-library-compilation"),
  };

  auto a = ca::generate_vector<int>(simt, 0);
  auto b = ca::generate_vector<int>(simt, 0);

  decltype(a) c;
  decltype(a) cref;

  std::transform(std::begin(a), std::end(a), std::begin(b),
                 std::back_inserter(cref),
                 [](auto x, auto y) { return x + y; });

  ca::test::input(a);
  ca::test::input(b);
  ca::test::output(c, cref.size());

  ca::test::kernel(simt, "vector_add", programs);

  REQUIRE_THAT(c, Catch::Equals(cref));
}
