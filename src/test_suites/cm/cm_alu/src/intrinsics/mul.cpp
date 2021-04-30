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

using std::abs;
using std::sqrt;

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

template <typename T> T saturate(T value) {
  const auto min =
      ca::is_floating_point_v<T> ? T(0) : std::numeric_limits<T>::lowest();
  const auto max =
      ca::is_floating_point_v<T> ? T(1) : std::numeric_limits<T>::max();

  if (value < min) {
    return min;
  }

  if (value > max) {
    return max;
  }

  return T(value);
}

TEMPLATE_LIST_TEST_CASE("cm_mul", "[cm][intrinsics][arithmetic]", cases_t) {
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

  auto reference = [](src0_t a, src1_t b) { return dst_t(a * b); };
  auto reference_sat = [](src0_t a, src1_t b) {
    return saturate<dst_t>(dst_t(a * b));
  };

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

  auto g0_sqrt = []() {
    const dst_t lowest = std::numeric_limits<dst_t>::lowest();
    const dst_t max = std::numeric_limits<dst_t>::max();
    const auto from = lowest ? src0_t(-sqrt(-(lowest + 1))) : src0_t(0);
    const auto to = src0_t(sqrt(max));
    return ca::generate_value<src0_t>(from, to, 0);
  };
  auto g1_sqrt = []() {
    const dst_t lowest = std::numeric_limits<dst_t>::lowest();
    const dst_t max = std::numeric_limits<dst_t>::max();
    const auto from = lowest ? src1_t(-sqrt(-(lowest + 1))) : src1_t(0);
    const auto to = src1_t(sqrt(max));
    return ca::generate_value<src1_t>(from, to, 1);
  };

  SECTION("cm_mul(Vector, Vector)") {
    VectorGenerator<src0_t, src1_t, dst_t> gen(reference, is_overflow, g0_sqrt,
                                               g1_sqrt);
    test_case_t(config.runtime(), config.program_type(), "cm_mul", gen,
                OperandType::vector, {OperandType::vector, OperandType::vector},
                {"-DFLAGS=0"})
        .check();
  }

  if constexpr (ca::is_floating_point_v<dst_t>) {
    SECTION("cm_mul(Vector, Vector, SAT)") {
      VectorGenerator<src0_t, src1_t, dst_t> gen(reference_sat, is_overflow_sat,
                                                 g0, g1);
      test_case_t(config.runtime(), config.program_type(), "cm_mul", gen,
                  OperandType::vector,
                  {OperandType::vector, OperandType::vector}, {"-DFLAGS=SAT"})
          .check();
    }
  }

  {
    VectorScalarGenerator<false, src0_t, src1_t, dst_t> gen(
        reference, is_overflow, g0_sqrt, g1_sqrt);

    SECTION("cm_mul(Vector, Scalar)") {
      test_case_t(config.runtime(), config.program_type(), "cm_mul", gen,
                  OperandType::vector,
                  {OperandType::vector, OperandType::scalar}, {"-DFLAGS=0"})
          .check();
    }

    SECTION("cm_mul(Vector, Constant)") {
      test_case_t(config.runtime(), config.program_type(), "cm_mul", gen,
                  OperandType::vector,
                  {OperandType::vector, OperandType::constant}, {"-DFLAGS=0"})
          .check();
    }

    if constexpr (ca::is_floating_point_v<dst_t>) {
      VectorScalarGenerator<false, src0_t, src1_t, dst_t> gen_sat(
          reference_sat, is_overflow_sat, g0, g1);

      SECTION("cm_mul(Vector, Scalar, SAT)") {
        test_case_t(config.runtime(), config.program_type(), "cm_mul", gen_sat,
                    OperandType::vector,
                    {OperandType::vector, OperandType::scalar}, {"-DFLAGS=SAT"})
            .check();
      }

      SECTION("cm_mul(Vector, Constant, SAT)") {
        test_case_t(config.runtime(), config.program_type(), "cm_mul", gen_sat,
                    OperandType::vector,
                    {OperandType::vector, OperandType::constant},
                    {"-DFLAGS=SAT"})
            .check();
      }
    }
  }

  {
    VectorScalarGenerator<true, src0_t, src1_t, dst_t> gen(
        reference, is_overflow, g0_sqrt, g1_sqrt);

    SECTION("cm_mul(Scalar, Vector)") {
      test_case_t(config.runtime(), config.program_type(), "cm_mul", gen,
                  OperandType::vector,
                  {OperandType::scalar, OperandType::vector}, {"-DFLAGS=0"})
          .check();
    }

    SECTION("Constant + Vector") {
      test_case_t(config.runtime(), config.program_type(), "cm_mul", gen,
                  OperandType::vector,
                  {OperandType::constant, OperandType::vector}, {"-DFLAGS=0"})
          .check();
    }

    if constexpr (ca::is_floating_point_v<dst_t>) {
      VectorScalarGenerator<true, src0_t, src1_t, dst_t> gen_sat(
          reference_sat, is_overflow_sat, g0, g1);

      SECTION("cm_mul(Scalar, Vector, SAT)") {
        test_case_t(config.runtime(), config.program_type(), "cm_mul", gen_sat,
                    OperandType::vector,
                    {OperandType::scalar, OperandType::vector}, {"-DFLAGS=SAT"})
            .check();
      }

      SECTION("cm_mul(Constant, Vector, SAT)") {
        test_case_t(config.runtime(), config.program_type(), "cm_mul", gen_sat,
                    OperandType::vector,
                    {OperandType::constant, OperandType::vector},
                    {"-DFLAGS=SAT"})
            .check();
      }
    }
  }
}

} // namespace
