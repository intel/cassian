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

constexpr double inv_margin = std::numeric_limits<float>::epsilon();

TEMPLATE_TEST_CASE("cm_inv(vector)", "[cm][inv]", float, half) {
  SECTION("--no-sat") {
    test_math<TestType>(
        16, 1, "cm_inv",
        [](auto x) -> TestType {
          return static_cast<TestType>(1 / static_cast<double>(x));
        },
        NO_SAT_LEFT_LIM, NO_SAT_RIGHT_LIM, Saturation::NO_SAT,
        InputType::vector, inv_margin);
  }
  SECTION("--sat-near-zero") {
    test_math<TestType>(
        16, 1, "cm_inv",
        [](auto x) -> TestType {
          return static_cast<TestType>(1 / static_cast<double>(x));
        },
        SAT_LEFT_LIM, SAT_RIGHT_LIM, Saturation::USE_SAT_ZERO,
        InputType::vector, inv_margin);
  }
  SECTION("--sat-no-limits") {
    test_math<TestType>(
        16, 1, "cm_inv",
        [](auto x) -> TestType {
          return static_cast<TestType>(1 / static_cast<double>(x));
        },
        static_cast<float>(std::numeric_limits<TestType>::min()),
        static_cast<float>(std::numeric_limits<TestType>::max()),
        Saturation::USE_SAT_NOLIMITS, InputType::vector, inv_margin);
  }
}

TEMPLATE_TEST_CASE("cm_inv(matrix)", "[cm][inv]", float, half) {
  SECTION("--no-sat") {
    test_math<TestType>(
        2, 8, "cm_inv",
        [](auto x) -> TestType {
          return static_cast<TestType>(1 / static_cast<double>(x));
        },
        NO_SAT_LEFT_LIM, NO_SAT_RIGHT_LIM, Saturation::NO_SAT,
        InputType::matrix, inv_margin);
  }
  SECTION("--sat-near-zero") {
    test_math<TestType>(
        2, 8, "cm_inv",
        [](auto x) -> TestType {
          return static_cast<TestType>(1 / static_cast<double>(x));
        },
        SAT_LEFT_LIM, SAT_RIGHT_LIM, Saturation::USE_SAT_ZERO,
        InputType::matrix, inv_margin);
  }
  SECTION("--sat-no-limits") {
    test_math<TestType>(
        2, 8, "cm_inv",
        [](auto x) -> TestType {
          return static_cast<TestType>(1 / static_cast<double>(x));
        },
        static_cast<float>(std::numeric_limits<TestType>::min()),
        static_cast<float>(std::numeric_limits<TestType>::max()),
        Saturation::USE_SAT_NOLIMITS, InputType::matrix, inv_margin);
  }
}

TEMPLATE_TEST_CASE("cm_inv(scalar)", "[cm][inv]", float, half) {
  SECTION("--no-sat") {
    test_math<TestType>(
        -1, -1, "cm_inv",
        [](auto x) -> TestType {
          return static_cast<TestType>(1 / static_cast<double>(x));
        },
        NO_SAT_LEFT_LIM, NO_SAT_RIGHT_LIM, Saturation::NO_SAT,
        InputType::scalar, inv_margin);
  }
  SECTION("--sat-near-zero") {
    test_math<TestType>(
        -1, -1, "cm_inv",
        [](auto x) -> TestType {
          return static_cast<TestType>(1 / static_cast<double>(x));
        },
        SAT_LEFT_LIM, SAT_RIGHT_LIM, Saturation::USE_SAT_ZERO,
        InputType::scalar, inv_margin);
  }
  SECTION("--sat-no-limits") {
    test_math<TestType>(
        -1, -1, "cm_inv",
        [](auto x) -> TestType {
          return static_cast<TestType>(1 / static_cast<double>(x));
        },
        static_cast<float>(std::numeric_limits<TestType>::min()),
        static_cast<float>(std::numeric_limits<TestType>::max()),
        Saturation::USE_SAT_NOLIMITS, InputType::scalar, inv_margin);
  }
}

TEMPLATE_TEST_CASE("cm_inv(const)", "[cm][inv]", float, half) {
  SECTION("--no-sat") {
    test_math<TestType>(
        -1, -1, "cm_inv",
        [](auto x) -> TestType {
          return static_cast<TestType>(1 / static_cast<double>(x));
        },
        NO_SAT_LEFT_LIM, NO_SAT_RIGHT_LIM, Saturation::NO_SAT,
        InputType::constant, inv_margin);
  }
  SECTION("--sat-near-zero") {
    test_math<TestType>(
        -1, -1, "cm_inv",
        [](auto x) -> TestType {
          return static_cast<TestType>(1 / static_cast<double>(x));
        },
        SAT_LEFT_LIM, SAT_RIGHT_LIM, Saturation::USE_SAT_ZERO,
        InputType::constant, inv_margin);
  }
  SECTION("--sat-no-limits") {
    test_math<TestType>(
        -1, -1, "cm_inv",
        [](auto x) -> TestType {
          return static_cast<TestType>(1 / static_cast<double>(x));
        },
        static_cast<float>(std::numeric_limits<TestType>::min()),
        static_cast<float>(std::numeric_limits<TestType>::max()),
        Saturation::USE_SAT_NOLIMITS, InputType::constant, inv_margin);
  }
}