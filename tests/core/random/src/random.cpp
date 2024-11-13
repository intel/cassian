/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/fp_types/bfloat16.hpp>
#include <cassian/fp_types/half.hpp>
#include <cassian/fp_types/tfloat.hpp>
#include <cassian/random/random.hpp>
#include <cassian/vector/vector.hpp>
#include <catch2/catch.hpp>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <vector>

namespace ca = cassian;

namespace {

using int16_4_t = ca::Vector<int16_t, 4>;

TEST_CASE("generate_value") {
  const int seed = 0;
  const int iterations = 100;

  SECTION("explicit min/max") {
    SECTION("int32_t") {
      const int32_t min = -10;
      const int32_t max = 20;
      for (int i = 0; i < iterations; ++i) {
        const int32_t output = ca::generate_value(min, max, seed);
        REQUIRE(output >= min);
        REQUIRE(output <= max);
      }
    }
    SECTION("int32_t with exceptions - left endpoint") {
      const int32_t left_endpoint = 10;
      const int32_t middle = 11;
      const int32_t right_endpoint = 12;
      for (int i = 0; i < iterations; ++i) {
        const int32_t output = ca::generate_value(
            left_endpoint, right_endpoint, seed, {middle, right_endpoint});
        REQUIRE(output == left_endpoint);
      }
    }
    SECTION("int32_t with exceptions - middle") {
      const int32_t left_endpoint = 10;
      const int32_t middle = 11;
      const int32_t right_endpoint = 12;
      for (int i = 0; i < iterations; ++i) {
        const int32_t output =
            ca::generate_value(left_endpoint, right_endpoint, seed,
                               {left_endpoint, right_endpoint});
        REQUIRE(output == middle);
      }
    }
    SECTION("int32_t with exceptions - right endpoint") {
      const int32_t left_endpoint = 10;
      const int32_t middle = 11;
      const int32_t right_endpoint = 12;
      for (int i = 0; i < iterations; ++i) {
        const int32_t output = ca::generate_value(
            left_endpoint, right_endpoint, seed, {left_endpoint, middle});
        REQUIRE(output == right_endpoint);
      }
    }
    SECTION("int8_t") {
      const int8_t min = -10;
      const int8_t max = 20;
      for (int i = 0; i < iterations; ++i) {
        const int8_t output = ca::generate_value(min, max, seed);
        REQUIRE(output >= min);
        REQUIRE(output <= max);
      }
    }
    SECTION("uint8_t") {
      const uint8_t min = 10;
      const uint8_t max = 20;
      for (int i = 0; i < iterations; ++i) {
        const uint8_t output = ca::generate_value(min, max, seed);
        REQUIRE(output >= min);
        REQUIRE(output <= max);
      }
    }
    SECTION("float") {
      const float min = 10.1F;
      const float max = 11.5F;
      for (int i = 0; i < iterations; ++i) {
        const float output = ca::generate_value(min, max, seed);
        REQUIRE(output >= min);
        REQUIRE(output <= max);
      }
    }
    SECTION("float max range") {
      const float min = std::numeric_limits<float>::lowest();
      const float max = std::numeric_limits<float>::max();
      for (int i = 0; i < iterations; ++i) {
        const float output = ca::generate_value(min, max, seed);
        REQUIRE(output >= min);
        REQUIRE(output <= max);
      }
    }
    SECTION("float with exceptions - left endpoint") {
      const float float_max = std::numeric_limits<float>::max();
      const float left_endpoint = 0.123456789F;
      const float middle = std::nextafter(left_endpoint, float_max);
      const float right_endpoint = std::nextafter(middle, float_max);
      for (int i = 0; i < iterations; ++i) {
        const float output = ca::generate_value(left_endpoint, right_endpoint,
                                                seed, {right_endpoint, middle});
        REQUIRE(output == left_endpoint);
      }
    }
    SECTION("float with exceptions - middle") {
      const float float_max = std::numeric_limits<float>::max();
      const float left_endpoint = 0.123456789F;
      const float middle = std::nextafter(left_endpoint, float_max);
      const float right_endpoint = std::nextafter(middle, float_max);
      for (int i = 0; i < iterations; ++i) {
        const float output =
            ca::generate_value(left_endpoint, right_endpoint, seed,
                               {left_endpoint, right_endpoint});
        REQUIRE(output == middle);
      }
    }
    SECTION("float with exceptions - right endpoint") {
      const float float_max = std::numeric_limits<float>::max();
      const float left_endpoint = 0.123456789F;
      const float middle = std::nextafter(left_endpoint, float_max);
      const float right_endpoint = std::nextafter(middle, float_max);
      for (int i = 0; i < iterations; ++i) {
        const float output = ca::generate_value(left_endpoint, right_endpoint,
                                                seed, {left_endpoint, middle});
        REQUIRE(output == right_endpoint);
      }
    }
    SECTION("float with exceptions - left endpoint (extreme values)") {
      const float float_max = std::numeric_limits<float>::max();
      const float float_lowest = std::numeric_limits<float>::lowest();
      const float left_endpoint = float_lowest;
      const float middle = std::nextafter(left_endpoint, float_max);
      const float right_endpoint = std::nextafter(middle, float_max);
      for (int i = 0; i < iterations; ++i) {
        const float output = ca::generate_value(left_endpoint, right_endpoint,
                                                seed, {right_endpoint, middle});
        REQUIRE(output == left_endpoint);
      }
    }
    SECTION("float with exceptions - middle (extreme values)") {
      const float float_max = std::numeric_limits<float>::max();
      const float float_lowest = std::numeric_limits<float>::lowest();
      const float left_endpoint = float_lowest;
      const float middle = std::nextafter(left_endpoint, float_max);
      const float right_endpoint = std::nextafter(middle, float_max);
      for (int i = 0; i < iterations; ++i) {
        const float output =
            ca::generate_value(left_endpoint, right_endpoint, seed,
                               {left_endpoint, right_endpoint});
        REQUIRE(output == middle);
      }
    }
    SECTION("float with exceptions - right endpoint (extreme values)") {
      const float float_max = std::numeric_limits<float>::max();
      const float float_lowest = std::numeric_limits<float>::lowest();
      const float left_endpoint = float_lowest;
      const float middle = std::nextafter(left_endpoint, float_max);
      const float right_endpoint = std::nextafter(middle, float_max);
      for (int i = 0; i < iterations; ++i) {
        const float output = ca::generate_value(left_endpoint, right_endpoint,
                                                seed, {left_endpoint, middle});
        REQUIRE(output == right_endpoint);
      }
    }
    SECTION("double") {
      const double min = 10.1;
      const double max = 11.5;
      for (int i = 0; i < iterations; ++i) {
        const double output = ca::generate_value(min, max, seed);
        REQUIRE(output >= min);
        REQUIRE(output <= max);
      }
    }
    SECTION("double max range") {
      const double min = -std::numeric_limits<double>::max();
      const double max = std::numeric_limits<double>::max();
      for (int i = 0; i < iterations; ++i) {
        const double output = ca::generate_value(min, max, seed);
        REQUIRE(output >= min);
        REQUIRE(output <= max);
      }
    }

    SECTION("double with exceptions - left endpoint") {
      const double double_max = std::numeric_limits<double>::max();
      const double left_endpoint = 0.123456789;
      const double middle = std::nextafter(left_endpoint, double_max);
      const double right_endpoint = std::nextafter(middle, double_max);
      for (int i = 0; i < iterations; ++i) {
        const double output = ca::generate_value(
            left_endpoint, right_endpoint, seed, {middle, right_endpoint});
        REQUIRE(output == left_endpoint);
      }
    }
    SECTION("double with exceptions - middle") {
      const double double_max = std::numeric_limits<double>::max();
      const double left_endpoint = 0.123456789;
      const double middle = std::nextafter(left_endpoint, double_max);
      const double right_endpoint = std::nextafter(middle, double_max);
      for (int i = 0; i < iterations; ++i) {
        const double output =
            ca::generate_value(left_endpoint, right_endpoint, seed,
                               {left_endpoint, right_endpoint});
        REQUIRE(output == middle);
      }
    }
    SECTION("double with exceptions - right endpoint") {
      const double double_max = std::numeric_limits<double>::max();
      const double left_endpoint = 0.123456789;
      const double middle = std::nextafter(left_endpoint, double_max);
      const double right_endpoint = std::nextafter(middle, double_max);
      for (int i = 0; i < iterations; ++i) {
        const double output = ca::generate_value(left_endpoint, right_endpoint,
                                                 seed, {left_endpoint, middle});
        REQUIRE(output == right_endpoint);
      }
    }
    SECTION("double with exceptions - left endpoint (extreme values)") {
      const double double_max = std::numeric_limits<double>::max();
      const double double_lowest = std::numeric_limits<double>::lowest();
      const double left_endpoint = double_lowest;
      const double middle = std::nextafter(left_endpoint, double_max);
      const double right_endpoint = std::nextafter(middle, double_max);
      for (int i = 0; i < iterations; ++i) {
        const double output = ca::generate_value(
            left_endpoint, right_endpoint, seed, {right_endpoint, middle});
        REQUIRE(output == left_endpoint);
      }
    }
    SECTION("double with exceptions - middle (extreme values)") {
      const double double_max = std::numeric_limits<double>::max();
      const double double_lowest = std::numeric_limits<double>::lowest();
      const double left_endpoint = double_lowest;
      const double middle = std::nextafter(left_endpoint, double_max);
      const double right_endpoint = std::nextafter(middle, double_max);
      for (int i = 0; i < iterations; ++i) {
        const double output =
            ca::generate_value(left_endpoint, right_endpoint, seed,
                               {left_endpoint, right_endpoint});
        REQUIRE(output == middle);
      }
    }
    SECTION("double with exceptions - right endpoint (extreme values)") {
      const double double_max = std::numeric_limits<double>::max();
      const double double_lowest = std::numeric_limits<double>::lowest();
      const double left_endpoint = double_lowest;
      const double middle = std::nextafter(left_endpoint, double_max);
      const double right_endpoint = std::nextafter(middle, double_max);
      for (int i = 0; i < iterations; ++i) {
        const double output = ca::generate_value(left_endpoint, right_endpoint,
                                                 seed, {left_endpoint, middle});
        REQUIRE(output == right_endpoint);
      }
    }
    SECTION("bfloat16") {
      const ca::bfloat16 min(2.25F);
      const ca::bfloat16 max(4.0F);
      for (int i = 0; i < iterations; ++i) {
        const ca::bfloat16 output = ca::generate_value(min, max, seed);
        REQUIRE(output >= min);
        REQUIRE(output <= max);
      }
    }
    SECTION("half") {
      const ca::half min(10.1F);
      const ca::half max(11.5F);
      for (int i = 0; i < iterations; ++i) {
        const ca::half output = ca::generate_value(min, max, seed);
        REQUIRE(output >= min);
        REQUIRE(output <= max);
      }
    }
    SECTION("tfloat") {
      const ca::tfloat min(10.1F);
      const ca::tfloat max(11.5F);
      for (int i = 0; i < iterations; ++i) {
        const ca::tfloat output = ca::generate_value(min, max, seed);
        REQUIRE(output >= min);
        REQUIRE(output <= max);
        const uint32_t unused_bits = output.decode() & 0x00003ff;
        REQUIRE(unused_bits == 0);
      }
    }
    SECTION("int16_4_t") {
      const int16_t min(-2);
      const int16_t max(1);
      for (int i = 0; i < iterations; ++i) {
        const auto output = ca::generate_value<int16_4_t>(min, max, seed);
        REQUIRE(output >= min);
        REQUIRE(output <= max);
      }
    }
  }

  SECTION("implicit min/max") {
    SECTION("scalar") { REQUIRE_NOTHROW(ca::generate_value<int16_t>(seed)); }
    SECTION("vector") { REQUIRE_NOTHROW(ca::generate_value<int16_4_t>(seed)); }
  }
}

TEST_CASE("generate_vector") {
  const int seed = 0;
  const int size = 100;

  SECTION("explicit min/max") {
    SECTION("scalar") {
      const int16_t min = -10;
      const int16_t max = 20;
      const std::vector<int16_t> output =
          ca::generate_vector(size, min, max, seed);
      REQUIRE(output.size() == size);
      for (const auto &e : output) {
        REQUIRE(e >= min);
        REQUIRE(e <= max);
      }
    }
    SECTION("vector") {
      const int16_t min = -10;
      const int16_t max = 20;
      const std::vector<int16_4_t> output =
          ca::generate_vector<int16_4_t>(size, min, max, seed);
      REQUIRE(output.size() == size);
      for (const auto &e : output) {
        REQUIRE(e >= min);
        REQUIRE(e <= max);
      }
    }
  }

  SECTION("implicit min/max") {
    SECTION("scalar") {
      const std::vector<int16_t> output =
          ca::generate_vector<int16_t>(size, seed);
      REQUIRE(output.size() == size);
    }
    SECTION("vector") {
      const std::vector<int16_4_t> output =
          ca::generate_vector<int16_4_t>(size, seed);
      REQUIRE(output.size() == size);
    }
  }
}

} // namespace
