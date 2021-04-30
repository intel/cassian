/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <algorithm>
#include <cassian/runtime/openclc_types.hpp>
#include <cassian/utility/math.hpp>
#include <cassian/utility/utility.hpp>
#include <cassian/vector/vector.hpp>
#include <catch2/catch.hpp>
#include <cmath>
#include <cstdint>
#include <limits>
#include <vector>

namespace ca = cassian;

namespace {
TEST_CASE("dot_product", "") {
  SECTION("vectors - integers") {
    const ca::Vector<int32_t, 5> vector_a = {0, 1, 2, 3, 4};
    const ca::Vector<int32_t, 5> vector_b = {5, 4, 2, 3, 1};
    const int32_t result = ca::dot_product(vector_a, vector_b);
    const int32_t reference = 21;
    REQUIRE(reference == result);
  }
  SECTION("vectors - floating point") {
    const ca::Vector<double, 3, 4> vector_a = {1.0, 1.0, 1.0};
    const ca::Vector<double, 3, 4> vector_b = {2.0, 2.0, 2.0};
    const double result = ca::dot_product(vector_a, vector_b);
    const double reference = 6.0;
    REQUIRE(reference == result);
  }
}
TEST_CASE("normalize", "") {
  SECTION("3 element vector with one infinity") {
    const ca::Vector<float, 3, 4> vector = {
        1.0, 2.0, std::numeric_limits<float>::infinity()};
    const ca::Vector<float, 3, 4> result = ca::normalize(vector);
    const ca::Vector<float, 3, 4> reference = {0.0, 0.0, 1.0};
    REQUIRE(reference == result);
  }
  SECTION("3 element vector with one NaN") {
    constexpr auto nan = std::numeric_limits<float>::quiet_NaN();
    const ca::Vector<float, 3, 4> vector = {1.0, 2.0, nan};
    const ca::Vector<float, 3, 4> result = ca::normalize(vector);
    REQUIRE(std::all_of(result.begin(),
                        result.begin() + ca::Vector<float, 3, 4>::vector_size,
                        [](const float &value) { return std::isnan(value); }));
  }
  SECTION("null vector") {
    const ca::Vector<float, 3, 4> vector = {0.0, 0.0, 0.0};
    const ca::Vector<float, 3, 4> result = ca::normalize(vector);
    const ca::Vector<float, 3, 4> reference = {0.0, 0.0, 0.0};
    REQUIRE(reference == result);
  }
  SECTION("3 element vector") {
    const ca::Vector<float, 3, 4> vector = {-3.0, 8.0, -4.0};
    const ca::Vector<float, 3, 4> result = ca::normalize(vector);
    float sum = 0.0;
    for (auto i = 0; i < ca::Vector<float, 3, 4>::vector_size; i++) {
      sum += vector[i] * vector[i];
    }
    const auto norm = std::sqrt(sum);
    const ca::Vector<float, 3, 4> reference = {
        vector[0] / norm, vector[1] / norm, vector[2] / norm};
    REQUIRE(reference == result);
  }

  SECTION("scalar - infinity") {
    constexpr float scalar = std::numeric_limits<float>::infinity();
    const float result = ca::normalize(scalar);
    const float reference = 1.0;
    REQUIRE(reference == result);
  }
  SECTION("scalar - NaN") {
    constexpr auto scalar = std::numeric_limits<float>::quiet_NaN();
    const float result = ca::normalize(scalar);
    REQUIRE(std::isnan(result));
  }
  SECTION("scalar - null") {
    const float scalar = 0.0;
    const float result = ca::normalize(scalar);
    const float reference = 0.0;
    REQUIRE(reference == result);
  }
  SECTION("scalar - negative number") {
    const float scalar = -9.0;
    const float result = ca::normalize(scalar);
    const float reference = -1.0;
    REQUIRE(reference == result);
  }
  SECTION("scalar - positive number") {
    const float scalar = 5.432;
    const float result = ca::normalize(scalar);
    const float reference = 1.0;
    REQUIRE(reference == result);
  }
}

} // namespace