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
#include <type_traits>
#include <vector>

#include <catch2/catch.hpp>

#include <cassian/fp_types/half.hpp>

#include "common.hpp"

namespace ca = cassian;

namespace {
const float float_certain_value = -0.1572265625F;
const float float_half_subnormal_value = 0.0000254511833191F;
const float float_lowest_half_subnormal_value = 5.960464477539063e-8F;
const uint32_t float_random = 0x0000000f;
const uint32_t float_random_carry_over = 0x00001000;
const uint32_t float_to_be_rounded_up_srnd = 0x427ff000; // 63.9844
const std::vector<float> float_values{
    {0.0F, -0.0F, 1.0F, 2.0F, float_certain_value, float_half_subnormal_value,
     float_lowest_half_subnormal_value,
     std::numeric_limits<float>::infinity()}};

const std::vector<float> float_expected{
    {0.0F, -0.0F, 1.0F, 2.0F, float_certain_value, float_half_subnormal_value,
     float_lowest_half_subnormal_value,
     std::numeric_limits<float>::infinity()}};

const uint32_t float_to_be_rounded_down = 0x42006140;
const uint32_t float_to_be_rounded_down_to_even = 0x42005000;
const uint32_t float_to_be_rounded_up = 0x42005090;
const uint32_t float_to_be_rounded_up_to_even = 0x42007000;
const uint32_t float_to_be_rounded_max = 0xffffffff;
const uint32_t float_to_be_rounded_to_zero = 0x17a04120;
const uint32_t float_to_be_rounded_to_inf = 0x7ba04120;
const uint32_t float_to_be_rounded_up_to_denorm = 0x37864288;
const uint32_t float_to_be_rounded_up_to_even_to_denorm = 0x3786c000;
const uint32_t float_to_be_rounded_down_to_denorm = 0x37860a88;
const uint32_t float_to_be_rounded_down_to_even_to_denorm = 0x37864000;
const std::vector<float> float_rnde_rounding_values{
    {uint32_to_float(float_to_be_rounded_down),
     uint32_to_float(float_to_be_rounded_down_to_even),
     uint32_to_float(float_to_be_rounded_up),
     uint32_to_float(float_to_be_rounded_up_to_even),
     uint32_to_float(float_to_be_rounded_max),
     uint32_to_float(float_to_be_rounded_to_zero),
     uint32_to_float(float_to_be_rounded_to_inf),
     uint32_to_float(float_to_be_rounded_up_to_denorm),
     uint32_to_float(float_to_be_rounded_up_to_even_to_denorm),
     uint32_to_float(float_to_be_rounded_down_to_denorm),
     uint32_to_float(float_to_be_rounded_down_to_even_to_denorm)}};
const std::vector<float> float_srnd_rounding_values{
    {0.0F, -0.0F, 1.0F, 2.0F, std::numeric_limits<float>::infinity(),
     uint32_to_float(float_to_be_rounded_up_srnd),
     uint32_to_float(float_to_be_rounded_up_srnd)}};
const std::vector<float> float_srnd_rounding_random_values{
    {uint32_to_float(float_random_carry_over),
     uint32_to_float(float_random_carry_over),
     uint32_to_float(float_random_carry_over),
     uint32_to_float(float_random_carry_over),
     uint32_to_float(float_random_carry_over), uint32_to_float(float_random),
     uint32_to_float(float_random_carry_over)}};

const uint16_t half_zero = 0x0000;
const uint16_t half_minus_zero = 0x8000;
const uint16_t half_one = 0x3c00;
const uint16_t half_two = 0x4000;
const uint16_t half_certain_value = 0xb108;
const uint16_t half_subnormal_value = 0x01ab;
const uint16_t half_lowest_subnormal_value = 0x0001;
const uint16_t half_infinity = 0x7c00;
const uint16_t half_srnd_no_carry_over = 0x53ff; // 63.96875
const uint16_t half_srnd_carry_over = 0x5400;    // 64

const std::vector<uint16_t> half_values{
    {half_zero, half_minus_zero, half_one, half_two, half_certain_value,
     half_subnormal_value, half_lowest_subnormal_value, half_infinity}};
const std::vector<uint16_t> half_expected{
    {half_zero, half_minus_zero, half_one, half_two, half_certain_value,
     half_subnormal_value, half_lowest_subnormal_value, half_infinity}};
const uint16_t half_rounded_down = 0x5003;
const uint16_t half_rounded_down_to_even = 0x5002;
const uint16_t half_rounded_up = 0x5003;
const uint16_t half_rounded_up_to_even = 0x5004;
const uint16_t half_rounded_max = 0xffff;
const uint16_t half_denorm_rounded_up = 0x010d;
const uint16_t half_denorm_rounded_up_to_even = 0x010e;
const uint16_t half_denorm_rounded_down = 0x010c;
const uint16_t half_denorm_rounded_down_to_even = 0x010c;
const std::vector<uint16_t> half_rnde_rounding_expected{
    {half_rounded_down, half_rounded_down_to_even, half_rounded_up,
     half_rounded_up_to_even, half_rounded_max, half_zero, half_infinity,
     half_denorm_rounded_up, half_denorm_rounded_up_to_even,
     half_denorm_rounded_down, half_denorm_rounded_down_to_even}};
const std::vector<uint16_t> half_srnd_rounding_expected{
    {half_zero, half_minus_zero, half_one, half_two, half_infinity,
     half_srnd_no_carry_over, half_srnd_carry_over}};

void test_srnd(float value, float random, uint16_t expected) {
  ca::Half result = ca::Half(value, random);
  REQUIRE(result.decode() == expected);
}

template <typename In, typename Out> void test(In value, Out expected) {
  auto result = cast<In, ca::half, Out>(value);
  if (std::is_same<Out, float>::value) {
    uint32_t result_u32 = *reinterpret_cast<uint32_t *>(&result);
    uint32_t expected_u32 = *reinterpret_cast<uint32_t *>(&expected);
    REQUIRE(result_u32 == expected_u32);
  } else {
    REQUIRE(result == expected);
  }
}

// The point of these two tests is to see if value going through
// half is not altered. Values need to be representable in 16-bits,
// so in uint16 case we use half_values, and in float we use
// float_expected, since it contains float values after half
// conversion (so representable in 16-bits).
TEST_CASE("half from uint16 as uint16", "") {
  auto test_params = GENERATE(from_range(zip(half_values, half_values)));
  test(std::get<0>(test_params), std::get<1>(test_params));
}
TEST_CASE("half from float as float", "") {
  auto test_params = GENERATE(from_range(zip(float_expected, float_expected)));
  test(std::get<0>(test_params), std::get<1>(test_params));
}
TEST_CASE("half from uint16 as float", "") {
  auto test_params = GENERATE(from_range(zip(half_values, float_expected)));
  test(std::get<0>(test_params), std::get<1>(test_params));
}
TEST_CASE("half from float as uint16", "") {
  auto test_params = GENERATE(from_range(zip(float_values, half_expected)));
  test(std::get<0>(test_params), std::get<1>(test_params));
}
TEST_CASE("half from float with rnde rounding", "") {
  auto test_params = GENERATE(
      from_range(zip(float_rnde_rounding_values, half_rnde_rounding_expected)));
  test(std::get<0>(test_params), std::get<1>(test_params));
}

TEST_CASE("half from float with stochastic rounding", "") {
  auto test_params = GENERATE(from_range(zip(float_srnd_rounding_values,
                                             float_srnd_rounding_random_values,
                                             half_srnd_rounding_expected)));
  test_srnd(std::get<0>(test_params), std::get<1>(test_params),
            std::get<2>(test_params));
}

const uint16_t half_nan = 0x7f00;
const uint16_t half_nan2 = 0x7f01;
TEST_CASE("half NaN sensitive comparison same NaN value") {
  const ca::half h(half_nan);
  REQUIRE(true == h.nan_sensitive_eq(h));
}

TEST_CASE("half NaN sensitive comparison different NaN value") {
  const ca::half h1(half_nan);
  const ca::half h2(half_nan2);
  REQUIRE(true == h1.nan_sensitive_eq(h2));
}
TEST_CASE("half NaN sensitive comparison NaN Inf") {
  const ca::half h1(half_nan);
  const ca::half h2(half_infinity);
  REQUIRE(false == h1.nan_sensitive_eq(h2));
}

TEST_CASE("half - Equality operator") {
  SECTION("with the same value") {
    const cassian::Half a(2.0F);
    REQUIRE(a == a);
  }

  SECTION("with different value") {
    const cassian::Half a(2.0F);
    const cassian::Half b(3.0F);
    REQUIRE_FALSE(a == b);
  }

  SECTION("with almost equal value") {
    const cassian::Half a = cassian::Half::encode(0x3c00);
    const cassian::Half b = cassian::Half::encode(0x3c01);
    REQUIRE_FALSE(a == b);
  }
}

TEST_CASE("half - Inequality operator") {
  SECTION("with the same value") {
    const cassian::Half a(2.0F);
    REQUIRE_FALSE(a != a);
  }

  SECTION("with different value") {
    const cassian::Half a(2.0F);
    const cassian::Half b(3.0F);
    REQUIRE(a != b);
  }

  SECTION("with almost equal value") {
    const cassian::Half a = cassian::Half::encode(0x3c00);
    const cassian::Half b = cassian::Half::encode(0x3c01);
    REQUIRE(a != b);
  }
}

TEST_CASE("half - Less than operator") {
  SECTION("with the same value") {
    const cassian::Half a(2.0F);
    REQUIRE_FALSE(a < a);
  }

  SECTION("with lesser value") {
    const cassian::Half a(2.0F);
    const cassian::Half b(1.0F);
    REQUIRE_FALSE(a < b);
  }

  SECTION("with greater value") {
    const cassian::Half a(2.0F);
    const cassian::Half b(3.0F);
    REQUIRE(a < b);
  }
}

TEST_CASE("half - Greater than operator") {
  SECTION("with the same value") {
    const cassian::Half a(2.0F);
    REQUIRE_FALSE(a > a);
  }

  SECTION("with lesser value") {
    const cassian::Half a(2.0F);
    const cassian::Half b(1.0F);
    REQUIRE(a > b);
  }

  SECTION("with greater value") {
    const cassian::Half a(2.0F);
    const cassian::Half b(3.0F);
    REQUIRE_FALSE(a > b);
  }
}

TEST_CASE("half - Less than or equal operator") {
  SECTION("with the same value") {
    const cassian::Half a(2.0F);
    REQUIRE(a <= a);
  }

  SECTION("with lesser value") {
    const cassian::Half a(2.0F);
    const cassian::Half b(1.0F);
    REQUIRE_FALSE(a <= b);
  }

  SECTION("with greater value") {
    const cassian::Half a(2.0F);
    const cassian::Half b(3.0F);
    REQUIRE(a <= b);
  }
}

TEST_CASE("half - Greater than or equal operator") {
  SECTION("with the same value") {
    const cassian::Half a(2.0F);
    REQUIRE(a >= a);
  }

  SECTION("with lesser value") {
    const cassian::Half a(2.0F);
    const cassian::Half b(1.0F);
    REQUIRE(a >= b);
  }

  SECTION("with greater value") {
    const cassian::Half a(2.0F);
    const cassian::Half b(3.0F);
    REQUIRE_FALSE(a >= b);
  }
}

TEST_CASE("half - Cast to bool") {
  SECTION("+zero") {
    REQUIRE_FALSE(static_cast<bool>(cassian::Half::encode(0x0000)));
  }

  SECTION("-zero") {
    REQUIRE_FALSE(static_cast<bool>(cassian::Half::encode(0x8000)));
  }

  SECTION("denorm") {
    REQUIRE(static_cast<bool>(cassian::Half::encode(0x0001)));
  }

  SECTION("one") { REQUIRE(static_cast<bool>(cassian::Half::encode(0x3c00))); }
}

TEST_CASE("half - Logical not operator") {
  SECTION("+zero") { REQUIRE(!cassian::Half::encode(0x0000)); }

  SECTION("-zero") { REQUIRE(!cassian::Half::encode(0x8000)); }

  SECTION("denorm") { REQUIRE_FALSE(!cassian::Half::encode(0x0001)); }

  SECTION("one") { REQUIRE_FALSE(!cassian::Half::encode(0x3c00)); }
}

TEST_CASE("half - Unary plus operator") {
  const cassian::Half a(2.0F);
  REQUIRE(+a == a);
}

TEST_CASE("half - Unary minus operator") {
  const cassian::Half a(2.0F);
  const cassian::Half b(-2.0F);
  REQUIRE(-a == b);
}

TEST_CASE("half - Addition operator") {
  SECTION("half + half") {
    const cassian::Half a(2.0F);
    const cassian::Half b(3.0F);
    const cassian::Half c(2.0F + 3.0F);
    REQUIRE(a + b == c);
  }

  SECTION("half + integer") {
    const cassian::Half a(2.0F);
    const int b = 3;
    const cassian::Half c(2.0F + 3.0F);
    REQUIRE(std::is_same_v<cassian::Half, decltype(a + b)>);
    REQUIRE(a + b == c);
  }

  SECTION("integer + half") {
    const int a = 2;
    const cassian::Half b(3.0F);
    const cassian::Half c(2.0F + 3.0F);
    REQUIRE(std::is_same_v<cassian::Half, decltype(a + b)>);
    REQUIRE(a + b == c);
  }

  SECTION("half + float") {
    const cassian::Half a(2.0F);
    const float b = 3.0F;
    const float c = 2.0F + 3.0F;
    REQUIRE(std::is_same_v<float, decltype(a + b)>);
    REQUIRE(a + b == c);
  }

  SECTION("float + half") {
    const float a = 2.0F;
    const cassian::Half b(3.0F);
    const float c = 2.0F + 3.0F;
    REQUIRE(std::is_same_v<float, decltype(a + b)>);
    REQUIRE(a + b == c);
  }
}

TEST_CASE("half - Subtraction operator") {
  SECTION("half - half") {
    const cassian::Half a(2.0F);
    const cassian::Half b(3.0F);
    const cassian::Half c(2.0F - 3.0F);
    REQUIRE(a - b == c);
  }

  SECTION("half - integer") {
    const cassian::Half a(2.0F);
    const int b = 3;
    const cassian::Half c(2.0F - 3.0F);
    REQUIRE(std::is_same_v<cassian::Half, decltype(a - b)>);
    REQUIRE(a - b == c);
  }

  SECTION("integer - half") {
    const int a = 2;
    const cassian::Half b(3.0F);
    const cassian::Half c(2.0F - 3.0F);
    REQUIRE(std::is_same_v<cassian::Half, decltype(a - b)>);
    REQUIRE(a - b == c);
  }

  SECTION("half - float") {
    const cassian::Half a(2.0F);
    const float b = 3.0F;
    const float c = 2.0F - 3.0F;
    REQUIRE(std::is_same_v<float, decltype(a - b)>);
    REQUIRE(a - b == c);
  }

  SECTION("float - half") {
    const float a = 2.0F;
    const cassian::Half b(3.0F);
    const float c = 2.0F - 3.0F;
    REQUIRE(std::is_same_v<float, decltype(a - b)>);
    REQUIRE(a - b == c);
  }
}

TEST_CASE("half - Multiplication operator") {
  SECTION("half * half") {
    const cassian::Half a(2.0F);
    const cassian::Half b(3.0F);
    const cassian::Half c(2.0F * 3.0F);
    REQUIRE(a * b == c);
  }

  SECTION("half * integer") {
    const cassian::Half a(2.0F);
    const int b = 3;
    const cassian::Half c(2.0F * 3.0F);
    REQUIRE(std::is_same_v<cassian::Half, decltype(a * b)>);
    REQUIRE(a * b == c);
  }

  SECTION("integer * half") {
    const int a = 2;
    const cassian::Half b(3.0F);
    const cassian::Half c(2.0F * 3.0F);
    REQUIRE(std::is_same_v<cassian::Half, decltype(a * b)>);
    REQUIRE(a * b == c);
  }

  SECTION("half * float") {
    const cassian::Half a(2.0F);
    const float b = 3.0F;
    const float c = 2.0F * 3.0F;
    REQUIRE(std::is_same_v<float, decltype(a * b)>);
    REQUIRE(a * b == c);
  }

  SECTION("float * half") {
    const float a = 2.0F;
    const cassian::Half b(3.0F);
    const float c = 2.0F * 3.0F;
    REQUIRE(std::is_same_v<float, decltype(a * b)>);
    REQUIRE(a * b == c);
  }
}

TEST_CASE("half - Division operator") {
  SECTION("half / half") {
    const cassian::Half a(2.0F);
    const cassian::Half b(3.0F);
    const cassian::Half c(2.0F / 3.0F);
    REQUIRE(a / b == c);
  }

  SECTION("half / integer") {
    const cassian::Half a(2.0F);
    const int b = 3;
    const cassian::Half c(2.0F / 3.0F);
    REQUIRE(std::is_same_v<cassian::Half, decltype(a / b)>);
    REQUIRE(a / b == c);
  }

  SECTION("integer / half") {
    const int a = 2;
    const cassian::Half b(3.0F);
    const cassian::Half c(2.0F / 3.0F);
    REQUIRE(std::is_same_v<cassian::Half, decltype(a / b)>);
    REQUIRE(a / b == c);
  }

  SECTION("half / float") {
    const cassian::Half a(2.0F);
    const float b = 3.0F;
    const float c = 2.0F / 3.0F;
    REQUIRE(std::is_same_v<float, decltype(a / b)>);
    REQUIRE(a / b == c);
  }

  SECTION("float / half") {
    const float a = 2.0F;
    const cassian::Half b(3.0F);
    const float c = 2.0F / 3.0F;
    REQUIRE(std::is_same_v<float, decltype(a / b)>);
    REQUIRE(a / b == c);
  }
}

TEST_CASE("half - abs") {
  SECTION("+zero") {
    const auto a = cassian::Half::encode(0x0000);
    REQUIRE(cassian::abs(a) == a);
  }

  SECTION("-zero") {
    const auto a = cassian::Half::encode(0x8000);
    REQUIRE(cassian::abs(a) == -a);
  }

  SECTION("one") {
    const auto a = cassian::Half::encode(0x3c00);
    REQUIRE(cassian::abs(a) == a);
  }

  SECTION("-one") {
    const auto a = cassian::Half::encode(0xbc00);
    REQUIRE(cassian::abs(a) == -a);
  }
}

TEST_CASE("half - sqrt") {
  const cassian::Half a(2.0F);
  const float b = 2.0F;
  REQUIRE(cassian::sqrt(a) == cassian::Half(std::sqrt(b)));
}

TEST_CASE("half - to_string") {
  SECTION("zero") {
    const cassian::Half input(0.0F);
    const std::string output = cassian::to_string(input);
    const std::string expected = "0x0000";
    REQUIRE(output == expected);
  }

  SECTION("denorm") {
    const cassian::Half input = cassian::Half::encode(0x0001);
    const std::string output = cassian::to_string(input);
    const std::string expected = "0x0001";
    REQUIRE(output == expected);
  }

  SECTION("one") {
    const cassian::Half input(1.0F);
    const std::string output = cassian::to_string(input);
    const std::string expected = "0x3c00";
    REQUIRE(output == expected);
  }
}

TEST_CASE("half - stream operator") {
  std::stringstream ss;
  SECTION("zero") {
    const cassian::Half input(0.0F);
    ss << input;
    const std::string expected = "0x0000";
    REQUIRE(ss.str() == expected);
  }

  SECTION("denorm") {
    const cassian::Half input = cassian::Half::encode(0x0001);
    ss << input;
    const std::string expected = "0x0001";
    REQUIRE(ss.str() == expected);
  }

  SECTION("one") {
    const cassian::Half input(1.0F);
    ss << input;
    const std::string expected = "0x3c00";
    REQUIRE(ss.str() == expected);
  }
}

TEST_CASE("half - isnan") {
  SECTION("with NaN") {
    const cassian::Half a = cassian::Half::encode(0x7f00);
    REQUIRE(cassian::isnan(a));
  }

  SECTION("with zero") {
    const cassian::Half a(0.0F);
    REQUIRE_FALSE(cassian::isnan(a));
  }
}

TEST_CASE("half - nextafter") {
  SECTION("zero -> one") {
    const cassian::Half a(0.0F);
    const cassian::Half b(1.0F);
    const cassian::Half epsilon = std::numeric_limits<cassian::Half>::epsilon();
    REQUIRE(cassian::nextafter(a, b) == epsilon);
  }

  SECTION("zero -> -one") {
    const cassian::Half a(0.0F);
    const cassian::Half b(-1.0F);
    const cassian::Half epsilon = std::numeric_limits<cassian::Half>::epsilon();
    REQUIRE(cassian::nextafter(a, b) == -epsilon);
  }

  SECTION("zero -> zero") {
    const cassian::Half a(0.0F);
    const cassian::Half b(0.0F);
    REQUIRE(cassian::nextafter(a, b) == b);
  }
}

} // namespace
