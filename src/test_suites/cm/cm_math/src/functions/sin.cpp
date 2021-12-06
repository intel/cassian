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

constexpr double sin_margin = 0.0008;

TEMPLATE_TEST_CASE("cm_sin(vector)", "[cm][sin]", float, half) {
  SECTION("--no-sat") {
    test_math<TestType>(
        16, 1, "cm_sin",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::sin(static_cast<double>(x)));
        },
        NO_SAT_LEFT_LIM *pi, NO_SAT_RIGHT_LIM *pi, Saturation::NO_SAT,
        InputType::vector, sin_margin);
  }
  SECTION("--sat-near-zero") {
    test_math<TestType>(
        16, 1, "cm_sin",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::sin(static_cast<double>(x)));
        },
        SAT_LEFT_LIM *pi, SAT_RIGHT_LIM *pi, Saturation::USE_SAT_ZERO,
        InputType::vector, sin_margin);
  }
  SECTION("--sat-no-limits") {
    test_math<TestType>(
        16, 1, "cm_sin",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::sin(static_cast<double>(x)));
        },
        -trig_lim *pi, trig_lim *pi, Saturation::USE_SAT_NOLIMITS,
        InputType::vector, sin_margin);
  }
}

TEMPLATE_TEST_CASE("cm_sin(matrix)", "[cm][sin]", float, half) {
  SECTION("--no-sat") {
    test_math<TestType>(
        2, 8, "cm_sin",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::sin(static_cast<double>(x)));
        },
        NO_SAT_LEFT_LIM *pi, NO_SAT_RIGHT_LIM *pi, Saturation::NO_SAT,
        InputType::matrix, sin_margin);
  }
  SECTION("--sat-near-zero") {
    test_math<TestType>(
        2, 8, "cm_sin",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::sin(static_cast<double>(x)));
        },
        SAT_LEFT_LIM *pi, SAT_RIGHT_LIM *pi, Saturation::USE_SAT_ZERO,
        InputType::matrix, sin_margin);
  }
  SECTION("--sat-no-limits") {
    test_math<TestType>(
        2, 8, "cm_sin",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::sin(static_cast<double>(x)));
        },
        -trig_lim *pi, trig_lim *pi, Saturation::USE_SAT_NOLIMITS,
        InputType::matrix, sin_margin);
  }
}

TEMPLATE_TEST_CASE("cm_sin(scalar)", "[cm][sin]", float, half) {
  SECTION("--no-sat") {
    test_math<TestType>(
        -1, -1, "cm_sin",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::sin(static_cast<double>(x)));
        },
        NO_SAT_LEFT_LIM *pi, NO_SAT_RIGHT_LIM *pi, Saturation::NO_SAT,
        InputType::scalar, sin_margin);
  }
  SECTION("--sat-near-zero") {
    test_math<TestType>(
        -1, -1, "cm_sin",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::sin(static_cast<double>(x)));
        },
        SAT_LEFT_LIM *pi, SAT_RIGHT_LIM *pi, Saturation::USE_SAT_ZERO,
        InputType::scalar, sin_margin);
  }
  SECTION("--sat-no-limits") {
    test_math<TestType>(
        -1, -1, "cm_sin",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::sin(static_cast<double>(x)));
        },
        -trig_lim *pi, trig_lim *pi, Saturation::USE_SAT_NOLIMITS,
        InputType::scalar, sin_margin);
  }
}

TEMPLATE_TEST_CASE("cm_sin(const)", "[cm][sin]", float, half) {
  SECTION("--no-sat") {
    test_math<TestType>(
        -1, -1, "cm_sin",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::sin(static_cast<double>(x)));
        },
        NO_SAT_LEFT_LIM *pi, NO_SAT_RIGHT_LIM *pi, Saturation::NO_SAT,
        InputType::constant, sin_margin);
  }
  SECTION("--sat-near-zero") {
    test_math<TestType>(
        -1, -1, "cm_sin",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::sin(static_cast<double>(x)));
        },
        SAT_LEFT_LIM *pi, SAT_RIGHT_LIM *pi, Saturation::USE_SAT_ZERO,
        InputType::constant, sin_margin);
  }
  SECTION("--sat-no-limits") {
    test_math<TestType>(
        -1, -1, "cm_sin",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::sin(static_cast<double>(x)));
        },
        -trig_lim *pi, trig_lim *pi, Saturation::USE_SAT_NOLIMITS,
        InputType::constant, sin_margin);
  }
}