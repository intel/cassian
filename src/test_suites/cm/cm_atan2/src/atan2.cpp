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
#include <cassian/reference/dp4a.hpp>
#include <cassian/utility/utility.hpp>

#include <common.hpp>

constexpr double atan2_margin = 0.00001;

TEMPLATE_TEST_CASE("cm_atan2(vector)", "", float) {
  SECTION("--no-sat") {
    test_atan2<TestType>(
        16, 1, "cmtl::cm_atan2",
        [](auto x, auto y) -> TestType {
          return static_cast<TestType>(
              std::atan2(static_cast<double>(x), static_cast<double>(y)));
        },
        -NO_SAT_RIGHT_LIM, NO_SAT_RIGHT_LIM, Saturation::NO_SAT,
        InputType::vector, atan2_margin);
  }
  SECTION("--sat-near-zero") {
    test_atan2<TestType>(
        16, 1, "cmtl::cm_atan2",
        [](auto x, auto y) -> TestType {
          return static_cast<TestType>(
              std::atan2(static_cast<double>(x), static_cast<double>(y)));
        },
        SAT_LEFT_LIM, SAT_RIGHT_LIM, Saturation::USE_SAT_ZERO,
        InputType::vector, atan2_margin);
  }
  SECTION("--sat-no-limits") {
    test_atan2<TestType>(
        16, 1, "cmtl::cm_atan2",
        [](auto x, auto y) -> TestType {
          return static_cast<TestType>(
              std::atan2(static_cast<double>(x), static_cast<double>(y)));
        },
        static_cast<float>(std::numeric_limits<TestType>::min()),
        static_cast<float>(std::numeric_limits<TestType>::max()),
        Saturation::USE_SAT_NOLIMITS, InputType::vector, atan2_margin);
  }
}

TEMPLATE_TEST_CASE("cm_atan2(vector_ref)", "", float) {
  SECTION("--no-sat") {
    test_atan2<TestType>(
        16, 1, "cmtl::cm_atan2",
        [](auto x, auto y) -> TestType {
          return static_cast<TestType>(
              std::atan2(static_cast<double>(x), static_cast<double>(y)));
        },
        -NO_SAT_RIGHT_LIM, NO_SAT_RIGHT_LIM, Saturation::NO_SAT,
        InputType::vector_ref, atan2_margin);
  }
  SECTION("--sat-near-zero") {
    test_atan2<TestType>(
        16, 1, "cmtl::cm_atan2",
        [](auto x, auto y) -> TestType {
          return static_cast<TestType>(
              std::atan2(static_cast<double>(x), static_cast<double>(y)));
        },
        SAT_LEFT_LIM, SAT_RIGHT_LIM, Saturation::USE_SAT_ZERO,
        InputType::vector_ref, atan2_margin);
  }
  SECTION("--sat-no-limits") {
    test_atan2<TestType>(
        16, 1, "cmtl::cm_atan2",
        [](auto x, auto y) -> TestType {
          return static_cast<TestType>(
              std::atan2(static_cast<double>(x), static_cast<double>(y)));
        },
        static_cast<float>(std::numeric_limits<TestType>::min()),
        static_cast<float>(std::numeric_limits<TestType>::max()),
        Saturation::USE_SAT_NOLIMITS, InputType::vector_ref, atan2_margin);
  }
}

TEMPLATE_TEST_CASE("cm_atan2(matrix)", "", float) {
  SECTION("--no-sat") {
    test_atan2<TestType>(
        2, 8, "cmtl::cm_atan2",
        [](auto x, auto y) -> TestType {
          return static_cast<TestType>(
              std::atan2(static_cast<double>(x), static_cast<double>(y)));
        },
        -NO_SAT_RIGHT_LIM, NO_SAT_RIGHT_LIM, Saturation::NO_SAT,
        InputType::matrix, atan2_margin);
  }
  SECTION("--sat-near-zero") {
    test_atan2<TestType>(
        2, 8, "cmtl::cm_atan2",
        [](auto x, auto y) -> TestType {
          return static_cast<TestType>(
              std::atan2(static_cast<double>(x), static_cast<double>(y)));
        },
        SAT_LEFT_LIM, SAT_RIGHT_LIM, Saturation::USE_SAT_ZERO,
        InputType::matrix, atan2_margin);
  }
  SECTION("--sat-no-limits") {
    test_atan2<TestType>(
        2, 8, "cmtl::cm_atan2",
        [](auto x, auto y) -> TestType {
          return static_cast<TestType>(
              std::atan2(static_cast<double>(x), static_cast<double>(y)));
        },
        static_cast<float>(std::numeric_limits<TestType>::min()),
        static_cast<float>(std::numeric_limits<TestType>::max()),
        Saturation::USE_SAT_NOLIMITS, InputType::matrix, atan2_margin);
  }
}

