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

namespace ca = cassian;
using ca::test::FlagsBuilder;
using ca::test::Language;

template <typename T> T bfn(uint8_t op, T s0, T s1, T s2) {
  T mask = std::numeric_limits<T>::max();
  T ans = (((s0 | s1 | s2) | (mask * (1 & (op >> 0)))) &
           ((~s0 | s1 | s2) | (mask * (1 & (op >> 1)))) &
           ((s0 | ~s1 | s2) | (mask * (1 & (op >> 2)))) &
           ((~s0 | ~s1 | s2) | (mask * (1 & (op >> 3)))) &
           ((s0 | s1 | ~s2) | (mask * (1 & (op >> 4)))) &
           ((~s0 | s1 | ~s2) | (mask * (1 & (op >> 5)))) &
           ((s0 | ~s1 | ~s2) | (mask * (1 & (op >> 6)))) &
           ((~s0 | ~s1 | ~s2) | (mask * (1 & (op >> 7)))));

  return ans;
}

template <typename T>
std::vector<T> apply_bfn(uint8_t op, std::vector<T> &a, std::vector<T> &b,
                         std::vector<T> &c) {
  std::vector<T> ans(a.size());
  for (int i = 0; i <= a.size(); ++i) {
    ans[i] = bfn<T>(op, a[i], b[i], c[i]);
  }
  return ans;
}

TEMPLATE_TEST_CASE("cm_bfn(val, val, val)", "", uint16_t, uint32_t) {
  const std::string source = cassian::load_text_file(
      cassian::get_asset("kernels/cm_bfn/bfn_genx.cpp"));
  FlagsBuilder fb = FlagsBuilder(Language::cm);
  fb.define("OUT_TYPE", ca::to_cm_string<TestType>())
      .define("IN_TYPE", ca::to_cm_string<TestType>());

  auto in0_value = ca::generate_value<TestType>(0);
  auto in1_value = ca::generate_value<TestType>(0);
  auto in2_value = ca::generate_value<TestType>(0);

  std::vector<TestType> in0(8);
  std::vector<TestType> in1(8);
  std::vector<TestType> in2(8);

  std::fill(in0.begin(), in0.end(), in0_value);
  std::fill(in1.begin(), in1.end(), in1_value);
  std::fill(in2.begin(), in2.end(), in2_value);

  decltype(in0) out;
  ca::test::output(out, in0.size());

  SECTION("bfn(scalar, scalar, scalar)") {
    fb.define("INPUT_SCALAR");
    ca::test::pass(in0_value);
    ca::test::pass(in1_value);
    ca::test::pass(in2_value);

    for (uint16_t i = std::numeric_limits<uint8_t>::min();
         i <= std::numeric_limits<uint8_t>::max(); ++i) {
      DYNAMIC_SECTION("	Boolean Function 0x" << std::hex << i) {
        auto op = static_cast<uint8_t>(i);
        auto ref = apply_bfn<TestType>(op, in0, in1, in2);

        ca::test::kernel("kernel", source,
                         fb.define("OP_NUMBER", std::to_string(op)).str());

        REQUIRE_THAT(out, Catch::Equals(ref));
      }
    }
  }
  SECTION("bfn(const, const, const)") {
    fb.define("INPUT_CONST")
        .define("INPUT0", std::to_string(in0_value))
        .define("INPUT1", std::to_string(in1_value))
        .define("INPUT2", std::to_string(in2_value));

    for (uint16_t i = std::numeric_limits<uint8_t>::min();
         i <= std::numeric_limits<uint8_t>::max(); ++i) {
      DYNAMIC_SECTION("	Boolean Function 0x" << std::hex << i) {
        auto op = static_cast<uint8_t>(i);
        auto ref = apply_bfn<TestType>(op, in0, in1, in2);

        ca::test::kernel("kernel", source,
                         fb.define("OP_NUMBER", std::to_string(op)).str());

        REQUIRE_THAT(out, Catch::Equals(ref));
      }
    }
  }
}

