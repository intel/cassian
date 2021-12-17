/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <catch2/catch.hpp>

#include <cassian/fp_types/half.hpp>
#include <cassian/main/flags_builder.hpp>
#include <cassian/main/test_helper.hpp>
#include <cassian/utility/utility.hpp>

namespace ca = cassian;
using ca::half;
using ca::test::FlagsBuilder;
using ca::test::Language;

TEST_CASE("cm_stochastic_rounding_float_to_half", "[cm][srnd][half]") {
  constexpr auto simd = 16;
  const std::string source = cassian::load_text_file(cassian::get_asset(
      "kernels/cm_stochastic_rounding/stochastic_rounding_genx.cpp"));

  const float min = static_cast<float>(std::numeric_limits<half>::min());
  const float max = static_cast<float>(std::numeric_limits<half>::max());

  SECTION("Vector") {
    auto in = ca::generate_vector<float>(simd, min, max, 0);
    auto rnd = ca::generate_vector<float>(simd, 0);
    std::vector<half> ref;

    std::transform(std::begin(in), std::end(in), std::begin(rnd),
                   std::back_inserter(ref),
                   [](float in, float rnd) { return half(in, rnd); });

    decltype(ref) out;

    ca::test::input(in);
    ca::test::input(rnd);
    ca::test::output(out, ref.size());

    ca::test::kernel("kernel", source,
                     FlagsBuilder(Language::cm)
                         .define("SRC_VECTOR")
                         .define("SIMD", std::to_string(simd))
                         .define("IN_TYPE", ca::to_cm_string<float>())
                         .define("OUT_TYPE", ca::to_cm_string<half>())
                         .str());

    REQUIRE_THAT(out, Catch::Equals(ref));
  }

  SECTION("Matrix") {
    constexpr auto width = 8;
    constexpr auto height = simd / width;

    auto in = ca::generate_vector<float>(simd, min, max, 0);
    auto rnd = ca::generate_vector<float>(simd, 0);
    std::vector<half> ref;

    std::transform(std::begin(in), std::end(in), std::begin(rnd),
                   std::back_inserter(ref),
                   [](float in, float rnd) { return half(in, rnd); });

    decltype(ref) out;

    ca::test::input(in);
    ca::test::input(rnd);
    ca::test::output(out, ref.size());

    ca::test::kernel("kernel", source,
                     FlagsBuilder(Language::cm)
                         .define("SRC_MATRIX")
                         .define("SIMD", std::to_string(simd))
                         .define("WIDTH", std::to_string(width))
                         .define("HEIGHT", std::to_string(height))
                         .define("IN_TYPE", ca::to_cm_string<float>())
                         .define("OUT_TYPE", ca::to_cm_string<half>())
                         .str());

    REQUIRE_THAT(out, Catch::Equals(ref));
  }

  SECTION("Scalar") {
    auto in = ca::generate_value<float>(min, max, 0);
    auto rnd = ca::generate_value<float>(0);
    std::vector<half> ref(simd, half(in, rnd));

    decltype(ref) out;

    ca::test::pass(in);
    ca::test::pass(rnd);
    ca::test::output(out, ref.size());

    ca::test::kernel("kernel", source,
                     FlagsBuilder(Language::cm)
                         .define("SRC_SCALAR")
                         .define("SIMD", std::to_string(simd))
                         .define("IN_TYPE", ca::to_cm_string<float>())
                         .define("OUT_TYPE", ca::to_cm_string<half>())
                         .str());

    REQUIRE_THAT(out, Catch::Equals(ref));
  }
}
