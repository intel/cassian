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
const T sqrt_margin = std::numeric_limits<T>::epsilon() * 4;

TEMPLATE_TEST_CASE("cm_sqrt(vector)", "[cm][sqrt]", float, half) {
  SECTION("--no-sat") {
    test_math<TestType>(
        16, 1, "cm_sqrt",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::sqrt(static_cast<double>(x)));
        },
        0, NO_SAT_RIGHT_LIM, Saturation::NO_SAT, InputType::vector,
        sqrt_margin<TestType>);
  }
  SECTION("--sat-near-zero") {
    test_math<TestType>(
        16, 1, "cm_sqrt",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::sqrt(static_cast<double>(x)));
        },
        0, SAT_RIGHT_LIM, Saturation::USE_SAT_ZERO, InputType::vector,
        sqrt_margin<TestType>);
  }
  SECTION("--sat-no-limits") {
    test_math<TestType>(
        16, 1, "cm_sqrt",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::sqrt(static_cast<double>(x)));
        },
        0, static_cast<float>(std::numeric_limits<TestType>::max()),
        Saturation::USE_SAT_NOLIMITS, InputType::vector, sqrt_margin<TestType>);
  }
}

TEMPLATE_TEST_CASE("cm_sqrt(matrix)", "[cm][sqrt]", float, half) {
  SECTION("--no-sat") {
    test_math<TestType>(
        2, 8, "cm_sqrt",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::sqrt(static_cast<double>(x)));
        },
        0, NO_SAT_RIGHT_LIM, Saturation::NO_SAT, InputType::matrix,
        sqrt_margin<TestType>);
  }
  SECTION("--sat-near-zero") {
    test_math<TestType>(
        2, 8, "cm_sqrt",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::sqrt(static_cast<double>(x)));
        },
        0, SAT_RIGHT_LIM, Saturation::USE_SAT_ZERO, InputType::matrix,
        sqrt_margin<TestType>);
  }
  SECTION("--sat-no-limits") {
    test_math<TestType>(
        2, 8, "cm_sqrt",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::sqrt(static_cast<double>(x)));
        },
        0, static_cast<float>(std::numeric_limits<TestType>::max()),
        Saturation::USE_SAT_NOLIMITS, InputType::matrix, sqrt_margin<TestType>);
  }
}

TEMPLATE_TEST_CASE("cm_sqrt(scalar)", "[cm][sqrt]", float, half) {
  SECTION("--no-sat") {
    test_math<TestType>(
        -1, -1, "cm_sqrt",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::sqrt(static_cast<double>(x)));
        },
        0, NO_SAT_RIGHT_LIM, Saturation::NO_SAT, InputType::scalar,
        sqrt_margin<TestType>);
  }
  SECTION("--sat-near-zero") {
    test_math<TestType>(
        -1, -1, "cm_sqrt",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::sqrt(static_cast<double>(x)));
        },
        0, SAT_RIGHT_LIM, Saturation::USE_SAT_ZERO, InputType::scalar,
        sqrt_margin<TestType>);
  }
  SECTION("--sat-no-limits") {
    test_math<TestType>(
        -1, -1, "cm_sqrt",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::sqrt(static_cast<double>(x)));
        },
        0, static_cast<float>(std::numeric_limits<TestType>::max()),
        Saturation::USE_SAT_NOLIMITS, InputType::scalar, sqrt_margin<TestType>);
  }
}

TEMPLATE_TEST_CASE("cm_sqrt(const)", "[cm][sqrt]", float, half) {
  SECTION("--no-sat") {
    test_math<TestType>(
        -1, -1, "cm_sqrt",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::sqrt(static_cast<double>(x)));
        },
        0, NO_SAT_RIGHT_LIM, Saturation::NO_SAT, InputType::constant,
        sqrt_margin<TestType>);
  }
  SECTION("--sat-near-zero") {
    test_math<TestType>(
        -1, -1, "cm_sqrt",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::sqrt(static_cast<double>(x)));
        },
        0, SAT_RIGHT_LIM, Saturation::USE_SAT_ZERO, InputType::constant,
        sqrt_margin<TestType>);
  }
  SECTION("--sat-no-limits") {
    test_math<TestType>(
        -1, -1, "cm_sqrt",
        [](auto x) -> TestType {
          return static_cast<TestType>(std::sqrt(static_cast<double>(x)));
        },
        0, static_cast<float>(std::numeric_limits<TestType>::max()),
        Saturation::USE_SAT_NOLIMITS, InputType::constant,
        sqrt_margin<TestType>);
  }
}