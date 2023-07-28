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

template <typename T>
const T exp_margin = std::numeric_limits<T>::epsilon() * 4;

TEMPLATE_TEST_CASE("cm_exp(vector)", "[cm][exp]", float, half) {
  SECTION("--no-sat") {
    test_math<TestType>(
        16, 1, "cm_exp",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::pow(2, static_cast<double>(x)));
        },
        NO_SAT_LEFT_LIM, NO_SAT_RIGHT_LIM, Saturation::NO_SAT,
        InputType::vector, exp_margin<TestType>);
  }
  SECTION("--sat-near-zero") {
    test_math<TestType>(
        16, 1, "cm_exp",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::pow(2, static_cast<double>(x)));
        },
        SAT_LEFT_LIM, SAT_RIGHT_LIM, Saturation::USE_SAT_ZERO,
        InputType::vector, exp_margin<TestType>);
  }
  SECTION("--sat-no-limits") {
    test_math<TestType>(
        16, 1, "cm_exp",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::pow(2, static_cast<double>(x)));
        },
        static_cast<float>(std::numeric_limits<TestType>::min()),
        static_cast<float>(std::numeric_limits<TestType>::max()),
        Saturation::USE_SAT_NOLIMITS, InputType::vector, exp_margin<TestType>);
  }
}

TEMPLATE_TEST_CASE("cm_exp(matrix)", "[cm][exp]", float, half) {
  SECTION("--no-sat") {
    test_math<TestType>(
        2, 8, "cm_exp",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::pow(2, static_cast<double>(x)));
        },
        NO_SAT_LEFT_LIM, NO_SAT_RIGHT_LIM, Saturation::NO_SAT,
        InputType::matrix, exp_margin<TestType>);
  }
  SECTION("--sat-near-zero") {
    test_math<TestType>(
        2, 8, "cm_exp",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::pow(2, static_cast<double>(x)));
        },
        SAT_LEFT_LIM, SAT_RIGHT_LIM, Saturation::USE_SAT_ZERO,
        InputType::matrix, exp_margin<TestType>);
  }
  SECTION("--sat-no-limits") {
    test_math<TestType>(
        2, 8, "cm_exp",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::pow(2, static_cast<double>(x)));
        },
        static_cast<float>(std::numeric_limits<TestType>::min()),
        static_cast<float>(std::numeric_limits<TestType>::max()),
        Saturation::USE_SAT_NOLIMITS, InputType::matrix, exp_margin<TestType>);
  }
}

TEMPLATE_TEST_CASE("cm_exp(scalar)", "[cm][exp]", float, half) {
  SECTION("--no-sat") {
    test_math<TestType>(
        -1, -1, "cm_exp",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::pow(2, static_cast<double>(x)));
        },
        NO_SAT_LEFT_LIM, NO_SAT_RIGHT_LIM, Saturation::NO_SAT,
        InputType::scalar, exp_margin<TestType>);
  }
  SECTION("--sat-near-zero") {
    test_math<TestType>(
        -1, -1, "cm_exp",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::pow(2, static_cast<double>(x)));
        },
        SAT_LEFT_LIM, SAT_RIGHT_LIM, Saturation::USE_SAT_ZERO,
        InputType::scalar, exp_margin<TestType>);
  }
  SECTION("--sat-no-limits") {
    test_math<TestType>(
        -1, -1, "cm_exp",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::pow(2, static_cast<double>(x)));
        },
        static_cast<float>(std::numeric_limits<TestType>::min()),
        static_cast<float>(std::numeric_limits<TestType>::max()),
        Saturation::USE_SAT_NOLIMITS, InputType::scalar, exp_margin<TestType>);
  }
}

TEMPLATE_TEST_CASE("cm_exp(const)", "[cm][exp]", float, half) {
  SECTION("--no-sat") {
    test_math<TestType>(
        -1, -1, "cm_exp",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::pow(2, static_cast<double>(x)));
        },
        NO_SAT_LEFT_LIM, NO_SAT_RIGHT_LIM, Saturation::NO_SAT,
        InputType::constant, exp_margin<TestType>);
  }
  SECTION("--sat-near-zero") {
    test_math<TestType>(
        -1, -1, "cm_exp",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::pow(2, static_cast<double>(x)));
        },
        SAT_LEFT_LIM, SAT_RIGHT_LIM, Saturation::USE_SAT_ZERO,
        InputType::constant, exp_margin<TestType>);
  }
  SECTION("--sat-no-limits") {
    test_math<TestType>(
        -1, -1, "cm_exp",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::pow(2, static_cast<double>(x)));
        },
        static_cast<float>(std::numeric_limits<TestType>::min()),
        static_cast<float>(std::numeric_limits<TestType>::max()),
        Saturation::USE_SAT_NOLIMITS, InputType::constant,
        exp_margin<TestType>);
  }
}