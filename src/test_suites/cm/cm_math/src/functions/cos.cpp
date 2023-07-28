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

template <typename T> const T cos_margin = static_cast<T>(.0008);

TEMPLATE_TEST_CASE("cm_cos(vector)", "[cm][cos]", float, half) {
  SECTION("--no-sat") {
    test_math<TestType>(
        16, 1, "cm_cos",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::cos(static_cast<double>(x)));
        },
        NO_SAT_LEFT_LIM *pi, NO_SAT_RIGHT_LIM *pi, Saturation::NO_SAT,
        InputType::vector, cos_margin<TestType>);
  }
  SECTION("--sat-near-zero") {
    test_math<TestType>(
        16, 1, "cm_cos",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::cos(static_cast<double>(x)));
        },
        SAT_LEFT_LIM *pi, SAT_RIGHT_LIM *pi, Saturation::USE_SAT_ZERO,
        InputType::vector, cos_margin<TestType>);
  }
  SECTION("--sat-no-limits") {
    test_math<TestType>(
        16, 1, "cm_cos",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::cos(static_cast<double>(x)));
        },
        -trig_lim *pi, trig_lim *pi, Saturation::USE_SAT_NOLIMITS,
        InputType::vector, cos_margin<TestType>);
  }
}

TEMPLATE_TEST_CASE("cm_cos(matrix)", "[cm][cos]", float, half) {
  SECTION("--no-sat") {
    test_math<TestType>(
        2, 8, "cm_cos",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::cos(static_cast<double>(x)));
        },
        NO_SAT_LEFT_LIM *pi, NO_SAT_RIGHT_LIM *pi, Saturation::NO_SAT,
        InputType::matrix, cos_margin<TestType>);
  }
  SECTION("--sat-near-zero") {
    test_math<TestType>(
        2, 8, "cm_cos",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::cos(static_cast<double>(x)));
        },
        SAT_LEFT_LIM *pi, SAT_RIGHT_LIM *pi, Saturation::USE_SAT_ZERO,
        InputType::matrix, cos_margin<TestType>);
  }
  SECTION("--sat-no-limits") {
    test_math<TestType>(
        2, 8, "cm_cos",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::cos(static_cast<double>(x)));
        },
        -trig_lim *pi, trig_lim *pi, Saturation::USE_SAT_NOLIMITS,
        InputType::matrix, cos_margin<TestType>);
  }
}

TEMPLATE_TEST_CASE("cm_cos(scalar)", "[cm][cos]", float, half) {
  SECTION("--no-sat") {
    test_math<TestType>(
        -1, -1, "cm_cos",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::cos(static_cast<double>(x)));
        },
        NO_SAT_LEFT_LIM *pi, NO_SAT_RIGHT_LIM *pi, Saturation::NO_SAT,
        InputType::scalar, cos_margin<TestType>);
  }
  SECTION("--sat-near-zero") {
    test_math<TestType>(
        -1, -1, "cm_cos",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::cos(static_cast<double>(x)));
        },
        SAT_LEFT_LIM *pi, SAT_RIGHT_LIM *pi, Saturation::USE_SAT_ZERO,
        InputType::scalar, cos_margin<TestType>);
  }
  SECTION("--sat-no-limits") {
    test_math<TestType>(
        -1, -1, "cm_cos",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::cos(static_cast<double>(x)));
        },
        -trig_lim *pi, trig_lim *pi, Saturation::USE_SAT_NOLIMITS,
        InputType::scalar, cos_margin<TestType>);
  }
}

TEMPLATE_TEST_CASE("cm_cos(const)", "[cm][cos]", float, half) {
  SECTION("--no-sat") {
    test_math<TestType>(
        -1, -1, "cm_cos",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::cos(static_cast<double>(x)));
        },
        NO_SAT_LEFT_LIM *pi, NO_SAT_RIGHT_LIM *pi, Saturation::NO_SAT,
        InputType::constant, cos_margin<TestType>);
  }
  SECTION("--sat-near-zero") {
    test_math<TestType>(
        -1, -1, "cm_cos",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::cos(static_cast<double>(x)));
        },
        SAT_LEFT_LIM *pi, SAT_RIGHT_LIM *pi, Saturation::USE_SAT_ZERO,
        InputType::constant, cos_margin<TestType>);
  }
  SECTION("--sat-no-limits") {
    test_math<TestType>(
        -1, -1, "cm_cos",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::cos(static_cast<double>(x)));
        },
        -trig_lim *pi, trig_lim *pi, Saturation::USE_SAT_NOLIMITS,
        InputType::constant, cos_margin<TestType>);
  }
}