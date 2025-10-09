/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cstdint>
#include <limits>
#include <sstream>
#include <tuple>
#include <vector>

#include <catch2/catch.hpp>

#include <cassian/fp_types/tfloat.hpp>

#include "common.hpp"

namespace ca = cassian;
namespace {
const float float_certain_value = -0.15625F;
const uint32_t float_lowest_denorm = 0x00000001;
const uint32_t float_nan = 0x7f800001;
const std::vector<float> float_values{
    {0.0F, -0.0F, 1.0F, 2.0F, float_certain_value, uint32_to_float(float_nan),
     uint32_to_float(float_lowest_denorm)}};

const uint32_t float_nan_covertable_from_tfloat = 0x7fffe000;
const uint32_t float_lowest_tfloat_denorm = 0x00002000;
const std::vector<float> float_expected{
    {0.0F, -0.0F, 1.0F, 2.0F, float_certain_value,
     uint32_to_float(float_nan_covertable_from_tfloat),
     uint32_to_float(float_lowest_tfloat_denorm)}};

const uint32_t float_to_be_rounded_down = 0x7f7aefff;
const uint32_t float_to_be_rounded_even = 0x7f7af000;
const uint32_t float_to_be_rounded_up = 0x7f7af001;
const uint32_t float_to_be_rounded_max = 0xffffffff;
const std::vector<float> float_rnde_rounding_values{
    {uint32_to_float(float_to_be_rounded_down),
     uint32_to_float(float_to_be_rounded_even),
     uint32_to_float(float_to_be_rounded_up),
     uint32_to_float(float_to_be_rounded_max)}};

const uint32_t tfloat_zero = 0x00000000;
const uint32_t tfloat_minus_zero = 0x80000000;
const uint32_t tfloat_one = 0x3f800000;
const uint32_t tfloat_two = 0x40000000;
const uint32_t tfloat_certain_value = 0xbe200000;
const uint32_t tfloat_nan = 0x7fffe000;
const uint32_t tfloat_lowest_denorm = 0x00002000;
const std::vector<uint32_t> tfloat_values{
    {tfloat_zero, tfloat_minus_zero, tfloat_one, tfloat_two,
     tfloat_certain_value, tfloat_nan, tfloat_lowest_denorm}};
const std::vector<uint32_t> tfloat_expected{{
    tfloat_zero, tfloat_minus_zero, tfloat_one, tfloat_two,
    tfloat_certain_value, tfloat_nan,
    tfloat_zero // rounded from float lowest denorm
}};

const uint32_t tfloat_rounded_down = 0x7f7ae000;
const uint32_t tfloat_rounded_even = 0x7f7b0000;
const uint32_t tfloat_rounded_up = 0x7f7b0000;
const uint32_t tfloat_rounded_max = 0xffffe000;
const std::vector<uint32_t> tfloat_rnde_rounding_expected{
    {tfloat_rounded_down, tfloat_rounded_even, tfloat_rounded_up,
     tfloat_rounded_max}};

template <typename In, typename Out> void test(In value, Out expected) {
  auto result = cast<In, ca::Tfloat, Out>(value);
  if (std::is_same<Out, float>::value) {
    uint32_t result_u32 = *reinterpret_cast<uint32_t *>(&result);
    uint32_t expected_u32 = *reinterpret_cast<uint32_t *>(&expected);
    REQUIRE(result_u32 == expected_u32);
  } else {
    REQUIRE(result == expected);
  }
}

// The point of these two tests is to see if value going through
// tfloat is not altered. Values need to be representable in 19-bits,
// so in uint32 case we use tfloat_values, and in float we use
// float_expected, since it contains float values after tfloat
// conversion (so representable in 19-bits).
TEST_CASE("tfloat from uint32 as uint32", "") {
  auto test_params = GENERATE(from_range(zip(tfloat_values, tfloat_values)));
  test(std::get<0>(test_params), std::get<1>(test_params));
}
TEST_CASE("tfloat from float as float", "") {
  auto test_params = GENERATE(from_range(zip(float_expected, float_expected)));
  test(std::get<0>(test_params), std::get<1>(test_params));
}
TEST_CASE("tfloat from uint32 as float", "") {
  auto test_params = GENERATE(from_range(zip(tfloat_values, float_expected)));
  test(std::get<0>(test_params), std::get<1>(test_params));
}
TEST_CASE("tfloat from float as uint32", "") {
  auto test_params = GENERATE(from_range(zip(float_values, tfloat_expected)));
  test(std::get<0>(test_params), std::get<1>(test_params));
}
TEST_CASE("tfloat from float with rnde rounding", "") {
  auto test_params = GENERATE(from_range(
      zip(float_rnde_rounding_values, tfloat_rnde_rounding_expected)));
  test(std::get<0>(test_params), std::get<1>(test_params));
}

const uint32_t tfloat_nan2 = 0x7f830000;
const uint32_t tfloat_nan3 = 0x7f810000;
const uint32_t tfloat_inf = 0x7f800000;
TEST_CASE("tfloat NaN sensitive comparison same NaN value") {
  ca::Tfloat bf = ca::Tfloat::encode(tfloat_nan2);
  REQUIRE(true == bf.nan_sensitive_eq(bf));
}
TEST_CASE("tfloat NaN sensitive comparison different NaN value") {
  ca::Tfloat bf1 = ca::Tfloat::encode(tfloat_nan);
  ca::Tfloat bf2 = ca::Tfloat::encode(tfloat_nan3);
  REQUIRE(true == bf1.nan_sensitive_eq(bf2));
}
TEST_CASE("tfloat NaN sensitive comparison NaN Inf") {
  ca::Tfloat bf1 = ca::Tfloat::encode(tfloat_nan3);
  ca::Tfloat bf2 = ca::Tfloat::encode(tfloat_inf);
  REQUIRE(false == bf1.nan_sensitive_eq(bf2));
}

TEST_CASE("tfloat - Equality operator") {
  SECTION("with the same value") {
    const cassian::Tfloat a(2.0F);
    REQUIRE(a == a);
  }

  SECTION("with different value") {
    const cassian::Tfloat a(2.0F);
    const cassian::Tfloat b(3.0F);
    REQUIRE_FALSE(a == b);
  }

  SECTION("with almost equal value") {
    const cassian::Tfloat a = cassian::Tfloat::encode(0x3f800000);
    const cassian::Tfloat b = cassian::Tfloat::encode(0x3f802000);
    REQUIRE_FALSE(a == b);
  }
}

TEST_CASE("tfloat - Inequality operator") {
  SECTION("with the same value") {
    const cassian::Tfloat a(2.0F);
    REQUIRE_FALSE(a != a);
  }

  SECTION("with different value") {
    const cassian::Tfloat a(2.0F);
    const cassian::Tfloat b(3.0F);
    REQUIRE(a != b);
  }

  SECTION("with almost equal value") {
    const cassian::Tfloat a = cassian::Tfloat::encode(0x3f800000);
    const cassian::Tfloat b = cassian::Tfloat::encode(0x3f802000);
    REQUIRE(a != b);
  }
}

TEST_CASE("tfloat - Less than operator") {
  SECTION("with the same value") {
    const cassian::Tfloat a(2.0F);
    REQUIRE_FALSE(a < a);
  }

  SECTION("with lesser value") {
    const cassian::Tfloat a(2.0F);
    const cassian::Tfloat b(1.0F);
    REQUIRE_FALSE(a < b);
  }

  SECTION("with greater value") {
    const cassian::Tfloat a(2.0F);
    const cassian::Tfloat b(3.0F);
    REQUIRE(a < b);
  }
}

TEST_CASE("tfloat - Greater than operator") {
  SECTION("with the same value") {
    const cassian::Tfloat a(2.0F);
    REQUIRE_FALSE(a > a);
  }

  SECTION("with lesser value") {
    const cassian::Tfloat a(2.0F);
    const cassian::Tfloat b(1.0F);
    REQUIRE(a > b);
  }

  SECTION("with greater value") {
    const cassian::Tfloat a(2.0F);
    const cassian::Tfloat b(3.0F);
    REQUIRE_FALSE(a > b);
  }
}

TEST_CASE("tfloat - Less than or equal operator") {
  SECTION("with the same value") {
    const cassian::Tfloat a(2.0F);
    REQUIRE(a <= a);
  }

  SECTION("with lesser value") {
    const cassian::Tfloat a(2.0F);
    const cassian::Tfloat b(1.0F);
    REQUIRE_FALSE(a <= b);
  }

  SECTION("with greater value") {
    const cassian::Tfloat a(2.0F);
    const cassian::Tfloat b(3.0F);
    REQUIRE(a <= b);
  }
}

TEST_CASE("tfloat - Greater than or equal operator") {
  SECTION("with the same value") {
    const cassian::Tfloat a(2.0F);
    REQUIRE(a >= a);
  }

  SECTION("with lesser value") {
    const cassian::Tfloat a(2.0F);
    const cassian::Tfloat b(1.0F);
    REQUIRE(a >= b);
  }

  SECTION("with greater value") {
    const cassian::Tfloat a(2.0F);
    const cassian::Tfloat b(3.0F);
    REQUIRE_FALSE(a >= b);
  }
}

TEST_CASE("tfloat - Unary plus operator") {
  const cassian::Tfloat a(2.0F);
  REQUIRE(+a == a);
}

TEST_CASE("tfloat - Unary minus operator") {
  const cassian::Tfloat a(2.0F);
  const cassian::Tfloat b(-2.0F);
  REQUIRE(-a == b);
}

TEST_CASE("tfloat - Addition operator") {
  SECTION("tfloat + tfloat") {
    const cassian::Tfloat a(2.0F);
    const cassian::Tfloat b(3.0F);
    const cassian::Tfloat c(2.0F + 3.0F);
    REQUIRE(a + b == c);
  }

  SECTION("tfloat + integer") {
    const cassian::Tfloat a(2.0F);
    const int b = 3;
    const cassian::Tfloat c(2.0F + 3.0F);
    REQUIRE(std::is_same_v<cassian::Tfloat, decltype(a + b)>);
    REQUIRE(a + b == c);
  }

  SECTION("integer + tfloat") {
    const int a = 2;
    const cassian::Tfloat b(3.0F);
    const cassian::Tfloat c(2.0F + 3.0F);
    REQUIRE(std::is_same_v<cassian::Tfloat, decltype(a + b)>);
    REQUIRE(a + b == c);
  }

  SECTION("tfloat + float") {
    const cassian::Tfloat a(2.0F);
    const float b = 3.0F;
    const float c = 2.0F + 3.0F;
    REQUIRE(std::is_same_v<float, decltype(a + b)>);
    REQUIRE(a + b == c);
  }

  SECTION("float + tfloat") {
    const float a = 2.0F;
    const cassian::Tfloat b(3.0F);
    const float c = 2.0F + 3.0F;
    REQUIRE(std::is_same_v<float, decltype(a + b)>);
    REQUIRE(a + b == c);
  }
}

TEST_CASE("tfloat - Subtraction operator") {
  SECTION("tfloat - tfloat") {
    const cassian::Tfloat a(2.0F);
    const cassian::Tfloat b(3.0F);
    const cassian::Tfloat c(2.0F - 3.0F);
    REQUIRE(a - b == c);
  }

  SECTION("tfloat - integer") {
    const cassian::Tfloat a(2.0F);
    const int b = 3;
    const cassian::Tfloat c(2.0F - 3.0F);
    REQUIRE(std::is_same_v<cassian::Tfloat, decltype(a - b)>);
    REQUIRE(a - b == c);
  }

  SECTION("integer - tfloat") {
    const int a = 2;
    const cassian::Tfloat b(3.0F);
    const cassian::Tfloat c(2.0F - 3.0F);
    REQUIRE(std::is_same_v<cassian::Tfloat, decltype(a - b)>);
    REQUIRE(a - b == c);
  }

  SECTION("tfloat - float") {
    const cassian::Tfloat a(2.0F);
    const float b = 3.0F;
    const float c = 2.0F - 3.0F;
    REQUIRE(std::is_same_v<float, decltype(a - b)>);
    REQUIRE(a - b == c);
  }

  SECTION("float - tfloat") {
    const float a = 2.0F;
    const cassian::Tfloat b(3.0F);
    const float c = 2.0F - 3.0F;
    REQUIRE(std::is_same_v<float, decltype(a - b)>);
    REQUIRE(a - b == c);
  }
}

TEST_CASE("tfloat - Multiplication operator") {
  SECTION("tfloat * tfloat") {
    const cassian::Tfloat a(2.0F);
    const cassian::Tfloat b(3.0F);
    const cassian::Tfloat c(2.0F * 3.0F);
    REQUIRE(a * b == c);
  }

  SECTION("tfloat * integer") {
    const cassian::Tfloat a(2.0F);
    const int b = 3;
    const cassian::Tfloat c(2.0F * 3.0F);
    REQUIRE(std::is_same_v<cassian::Tfloat, decltype(a * b)>);
    REQUIRE(a * b == c);
  }

  SECTION("integer * tfloat") {
    const int a = 2;
    const cassian::Tfloat b(3.0F);
    const cassian::Tfloat c(2.0F * 3.0F);
    REQUIRE(std::is_same_v<cassian::Tfloat, decltype(a * b)>);
    REQUIRE(a * b == c);
  }

  SECTION("tfloat * float") {
    const cassian::Tfloat a(2.0F);
    const float b = 3.0F;
    const float c = 2.0F * 3.0F;
    REQUIRE(std::is_same_v<float, decltype(a * b)>);
    REQUIRE(a * b == c);
  }

  SECTION("float * tfloat") {
    const float a = 2.0F;
    const cassian::Tfloat b(3.0F);
    const float c = 2.0F * 3.0F;
    REQUIRE(std::is_same_v<float, decltype(a * b)>);
    REQUIRE(a * b == c);
  }
}

TEST_CASE("tfloat - Division operator") {
  SECTION("tfloat / tfloat") {
    const cassian::Tfloat a(2.0F);
    const cassian::Tfloat b(3.0F);
    const cassian::Tfloat c(2.0F / 3.0F);
    REQUIRE(a / b == c);
  }

  SECTION("tfloat / integer") {
    const cassian::Tfloat a(2.0F);
    const int b = 3;
    const cassian::Tfloat c(2.0F / 3.0F);
    REQUIRE(std::is_same_v<cassian::Tfloat, decltype(a / b)>);
    REQUIRE(a / b == c);
  }

  SECTION("integer / tfloat") {
    const int a = 2;
    const cassian::Tfloat b(3.0F);
    const cassian::Tfloat c(2.0F / 3.0F);
    REQUIRE(std::is_same_v<cassian::Tfloat, decltype(a / b)>);
    REQUIRE(a / b == c);
  }

  SECTION("tfloat / float") {
    const cassian::Tfloat a(2.0F);
    const float b = 3.0F;
    const float c = 2.0F / 3.0F;
    REQUIRE(std::is_same_v<float, decltype(a / b)>);
    REQUIRE(a / b == c);
  }

  SECTION("float / tfloat") {
    const float a = 2.0F;
    const cassian::Tfloat b(3.0F);
    const float c = 2.0F / 3.0F;
    REQUIRE(std::is_same_v<float, decltype(a / b)>);
    REQUIRE(a / b == c);
  }
}

TEST_CASE("tfloat - abs") {
  SECTION("+zero") {
    const auto a = cassian::Tfloat::encode(0x00000000);
    REQUIRE(abs(a) == a);
  }

  SECTION("-zero") {
    const auto a = cassian::Tfloat::encode(0x80000000);
    REQUIRE(abs(a) == -a);
  }

  SECTION("one") {
    const auto a = cassian::Tfloat::encode(0x3f800000);
    REQUIRE(abs(a) == a);
  }

  SECTION("-one") {
    const auto a = cassian::Tfloat::encode(0xbf800000);
    REQUIRE(abs(a) == -a);
  }
}

TEST_CASE("tfloat - sqrt") {
  const cassian::Tfloat a(2.0F);
  const float b = 2.0F;
  REQUIRE(cassian::sqrt(a) == cassian::Tfloat(std::sqrt(b)));
}

TEST_CASE("tfloat - to_string") {
  SECTION("zero") {
    const cassian::Tfloat input(0.0F);
    const std::string output = to_string(input);
    const std::string expected = "0x00000000";
    REQUIRE(output == expected);
  }

  SECTION("denorm") {
    const cassian::Tfloat input = cassian::Tfloat::encode(0x00002000U);
    const std::string output = to_string(input);
    const std::string expected = "0x00002000";
    REQUIRE(output == expected);
  }

  SECTION("one") {
    const cassian::Tfloat input(1.0F);
    const std::string output = to_string(input);
    const std::string expected = "0x3f800000";
    REQUIRE(output == expected);
  }
}

TEST_CASE("tfloat - stream operator") {
  std::stringstream ss;
  SECTION("zero") {
    const cassian::Tfloat input(0.0F);
    ss << input;
    const std::string expected = "0x00000000";
    REQUIRE(ss.str() == expected);
  }

  SECTION("denorm") {
    const cassian::Tfloat input = cassian::Tfloat::encode(0x00002000U);
    ss << input;
    const std::string expected = "0x00002000";
    REQUIRE(ss.str() == expected);
  }

  SECTION("one") {
    const cassian::Tfloat input(1.0F);
    ss << input;
    const std::string expected = "0x3f800000";
    REQUIRE(ss.str() == expected);
  }
}

TEST_CASE("tfloat - isnan") {
  SECTION("with NaN") {
    const cassian::Tfloat a = cassian::Tfloat::encode(0x7fffe000);
    REQUIRE(isnan(a));
  }

  SECTION("with zero") {
    const cassian::Tfloat a(0.0F);
    REQUIRE_FALSE(isnan(a));
  }
}

TEST_CASE("tfloat - nextafter") {
  SECTION("zero -> one") {
    const cassian::Tfloat a(0.0F);
    const cassian::Tfloat b(1.0F);
    const cassian::Tfloat denorm =
        std::numeric_limits<cassian::Tfloat>::denorm_min();
    REQUIRE(nextafter(a, b) == denorm);
  }

  SECTION("zero -> -one") {
    const cassian::Tfloat a(0.0F);
    const cassian::Tfloat b(-1.0F);
    const cassian::Tfloat denorm =
        std::numeric_limits<cassian::Tfloat>::denorm_min();
    REQUIRE(nextafter(a, b) == -denorm);
  }

  SECTION("1.5 -> greater than 1.5") {
    const cassian::Tfloat a(1.5F);
    const cassian::Tfloat b(2.0F);
    const cassian::Tfloat epsilon =
        std::numeric_limits<cassian::Tfloat>::epsilon();
    REQUIRE(nextafter(a, b) == a + epsilon);
  }

  SECTION("1.5 -> lesser than 1.5") {
    const cassian::Tfloat a(1.5F);
    const cassian::Tfloat b(0.0F);
    const cassian::Tfloat epsilon =
        std::numeric_limits<cassian::Tfloat>::epsilon();
    REQUIRE(nextafter(a, b) == a - epsilon);
  }

  SECTION("-1.5 -> greater than -1.5") {
    const cassian::Tfloat a(-1.5F);
    const cassian::Tfloat b(-0.5F);
    const cassian::Tfloat epsilon =
        std::numeric_limits<cassian::Tfloat>::epsilon();
    REQUIRE(nextafter(a, b) == a + epsilon);
  }

  SECTION("-1.5 -> greater than zero") {
    const cassian::Tfloat a(-1.5F);
    const cassian::Tfloat b(0.5F);
    const cassian::Tfloat epsilon =
        std::numeric_limits<cassian::Tfloat>::epsilon();
    REQUIRE(nextafter(a, b) == a + epsilon);
  }

  SECTION("-1.5 -> lesser than -1.5") {
    const cassian::Tfloat a(-1.5F);
    const cassian::Tfloat b(-2.0F);
    const cassian::Tfloat epsilon =
        std::numeric_limits<cassian::Tfloat>::epsilon();
    REQUIRE(nextafter(a, b) == a - epsilon);
  }

  SECTION("x -> greater than x (high exponent)") {
    const cassian::Tfloat a = cassian::Tfloat::encode(0x70000000);
    const cassian::Tfloat b = cassian::Tfloat::encode(0x78000000);
    const cassian::Tfloat expected = cassian::Tfloat::encode(0x70002000);
    REQUIRE(nextafter(a, b) == expected);
  }

  SECTION("x -> lesser than x (high exponent)") {
    const cassian::Tfloat a = cassian::Tfloat::encode(0x70000000);
    const cassian::Tfloat b = cassian::Tfloat::encode(0x60000000);
    const cassian::Tfloat expected = cassian::Tfloat::encode(0x6fffe000);
    REQUIRE(nextafter(a, b) == expected);
  }

  SECTION("zero -> zero") {
    const cassian::Tfloat a(0.0F);
    const cassian::Tfloat b(0.0F);
    REQUIRE(nextafter(a, b) == b);
  }
}

} // namespace
