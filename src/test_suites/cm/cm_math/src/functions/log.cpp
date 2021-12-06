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

constexpr double log_margin = std::numeric_limits<float>::epsilon() * 100;

TEMPLATE_TEST_CASE("cm_log(vector)", "[cm][log]", float, half) {
  SECTION("--no-sat") {
    test_math<TestType>(
        16, 1, "cm_log",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::log(static_cast<double>(x)));
        },
        NO_SAT_LEFT_LIM, NO_SAT_RIGHT_LIM, Saturation::NO_SAT,
        InputType::vector, log_margin);
  }
  SECTION("--sat-near-zero") {
    test_math<TestType>(
        16, 1, "cm_log",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::log(static_cast<double>(x)));
        },
        static_cast<float>(std::numeric_limits<TestType>::epsilon()),
        SAT_RIGHT_LIM, Saturation::USE_SAT_ZERO, InputType::vector, log_margin);
  }
  SECTION("--sat-no-limits") {
    test_math<TestType>(
        16, 1, "cm_log",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::log(static_cast<double>(x)));
        },
        static_cast<float>(std::numeric_limits<TestType>::min()),
        static_cast<float>(std::numeric_limits<TestType>::max()),
        Saturation::USE_SAT_NOLIMITS, InputType::vector, log_margin);
  }
}

TEMPLATE_TEST_CASE("cm_log(matrix)", "[cm][log]", float, half) {
  SECTION("--no-sat") {
    test_math<TestType>(
        2, 8, "cm_log",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::log(static_cast<double>(x)));
        },
        NO_SAT_LEFT_LIM, NO_SAT_RIGHT_LIM, Saturation::NO_SAT,
        InputType::matrix, log_margin);
  }
  SECTION("--sat-near-zero") {
    test_math<TestType>(
        2, 8, "cm_log",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::log(static_cast<double>(x)));
        },
        static_cast<float>(std::numeric_limits<TestType>::epsilon()),
        SAT_RIGHT_LIM, Saturation::USE_SAT_ZERO, InputType::matrix, log_margin);
  }
  SECTION("--sat-no-limits") {
    test_math<TestType>(
        2, 8, "cm_log",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::log(static_cast<double>(x)));
        },
        static_cast<float>(std::numeric_limits<TestType>::min()),
        static_cast<float>(std::numeric_limits<TestType>::max()),
        Saturation::USE_SAT_NOLIMITS, InputType::matrix, log_margin);
  }
}

TEMPLATE_TEST_CASE("cm_log(scalar)", "[cm][log]", float, half) {
  SECTION("--no-sat") {
    test_math<TestType>(
        -1, -1, "cm_log",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::log(static_cast<double>(x)));
        },
        NO_SAT_LEFT_LIM, NO_SAT_RIGHT_LIM, Saturation::NO_SAT,
        InputType::scalar, log_margin);
  }
  SECTION("--sat-near-zero") {
    test_math<TestType>(
        -1, -1, "cm_log",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::log(static_cast<double>(x)));
        },
        static_cast<float>(std::numeric_limits<TestType>::epsilon()),
        SAT_RIGHT_LIM, Saturation::USE_SAT_ZERO, InputType::scalar, log_margin);
  }
  SECTION("--sat-no-limits") {
    test_math<TestType>(
        -1, -1, "cm_log",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::log(static_cast<double>(x)));
        },
        static_cast<float>(std::numeric_limits<TestType>::min()),
        static_cast<float>(std::numeric_limits<TestType>::max()),
        Saturation::USE_SAT_NOLIMITS, InputType::scalar, log_margin);
  }
}

TEMPLATE_TEST_CASE("cm_log(const)", "[cm][log]", float, half) {
  SECTION("--no-sat") {
    test_math<TestType>(
        -1, -1, "cm_log",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::log(static_cast<double>(x)));
        },
        NO_SAT_LEFT_LIM, NO_SAT_RIGHT_LIM, Saturation::NO_SAT,
        InputType::constant, log_margin);
  }
  SECTION("--sat-near-zero") {
    test_math<TestType>(
        -1, -1, "cm_log",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::log(static_cast<double>(x)));
        },
        static_cast<float>(std::numeric_limits<TestType>::epsilon()),
        SAT_RIGHT_LIM, Saturation::USE_SAT_ZERO, InputType::constant,
        log_margin);
  }
  SECTION("--sat-no-limits") {
    test_math<TestType>(
        -1, -1, "cm_log",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::log(static_cast<double>(x)));
        },
        static_cast<float>(std::numeric_limits<TestType>::min()),
        static_cast<float>(std::numeric_limits<TestType>::max()),
        Saturation::USE_SAT_NOLIMITS, InputType::constant, log_margin);
  }
}