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

using std::abs;

TEMPLATE_LIST_TEST_CASE("cm_abs", "[cm][intrinsics][unary]", arith_list_t) {
  using src_t = TestType;
  using dst_t = src_t;

  using test_case_t = TestCase<UnaryIntrinsicSource, dst_t, src_t>;

  const TestConfig &config = get_test_config();

  ca::Requirements requirements;
  requirements.arithmetic_type<dst_t>();
  requirements.arithmetic_type<src_t>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  auto is_overflow = [](src_t s) {
    return !ca::is_floating_point_v<src_t> && std::is_signed_v<src_t> &&
           s == std::numeric_limits<src_t>::lowest();
  };
  auto is_overflow_sat = [](src_t /*unused*/) { return false; };

  auto reference = [](src_t a) {
    if constexpr (std::is_unsigned_v<src_t>) {
      return a;
    } else {
      return abs(+a);
    }
  };

  auto reference_sat = [is_overflow](src_t a) {
    if constexpr (ca::is_floating_point_v<src_t>) {
      const auto r = abs(a);
      if (r > 1) {
        return src_t(1);
      }
      return r;
    } else if constexpr (std::is_unsigned_v<src_t>) {
      return a;
    } else {
      return is_overflow(a) ? std::numeric_limits<src_t>::max() : abs(+a);
    }
  };

  SECTION("cm_abs(Vector)") {
    UnaryVectorGenerator<src_t, dst_t> gen(reference, is_overflow);
    test_case_t(config.runtime(), config.program_type(), "cm_abs", gen,
                OperandType::vector, {OperandType::vector})
        .check();
  }

  SECTION("cm_abs(Vector, SAT)") {
    std::function<src_t()> g;
    if constexpr (ca::is_floating_point_v<src_t>) {
      g = []() { return ca::generate_value<src_t>(src_t(-1), src_t(2), 0); };
    } else if constexpr (std::is_signed_v<src_t>) {
      g = []() {
        return ca::generate_value<int>(0, 1, 0)
                   ? std::numeric_limits<src_t>::min()
                   : ca::generate_value<src_t>(0);
      };
    } else {
      g = []() { return ca::generate_value<src_t>(0); };
    }

    UnaryVectorGenerator<src_t, dst_t> gen(reference_sat, is_overflow_sat, g);
    test_case_t(config.runtime(), config.program_type(), "cm_abs", gen,
                OperandType::vector, {OperandType::vector}, {"-DFLAGS=SAT"})
        .check();
  }

  auto scalar_gen =
      [reference](int size, std::vector<dst_t> &ref,
                  std::tuple<std::variant<std::vector<src_t>, src_t>> &srcs,
                  const std::array<OperandType, 1> &ops) {
        assert(ops[0] == OperandType::scalar ||
               ops[0] == OperandType::constant);

        auto src = ca::generate_value<src_t>(0);
        std::get<0>(srcs) = src;

        std::fill_n(std::back_inserter(ref), size, reference(src));

        return true;
      };
  auto scalar_gen_sat =
      [reference_sat](int size, std::vector<dst_t> &ref,
                      std::tuple<std::variant<std::vector<src_t>, src_t>> &srcs,
                      const std::array<OperandType, 1> &ops) {
        assert(ops[0] == OperandType::scalar ||
               ops[0] == OperandType::constant);

        auto src = ca::generate_value<src_t>(0);
        std::get<0>(srcs) = src;

        std::fill_n(std::back_inserter(ref), size, reference_sat(src));

        return true;
      };

  SECTION("cm_abs(Scalar)") {
    test_case_t(config.runtime(), config.program_type(), "cm_abs", scalar_gen,
                OperandType::vector, {OperandType::scalar})
        .check();
  }

  SECTION("cm_abs(Scalar, SAT)") {
    test_case_t(config.runtime(), config.program_type(), "cm_abs",
                scalar_gen_sat, OperandType::vector, {OperandType::scalar},
                {"-DFLAGS=SAT"})
        .check();
  }

  SECTION("cm_abs(Constant)") {
    test_case_t(config.runtime(), config.program_type(), "cm_abs", scalar_gen,
                OperandType::vector, {OperandType::constant})
        .check();
  }

  SECTION("cm_abs(Constant, SAT)") {
    test_case_t(config.runtime(), config.program_type(), "cm_abs",
                scalar_gen_sat, OperandType::vector, {OperandType::constant},
                {"-DFLAGS=SAT"})
        .check();
  }

  if constexpr (!ca::is_floating_point_v<src_t> && std::is_signed_v<src_t>) {
    auto gen = [](int size, std::vector<dst_t> &ref,
                  std::tuple<std::variant<std::vector<src_t>, src_t>> &srcs,
                  const std::array<OperandType, 1> &ops) {
      assert(ops[0] == OperandType::scalar || ops[0] == OperandType::constant);

      auto src = std::numeric_limits<src_t>::min();
      std::get<0>(srcs) = src;

      std::fill_n(std::back_inserter(ref), size,
                  std::numeric_limits<src_t>::max());

      return true;
    };

    SECTION("cm_abs(Scalar, SAT) -- minimal") {
      test_case_t(config.runtime(), config.program_type(), "cm_abs", gen,
                  OperandType::vector, {OperandType::scalar}, {"-DFLAGS=SAT"})
          .check();
    }

    SECTION("cm_abs(Constant, SAT) -- minimal") {
      test_case_t(config.runtime(), config.program_type(), "cm_abs", gen,
                  OperandType::vector, {OperandType::constant}, {"-DFLAGS=SAT"})
          .check();
    }
  }
}

} // namespace
