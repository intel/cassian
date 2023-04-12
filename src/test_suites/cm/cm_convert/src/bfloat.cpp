/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <catch2/catch.hpp>

#include <cassian/fp_types/bfloat16.hpp>
#include <cassian/fp_types/half.hpp>
#include <cassian/main/flags_builder.hpp>
#include <cassian/main/test_helper.hpp>
#include <cassian/utility/utility.hpp>

namespace ca = cassian;
using ca::bfloat16;
using ca::half;
using ca::test::FlagsBuilder;
using ca::test::Language;

TEST_CASE("cm_convert_float_to_bfloat", "[cm][bfloat]") {
  constexpr auto simd = 16;
  const std::string source = cassian::load_text_file(
      cassian::get_asset("kernels/cm_convert/convert_genx.cpp"));

  SECTION("Vector") {
    auto in = ca::generate_vector<float>(simd, 0);
    std::vector<bfloat16> ref(std::begin(in), std::end(in));

    decltype(ref) out;

    ca::test::input(in);
    ca::test::output(out, ref.size());

    ca::test::kernel("kernel", source,
                     FlagsBuilder(Language::cm)
                         .define("CONVERT", "cm_bf_cvt")
                         .define("SRC_VECTOR")
                         .define("SIMD", std::to_string(simd))
                         .define("IN_TYPE", ca::to_cm_string<float>())
                         // CM interprets half as bfloat16 in convert operations
                         .define("OUT_TYPE", ca::to_cm_string<half>())
                         .str());

    REQUIRE_THAT(out, Catch::Equals(ref));
  }

  SECTION("VectorRef") {
    auto in = ca::generate_vector<float>(simd, 1);
    std::vector<bfloat16> ref(std::begin(in), std::end(in));

    decltype(ref) out;

    ca::test::input(in);
    ca::test::output(out, ref.size());

    ca::test::kernel("kernel", source,
                     FlagsBuilder(Language::cm)
                         .define("CONVERT", "cm_bf_cvt")
                         .define("SRC_VECTOR_REF")
                         .define("SIMD", std::to_string(simd))
                         .define("IN_TYPE", ca::to_cm_string<float>())
                         // CM interprets half as bfloat16 in convert operations
                         .define("OUT_TYPE", ca::to_cm_string<half>())
                         .str());

    REQUIRE_THAT(out, Catch::Equals(ref));
  }

  SECTION("Matrix") {
    constexpr auto width = 8;
    constexpr auto height = simd / width;

    auto in = ca::generate_vector<float>(simd, 0);
    std::vector<bfloat16> ref(std::begin(in), std::end(in));

    decltype(ref) out;

    ca::test::input(in);
    ca::test::output(out, ref.size());

    ca::test::kernel("kernel", source,
                     FlagsBuilder(Language::cm)
                         .define("CONVERT", "cm_bf_cvt")
                         .define("SRC_MATRIX")
                         .define("SIMD", std::to_string(simd))
                         .define("WIDTH", std::to_string(width))
                         .define("HEIGHT", std::to_string(height))
                         .define("IN_TYPE", ca::to_cm_string<float>())
                         // CM interprets half as bfloat16 in convert operations
                         .define("OUT_TYPE", ca::to_cm_string<half>())
                         .str());

    REQUIRE_THAT(out, Catch::Equals(ref));
  }

  SECTION("MatrixRef") {
    constexpr auto width = 8;
    constexpr auto height = simd / width;

    auto in = ca::generate_vector<float>(simd, 1);
    std::vector<bfloat16> ref(std::begin(in), std::end(in));

    decltype(ref) out;

    ca::test::input(in);
    ca::test::output(out, ref.size());

    ca::test::kernel("kernel", source,
                     FlagsBuilder(Language::cm)
                         .define("CONVERT", "cm_bf_cvt")
                         .define("SRC_MATRIX_REF")
                         .define("SIMD", std::to_string(simd))
                         .define("WIDTH", std::to_string(width))
                         .define("HEIGHT", std::to_string(height))
                         .define("IN_TYPE", ca::to_cm_string<float>())
                         // CM interprets half as bfloat16 in convert operations
                         .define("OUT_TYPE", ca::to_cm_string<half>())
                         .str());

    REQUIRE_THAT(out, Catch::Equals(ref));
  }

  SECTION("Scalar") {
    auto in = ca::generate_value<float>(0);
    std::vector<bfloat16> ref(simd, bfloat16(in));

    decltype(ref) out;

    ca::test::pass(in);
    ca::test::output(out, ref.size());

    ca::test::kernel("kernel", source,
                     FlagsBuilder(Language::cm)
                         .define("CONVERT", "cm_bf_cvt")
                         .define("SRC_SCALAR")
                         .define("SIMD", std::to_string(simd))
                         .define("IN_TYPE", ca::to_cm_string<float>())
                         // CM interprets half as bfloat16 in convert operations
                         .define("OUT_TYPE", ca::to_cm_string<half>())
                         .str());

    REQUIRE_THAT(out, Catch::Equals(ref));
  }

  SECTION("Constant") {
    auto in = ca::generate_value<float>(0);
    std::vector<bfloat16> ref(simd, bfloat16(in));

    decltype(ref) out;

    ca::test::output(out, ref.size());

    ca::test::kernel("kernel", source,
                     FlagsBuilder(Language::cm)
                         .define("CONVERT", "cm_bf_cvt")
                         .define("SRC_CONST")
                         .define("SRC_VALUE", std::to_string(in))
                         .define("SIMD", std::to_string(simd))
                         .define("IN_TYPE", ca::to_cm_string<float>())
                         // CM interprets half as bfloat16 in convert operations
                         .define("OUT_TYPE", ca::to_cm_string<half>())
                         .str());

    REQUIRE_THAT(out, Catch::Equals(ref));
  }
}