TEMPLATE_TEST_CASE("cm_atan2(matrix_ref)", "", float) {
  SECTION("--no-sat") {
    test_atan2<TestType>(
        2, 8, "cmtl::cm_atan2",
        [](auto x, auto y) -> TestType {
          return static_cast<TestType>(
              std::atan2(static_cast<double>(x), static_cast<double>(y)));
        },
        -NO_SAT_RIGHT_LIM, NO_SAT_RIGHT_LIM, Saturation::NO_SAT,
        InputType::matrix_ref, atan2_margin);
  }
  SECTION("--sat-near-zero") {
    test_atan2<TestType>(
        2, 8, "cmtl::cm_atan2",
        [](auto x, auto y) -> TestType {
          return static_cast<TestType>(
              std::atan2(static_cast<double>(x), static_cast<double>(y)));
        },
        SAT_LEFT_LIM, SAT_RIGHT_LIM, Saturation::USE_SAT_ZERO,
        InputType::matrix_ref, atan2_margin);
  }
  SECTION("--sat-no-limits") {
    test_atan2<TestType>(
        2, 8, "cmtl::cm_atan2",
        [](auto x, auto y) -> TestType {
          return static_cast<TestType>(
              std::atan2(static_cast<double>(x), static_cast<double>(y)));
        },
        static_cast<float>(std::numeric_limits<TestType>::min()),
        static_cast<float>(std::numeric_limits<TestType>::max()),
        Saturation::USE_SAT_NOLIMITS, InputType::matrix_ref, atan2_margin);
  }
}

TEMPLATE_TEST_CASE("cm_atan2(scalar)", "", float) {
  SECTION("--no-sat") {
    test_atan2<TestType>(
        -1, -1, "cmtl::cm_atan2",
        [](auto x, auto y) -> TestType {
          return static_cast<TestType>(
              std::atan2(static_cast<double>(x), static_cast<double>(y)));
        },
        -NO_SAT_RIGHT_LIM, NO_SAT_RIGHT_LIM, Saturation::NO_SAT,
        InputType::scalar, atan2_margin);
  }
  SECTION("--sat-near-zero") {
    test_atan2<TestType>(
        -1, -1, "cmtl::cm_atan2",
        [](auto x, auto y) -> TestType {
          return static_cast<TestType>(
              std::atan2(static_cast<double>(x), static_cast<double>(y)));
        },
        SAT_LEFT_LIM, SAT_RIGHT_LIM, Saturation::USE_SAT_ZERO,
        InputType::scalar, atan2_margin);
  }
  SECTION("--sat-no-limits") {
    test_atan2<TestType>(
        -1, -1, "cmtl::cm_atan2",
        [](auto x, auto y) -> TestType {
          return static_cast<TestType>(
              std::atan2(static_cast<double>(x), static_cast<double>(y)));
        },
        static_cast<float>(std::numeric_limits<TestType>::min()),
        static_cast<float>(std::numeric_limits<TestType>::max()),
        Saturation::USE_SAT_NOLIMITS, InputType::scalar, atan2_margin);
  }
}

TEMPLATE_TEST_CASE("cm_atan2(const)", "", float) {
  SECTION("--no-sat") {
    test_atan2<TestType>(
        -1, -1, "cmtl::cm_atan2",
        [](auto x, auto y) -> TestType {
          return static_cast<TestType>(
              std::atan2(static_cast<double>(x), static_cast<double>(y)));
        },
        -NO_SAT_RIGHT_LIM, NO_SAT_RIGHT_LIM, Saturation::NO_SAT,
        InputType::constant, atan2_margin);
  }
  SECTION("--sat-near-zero") {
    test_atan2<TestType>(
        -1, -1, "cmtl::cm_atan2",
        [](auto x, auto y) -> TestType {
          return static_cast<TestType>(
              std::atan2(static_cast<double>(x), static_cast<double>(y)));
        },
        SAT_LEFT_LIM, SAT_RIGHT_LIM, Saturation::USE_SAT_ZERO,
        InputType::constant, atan2_margin);
  }
  SECTION("--sat-no-limits") {
    test_atan2<TestType>(
        -1, -1, "cmtl::cm_atan2",
        [](auto x, auto y) -> TestType {
          return static_cast<TestType>(
              std::atan2(static_cast<double>(x), static_cast<double>(y)));
        },
        static_cast<float>(std::numeric_limits<TestType>::min()),
        static_cast<float>(std::numeric_limits<TestType>::max()),
        Saturation::USE_SAT_NOLIMITS, InputType::constant, atan2_margin);
  }
}