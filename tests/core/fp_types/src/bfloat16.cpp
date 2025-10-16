/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cstdint>
#include <sstream>
#include <tuple>
#include <type_traits>
#include <vector>

#include <catch2/catch.hpp>

#include <cassian/fp_types/bfloat16.hpp>

#include "common.hpp"

namespace ca = cassian;
namespace {
const float float_certain_value = -0.15625F;
const uint32_t float_lowest_denorm = 0x00000001;
const uint32_t float_nan = 0x7f800001;
const std::vector<float> float_values{
    {0.0F, -0.0F, 1.0F, 2.0F, float_certain_value, uint32_to_float(float_nan),
     uint32_to_float(float_lowest_denorm)}};

const uint32_t float_nan_covertable_from_bfloat16 = 0x7fff0000;
const uint32_t float_lowest_bfloat16_denorm = 0x00010000;
const std::vector<float> float_expected{
    {0.0F, -0.0F, 1.0F, 2.0F, float_certain_value,
     uint32_to_float(float_nan_covertable_from_bfloat16),
     uint32_to_float(float_lowest_bfloat16_denorm)}};

const uint32_t float_to_be_rounded_down = 0x7f7a7fff;
const uint32_t float_to_be_rounded_even = 0x7f7a8000;
const uint32_t float_to_be_rounded_up = 0x7f7a8001;
const uint32_t float_to_be_rounded_max = 0xffffffff;
const std::vector<float> float_rnde_rounding_values{
    {uint32_to_float(float_to_be_rounded_down),
     uint32_to_float(float_to_be_rounded_even),
     uint32_to_float(float_to_be_rounded_up),
     uint32_to_float(float_to_be_rounded_max)}};

const uint16_t bfloat16_zero = 0x0000;
const uint16_t bfloat16_minus_zero = 0x8000;
const uint16_t bfloat16_one = 0x3f80;
const uint16_t bfloat16_two = 0x4000;
const uint16_t bfloat16_certain_value = 0xbe20;
const uint16_t bfloat16_nan = 0x7fff;
const uint16_t bfloat16_lowest_denorm = 0x0001;
const std::vector<uint16_t> bfloat16_values{
    {bfloat16_zero, bfloat16_minus_zero, bfloat16_one, bfloat16_two,
     bfloat16_certain_value, bfloat16_nan, bfloat16_lowest_denorm}};
const std::vector<uint16_t> bfloat16_expected{{
    bfloat16_zero, bfloat16_minus_zero, bfloat16_one, bfloat16_two,
    bfloat16_certain_value, bfloat16_nan,
    bfloat16_zero // rounded from float lowest denorm
}};

const uint16_t bfloat16_rounded_down = 0x7f7a;
const uint16_t bfloat16_rounded_even = 0x7f7a;
const uint16_t bfloat16_rounded_up = 0x7f7b;
const uint16_t bfloat16_rounded_max = 0xffff;
const std::vector<uint16_t> bfloat16_rnde_rounding_expected{
    {bfloat16_rounded_down, bfloat16_rounded_even, bfloat16_rounded_up,
     bfloat16_rounded_max}};

template <typename In, typename Out> void test(In value, Out expected) {
  auto result = cast<In, ca::bfloat16, Out>(value);
  if (std::is_same<Out, float>::value) {
    uint32_t result_u32 = *reinterpret_cast<uint32_t *>(&result);
    uint32_t expected_u32 = *reinterpret_cast<uint32_t *>(&expected);
    REQUIRE(result_u32 == expected_u32);
  } else {
    REQUIRE(result == expected);
  }
}

// The point of these two tests is to see if value going through
// bfloat16 is not altered. Values need to be representable in 16-bits,
// so in uint16 case we use bfloat16_values, and in float we use
// float_expected, since it contains float values after bfloat16
// conversion (so representable in 16-bits).
TEST_CASE("bfloat16 from uint16 as uint16", "") {
  auto test_params =
      GENERATE(from_range(zip(bfloat16_values, bfloat16_values)));
  test(std::get<0>(test_params), std::get<1>(test_params));
}
TEST_CASE("bfloat16 from float as float", "") {
  auto test_params = GENERATE(from_range(zip(float_expected, float_expected)));
  test(std::get<0>(test_params), std::get<1>(test_params));
}
TEST_CASE("bfloat16 from uint16 as float", "") {
  auto test_params = GENERATE(from_range(zip(bfloat16_values, float_expected)));
  test(std::get<0>(test_params), std::get<1>(test_params));
}
TEST_CASE("bfloat16 from float as uint16", "") {
  auto test_params = GENERATE(from_range(zip(float_values, bfloat16_expected)));
  test(std::get<0>(test_params), std::get<1>(test_params));
}
TEST_CASE("bfloat16 from float with rnde rounding", "") {
  auto test_params = GENERATE(from_range(
      zip(float_rnde_rounding_values, bfloat16_rnde_rounding_expected)));
  test(std::get<0>(test_params), std::get<1>(test_params));
}

const uint16_t bfloat16_nan2 = 0x7f83;
const uint16_t bfloat16_nan3 = 0x7f81;
const uint16_t bfloat16_inf = 0x7f80;
TEST_CASE("bfloat16 NaN sensitive comparison same NaN value") {
  ca::bfloat16 bf = ca::bfloat16::encode(bfloat16_nan2);
  REQUIRE(true == bf.nan_sensitive_eq(bf));
}
TEST_CASE("bfloat16 NaN sensitive comparison different NaN value") {
  ca::bfloat16 bf1 = ca::bfloat16::encode(bfloat16_nan);
  ca::bfloat16 bf2 = ca::bfloat16::encode(bfloat16_nan3);
  REQUIRE(true == bf1.nan_sensitive_eq(bf2));
}
TEST_CASE("bfloat16 NaN sensitive comparison NaN Inf") {
  ca::bfloat16 bf1 = ca::bfloat16::encode(bfloat16_nan3);
  ca::bfloat16 bf2 = ca::bfloat16::encode(bfloat16_inf);
  REQUIRE(false == bf1.nan_sensitive_eq(bf2));
}

TEST_CASE("bfloat16 - Equality operator") {
  SECTION("with the same value") {
    const cassian::Bfloat16 a(2.0F);
    REQUIRE(a == a);
  }

  SECTION("with different value") {
    const cassian::Bfloat16 a(2.0F);
    const cassian::Bfloat16 b(3.0F);
    REQUIRE_FALSE(a == b);
  }

  SECTION("with almost equal value") {
    const cassian::Bfloat16 a = cassian::Bfloat16::encode(0x3f80);
    const cassian::Bfloat16 b = cassian::Bfloat16::encode(0x3f81);
    REQUIRE_FALSE(a == b);
  }
}

TEST_CASE("bfloat16 - Inequality operator") {
  SECTION("with the same value") {
    const cassian::Bfloat16 a(2.0F);
    REQUIRE_FALSE(a != a);
  }

  SECTION("with different value") {
    const cassian::Bfloat16 a(2.0F);
    const cassian::Bfloat16 b(3.0F);
    REQUIRE(a != b);
  }

  SECTION("with almost equal value") {
    const cassian::Bfloat16 a = cassian::Bfloat16::encode(0x3f80);
    const cassian::Bfloat16 b = cassian::Bfloat16::encode(0x3f81);
    REQUIRE(a != b);
  }
}

TEST_CASE("bfloat16 - Less than operator") {
  SECTION("with the same value") {
    const cassian::Bfloat16 a(2.0F);
    REQUIRE_FALSE(a < a);
  }

  SECTION("with lesser value") {
    const cassian::Bfloat16 a(2.0F);
    const cassian::Bfloat16 b(1.0F);
    REQUIRE_FALSE(a < b);
  }

  SECTION("with greater value") {
    const cassian::Bfloat16 a(2.0F);
    const cassian::Bfloat16 b(3.0F);
    REQUIRE(a < b);
  }
}

TEST_CASE("bfloat16 - Greater than operator") {
  SECTION("with the same value") {
    const cassian::Bfloat16 a(2.0F);
    REQUIRE_FALSE(a > a);
  }

  SECTION("with lesser value") {
    const cassian::Bfloat16 a(2.0F);
    const cassian::Bfloat16 b(1.0F);
    REQUIRE(a > b);
  }

  SECTION("with greater value") {
    const cassian::Bfloat16 a(2.0F);
    const cassian::Bfloat16 b(3.0F);
    REQUIRE_FALSE(a > b);
  }
}

TEST_CASE("bfloat16 - Less than or equal operator") {
  SECTION("with the same value") {
    const cassian::Bfloat16 a(2.0F);
    REQUIRE(a <= a);
  }

  SECTION("with lesser value") {
    const cassian::Bfloat16 a(2.0F);
    const cassian::Bfloat16 b(1.0F);
    REQUIRE_FALSE(a <= b);
  }

  SECTION("with greater value") {
    const cassian::Bfloat16 a(2.0F);
    const cassian::Bfloat16 b(3.0F);
    REQUIRE(a <= b);
  }
}

TEST_CASE("bfloat16 - Greater than or equal operator") {
  SECTION("with the same value") {
    const cassian::Bfloat16 a(2.0F);
    REQUIRE(a >= a);
  }

  SECTION("with lesser value") {
    const cassian::Bfloat16 a(2.0F);
    const cassian::Bfloat16 b(1.0F);
    REQUIRE(a >= b);
  }

  SECTION("with greater value") {
    const cassian::Bfloat16 a(2.0F);
    const cassian::Bfloat16 b(3.0F);
    REQUIRE_FALSE(a >= b);
  }
}

TEST_CASE("bfloat16 - Unary plus operator") {
  const cassian::Bfloat16 a(2.0F);
  REQUIRE(+a == a);
}

TEST_CASE("bfloat16 - Unary minus operator") {
  const cassian::Bfloat16 a(2.0F);
  const cassian::Bfloat16 b(-2.0F);
  REQUIRE(-a == b);
}

TEST_CASE("bfloat16 - Addition operator") {
  SECTION("bfloat16 + bfloat16") {
    const cassian::Bfloat16 a(2.0F);
    const cassian::Bfloat16 b(3.0F);
    const cassian::Bfloat16 c(2.0F + 3.0F);
    REQUIRE(a + b == c);
  }

  SECTION("bfloat16 + integer") {
    const cassian::Bfloat16 a(2.0F);
    const int b = 3;
    const cassian::Bfloat16 c(2.0F + 3.0F);
    REQUIRE(std::is_same_v<cassian::Bfloat16, decltype(a + b)>);
    REQUIRE(a + b == c);
  }

  SECTION("integer + bfloat16") {
    const int a = 2;
    const cassian::Bfloat16 b(3.0F);
    const cassian::Bfloat16 c(2.0F + 3.0F);
    REQUIRE(std::is_same_v<cassian::Bfloat16, decltype(a + b)>);
    REQUIRE(a + b == c);
  }

  SECTION("bfloat16 + float") {
    const cassian::Bfloat16 a(2.0F);
    const float b = 3.0F;
    const float c = 2.0F + 3.0F;
    REQUIRE(std::is_same_v<float, decltype(a + b)>);
    REQUIRE(a + b == c);
  }

  SECTION("float + bfloat16") {
    const float a = 2.0F;
    const cassian::Bfloat16 b(3.0F);
    const float c = 2.0F + 3.0F;
    REQUIRE(std::is_same_v<float, decltype(a + b)>);
    REQUIRE(a + b == c);
  }
}

TEST_CASE("bfloat16 - Subtraction operator") {
  SECTION("bfloat16 - bfloat16") {
    const cassian::Bfloat16 a(2.0F);
    const cassian::Bfloat16 b(3.0F);
    const cassian::Bfloat16 c(2.0F - 3.0F);
    REQUIRE(a - b == c);
  }

  SECTION("bfloat16 - integer") {
    const cassian::Bfloat16 a(2.0F);
    const int b = 3;
    const cassian::Bfloat16 c(2.0F - 3.0F);
    REQUIRE(std::is_same_v<cassian::Bfloat16, decltype(a - b)>);
    REQUIRE(a - b == c);
  }

  SECTION("integer - bfloat16") {
    const int a = 2;
    const cassian::Bfloat16 b(3.0F);
    const cassian::Bfloat16 c(2.0F - 3.0F);
    REQUIRE(std::is_same_v<cassian::Bfloat16, decltype(a - b)>);
    REQUIRE(a - b == c);
  }

  SECTION("bfloat16 - float") {
    const cassian::Bfloat16 a(2.0F);
    const float b = 3.0F;
    const float c = 2.0F - 3.0F;
    REQUIRE(std::is_same_v<float, decltype(a - b)>);
    REQUIRE(a - b == c);
  }

  SECTION("float - bfloat16") {
    const float a = 2.0F;
    const cassian::Bfloat16 b(3.0F);
    const float c = 2.0F - 3.0F;
    REQUIRE(std::is_same_v<float, decltype(a - b)>);
    REQUIRE(a - b == c);
  }
}

TEST_CASE("bfloat16 - Multiplication operator") {
  SECTION("bfloat16 * bfloat16") {
    const cassian::Bfloat16 a(2.0F);
    const cassian::Bfloat16 b(3.0F);
    const cassian::Bfloat16 c(2.0F * 3.0F);
    REQUIRE(a * b == c);
  }

  SECTION("bfloat16 * integer") {
    const cassian::Bfloat16 a(2.0F);
    const int b = 3;
    const cassian::Bfloat16 c(2.0F * 3.0F);
    REQUIRE(std::is_same_v<cassian::Bfloat16, decltype(a * b)>);
    REQUIRE(a * b == c);
  }

  SECTION("integer * bfloat16") {
    const int a = 2;
    const cassian::Bfloat16 b(3.0F);
    const cassian::Bfloat16 c(2.0F * 3.0F);
    REQUIRE(std::is_same_v<cassian::Bfloat16, decltype(a * b)>);
    REQUIRE(a * b == c);
  }

  SECTION("bfloat16 * float") {
    const cassian::Bfloat16 a(2.0F);
    const float b = 3.0F;
    const float c = 2.0F * 3.0F;
    REQUIRE(std::is_same_v<float, decltype(a * b)>);
    REQUIRE(a * b == c);
  }

  SECTION("float * bfloat16") {
    const float a = 2.0F;
    const cassian::Bfloat16 b(3.0F);
    const float c = 2.0F * 3.0F;
    REQUIRE(std::is_same_v<float, decltype(a * b)>);
    REQUIRE(a * b == c);
  }
}

TEST_CASE("bfloat16 - Division operator") {
  SECTION("bfloat16 / bfloat16") {
    const cassian::Bfloat16 a(2.0F);
    const cassian::Bfloat16 b(3.0F);
    const cassian::Bfloat16 c(2.0F / 3.0F);
    REQUIRE(a / b == c);
  }

  SECTION("bfloat16 / integer") {
    const cassian::Bfloat16 a(2.0F);
    const int b = 3;
    const cassian::Bfloat16 c(2.0F / 3.0F);
    REQUIRE(std::is_same_v<cassian::Bfloat16, decltype(a / b)>);
    REQUIRE(a / b == c);
  }

  SECTION("integer / bfloat16") {
    const int a = 2;
    const cassian::Bfloat16 b(3.0F);
    const cassian::Bfloat16 c(2.0F / 3.0F);
    REQUIRE(std::is_same_v<cassian::Bfloat16, decltype(a / b)>);
    REQUIRE(a / b == c);
  }

  SECTION("bfloat16 / float") {
    const cassian::Bfloat16 a(2.0F);
    const float b = 3.0F;
    const float c = 2.0F / 3.0F;
    REQUIRE(std::is_same_v<float, decltype(a / b)>);
    REQUIRE(a / b == c);
  }

  SECTION("float / bfloat16") {
    const float a = 2.0F;
    const cassian::Bfloat16 b(3.0F);
    const float c = 2.0F / 3.0F;
    REQUIRE(std::is_same_v<float, decltype(a / b)>);
    REQUIRE(a / b == c);
  }
}

TEST_CASE("bfloat16 - abs") {
  SECTION("+zero") {
    const auto a = cassian::Bfloat16::encode(0x0000);
    REQUIRE(abs(a) == a);
  }

  SECTION("-zero") {
    const auto a = cassian::Bfloat16::encode(0x8000);
    REQUIRE(abs(a) == -a);
  }

  SECTION("one") {
    const auto a = cassian::Bfloat16::encode(0x3f80);
    REQUIRE(abs(a) == a);
  }

  SECTION("-one") {
    const auto a = cassian::Bfloat16::encode(0xbf80);
    REQUIRE(abs(a) == -a);
  }
}

TEST_CASE("bfloat16 - sqrt") {
  const cassian::Bfloat16 a(2.0F);
  const float b = 2.0F;
  REQUIRE(sqrt(a) == cassian::Bfloat16(std::sqrt(b)));
}

TEST_CASE("bfloat16 - to_string") {
  SECTION("zero") {
    const cassian::Bfloat16 input(0.0F);
    const std::string output = to_string(input);
    const std::string expected = "0x0000";
    REQUIRE(output == expected);
  }

  SECTION("denorm") {
    const cassian::Bfloat16 input = cassian::Bfloat16::encode(0x0001);
    const std::string output = to_string(input);
    const std::string expected = "0x0001";
    REQUIRE(output == expected);
  }

  SECTION("one") {
    const cassian::Bfloat16 input(1.0F);
    const std::string output = to_string(input);
    const std::string expected = "0x3f80";
    REQUIRE(output == expected);
  }
}

TEST_CASE("bfloat16 - stream operator") {
  std::stringstream ss;
  SECTION("zero") {
    const cassian::Bfloat16 input(0.0F);
    ss << input;
    const std::string expected = "0x0000";
    REQUIRE(ss.str() == expected);
  }

  SECTION("denorm") {
    const cassian::Bfloat16 input = cassian::Bfloat16::encode(0x0001);
    ss << input;
    const std::string expected = "0x0001";
    REQUIRE(ss.str() == expected);
  }

  SECTION("one") {
    const cassian::Bfloat16 input(1.0F);
    ss << input;
    const std::string expected = "0x3f80";
    REQUIRE(ss.str() == expected);
  }
}

TEST_CASE("bfloat16 - isnan") {
  SECTION("with NaN") {
    const cassian::Bfloat16 a = cassian::Bfloat16::encode(0x7fff);
    REQUIRE(isnan(a));
  }

  SECTION("with zero") {
    const cassian::Bfloat16 a(0.0F);
    REQUIRE_FALSE(isnan(a));
  }
}

TEST_CASE("bfloat16 - nextafter") {
  SECTION("zero -> one") {
    const cassian::Bfloat16 a(0.0F);
    const cassian::Bfloat16 b(1.0F);
    const cassian::Bfloat16 denorm =
        std::numeric_limits<cassian::Bfloat16>::denorm_min();
    REQUIRE(nextafter(a, b) == denorm);
  }

  SECTION("zero -> -one") {
    const cassian::Bfloat16 a(0.0F);
    const cassian::Bfloat16 b(-1.0F);
    const cassian::Bfloat16 denorm =
        std::numeric_limits<cassian::Bfloat16>::denorm_min();
    REQUIRE(nextafter(a, b) == -denorm);
  }

  SECTION("1.5 -> greater than 1.5") {
    const cassian::Bfloat16 a(1.5F);
    const cassian::Bfloat16 b(2.0F);
    const cassian::Bfloat16 epsilon =
        std::numeric_limits<cassian::Bfloat16>::epsilon();
    REQUIRE(nextafter(a, b) == a + epsilon);
  }

  SECTION("1.5 -> lesser than 1.5") {
    const cassian::Bfloat16 a(1.5F);
    const cassian::Bfloat16 b(0.0F);
    const cassian::Bfloat16 epsilon =
        std::numeric_limits<cassian::Bfloat16>::epsilon();
    REQUIRE(nextafter(a, b) == a - epsilon);
  }

  SECTION("-1.5 -> greater than -1.5") {
    const cassian::Bfloat16 a(-1.5F);
    const cassian::Bfloat16 b(-0.5F);
    const cassian::Bfloat16 epsilon =
        std::numeric_limits<cassian::Bfloat16>::epsilon();
    REQUIRE(nextafter(a, b) == a + epsilon);
  }

  SECTION("-1.5 -> greater than zero") {
    const cassian::Bfloat16 a(-1.5F);
    const cassian::Bfloat16 b(0.5F);
    const cassian::Bfloat16 epsilon =
        std::numeric_limits<cassian::Bfloat16>::epsilon();
    REQUIRE(nextafter(a, b) == a + epsilon);
  }

  SECTION("-1.5 -> lesser than -1.5") {
    const cassian::Bfloat16 a(-1.5F);
    const cassian::Bfloat16 b(-2.0F);
    const cassian::Bfloat16 epsilon =
        std::numeric_limits<cassian::Bfloat16>::epsilon();
    REQUIRE(nextafter(a, b) == a - epsilon);
  }

  SECTION("x -> greater than x (high exponent)") {
    const cassian::Bfloat16 a = cassian::Bfloat16::encode(0x7000);
    const cassian::Bfloat16 b = cassian::Bfloat16::encode(0x7800);
    const cassian::Bfloat16 expected = cassian::Bfloat16::encode(0x7001);
    REQUIRE(nextafter(a, b) == expected);
  }

  SECTION("x -> lesser than x (high exponent)") {
    const cassian::Bfloat16 a = cassian::Bfloat16::encode(0x7000);
    const cassian::Bfloat16 b = cassian::Bfloat16::encode(0x6000);
    const cassian::Bfloat16 expected = cassian::Bfloat16::encode(0x6fff);
    REQUIRE(nextafter(a, b) == expected);
  }

  SECTION("zero -> zero") {
    const cassian::Bfloat16 a(0.0F);
    const cassian::Bfloat16 b(0.0F);
    REQUIRE(nextafter(a, b) == b);
  }
}

TEST_CASE("bfloat16 isinf") {
  SECTION("with +inf") {
    const cassian::Bfloat16 a = cassian::Bfloat16::encode(0x7f80);
    REQUIRE(isinf(a));
  }

  SECTION("with -inf") {
    const cassian::Bfloat16 a = cassian::Bfloat16::encode(0xff80);
    REQUIRE(isinf(a));
  }

  SECTION("with NaN") {
    const cassian::Bfloat16 a = cassian::Bfloat16::encode(0x7fff);
    REQUIRE_FALSE(isinf(a));
  }

  SECTION("with zero") {
    const cassian::Bfloat16 a(0.0F);
    REQUIRE_FALSE(isinf(a));
  }
}

TEST_CASE("bfloat16 isdenorm") {
  SECTION("with denorm") {
    const cassian::Bfloat16 a = cassian::Bfloat16::encode(0x007f);
    REQUIRE(isdenorm(a));
  }

  SECTION("with -denorm") {
    const cassian::Bfloat16 a = cassian::Bfloat16::encode(0x807f);
    REQUIRE(isdenorm(a));
  }

  SECTION("with norm") {
    const cassian::Bfloat16 a = cassian::Bfloat16::encode(0x3f80);
    REQUIRE_FALSE(isdenorm(a));
  }

  SECTION("with zero") {
    const cassian::Bfloat16 a = cassian::Bfloat16::encode(0x0000);
    REQUIRE_FALSE(isdenorm(a));
  }

  SECTION("with -zero") {
    const cassian::Bfloat16 a = cassian::Bfloat16::encode(0x8000);
    REQUIRE_FALSE(isdenorm(a));
  }

  SECTION("with inf") {
    const cassian::Bfloat16 a = cassian::Bfloat16::encode(0x7f80);
    REQUIRE_FALSE(isdenorm(a));
  }

  SECTION("with NaN") {
    const cassian::Bfloat16 a = cassian::Bfloat16::encode(0x7fff);
    REQUIRE_FALSE(isdenorm(a));
  }
}

TEST_CASE("bfloat16 flush_to_zero") {
  SECTION("with denorm") {
    const cassian::Bfloat16 a = cassian::Bfloat16::encode(0x007f);
    REQUIRE(flush_to_zero(a) == cassian::Bfloat16::encode(0x0000));
  }

  SECTION("with -denorm") {
    const cassian::Bfloat16 a = cassian::Bfloat16::encode(0x807f);
    REQUIRE(flush_to_zero(a) == cassian::Bfloat16::encode(0x8000));
  }

  SECTION("with norm") {
    const cassian::Bfloat16 a = cassian::Bfloat16::encode(0x3f80);
    REQUIRE(flush_to_zero(a) == a);
  }

  SECTION("with zero") {
    const cassian::Bfloat16 a = cassian::Bfloat16::encode(0x0000);
    REQUIRE(flush_to_zero(a) == a);
  }

  SECTION("with -zero") {
    const cassian::Bfloat16 a = cassian::Bfloat16::encode(0x8000);
    REQUIRE(flush_to_zero(a) == a);
  }

  SECTION("with inf") {
    const cassian::Bfloat16 a = cassian::Bfloat16::encode(0x7f80);
    REQUIRE(flush_to_zero(a) == a);
  }

  SECTION("with NaN") {
    const cassian::Bfloat16 a = cassian::Bfloat16::encode(0x7fff);
    REQUIRE(flush_to_zero(a).nan_sensitive_eq(a));
  }
}

} // namespace
