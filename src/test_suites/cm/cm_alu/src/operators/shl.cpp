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

using cases_t =
    typename ca::CartesianProduct<bitwise_list_t, bitwise_list_t>::type;

TEMPLATE_LIST_TEST_CASE("shift left", "[cm][operators][shift]",
                        bitwise_list_t) {
  using src0_t = TestType;
  using src1_t = int;
  using dst_t = decltype(std::declval<src0_t>() << std::declval<src1_t>());

  using test_case_t = TestCase<BinarySource, dst_t, src0_t, src1_t>;

  const TestConfig &config = get_test_config();

  ca::Requirements requirements;
  requirements.arithmetic_type<dst_t>();
  requirements.arithmetic_type<src0_t>();
  requirements.arithmetic_type<src1_t>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  auto reference = [](src0_t a, src1_t b) { return a << b; };
  auto is_overflow = [](src0_t /*unused*/, src1_t /*unused*/) { return false; };
  auto gen0 = []() { return ca::generate_value<src0_t>(0); };
  auto gen1 = []() {
    return ca::generate_value<src1_t>(
        1, std::numeric_limits<src0_t>::digits - 1, 0);
  };

  SECTION("Vector << Vector") {
    VectorGenerator<src0_t, src1_t> gen(reference, is_overflow, gen0, gen1);
    test_case_t(config.runtime(), config.program_type(), "<<", gen,
                OperandType::vector, {OperandType::vector, OperandType::vector})
        .check();
  }

  {
    VectorScalarGenerator<false, src0_t, src1_t> gen(reference, is_overflow,
                                                     gen0, gen1);

    SECTION("Vector << Scalar") {
      test_case_t(config.runtime(), config.program_type(), "<<", gen,
                  OperandType::vector,
                  {OperandType::vector, OperandType::scalar})
          .check();
    }

    SECTION("Vector << Constant") {
      test_case_t(config.runtime(), config.program_type(), "<<", gen,
                  OperandType::vector,
                  {OperandType::vector, OperandType::constant})
          .check();
    }
  }

  {
    VectorScalarGenerator<true, src0_t, src1_t> gen(reference, is_overflow,
                                                    gen0, gen1);
    SECTION("Scalar << Vector") {
      test_case_t(config.runtime(), config.program_type(), "<<", gen,
                  OperandType::vector,
                  {OperandType::scalar, OperandType::vector})
          .check();
    }

    SECTION("Constant << Vector") {
      test_case_t(config.runtime(), config.program_type(), "<<", gen,
                  OperandType::vector,
                  {OperandType::constant, OperandType::vector})
          .check();
    }
  }
}

} // namespace