TEST_CASE("cm_convert_bfloat_to_float", "[cm][bfloat]") {
  constexpr auto simd = 16;
  const std::string source = cassian::load_text_file(
      cassian::get_asset("kernels/cm_convert/convert_genx.cpp"));

  SECTION("Vector") {
    auto in = ca::generate_vector<bfloat16>(simd, 0);
    std::vector<float> ref(std::begin(in), std::end(in));

    decltype(ref) out;

    ca::test::input(in);
    ca::test::output(out, ref.size());

    ca::test::kernel("kernel", source,
                     FlagsBuilder(Language::cm)
                         .define("CONVERT", "cm_bf_cvt")
                         .define("SRC_VECTOR")
                         .define("SIMD", std::to_string(simd))
                         // CM interprets half as bfloat16 in convert operations
                         .define("IN_TYPE", ca::to_cm_string<half>())
                         .define("OUT_TYPE", ca::to_cm_string<float>())
                         .str());

    REQUIRE_THAT(out, Catch::Equals(ref));
  }

  SECTION("VectorRef") {
    auto in = ca::generate_vector<bfloat16>(simd, 1);
    std::vector<float> ref(std::begin(in), std::end(in));

    decltype(ref) out;

    ca::test::input(in);
    ca::test::output(out, ref.size());

    ca::test::kernel("kernel", source,
                     FlagsBuilder(Language::cm)
                         .define("CONVERT", "cm_bf_cvt")
                         .define("SRC_VECTOR_REF")
                         .define("SIMD", std::to_string(simd))
                         // CM interprets half as bfloat16 in convert operations
                         .define("IN_TYPE", ca::to_cm_string<half>())
                         .define("OUT_TYPE", ca::to_cm_string<float>())
                         .str());

    REQUIRE_THAT(out, Catch::Equals(ref));
  }

  SECTION("Matrix") {
    constexpr auto width = 8;
    constexpr auto height = simd / width;

    auto in = ca::generate_vector<bfloat16>(simd, 0);
    std::vector<float> ref(std::begin(in), std::end(in));

    decltype(ref) out;

    ca::test::input(in);
    ca::test::output(out, ref.size());

    ca::test::kernel("kernel", source,
                     FlagsBuilder(Language::cm)
                         .define("CONVERT", "cm_bf_cvt")
                         .define("SRC_MATRIX")
                         .define("SIMD", std::to_string(simd))
                         .define("WIDTH", std::to_string(width))
                         .define("HEIGHT", std::to_string(height))
                         // CM interprets half as bfloat16 in convert operations
                         .define("IN_TYPE", ca::to_cm_string<half>())
                         .define("OUT_TYPE", ca::to_cm_string<float>())
                         .str());

    REQUIRE_THAT(out, Catch::Equals(ref));
  }

  SECTION("MatrixRef") {
    constexpr auto width = 8;
    constexpr auto height = simd / width;

    auto in = ca::generate_vector<bfloat16>(simd, 1);
    std::vector<float> ref(std::begin(in), std::end(in));

    decltype(ref) out;

    ca::test::input(in);
    ca::test::output(out, ref.size());

    ca::test::kernel("kernel", source,
                     FlagsBuilder(Language::cm)
                         .define("CONVERT", "cm_bf_cvt")
                         .define("SRC_MATRIX_REF")
                         .define("SIMD", std::to_string(simd))
                         .define("WIDTH", std::to_string(width))
                         .define("HEIGHT", std::to_string(height))
                         // CM interprets half as bfloat16 in convert operations
                         .define("IN_TYPE", ca::to_cm_string<half>())
                         .define("OUT_TYPE", ca::to_cm_string<float>())
                         .str());

    REQUIRE_THAT(out, Catch::Equals(ref));
  }

  SECTION("Scalar") {
    auto in = ca::generate_value<bfloat16>(0);
    std::vector<float> ref(simd, float(in));

    decltype(ref) out;

    ca::test::pass(in);
    ca::test::output(out, ref.size());

    ca::test::kernel("kernel", source,
                     FlagsBuilder(Language::cm)
                         .define("CONVERT", "cm_bf_cvt")
                         .define("SRC_SCALAR")
                         .define("SIMD", std::to_string(simd))
                         // CM interprets half as bfloat16 in convert operations
                         .define("IN_TYPE", ca::to_cm_string<half>())
                         .define("OUT_TYPE", ca::to_cm_string<float>())
                         .str());

    REQUIRE_THAT(out, Catch::Equals(ref));
  }
}