TEMPLATE_TEST_CASE("cm_bfn", "", uint16_t, uint32_t) {
  const std::string source = cassian::load_text_file(
      cassian::get_asset("kernels/cm_bfn/bfn_genx.cpp"));
  constexpr auto simd = 16;
  FlagsBuilder fb = FlagsBuilder(Language::cm);
  fb.define("OUT_TYPE", ca::to_cm_string<TestType>())
      .define("IN_TYPE", ca::to_cm_string<TestType>())
      .define("INPUT_VECTOR")
      .define("SIMD", std::to_string(simd));

  auto in0 = ca::generate_vector<TestType>(simd, 0);
  auto in1 = ca::generate_vector<TestType>(simd, 0);
  auto in2 = ca::generate_vector<TestType>(simd, 0);

  decltype(in0) out;

  ca::test::output(out, in0.size());
  ca::test::input(in0);
  ca::test::input(in1);
  ca::test::input(in2);

  SECTION("bfn(vec, vec, vec)") {
    fb.define("INPUT0_VEC").define("INPUT1_VEC").define("INPUT2_VEC");

    for (uint16_t i = std::numeric_limits<uint8_t>::min();
         i <= std::numeric_limits<uint8_t>::max(); ++i) {
      DYNAMIC_SECTION("	Boolean Function 0x" << std::hex << i) {
        auto op = static_cast<uint8_t>(i);
        auto ref = apply_bfn<TestType>(op, in0, in1, in2);
        ca::test::kernel("kernel", source,
                         fb.define("OP_NUMBER", std::to_string(op)).str());
        REQUIRE_THAT(out, Catch::Equals(ref));
      }
    }
  }
  SECTION("bfn(vec_ref, vec, vec") {
    fb.define("INPUT0_VEC_REF").define("INPUT1_VEC").define("INPUT2_VEC");

    for (uint16_t i = std::numeric_limits<uint8_t>::min();
         i <= std::numeric_limits<uint8_t>::max(); ++i) {
      DYNAMIC_SECTION("	Boolean Function 0x" << std::hex << i) {
        auto op = static_cast<uint8_t>(i);
        auto ref = apply_bfn<TestType>(op, in0, in1, in2);
        ca::test::kernel("kernel", source,
                         fb.define("OP_NUMBER", std::to_string(op)).str());
        REQUIRE_THAT(out, Catch::Equals(ref));
      }
    }
  }
  SECTION("bfn(vec, vec_ref, vec") {
    fb.define("INPUT0_VEC").define("INPUT1_VEC_REF").define("INPUT2_VEC");

    for (uint16_t i = std::numeric_limits<uint8_t>::min();
         i <= std::numeric_limits<uint8_t>::max(); ++i) {
      DYNAMIC_SECTION("	Boolean Function 0x" << std::hex << i) {
        auto op = static_cast<uint8_t>(i);
        auto ref = apply_bfn<TestType>(op, in0, in1, in2);
        ca::test::kernel("kernel", source,
                         fb.define("OP_NUMBER", std::to_string(op)).str());
        REQUIRE_THAT(out, Catch::Equals(ref));
      }
    }
  }
  SECTION("bfn(vec_ref, vec_ref, vec") {
    fb.define("INPUT0_VEC_REF").define("INPUT1_VEC_REF").define("INPUT2_VEC");

    for (uint16_t i = std::numeric_limits<uint8_t>::min();
         i <= std::numeric_limits<uint8_t>::max(); ++i) {
      DYNAMIC_SECTION("	Boolean Function 0x" << std::hex << i) {
        auto op = static_cast<uint8_t>(i);
        auto ref = apply_bfn<TestType>(op, in0, in1, in2);
        ca::test::kernel("kernel", source,
                         fb.define("OP_NUMBER", std::to_string(op)).str());
        REQUIRE_THAT(out, Catch::Equals(ref));
      }
    }
  }
  SECTION("bfn(vec, vec, vec_ref") {
    fb.define("INPUT0_VEC").define("INPUT1_VEC").define("INPUT2_VEC_REF");

    for (uint16_t i = std::numeric_limits<uint8_t>::min();
         i <= std::numeric_limits<uint8_t>::max(); ++i) {
      DYNAMIC_SECTION("	Boolean Function 0x" << std::hex << i) {
        auto op = static_cast<uint8_t>(i);
        auto ref = apply_bfn<TestType>(op, in0, in1, in2);
        ca::test::kernel("kernel", source,
                         fb.define("OP_NUMBER", std::to_string(op)).str());
        REQUIRE_THAT(out, Catch::Equals(ref));
      }
    }
  }
  SECTION("bfn(vec_ref, vec, vec_ref") {
    fb.define("INPUT0_VEC_REF").define("INPUT1_VEC").define("INPUT2_VEC_REF");

    for (uint16_t i = std::numeric_limits<uint8_t>::min();
         i <= std::numeric_limits<uint8_t>::max(); ++i) {
      DYNAMIC_SECTION("	Boolean Function 0x" << std::hex << i) {
        auto op = static_cast<uint8_t>(i);
        auto ref = apply_bfn<TestType>(op, in0, in1, in2);
        ca::test::kernel("kernel", source,
                         fb.define("OP_NUMBER", std::to_string(op)).str());
        REQUIRE_THAT(out, Catch::Equals(ref));
      }
    }
  }
  SECTION("bfn(vec, vec_ref, vec_ref") {
    fb.define("INPUT0_VEC").define("INPUT1_VEC_REF").define("INPUT2_VEC_REF");

    for (uint16_t i = std::numeric_limits<uint8_t>::min();
         i <= std::numeric_limits<uint8_t>::max(); ++i) {
      DYNAMIC_SECTION("	Boolean Function 0x" << std::hex << i) {
        auto op = static_cast<uint8_t>(i);
        auto ref = apply_bfn<TestType>(op, in0, in1, in2);
        ca::test::kernel("kernel", source,
                         fb.define("OP_NUMBER", std::to_string(op)).str());
        REQUIRE_THAT(out, Catch::Equals(ref));
      }
    }
  }
  SECTION("bfn(vec_ref, vec_ref, vec_ref") {
    fb.define("INPUT0_VEC_REF")
        .define("INPUT1_VEC_REF")
        .define("INPUT2_VEC_REF");

    for (uint16_t i = std::numeric_limits<uint8_t>::min();
         i <= std::numeric_limits<uint8_t>::max(); ++i) {
      DYNAMIC_SECTION("	Boolean Function 0x" << std::hex << i) {
        auto op = static_cast<uint8_t>(i);
        auto ref = apply_bfn<TestType>(op, in0, in1, in2);
        ca::test::kernel("kernel", source,
                         fb.define("OP_NUMBER", std::to_string(op)).str());
        REQUIRE_THAT(out, Catch::Equals(ref));
      }
    }
  }
}