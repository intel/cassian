/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cstdint>
#include <tuple>
#include <type_traits>
#include <vector>

#include <catch2/catch.hpp>

#include <cassian/fp_types/bfloat.hpp>

#include "common.hpp"

namespace ca = cassian;
namespace {
const float float_certain_value = -0.15625F;
const uint32_t float_lowest_denorm = 0x00000001;
const uint32_t float_nan = 0x7f800001;
const std::vector<float> float_values{
    {0.0F, -0.0F, 1.0F, 2.0F, float_certain_value, uint32_to_float(float_nan),
     uint32_to_float(float_lowest_denorm)}};

const uint32_t float_nan_covertable_from_bfloat = 0x7fff0000;
const uint32_t float_lowest_bfloat_denorm = 0x00010000;
const std::vector<float> float_expected{
    {0.0F, -0.0F, 1.0F, 2.0F, float_certain_value,
     uint32_to_float(float_nan_covertable_from_bfloat),
     uint32_to_float(float_lowest_bfloat_denorm)}};

const uint32_t float_to_be_rounded_down = 0x7f7a7fff;
const uint32_t float_to_be_rounded_even = 0x7f7a8000;
const uint32_t float_to_be_rounded_up = 0x7f7a8001;
const uint32_t float_to_be_rounded_max = 0xffffffff;
const std::vector<float> float_rnde_rounding_values{
    {uint32_to_float(float_to_be_rounded_down),
     uint32_to_float(float_to_be_rounded_even),
     uint32_to_float(float_to_be_rounded_up),
     uint32_to_float(float_to_be_rounded_max)}};

const uint16_t bfloat_zero = 0x0000;
const uint16_t bfloat_minus_zero = 0x8000;
const uint16_t bfloat_one = 0x3f80;
const uint16_t bfloat_two = 0x4000;
const uint16_t bfloat_certain_value = 0xbe20;
const uint16_t bfloat_nan = 0x7fff;
const uint16_t bfloat_lowest_denorm = 0x0001;
const std::vector<uint16_t> bfloat_values{
    {bfloat_zero, bfloat_minus_zero, bfloat_one, bfloat_two,
     bfloat_certain_value, bfloat_nan, bfloat_lowest_denorm}};
const std::vector<uint16_t> bfloat_expected{{
    bfloat_zero, bfloat_minus_zero, bfloat_one, bfloat_two,
    bfloat_certain_value, bfloat_nan,
    bfloat_zero // rounded from float lowest denorm
}};

const uint16_t bfloat_rounded_down = 0x7f7a;
const uint16_t bfloat_rounded_even = 0x7f7a;
const uint16_t bfloat_rounded_up = 0x7f7b;
const uint16_t bfloat_rounded_max = 0xffff;
const std::vector<uint16_t> bfloat_rnde_rounding_expected{
    {bfloat_rounded_down, bfloat_rounded_even, bfloat_rounded_up,
     bfloat_rounded_max}};

template <typename In, typename Out> void test(In value, Out expected) {
  auto result = cast<In, ca::bfloat, Out>(value);
  if (std::is_same<Out, float>::value) {
    uint32_t result_u32 = *reinterpret_cast<uint32_t *>(&result);
    uint32_t expected_u32 = *reinterpret_cast<uint32_t *>(&expected);
    REQUIRE(result_u32 == expected_u32);
  } else {
    REQUIRE(result == expected);
  }
}

// The point of these two tests is to see if value going through
// bfloat is not altered. Values need to be representable in 16-bits,
// so in uint16 case we use bfloat_values, and in float we use
// float_expected, since it contains float values after bfloat
// conversion (so representable in 16-bits).
TEST_CASE("bfloat from uint16 as uint16", "") {
  auto test_params = GENERATE(from_range(zip(bfloat_values, bfloat_values)));
  test(std::get<0>(test_params), std::get<1>(test_params));
}
TEST_CASE("bfloat from float as float", "") {
  auto test_params = GENERATE(from_range(zip(float_expected, float_expected)));
  test(std::get<0>(test_params), std::get<1>(test_params));
}
TEST_CASE("bfloat from uint16 as float", "") {
  auto test_params = GENERATE(from_range(zip(bfloat_values, float_expected)));
  test(std::get<0>(test_params), std::get<1>(test_params));
}
TEST_CASE("bfloat from float as uint16", "") {
  auto test_params = GENERATE(from_range(zip(float_values, bfloat_expected)));
  test(std::get<0>(test_params), std::get<1>(test_params));
}
TEST_CASE("bfloat from float with rnde rounding", "") {
  auto test_params = GENERATE(from_range(
      zip(float_rnde_rounding_values, bfloat_rnde_rounding_expected)));
  test(std::get<0>(test_params), std::get<1>(test_params));
}

const uint16_t bfloat_nan2 = 0x7f83;
const uint16_t bfloat_nan3 = 0x7f81;
const uint16_t bfloat_inf = 0x7f80;
TEST_CASE("bfloat NaN sensitive comparison same NaN value") {
  ca::bfloat bf = ca::bfloat::encode(bfloat_nan2);
  REQUIRE(true == bf.nan_sensitive_eq(bf));
}
TEST_CASE("bfloat NaN sensitive comparison different NaN value") {
  ca::bfloat bf1 = ca::bfloat::encode(bfloat_nan);
  ca::bfloat bf2 = ca::bfloat::encode(bfloat_nan3);
  REQUIRE(true == bf1.nan_sensitive_eq(bf2));
}
TEST_CASE("bfloat NaN sensitive comparison NaN Inf") {
  ca::bfloat bf1 = ca::bfloat::encode(bfloat_nan3);
  ca::bfloat bf2 = ca::bfloat::encode(bfloat_inf);
  REQUIRE(false == bf1.nan_sensitive_eq(bf2));
}

TEST_CASE("bfloat - Equality operator") {
  SECTION("with the same value") {
    const cassian::Bfloat a(2.0F);
    REQUIRE(a == a);
  }

  SECTION("with different value") {
    const cassian::Bfloat a(2.0F);
    const cassian::Bfloat b(3.0F);
    REQUIRE_FALSE(a == b);
  }

  SECTION("with almost equal value") {
    const cassian::Bfloat a = cassian::Bfloat::encode(0x3f80);
    const cassian::Bfloat b = cassian::Bfloat::encode(0x3f81);
    REQUIRE_FALSE(a == b);
  }
}

TEST_CASE("bfloat - Inequality operator") {
  SECTION("with the same value") {
    const cassian::Bfloat a(2.0F);
    REQUIRE_FALSE(a != a);
  }

  SECTION("with different value") {
    const cassian::Bfloat a(2.0F);
    const cassian::Bfloat b(3.0F);
    REQUIRE(a != b);
  }

  SECTION("with almost equal value") {
    const cassian::Bfloat a = cassian::Bfloat::encode(0x3f80);
    const cassian::Bfloat b = cassian::Bfloat::encode(0x3f81);
    REQUIRE(a != b);
  }
}

TEST_CASE("bfloat - Less than operator") {
  SECTION("with the same value") {
    const cassian::Bfloat a(2.0F);
    REQUIRE_FALSE(a < a);
  }

  SECTION("with lesser value") {
    const cassian::Bfloat a(2.0F);
    const cassian::Bfloat b(1.0F);
    REQUIRE_FALSE(a < b);
  }

  SECTION("with greater value") {
    const cassian::Bfloat a(2.0F);
    const cassian::Bfloat b(3.0F);
    REQUIRE(a < b);
  }
}

TEST_CASE("bfloat - Greater than operator") {
  SECTION("with the same value") {
    const cassian::Bfloat a(2.0F);
    REQUIRE_FALSE(a > a);
  }

  SECTION("with lesser value") {
    const cassian::Bfloat a(2.0F);
    const cassian::Bfloat b(1.0F);
    REQUIRE(a > b);
  }

  SECTION("with greater value") {
    const cassian::Bfloat a(2.0F);
    const cassian::Bfloat b(3.0F);
    REQUIRE_FALSE(a > b);
  }
}

TEST_CASE("bfloat - Less than or equal operator") {
  SECTION("with the same value") {
    const cassian::Bfloat a(2.0F);
    REQUIRE(a <= a);
  }

  SECTION("with lesser value") {
    const cassian::Bfloat a(2.0F);
    const cassian::Bfloat b(1.0F);
    REQUIRE_FALSE(a <= b);
  }

  SECTION("with greater value") {
    const cassian::Bfloat a(2.0F);
    const cassian::Bfloat b(3.0F);
    REQUIRE(a <= b);
  }
}

TEST_CASE("bfloat - Greater than or equal operator") {
  SECTION("with the same value") {
    const cassian::Bfloat a(2.0F);
    REQUIRE(a >= a);
  }

  SECTION("with lesser value") {
    const cassian::Bfloat a(2.0F);
    const cassian::Bfloat b(1.0F);
    REQUIRE(a >= b);
  }

  SECTION("with greater value") {
    const cassian::Bfloat a(2.0F);
    const cassian::Bfloat b(3.0F);
    REQUIRE_FALSE(a >= b);
  }
}

} // namespace
