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
#include <cassian/utility/utility.hpp>

#include <common.hpp>

namespace ca = cassian;
using ca::half;

constexpr double rsqrt_margin = std::numeric_limits<float>::epsilon() * 3;

TEMPLATE_TEST_CASE("cm_rsqrt(vector)", "[cm][rsqrt]", float, half) {
  SECTION("--no-sat") {
    test_math<TestType>(
        16, 1, "cm_rsqrt",
        [](auto x) -> TestType {
          return static_cast<TestType>(1 / std::sqrt(static_cast<double>(x)));
        },
        NO_SAT_LEFT_LIM, NO_SAT_RIGHT_LIM, Saturation::NO_SAT,
        InputType::vector, rsqrt_margin);
  }
  SECTION("--sat-near-zero") {
    test_math<TestType>(
        16, 1, "cm_rsqrt",
        [](auto x) -> TestType {
          return static_cast<TestType>(1 / std::sqrt(static_cast<double>(x)));
        },
        static_cast<float>(std::numeric_limits<TestType>::epsilon()),
        SAT_RIGHT_LIM, Saturation::USE_SAT_ZERO, InputType::vector,
        rsqrt_margin);
  }
  SECTION("--sat-no-limits") {
    test_math<TestType>(
        16, 1, "cm_rsqrt",
        [](auto x) -> TestType {
          return static_cast<TestType>(1 / std::sqrt(static_cast<double>(x)));
        },
        static_cast<float>(std::numeric_limits<TestType>::epsilon()),
        static_cast<float>(std::numeric_limits<TestType>::max()),
        Saturation::USE_SAT_NOLIMITS, InputType::vector, rsqrt_margin);
  }
}

TEMPLATE_TEST_CASE("cm_rsqrt(matrix)", "[cm][rsqrt]", float, half) {
  SECTION("--no-sat") {
    test_math<TestType>(
        2, 8, "cm_rsqrt",
        [](auto x) -> TestType {
          return static_cast<TestType>(1 / std::sqrt(static_cast<double>(x)));
        },
        NO_SAT_LEFT_LIM, NO_SAT_RIGHT_LIM, Saturation::NO_SAT,
        InputType::matrix, rsqrt_margin);
  }
  SECTION("--sat-near-zero") {
    test_math<TestType>(
        2, 8, "cm_rsqrt",
        [](auto x) -> TestType {
          return static_cast<TestType>(1 / std::sqrt(static_cast<double>(x)));
        },
        static_cast<float>(std::numeric_limits<TestType>::epsilon()),
        SAT_RIGHT_LIM, Saturation::USE_SAT_ZERO, InputType::matrix,
        rsqrt_margin);
  }
  SECTION("--sat-no-limits") {
    test_math<TestType>(
        2, 8, "cm_rsqrt",
        [](auto x) -> TestType {
          return static_cast<TestType>(1 / std::sqrt(static_cast<double>(x)));
        },
        static_cast<float>(std::numeric_limits<TestType>::epsilon()),
        static_cast<float>(std::numeric_limits<TestType>::max()),
        Saturation::USE_SAT_NOLIMITS, InputType::matrix, rsqrt_margin);
  }
}

TEMPLATE_TEST_CASE("cm_rsqrt(scalar)", "[cm][rsqrt]", float, half) {
  SECTION("--no-sat") {
    test_math<TestType>(
        -1, -1, "cm_rsqrt",
        [](auto x) -> TestType {
          return static_cast<TestType>(1 / std::sqrt(static_cast<double>(x)));
        },
        NO_SAT_LEFT_LIM, NO_SAT_RIGHT_LIM, Saturation::NO_SAT,
        InputType::scalar, rsqrt_margin);
  }
  SECTION("--sat-near-zero") {
    test_math<TestType>(
        -1, -1, "cm_rsqrt",
        [](auto x) -> TestType {
          return static_cast<TestType>(1 / std::sqrt(static_cast<double>(x)));
        },
        static_cast<float>(std::numeric_limits<TestType>::epsilon()),
        SAT_RIGHT_LIM, Saturation::USE_SAT_ZERO, InputType::scalar,
        rsqrt_margin);
  }
  SECTION("--sat-no-limits") {
    test_math<TestType>(
        -1, -1, "cm_rsqrt",
        [](auto x) -> TestType {
          return static_cast<TestType>(1 / std::sqrt(static_cast<double>(x)));
        },
        static_cast<float>(std::numeric_limits<TestType>::epsilon()),
        static_cast<float>(std::numeric_limits<TestType>::max()),
        Saturation::USE_SAT_NOLIMITS, InputType::scalar, rsqrt_margin);
  }
}

TEMPLATE_TEST_CASE("cm_rsqrt(const)", "[cm][rsqrt]", float, half) {
  SECTION("--no-sat") {
    test_math<TestType>(
        -1, -1, "cm_rsqrt",
        [](auto x) -> TestType {
          return static_cast<TestType>(1 / std::sqrt(static_cast<double>(x)));
        },
        NO_SAT_LEFT_LIM, NO_SAT_RIGHT_LIM, Saturation::NO_SAT,
        InputType::constant, rsqrt_margin);
  }
  SECTION("--sat-near-zero") {
    test_math<TestType>(
        -1, -1, "cm_rsqrt",
        [](auto x) -> TestType {
          return static_cast<TestType>(1 / std::sqrt(static_cast<double>(x)));
        },
        static_cast<float>(std::numeric_limits<TestType>::epsilon()),
        SAT_RIGHT_LIM, Saturation::USE_SAT_ZERO, InputType::constant,
        rsqrt_margin);
  }
  SECTION("--sat-no-limits") {
    test_math<TestType>(
        -1, -1, "cm_rsqrt",
        [](auto x) -> TestType {
          return static_cast<TestType>(1 / std::sqrt(static_cast<double>(x)));
        },
        static_cast<float>(std::numeric_limits<TestType>::epsilon()),
        static_cast<float>(std::numeric_limits<TestType>::max()),
        Saturation::USE_SAT_NOLIMITS, InputType::constant, rsqrt_margin);
  }
}