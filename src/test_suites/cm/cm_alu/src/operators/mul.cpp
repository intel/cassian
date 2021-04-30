/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/cli/cli.hpp>
#include <cassian/test_harness/test_harness.hpp>

#include <common.hpp>
#include <test_config.hpp>

namespace ca = cassian;

namespace {

using cases_t = typename ca::CartesianProduct<arith_list_t, arith_list_t>::type;

using std::abs;
using std::sqrt;

TEMPLATE_LIST_TEST_CASE("mul", "[cm][operators][arithmetic]", cases_t) {
  using src0_t = std::tuple_element_t<0, TestType>;
  using src1_t = std::tuple_element_t<1, TestType>;
  using dst_t = decltype(std::declval<src0_t>() * std::declval<src1_t>());

  using test_case_t = TestCase<BinarySource, dst_t, src0_t, src1_t>;

  const TestConfig &config = get_test_config();

  ca::Requirements requirements;
  requirements.arithmetic_type<dst_t>();
  requirements.arithmetic_type<src0_t>();
  requirements.arithmetic_type<src1_t>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  auto reference = [](src0_t a, src1_t b) { return a * b; };
  auto is_overflow = [](src0_t s0, src1_t s1) {
    if constexpr (std::is_unsigned_v<dst_t>) {
      return false;
    }

    src0_t a0;
    if constexpr (std::is_unsigned_v<src0_t>) {
      a0 = s0;
    } else {
      a0 = abs(+s0);
    }

    src1_t a1;
    if constexpr (std::is_unsigned_v<src1_t>) {
      a1 = s1;
    } else {
      a1 = abs(+s1);
    }

    return s1 != 0 && a0 > std::numeric_limits<dst_t>::max() / a1;
  };

  const auto max =
      std::numeric_limits<dst_t>::max() /
      std::min(static_cast<dst_t>(std::numeric_limits<src0_t>::max()),
               static_cast<dst_t>(std::numeric_limits<src1_t>::max()));
  const auto min =
      std::numeric_limits<dst_t>::min() /
      std::min(static_cast<dst_t>(std::numeric_limits<src0_t>::max()),
               static_cast<dst_t>(std::numeric_limits<src1_t>::max()));

  auto src0_reduced = [min, max]() {
    auto from = static_cast<src0_t>(
        std::max(static_cast<dst_t>(std::numeric_limits<src0_t>::min()), min));
    auto to = static_cast<src0_t>(
        std::min(static_cast<dst_t>(std::numeric_limits<src0_t>::max()), max));
    return ca::generate_value<src0_t>(from, to, 0);
  };
  auto src1_reduced = [min, max]() {
    auto from = static_cast<src1_t>(
        std::max(static_cast<dst_t>(std::numeric_limits<src1_t>::min()), min));
    auto to = static_cast<src1_t>(
        std::min(static_cast<dst_t>(std::numeric_limits<src1_t>::max()), max));
    return ca::generate_value<src1_t>(from, to, 0);
  };

  auto src0_same = []() {
    const src0_t lowest =
        std::is_same_v<ca::Half, dst_t>
            ? static_cast<src0_t>(std::numeric_limits<ca::Half>::lowest() *
                                  std::is_signed_v<src0_t>)
            : std::numeric_limits<src0_t>::lowest();
    const src0_t max =
        std::is_same_v<ca::Half, dst_t>
            ? static_cast<src0_t>(std::numeric_limits<ca::Half>::max())
            : std::numeric_limits<src0_t>::max();
    const src0_t from = lowest ? -sqrt(-(lowest + 1)) : src0_t(0);
    const src0_t to = sqrt(max);
    return ca::generate_value<src0_t>(from, to, 0);
  };
  auto src1_same = []() {
    const src1_t lowest =
        std::is_same_v<ca::Half, dst_t>
            ? static_cast<src1_t>(std::numeric_limits<ca::Half>::lowest() *
                                  std::is_signed_v<src1_t>)
            : std::numeric_limits<src1_t>::lowest();
    const src1_t max =
        std::is_same_v<ca::Half, dst_t>
            ? static_cast<src1_t>(std::numeric_limits<ca::Half>::max())
            : std::numeric_limits<src1_t>::max();
    const src1_t from = lowest ? -sqrt(-(lowest + 1)) : src1_t(0);
    const src1_t to = sqrt(max);
    return ca::generate_value<src1_t>(from, to, 0);
  };

  auto src0_exp2 = []() {
    int sign = 1;
    int limit = std::min(std::numeric_limits<src0_t>::digits,
                         std::numeric_limits<src1_t>::digits) /
                2;
    int n = ca::generate_value<int>(1, limit, 0);
    return src0_t(sign * (1LL << n));
  };
  auto src1_exp2 = []() {
    int sign = 1;
    int limit = std::min(std::numeric_limits<src0_t>::digits,
                         std::numeric_limits<src1_t>::digits) /
                2;
    int n = ca::generate_value<int>(1, limit, 0);
    return src1_t(sign * (1LL << n));
  };

  SECTION("Vector * Vector") {
    VectorGenerator<src0_t, src1_t> gen_default(reference, is_overflow);
    VectorGenerator<src0_t, src1_t> gen_reduced(reference, is_overflow,
                                                src0_reduced, src1_reduced);
    VectorGenerator<src0_t, src1_t> gen_same(reference, is_overflow, src0_same,
                                             src1_same);

    auto &gen =
        std::is_same_v<src0_t, src1_t> || ca::is_floating_point_v<dst_t>
            ? gen_same
            : std::numeric_limits<dst_t>::max() >
                          static_cast<dst_t>(
                              std::numeric_limits<src0_t>::max()) &&
                      std::numeric_limits<dst_t>::max() >
                          static_cast<dst_t>(std::numeric_limits<src1_t>::max())
                  ? gen_default
                  : gen_reduced;

    test_case_t(config.runtime(), config.program_type(), "*", gen,
                OperandType::vector, {OperandType::vector, OperandType::vector})
        .check();
  }

  {
    VectorScalarGenerator<false, src0_t, src1_t> gen(reference, is_overflow,
                                                     src0_same, src1_same);

    SECTION("Vector * Scalar") {
      test_case_t(config.runtime(), config.program_type(), "*", gen,
                  OperandType::vector,
                  {OperandType::vector, OperandType::scalar})
          .check();
    }

    SECTION("Vector * Constant") {
      test_case_t(config.runtime(), config.program_type(), "*", gen,
                  OperandType::vector,
                  {OperandType::vector, OperandType::constant})
          .check();
    }
  }

  SECTION("Vector * exp2(Constant)") {
    VectorScalarGenerator<false, src0_t, src1_t> gen(reference, is_overflow,
                                                     src0_same, src1_exp2);
    test_case_t(config.runtime(), config.program_type(), "*", gen,
                OperandType::vector,
                {OperandType::vector, OperandType::constant})
        .check();
  }

  {
    VectorScalarGenerator<true, src0_t, src1_t> gen(reference, is_overflow,
                                                    src0_same, src1_same);

    SECTION("Scalar * Vector") {
      test_case_t(config.runtime(), config.program_type(), "*", gen,
                  OperandType::vector,
                  {OperandType::scalar, OperandType::vector})
          .check();
    }

    SECTION("Constant * Vector") {
      test_case_t(config.runtime(), config.program_type(), "*", gen,
                  OperandType::vector,
                  {OperandType::constant, OperandType::vector})
          .check();
    }
  }

  SECTION("exp2(Constant) * Vector") {
    VectorScalarGenerator<true, src0_t, src1_t> gen(reference, is_overflow,
                                                    src0_exp2, src1_same);
    test_case_t(config.runtime(), config.program_type(), "*", gen,
                OperandType::vector,
                {OperandType::constant, OperandType::vector})
        .check();
  }
}

} // namespace
