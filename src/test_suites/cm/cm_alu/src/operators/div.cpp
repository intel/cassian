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

TEMPLATE_LIST_TEST_CASE("div", "[cm][operators][arithmetic]", cases_t) {
  using src0_t = std::tuple_element_t<0, TestType>;
  using src1_t = std::tuple_element_t<1, TestType>;
  using dst_t = decltype(std::declval<src0_t>() + std::declval<src1_t>());

  using test_case_t = TestCase<BinarySource, dst_t, src0_t, src1_t>;

  const TestConfig &config = get_test_config();

  ca::Requirements requirements;
  requirements.arithmetic_type<dst_t>();
  requirements.arithmetic_type<src0_t>();
  requirements.arithmetic_type<src1_t>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  const auto max0 = std::numeric_limits<src0_t>::max() - 1;
  const auto max1 = std::numeric_limits<src1_t>::max() - 1;
  const auto min =
      std::max(static_cast<dst_t>(std::numeric_limits<src0_t>::min()),
               static_cast<dst_t>(std::numeric_limits<src1_t>::min()));

  auto gen0 = [min, max0, max1]() {
    const src0_t max = max0 < max1 ? max0 : static_cast<src0_t>(max1);
    src0_t value;
    do {
      value = ca::generate_value<src0_t>(static_cast<src0_t>(min), max, 0);
    } while (value == 0);
    return value;
  };
  auto gen1 = [min, max0, max1]() {
    const src1_t max = max0 < max1 ? static_cast<src1_t>(max0) : max1;
    src1_t value;
    do {
      value = ca::generate_value<src1_t>(static_cast<src1_t>(min), max, 0);
    } while (value == 0);
    return value;
  };

  auto src1_exp2 = []() {
    int sign = 1; // std::is_unsigned_v<src1_t> || ca::generate_value<int>(0, 1,
                  // 0) ? 1 : -1;
    int limit = std::numeric_limits<src1_t>::digits / 2;
    int n = ca::generate_value<int>(1, limit, 0);
    return src1_t(sign * (1LL << n));
  };

  auto reference = [](src0_t a, src1_t b) { return a / b; };
  auto is_overflow = [](src0_t s0, src1_t s1) {
    if (s1 == 0) {
      return true;
    }
    if constexpr (std::is_unsigned_v<dst_t>) {
      return false;
    }
    return s0 == std::numeric_limits<dst_t>::lowest() && s1 == -1LL;
  };

  SECTION("Vector / Vector") {
    VectorGenerator<src0_t, src1_t> gen(reference, is_overflow, gen0, gen1);
    test_case_t(config.runtime(), config.program_type(), "/", gen,
                OperandType::vector, {OperandType::vector, OperandType::vector})
        .check();
  }

  {
    VectorScalarGenerator<false, src0_t, src1_t> gen(reference, is_overflow,
                                                     gen0, gen1);

    SECTION("Vector / Scalar") {
      test_case_t(config.runtime(), config.program_type(), "/", gen,
                  OperandType::vector,
                  {OperandType::vector, OperandType::scalar})
          .check();
    }

    SECTION("Vector / Constant") {
      test_case_t(config.runtime(), config.program_type(), "/", gen,
                  OperandType::vector,
                  {OperandType::vector, OperandType::constant})
          .check();
    }
  }

  {
    VectorScalarGenerator<true, src0_t, src1_t> gen(reference, is_overflow,
                                                    gen0, gen1);
    SECTION("Scalar / Vector") {
      test_case_t(config.runtime(), config.program_type(), "/", gen,
                  OperandType::vector,
                  {OperandType::scalar, OperandType::vector})
          .check();
    }

    SECTION("Constant / Vector") {
      test_case_t(config.runtime(), config.program_type(), "/", gen,
                  OperandType::vector,
                  {OperandType::constant, OperandType::vector})
          .check();
    }
  }

  SECTION("Vector / exp2(Constant)") {
    VectorScalarGenerator<false, src0_t, src1_t> gen(reference, is_overflow,
                                                     gen0, src1_exp2);
    test_case_t(config.runtime(), config.program_type(), "/", gen,
                OperandType::vector,
                {OperandType::vector, OperandType::constant})
        .check();
  }
}

} // namespace
