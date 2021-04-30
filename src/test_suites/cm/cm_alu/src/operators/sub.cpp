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

TEMPLATE_LIST_TEST_CASE("sub", "[cm][operators][arithmetic]", cases_t) {
  using src0_t = std::tuple_element_t<0, TestType>;
  using src1_t = std::tuple_element_t<1, TestType>;
  using dst_t = decltype(std::declval<src0_t>() - std::declval<src1_t>());

  using test_case_t = TestCase<BinarySource, dst_t, src0_t, src1_t>;

  const TestConfig &config = get_test_config();

  ca::Requirements requirements;
  requirements.arithmetic_type<dst_t>();
  requirements.arithmetic_type<src0_t>();
  requirements.arithmetic_type<src1_t>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  auto reference = [](src0_t a, src1_t b) { return a - b; };
  auto is_overflow = [](src0_t s0, src1_t s1) {
    if constexpr (std::is_unsigned_v<dst_t>) {
      return false;
    }
    return (s1 < 0 && std::numeric_limits<dst_t>::max() - -s1 < s0) ||
           (s1 > 0 && std::numeric_limits<dst_t>::lowest() + s1 > s0);
  };

  std::function<src0_t()> g0;
  if constexpr (std::is_same_v<ca::Half, dst_t> &&
                (std::is_same_v<int32_t, src0_t> ||
                 std::is_same_v<int64_t, src0_t> ||
                 std::is_same_v<uint32_t, src0_t> ||
                 std::is_same_v<uint64_t, src0_t>)) {
    g0 = []() {
      return ca::generate_value<src0_t>(
          static_cast<src0_t>(std::numeric_limits<ca::Half>::lowest() *
                              std::is_signed_v<src0_t>),
          static_cast<src0_t>(std::numeric_limits<ca::Half>::max()), 0);
    };
  } else {
    g0 = []() { return ca::generate_value<src0_t>(0); };
  }

  std::function<src1_t()> g1;
  if constexpr (std::is_same_v<ca::Half, dst_t> &&
                (std::is_same_v<int32_t, src1_t> ||
                 std::is_same_v<int64_t, src1_t> ||
                 std::is_same_v<uint32_t, src1_t> ||
                 std::is_same_v<uint64_t, src1_t>)) {
    g1 = []() {
      return ca::generate_value<src1_t>(
          static_cast<src1_t>(std::numeric_limits<ca::Half>::lowest() *
                              std::is_signed_v<src1_t>),
          static_cast<src1_t>(std::numeric_limits<ca::Half>::max()), 0);
    };
  } else {
    g1 = []() { return ca::generate_value<src1_t>(0); };
  }

  SECTION("Vector - Vector") {
    VectorGenerator<src0_t, src1_t> gen(reference, is_overflow, g0, g1);
    test_case_t(config.runtime(), config.program_type(), "-", gen,
                OperandType::vector, {OperandType::vector, OperandType::vector})
        .check();
  }

  {
    VectorScalarGenerator<false, src0_t, src1_t> gen(reference, is_overflow, g0,
                                                     g1);

    SECTION("Vector - Scalar") {
      test_case_t(config.runtime(), config.program_type(), "-", gen,
                  OperandType::vector,
                  {OperandType::vector, OperandType::scalar})
          .check();
    }

    SECTION("Vector - Constant") {
      test_case_t(config.runtime(), config.program_type(), "-", gen,
                  OperandType::vector,
                  {OperandType::vector, OperandType::constant})
          .check();
    }
  }

  {
    VectorScalarGenerator<true, src0_t, src1_t> gen(reference, is_overflow, g0,
                                                    g1);
    SECTION("Scalar - Vector") {
      test_case_t(config.runtime(), config.program_type(), "-", gen,
                  OperandType::vector,
                  {OperandType::scalar, OperandType::vector})
          .check();
    }

    SECTION("Constant - Vector") {
      test_case_t(config.runtime(), config.program_type(), "-", gen,
                  OperandType::vector,
                  {OperandType::constant, OperandType::vector})
          .check();
    }
  }
}

} // namespace
