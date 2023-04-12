/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <catch2/catch.hpp>

#include <cassian/fp_types/tfloat.hpp>
#include <cassian/main/flags_builder.hpp>
#include <cassian/main/test_helper.hpp>
#include <cassian/utility/utility.hpp>

namespace ca = cassian;
using ca::tfloat;
using ca::test::FlagsBuilder;
using ca::test::Language;

TEST_CASE("cm_convert_float_to_tfloat", "[cm][tfloat]") {
  constexpr auto simd = 16;
  const std::string source = cassian::load_text_file(
      cassian::get_asset("kernels/cm_convert/convert_genx.cpp"));

  SECTION("Vector") {
    auto in = ca::generate_vector<float>(simd, 0);
    std::vector<tfloat> ref(std::begin(in), std::end(in));

    decltype(ref) out;

    ca::test::input(in);
    ca::test::output(out, ref.size());

    ca::test::kernel(
        "kernel", source,
        FlagsBuilder(Language::cm)
            .define("CONVERT", "cm_tf32_cvt")
            .define("SRC_VECTOR")
            .define("SIMD", std::to_string(simd))
            .define("IN_TYPE", ca::to_cm_string<float>())
            // CM interprets int32_t as tfloat in convert operations
            .define("OUT_TYPE", ca::to_cm_string<int32_t>())
            .str());

    REQUIRE_THAT(out, Catch::Equals(ref));
  }

  SECTION("VectorRef") {
    auto in = ca::generate_vector<float>(simd, 1);
    std::vector<tfloat> ref(std::begin(in), std::end(in));

    decltype(ref) out;

    ca::test::input(in);
    ca::test::output(out, ref.size());

    ca::test::kernel(
        "kernel", source,
        FlagsBuilder(Language::cm)
            .define("CONVERT", "cm_tf32_cvt")
            .define("SRC_VECTOR_REF")
            .define("SIMD", std::to_string(simd))
            .define("IN_TYPE", ca::to_cm_string<float>())
            // CM interprets int32_t as tfloat in convert operations
            .define("OUT_TYPE", ca::to_cm_string<int32_t>())
            .str());

    REQUIRE_THAT(out, Catch::Equals(ref));
  }

  SECTION("Matrix") {
    constexpr auto width = 8;
    constexpr auto height = simd / width;

    auto in = ca::generate_vector<float>(simd, 0);
    std::vector<tfloat> ref(std::begin(in), std::end(in));

    decltype(ref) out;

    ca::test::input(in);
    ca::test::output(out, ref.size());

    ca::test::kernel(
        "kernel", source,
        FlagsBuilder(Language::cm)
            .define("CONVERT", "cm_tf32_cvt")
            .define("SRC_MATRIX")
            .define("SIMD", std::to_string(simd))
            .define("WIDTH", std::to_string(width))
            .define("HEIGHT", std::to_string(height))
            .define("IN_TYPE", ca::to_cm_string<float>())
            // CM interprets int32_t as tfloat in convert operations
            .define("OUT_TYPE", ca::to_cm_string<int32_t>())
            .str());

    REQUIRE_THAT(out, Catch::Equals(ref));
  }

  SECTION("MatrixRef") {
    constexpr auto width = 8;
    constexpr auto height = simd / width;

    auto in = ca::generate_vector<float>(simd, 1);
    std::vector<tfloat> ref(std::begin(in), std::end(in));

    decltype(ref) out;

    ca::test::input(in);
    ca::test::output(out, ref.size());

    ca::test::kernel(
        "kernel", source,
        FlagsBuilder(Language::cm)
            .define("CONVERT", "cm_tf32_cvt")
            .define("SRC_MATRIX_REF")
            .define("SIMD", std::to_string(simd))
            .define("WIDTH", std::to_string(width))
            .define("HEIGHT", std::to_string(height))
            .define("IN_TYPE", ca::to_cm_string<float>())
            // CM interprets int32_t as tfloat in convert operations
            .define("OUT_TYPE", ca::to_cm_string<int32_t>())
            .str());

    REQUIRE_THAT(out, Catch::Equals(ref));
  }

  SECTION("Scalar") {
    auto in = ca::generate_value<float>(0);
    std::vector<tfloat> ref(simd, tfloat(in));

    decltype(ref) out;

    ca::test::pass(in);
    ca::test::output(out, ref.size());

    ca::test::kernel(
        "kernel", source,
        FlagsBuilder(Language::cm)
            .define("CONVERT", "cm_tf32_cvt")
            .define("SRC_SCALAR")
            .define("SIMD", std::to_string(simd))
            .define("IN_TYPE", ca::to_cm_string<float>())
            // CM interprets int32_t as tfloat in convert operations
            .define("OUT_TYPE", ca::to_cm_string<int32_t>())
            .str());

    REQUIRE_THAT(out, Catch::Equals(ref));
  }

  SECTION("Constant") {
    auto in = ca::generate_value<float>(0);
    std::vector<tfloat> ref(simd, tfloat(in));

    decltype(ref) out;

    ca::test::output(out, ref.size());

    ca::test::kernel(
        "kernel", source,
        FlagsBuilder(Language::cm)
            .define("CONVERT", "cm_tf32_cvt")
            .define("SRC_CONST")
            .define("SRC_VALUE", std::to_string(in))
            .define("SIMD", std::to_string(simd))
            .define("IN_TYPE", ca::to_cm_string<float>())
            // CM interprets int32_t as tfloat in convert operations
            .define("OUT_TYPE", ca::to_cm_string<int32_t>())
            .str());

    REQUIRE_THAT(out, Catch::Equals(ref));
  }
}
