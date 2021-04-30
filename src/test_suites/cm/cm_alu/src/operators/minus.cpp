/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/cli/cli.hpp>
#include <cassian/test_harness/test_harness.hpp>

#include <catch2/catch.hpp>

#include <common.hpp>
#include <test_config.hpp>

namespace ca = cassian;

namespace {

TEMPLATE_LIST_TEST_CASE("minus", "[cm][operators][unary]", arith_list_t) {
  using src_t = TestType;
  using dst_t = decltype(-std::declval<src_t>());

  using test_case_t = TestCase<UnarySource, dst_t, src_t>;

  const TestConfig &config = get_test_config();

  ca::Requirements requirements;
  requirements.arithmetic_type<dst_t>();
  requirements.arithmetic_type<src_t>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  auto reference = [](src_t a) { return -a; };
  auto is_overflow = [](src_t /*unused*/) { return false; };

  SECTION("Vector") {
    UnaryVectorGenerator<src_t> gen(reference, is_overflow);
    test_case_t(config.runtime(), config.program_type(), "-", gen,
                OperandType::vector, {OperandType::vector})
        .check();
  }

  auto scalar_gen =
      [](int size, std::vector<dst_t> &ref,
         std::tuple<std::variant<std::vector<src_t>, src_t>> &srcs,
         const std::array<OperandType, 1> &ops) {
        assert(ops[0] == OperandType::scalar ||
               ops[0] == OperandType::constant);

        auto src = ca::generate_value<src_t>(0);
        std::get<0>(srcs) = src;

        std::generate_n(std::back_inserter(ref), size,
                        [src]() { return -src; });

        return true;
      };

  SECTION("Scalar") {
    test_case_t(config.runtime(), config.program_type(), "-", scalar_gen,
                OperandType::vector, {OperandType::scalar})
        .check();
  }

  SECTION("Constant") {
    test_case_t(config.runtime(), config.program_type(), "-", scalar_gen,
                OperandType::vector, {OperandType::constant})
        .check();
  }
}

} // namespace
