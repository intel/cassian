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

namespace ca = cassian;
using ca::test::FlagsBuilder;
using ca::test::Language;

TEMPLATE_TEST_CASE("cm_dp4a", "", int32_t, uint32_t) {
  const std::string source = cassian::load_text_file(
      cassian::get_asset("kernels/cm_dp4a/dp4a_genx.cpp"));

  constexpr auto simd = 16;

  auto acc = ca::generate_vector<int32_t>(simd, 0);
  auto a = ca::generate_vector<TestType>(simd, 0);
  auto b = ca::generate_vector<TestType>(simd, 0);

  auto ref = ca::dp4a(a, b, acc);

  decltype(ref) out;

  ca::test::output(out, acc.size());
  ca::test::input(acc);
  ca::test::input(a);
  ca::test::input(b);

  ca::test::kernel("kernel", source,
                   FlagsBuilder(Language::cm)
                       .define("SIMD", std::to_string(simd))
                       .define("OUT_TYPE", ca::to_cm_string<TestType>())
                       .define("ACC_TYPE", ca::to_cm_string<TestType>())
                       .define("A_TYPE", ca::to_cm_string<TestType>())
                       .define("B_TYPE", ca::to_cm_string<TestType>())
                       .str());

  REQUIRE_THAT(out, Catch::Equals(ref));
}
