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

using signed_types = std::tuple<int8_t, int16_t, int32_t, int64_t>;
using unsigned_types = std::tuple<uint8_t, uint16_t, uint32_t, uint64_t>;
using fp_types = std::tuple<ca::Half, float, double>;

using signed_cases_t =
    typename ca::CartesianProduct<signed_types, signed_types>::type;
using unsigned_cases_t =
    typename ca::CartesianProduct<unsigned_types, unsigned_types>::type;
using fp_cases_t = typename ca::CartesianProduct<fp_types, fp_types>::type;

using cases_t = decltype(std::tuple_cat(std::declval<signed_cases_t>(),
                                        std::declval<unsigned_cases_t>(),
                                        std::declval<fp_cases_t>()));

TEMPLATE_LIST_TEST_CASE("cm_add", "[cm][intrinsics][arithmetic]", cases_t) {
  using src0_t = std::tuple_element_t<0, TestType>;
  using src1_t = std::tuple_element_t<1, TestType>;
  using dst_t =
      std::conditional_t<sizeof(src0_t) < sizeof(src1_t), src0_t, src1_t>;

  using test_case_t = TestCase<BinaryIntrinsicSource, dst_t, src0_t, src1_t>;

  const TestConfig &config = get_test_config();

  ca::Requirements requirements;
  requirements.arithmetic_type<dst_t>();
  requirements.arithmetic_type<src0_t>();
  requirements.arithmetic_type<src1_t>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  auto reference = [](src0_t a, src1_t b) { return dst_t(a + b); };
  auto reference_sat = [](src0_t a, src1_t b) {
    constexpr bool is_fp = ca::is_floating_point_v<dst_t>;
    constexpr bool is_unsigned = std::is_unsigned_v<dst_t>;

    constexpr auto max = std::numeric_limits<dst_t>::max();
    constexpr auto min = std::numeric_limits<dst_t>::min();

    if constexpr (is_fp) {
      const dst_t result(a + b);
      if (result > 1.0) {
        return dst_t(1.0);
      }
      if (result < 0.0) {
        return dst_t(0.0);
      }
      return result;
    } else if constexpr (is_unsigned) {
      if (max - b < a) {
        return max;
      }
    } else {
      if (b > 0 && max - b < a) {
        return max;
      }
      if (b < 0 && min - b > a) {
        return min;
      }
    }
    return dst_t(a + b);
  };

  auto is_overflow = [](src0_t s0, src1_t s1) {
    if constexpr (std::is_unsigned_v<dst_t>) {
      return false;
    }
    return (s1 > 0 && std::numeric_limits<dst_t>::max() - s1 < s0) ||
           (s1 < 0 && std::numeric_limits<dst_t>::lowest() + -s1 > s0);
  };
  auto is_overflow_sat = [](auto /*unused*/, auto /*unused*/) { return false; };

  std::function<src0_t()> g0;
  if constexpr (ca::is_floating_point_v<src0_t>) {
    g0 = []() { return ca::generate_value<src0_t>(src0_t(-1), src0_t(1), 0); };
  } else {
    g0 = []() {
      constexpr auto max = std::numeric_limits<dst_t>::max();
      constexpr auto min = std::numeric_limits<dst_t>::min();
      return ca::generate_value<src0_t>(src0_t(min), src0_t(max), 0);
    };
  }

  std::function<src1_t()> g1;
  if constexpr (ca::is_floating_point_v<src1_t>) {
    g1 = []() { return ca::generate_value<src1_t>(src1_t(-1), src1_t(1), 0); };
  } else {
    g1 = []() {
      constexpr auto max = std::numeric_limits<dst_t>::max();
      constexpr auto min = std::numeric_limits<dst_t>::min();
      return ca::generate_value<src1_t>(src1_t(min), src1_t(max), 0);
    };
  }

  SECTION("cm_add(Vector, Vector)") {
    VectorGenerator<src0_t, src1_t, dst_t> gen(reference, is_overflow, g0, g1);
    test_case_t(config.runtime(), config.program_type(), "cm_add", gen,
                OperandType::vector, {OperandType::vector, OperandType::vector},
                {"-DFLAGS=0"})
        .check();
  }

  SECTION("cm_add(Vector, Vector, SAT)") {
    VectorGenerator<src0_t, src1_t, dst_t> gen(reference_sat, is_overflow_sat,
                                               g0, g1);
    test_case_t(config.runtime(), config.program_type(), "cm_add", gen,
                OperandType::vector, {OperandType::vector, OperandType::vector},
                {"-DFLAGS=SAT"})
        .check();
  }

  {
    VectorScalarGenerator<false, src0_t, src1_t, dst_t> gen(
        reference, is_overflow, g0, g1);
    VectorScalarGenerator<false, src0_t, src1_t, dst_t> gen_sat(
        reference_sat, is_overflow_sat, g0, g1);

    SECTION("cm_add(Vector, Scalar)") {
      test_case_t(config.runtime(), config.program_type(), "cm_add", gen,
                  OperandType::vector,
                  {OperandType::vector, OperandType::scalar}, {"-DFLAGS=0"})
          .check();
    }

    SECTION("cm_add(Vector, Scalar, SAT)") {
      test_case_t(config.runtime(), config.program_type(), "cm_add", gen_sat,
                  OperandType::vector,
                  {OperandType::vector, OperandType::scalar}, {"-DFLAGS=SAT"})
          .check();
    }

    SECTION("cm_add(Vector, Constant)") {
      test_case_t(config.runtime(), config.program_type(), "cm_add", gen,
                  OperandType::vector,
                  {OperandType::vector, OperandType::constant}, {"-DFLAGS=0"})
          .check();
    }

    SECTION("cm_add(Vector, Constant, SAT)") {
      test_case_t(config.runtime(), config.program_type(), "cm_add", gen_sat,
                  OperandType::vector,
                  {OperandType::vector, OperandType::constant}, {"-DFLAGS=SAT"})
          .check();
    }
  }

  {
    VectorScalarGenerator<true, src0_t, src1_t, dst_t> gen(reference,
                                                           is_overflow, g0, g1);
    VectorScalarGenerator<true, src0_t, src1_t, dst_t> gen_sat(
        reference_sat, is_overflow_sat, g0, g1);

    SECTION("cm_add(Scalar, Vector)") {
      test_case_t(config.runtime(), config.program_type(), "cm_add", gen,
                  OperandType::vector,
                  {OperandType::scalar, OperandType::vector}, {"-DFLAGS=0"})
          .check();
    }

    SECTION("cm_add(Scalar, Vector, SAT)") {
      test_case_t(config.runtime(), config.program_type(), "cm_add", gen_sat,
                  OperandType::vector,
                  {OperandType::scalar, OperandType::vector}, {"-DFLAGS=SAT"})
          .check();
    }

    SECTION("Constant + Vector") {
      test_case_t(config.runtime(), config.program_type(), "cm_add", gen,
                  OperandType::vector,
                  {OperandType::constant, OperandType::vector}, {"-DFLAGS=0"})
          .check();
    }

    SECTION("cm_add(Constant, Vector, SAT)") {
      test_case_t(config.runtime(), config.program_type(), "cm_add", gen_sat,
                  OperandType::vector,
                  {OperandType::constant, OperandType::vector}, {"-DFLAGS=SAT"})
          .check();
    }
  }
}

} // namespace
