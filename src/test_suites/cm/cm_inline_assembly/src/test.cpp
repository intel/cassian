/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <catch2/catch.hpp>

#include <cassian/fp_types/half.hpp>
#include <cassian/main/flags_builder.hpp>
#include <cassian/main/test_helper.hpp>
#include <cassian/random/random.hpp>
#include <cassian/test_harness/test_harness.hpp>
#include <cassian/utility/utility.hpp>

namespace ca = cassian;
using ca::test::FlagsBuilder;
using ca::test::Language;

using ca::half;

TEST_CASE("cm_asm_a", "[cm][asm]") {
  const std::string source = cassian::load_text_file(
      cassian::get_asset("kernels/cm_inline_assembly/asm_a_genx.cpp"));

  std::vector<int> input = ca::generate_vector<int>(8, 0);
  std::vector<int> test;
  std::vector<int> etalon;

  ca::test::input(input);
  ca::test::output(test, input.size());
  ca::test::output(etalon, input.size());

  ca::test::kernel("test", source, FlagsBuilder(Language::cm).str());

  REQUIRE_THAT(test, Catch::Equals(etalon));
}

TEST_CASE("cm_asm_cr", "[cm][asm]") {
  const std::string source = cassian::load_text_file(
      cassian::get_asset("kernels/cm_inline_assembly/asm_cr_genx.cpp"));

  std::vector<int> input = ca::generate_vector<int>(8, 0);
  std::vector<int> test;
  std::vector<int> etalon;

  ca::test::input(input);
  ca::test::output(test, input.size());
  ca::test::output(etalon, input.size());

  ca::test::kernel("test", source, FlagsBuilder(Language::cm).str());

  REQUIRE_THAT(test, Catch::Equals(etalon));
}

TEMPLATE_TEST_CASE("cm_asm_f", "[cm][asm]", half, float, double) {
  const std::string source = cassian::load_text_file(
      cassian::get_asset("kernels/cm_inline_assembly/asm_f_genx.cpp"));

  ca::Requirements requirements;
  requirements.arithmetic_type<TestType>();
  if (ca::test::should_skip_test(requirements)) {
    return;
  }

  std::vector<TestType> test;
  std::vector<TestType> etalon;

  ca::test::output(test, 8);
  ca::test::output(etalon, 8);

  ca::test::kernel("test", source,
                   FlagsBuilder(Language::cm)
                       .define("DATA_TYPE", ca::to_cm_string<TestType>())
                       .str());

  REQUIRE_THAT(test, Catch::Equals(etalon));
}

TEST_CASE("cm_asm_immediate_i", "[cm][asm]") {
  const std::string source = cassian::load_text_file(cassian::get_asset(
      "kernels/cm_inline_assembly/asm_immediate_i_genx.cpp"));

  std::vector<int> test;
  std::vector<int> etalon;

  ca::test::output(test, 8);
  ca::test::output(etalon, 8);

  ca::test::kernel("test", source, FlagsBuilder(Language::cm).str());

  REQUIRE_THAT(test, Catch::Equals(etalon));
}

TEST_CASE("cm_asm_immediate_n", "[cm][asm]") {
  const std::string source = cassian::load_text_file(cassian::get_asset(
      "kernels/cm_inline_assembly/asm_immediate_n_genx.cpp"));

  std::vector<int> input = ca::generate_vector<int>(8, 0);
  std::vector<int> test;
  std::vector<int> etalon;

  ca::test::input(input);
  ca::test::output(test, 8);
  ca::test::output(etalon, 8);

  ca::test::kernel("test", source, FlagsBuilder(Language::cm).str());

  REQUIRE_THAT(test, Catch::Equals(etalon));
}

TEST_CASE("cm_asm_plus_mod", "[cm][asm]") {
  const std::string source = cassian::load_text_file(
      cassian::get_asset("kernels/cm_inline_assembly/asm_plus_mod_genx.cpp"));

  std::vector<int> in0 = ca::generate_vector<int>(8, 0);
  std::vector<int> in1 = ca::generate_vector<int>(8, 0);
  std::vector<int> in2 = ca::generate_vector<int>(8, 0);
  std::vector<int> out;

  std::vector<int> ref;

  for (size_t i = 0; i < in0.size(); i++) {
    ref.push_back(in0[i] + in1[i] - in2[i]);
  }

  ca::test::input(in0);
  ca::test::input(in1);
  ca::test::input(in2);
  ca::test::output(out, 8);

  ca::test::kernel("test", source, FlagsBuilder(Language::cm).str());

  REQUIRE_THAT(out, Catch::Equals(ref));
}

TEST_CASE("cm_asm_r_3", "[cm][asm]") {
  const std::string source = cassian::load_text_file(
      cassian::get_asset("kernels/cm_inline_assembly/asm_r_3_genx.cpp"));

  std::vector<int> in0 = ca::generate_vector<int>(8, 0);
  std::vector<int> in1 = ca::generate_vector<int>(8, 0);
  std::vector<int> out;

  std::vector<int> ref;

  std::transform(std::begin(in0), std::end(in0), std::begin(in1),
                 std::back_inserter(ref), [](auto a, auto b) { return a + b; });

  ca::test::input(in0);
  ca::test::input(in1);
  ca::test::output(out, 8);

  ca::test::kernel("test", source, FlagsBuilder(Language::cm).str());

  REQUIRE_THAT(out, Catch::Equals(ref));
}

TEST_CASE("cm_asm_r_4", "[cm][asm]") {
  const std::string source = cassian::load_text_file(
      cassian::get_asset("kernels/cm_inline_assembly/asm_r_4_genx.cpp"));

  std::vector<int> in0 = ca::generate_vector<int>(4, 0);
  std::vector<int> in1 = ca::generate_vector<int>(4, 0);
  std::vector<int> in2 = ca::generate_vector<int>(4, 0);
  std::vector<int> out;

  std::vector<int> ref;

  for (size_t i = 0; i < in0.size(); i++) {
    ref.push_back(in0[i] * in1[i] + in2[i]);
  }

  ca::test::input(in0);
  ca::test::input(in1);
  ca::test::input(in2);
  ca::test::output(out, 4);

  ca::test::kernel("test", source, FlagsBuilder(Language::cm).str());

  REQUIRE_THAT(out, Catch::Equals(ref));
}

TEST_CASE("cm_asm_rw_const_vector", "[cm][asm]") {
  const std::string source = cassian::load_text_file(cassian::get_asset(
      "kernels/cm_inline_assembly/asm_rw_const_vector_genx.cpp"));

  std::vector<int> test;
  std::vector<int> etalon;

  ca::test::output(test, 8);
  ca::test::output(etalon, 8);

  ca::test::kernel("test", source, FlagsBuilder(Language::cm).str());

  REQUIRE_THAT(test, Catch::Equals(etalon));
}

TEST_CASE("cm_asm_rw", "[cm][asm]") {
  const std::string source = cassian::load_text_file(
      cassian::get_asset("kernels/cm_inline_assembly/asm_rw_genx.cpp"));

  std::vector<int> input = ca::generate_vector<int>(8, 0);
  std::vector<int> test;
  std::vector<int> etalon;

  ca::test::input(input);
  ca::test::output(test, input.size());
  ca::test::output(etalon, input.size());

  ca::test::kernel("test", source, FlagsBuilder(Language::cm).str());

  REQUIRE_THAT(test, Catch::Equals(etalon));
}

TEST_CASE("cm_asm_struct_of_vec_phi", "[cm][asm]") {
  const std::string source = cassian::load_text_file(cassian::get_asset(
      "kernels/cm_inline_assembly/asm_struct_of_vec_phi_genx.cpp"));

  constexpr int max = 99;

  std::vector<int> a = ca::generate_vector<int>(16, 0, max, 0);
  std::vector<int> b = ca::generate_vector<int>(16, 0, max, 0);
  std::vector<int> a_out;
  std::vector<int> b_out;

  ca::test::input(a);
  ca::test::input(b);
  ca::test::output(a_out, a.size());
  ca::test::output(b_out, b.size());

  bool cond = a[0] != 0;

  std::vector<int> ref;
  std::transform(std::begin(a), std::end(a), std::begin(b),
                 std::back_inserter(ref),
                 [cond](auto x, auto y) { return cond ? x + y : x - y; });

  ca::test::kernel("test", source, FlagsBuilder(Language::cm).str());

  REQUIRE_THAT(a_out, Catch::Equals(ref));
  REQUIRE_THAT(b_out, Catch::Equals(ref));
}

TEST_CASE("cm_asm_struct_phi", "[cm][asm]") {
  const std::string source = cassian::load_text_file(
      cassian::get_asset("kernels/cm_inline_assembly/asm_struct_phi_genx.cpp"));

  constexpr int max = 99;
  std::vector<int> inp = ca::generate_vector<int>(4, 0, max, 0);
  std::vector<int> out;

  ca::test::input(inp);
  ca::test::output(out, 4);

  bool cond = inp[2] != 0;

  std::vector<int> ref(4);
  ref[0] = ref[1] = cond ? inp[0] + inp[1] : inp[0] - inp[1];

  ca::test::kernel("test", source, FlagsBuilder(Language::cm).str());

  REQUIRE_THAT(out, Catch::Equals(ref));
}

TEST_CASE("cm_asm_template_empty", "[cm][asm]") {
  const std::string source = cassian::load_text_file(cassian::get_asset(
      "kernels/cm_inline_assembly/asm_template_empty_genx.cpp"));

  std::vector<int> inp = ca::generate_vector<int>(8, 0);
  std::vector<int> out;
  std::vector<int> ref;

  std::transform(std::begin(inp), std::end(inp), std::back_inserter(ref),
                 [](auto v) { return v - 1; });

  ca::test::input(inp);
  ca::test::output(out, inp.size());

  ca::test::kernel("test", source, FlagsBuilder(Language::cm).str());

  REQUIRE_THAT(out, Catch::Equals(ref));
}

TEST_CASE("cm_asm_template_no_ops", "[cm][asm]") {
  const std::string source = cassian::load_text_file(cassian::get_asset(
      "kernels/cm_inline_assembly/asm_template_no_ops_genx.cpp"));

  std::vector<int> inp = ca::generate_vector<int>(8, 0);
  std::vector<int> out;
  std::vector<int> ref;

  std::transform(std::begin(inp), std::end(inp), std::back_inserter(ref),
                 [](auto v) { return v - 1; });

  ca::test::input(inp);
  ca::test::output(out, inp.size());

  ca::test::kernel("test", source, FlagsBuilder(Language::cm).str());

  REQUIRE_THAT(out, Catch::Equals(ref));
}

TEST_CASE("cm_asm_template_symbolic_names", "[cm][asm]") {
  const std::string source = cassian::load_text_file(cassian::get_asset(
      "kernels/cm_inline_assembly/asm_template_symbolic_names_genx.cpp"));

  std::vector<int> input = ca::generate_vector<int>(8, 0);
  std::vector<int> test;
  std::vector<int> etalon;

  ca::test::input(input);
  ca::test::output(test, input.size());
  ca::test::output(etalon, input.size());

  ca::test::kernel("test", source, FlagsBuilder(Language::cm).str());

  REQUIRE_THAT(test, Catch::Equals(etalon));
}

TEST_CASE("cm_asm_template_symbolic_names_mix", "[cm][asm]") {
  const std::string source = cassian::load_text_file(cassian::get_asset(
      "kernels/cm_inline_assembly/asm_template_symbolic_names_mix_genx.cpp"));

  std::vector<int> input = ca::generate_vector<int>(8, 0);
  std::vector<int> test;
  std::vector<int> etalon;

  ca::test::input(input);
  ca::test::output(test, input.size());
  ca::test::output(etalon, input.size());

  ca::test::kernel("test", source, FlagsBuilder(Language::cm).str());

  REQUIRE_THAT(test, Catch::Equals(etalon));
}

TEST_CASE("cm_asm_two_kernels", "[cm][asm]") {
  const std::string source = cassian::load_text_file(cassian::get_asset(
      "kernels/cm_inline_assembly/asm_two_kernels_genx.cpp"));

  std::vector<int> input = ca::generate_vector<int>(8, 0);
  std::vector<int> out_asm;
  std::vector<int> out;

  ca::test::input(input);
  ca::test::output(out_asm, input.size());

  ca::test::kernel("test_asm", source, FlagsBuilder(Language::cm).str());

  ca::test::input(input);
  ca::test::output(out, input.size());

  ca::test::kernel("test", source, FlagsBuilder(Language::cm).str());

  REQUIRE_THAT(out_asm, Catch::Equals(out));
}

TEST_CASE("cm_asm_unroll", "[cm][asm]") {
  const std::string source = cassian::load_text_file(
      cassian::get_asset("kernels/cm_inline_assembly/asm_unroll_genx.cpp"));

  std::vector<int> test;
  std::vector<int> etalon;

  ca::test::output(test, 8);
  ca::test::output(etalon, 8);

  ca::test::kernel("test", source, FlagsBuilder(Language::cm).str());

  REQUIRE_THAT(test, Catch::Equals(etalon));
}

TEST_CASE("cm_asm_addr1") {
  const std::string source = cassian::load_text_file(
      cassian::get_asset("kernels/cm_inline_assembly/asm_addr1_genx.cpp"));

  std::vector<int> out;
  std::vector<int> ref(8);

  for (int i = 0; i < 8; i += 2) {
    ref[i] = 11 + i;
    ref[i + 1] = 10 + i;
  }

  ca::test::output(out, 8);

  ca::test::kernel("test", source, FlagsBuilder(Language::cm).str());

  REQUIRE_THAT(out, Catch::Equals(ref));
}

TEST_CASE("cm_asm_addr2") {
  const std::string source = cassian::load_text_file(
      cassian::get_asset("kernels/cm_inline_assembly/asm_addr2_genx.cpp"));

  std::vector<int> out;
  std::vector<int> ref(8);

  for (int i = 0; i < 8; i += 2) {
    ref[i] = 11 + i;
    ref[i + 1] = 10 + i;
  }

  ca::test::output(out, 8);

  ca::test::kernel("test", source, FlagsBuilder(Language::cm).str());

  REQUIRE_THAT(out, Catch::Equals(ref));
}

TEST_CASE("cm_asm_cr_docs") {
  const std::string source = cassian::load_text_file(
      cassian::get_asset("kernels/cm_inline_assembly/asm_cr_docs_genx.cpp"));

  auto in0 = ca::generate_vector<int>(16, 0);
  auto in1 = ca::generate_vector<int>(16, 0);

  std::vector<uint16_t> pred;
  std::vector<int> out;

  std::transform(
      std::begin(in0), std::end(in0), std::begin(in1), std::begin(in1),
      [](auto a, auto b) { return ca::generate_value<int>(0, 1, 0) ? a : b; });

  std::vector<uint16_t> pred_ref;
  std::transform(std::begin(in0), std::end(in0), std::back_inserter(pred_ref),
                 [](int v) { return v & 1; });

  std::vector<int> out_ref;
  std::transform(std::begin(in0), std::end(in0), std::begin(in1),
                 std::back_inserter(out_ref),
                 [](auto a, auto b) { return a == b ? -1 : 0; });

  ca::test::input(in0);
  ca::test::input(in1);
  ca::test::output(out, 16);
  ca::test::output(pred, 16);

  ca::test::kernel("test", source, FlagsBuilder(Language::cm).str());

  REQUIRE_THAT(out, Catch::Equals(out_ref));
  REQUIRE_THAT(pred, Catch::Equals(pred_ref));
}

TEST_CASE("cm_asm_cr_simd") {
  const std::string source = cassian::load_text_file(
      cassian::get_asset("kernels/cm_inline_assembly/asm_cr_simd_genx.cpp"));

  auto in0 = ca::generate_vector<int>(8, 0);
  auto in1 = ca::generate_vector<int>(8, 0);

  std::vector<int> out;

  std::transform(
      std::begin(in0), std::end(in0), std::begin(in1), std::begin(in1),
      [](auto a, auto b) { return ca::generate_value<int>(0, 1, 0) ? a : b; });

  std::vector<int> ref;
  std::transform(std::begin(in0), std::end(in0), std::begin(in1),
                 std::back_inserter(ref),
                 [](auto a, auto b) { return a == b ? 0 : a + b; });

  ca::test::input(in0);
  ca::test::input(in1);
  ca::test::output(out, ref.size());

  ca::test::kernel("test", source, FlagsBuilder(Language::cm).str());

  REQUIRE_THAT(out, Catch::Equals(ref));
}

TEST_CASE("cm_asm_cr_src") {
  const std::string source = cassian::load_text_file(
      cassian::get_asset("kernels/cm_inline_assembly/asm_cr_src_genx.cpp"));

  auto inp = ca::generate_vector<int>(8, 0);

  std::vector<int> out;
  std::vector<int> ref(inp.size());

  ca::test::input(inp);
  ca::test::output(out, inp.size());

  ca::test::kernel("test", source, FlagsBuilder(Language::cm).str());

  // when pred is zero, result is undefined, so copying output for false pred
  std::transform(std::begin(inp), std::end(inp), std::begin(out),
                 std::begin(ref),
                 [](auto a, auto b) { return (a % 2) ? ~a : b; });

  REQUIRE_THAT(out, Catch::Equals(ref));
}

TEST_CASE("cm_asm_format") {
  const std::string source = cassian::load_text_file(
      cassian::get_asset("kernels/cm_inline_assembly/asm_format1_genx.cpp"));

  std::vector<int> a0;
  std::vector<int> a1;
  auto b = ca::generate_vector<int>(8, 0);
  auto c = ca::generate_vector<int>(8, 0);
  auto d = ca::generate_vector<unsigned>(8, 0, 1, 0);

  constexpr int simd = 32;
  std::vector<int16_t> e;
  auto f = ca::generate_vector<int16_t>(simd, 0);
  auto g = ca::generate_vector<int16_t>(simd, 0);
  auto h = ca::generate_vector<int16_t>(simd, 0, 1, 0);

  std::vector<int> aref(16);

  for (size_t i = 0; i < b.size(); i++) {
    if (d[i] != 0) {
      aref[i * 2] = b[i] ^ c[i];
    }
  }

  ca::test::input(b);
  ca::test::input(c);
  ca::test::input(d);
  ca::test::output(a0, aref.size());
  ca::test::output(a1, aref.size());
  ca::test::input(f);
  ca::test::input(g);
  ca::test::output(e, f.size());
  ca::test::input(h);

  ca::test::kernel("test", source, FlagsBuilder(Language::cm).str());

  REQUIRE_THAT(a0, Catch::Equals(aref));
  REQUIRE_THAT(a1, Catch::Equals(aref));
  REQUIRE_THAT(e, Catch::Equals(h));
}

TEST_CASE("cm_asm_global") {
  const std::string source = cassian::load_text_file(
      cassian::get_asset("kernels/cm_inline_assembly/asm_global_genx.cpp"));

  constexpr int simd = 32;
  auto m1 = ca::generate_vector<float>(simd, 0);
  auto v2 = ca::generate_vector<float>(4, 0);
  auto v3 = ca::generate_vector<float>(4, 0);

  std::vector<float> v3x;
  std::vector<float> out;

  std::vector<float> v3ref;
  std::vector<float> outref;

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      int index = i * 4 * 2 + j * 2;
      v3ref.push_back(m1[index]);
      outref.push_back(m1[index]);
    }
  }

  ca::test::output(v3x, v3ref.size());
  ca::test::output(out, outref.size());
  ca::test::input(m1);
  ca::test::input(v2);
  ca::test::input(v3);

  ca::test::kernel("test", source, FlagsBuilder(Language::cm).str());

  REQUIRE_THAT(out, Catch::Equals(outref));
  REQUIRE_THAT(v3x, Catch::Equals(v3ref));
}

TEST_CASE("cm_asm_imm_docs", "[cm][asm]") {
  const std::string source = cassian::load_text_file(
      cassian::get_asset("kernels/cm_inline_assembly/asm_imm_docs_genx.cpp"));

  auto src0 = ca::generate_vector<uint16_t>(8, 0);
  auto src1 = ca::generate_vector<uint16_t>(8, 0);
  auto src2 = ca::generate_vector<uint16_t>(8, 0);
  std::vector<uint16_t> dst;
  std::vector<uint16_t> ref;

  for (size_t i = 0; i < src0.size(); i++) {
    ref.push_back(src0[i] * src1[i] + src2[i]);
  }

  ca::test::input(src0);
  ca::test::input(src1);
  ca::test::input(src2);
  ca::test::output(dst, ref.size());

  ca::test::kernel("test", source, FlagsBuilder(Language::cm).str());

  REQUIRE_THAT(dst, Catch::Equals(ref));
}

TEST_CASE("cm_asm_matrix", "[cm][asm]") {
  const std::string source = cassian::load_text_file(
      cassian::get_asset("kernels/cm_inline_assembly/asm_matrix_genx.cpp"));

  auto inp = ca::generate_vector<uint16_t>(8 * 8, 0);
  std::vector<uint16_t> out;

  ca::test::input(inp);
  ca::test::output(out, inp.size());

  ca::test::kernel("test", source, FlagsBuilder(Language::cm).str());

  // only low half is defined, the other is undef
  auto ref = out;
  auto mid = std::begin(inp) + inp.size() / 2;
  auto it = std::transform(std::begin(inp), mid, std::begin(ref),
                           [](auto v) { return -v; });

  REQUIRE_THAT(out, Catch::Equals(ref));
}

TEST_CASE("cm_asm_plus_mod_1", "[cm][asm]") {
  const std::string source = cassian::load_text_file(
      cassian::get_asset("kernels/cm_inline_assembly/asm_plus_mod_1_genx.cpp"));

  auto in0 = ca::generate_vector<int>(8, 0);
  auto in1 = ca::generate_vector<int>(8, 0);
  auto in2 = ca::generate_vector<int>(8, 0);
  std::vector<int> out;
  std::vector<int> ref;

  for (size_t i = 0; i < in0.size(); i++) {
    ref.push_back(in0[i] + in1[i] + in2[i]);
  }

  ca::test::input(in0);
  ca::test::input(in1);
  ca::test::input(in2);
  ca::test::output(out, ref.size());

  ca::test::kernel("test", source, FlagsBuilder(Language::cm).str());

  REQUIRE_THAT(out, Catch::Equals(ref));
}

TEST_CASE("cm_asm_r_dst", "[cm][asm]") {
  const std::string source = cassian::load_text_file(
      cassian::get_asset("kernels/cm_inline_assembly/asm_r_dst_genx.cpp"));

  constexpr int simd = 32;
  auto inp = ca::generate_vector<uint8_t>(simd, 0);
  std::vector<uint8_t> out;
  std::vector<uint8_t> ref;

  auto it = std::transform(std::begin(inp), std::end(inp),
                           std::back_inserter(ref), [](auto v) { return -v; });

  ca::test::input(inp);
  ca::test::output(out, ref.size());

  ca::test::kernel("test", source, FlagsBuilder(Language::cm).str());

  REQUIRE_THAT(out, Catch::Equals(ref));
}

TEST_CASE("cm_asm_select", "[cm][asm]") {
  const std::string source = cassian::load_text_file(
      cassian::get_asset("kernels/cm_inline_assembly/asm_select_genx.cpp"));

  auto inp = ca::generate_vector<float>(8 * 4, 0);
  std::vector<float> out;
  std::vector<float> ref = {inp[0], inp[2], inp[8], inp[10]};

  ca::test::input(inp);
  ca::test::output(out, ref.size());

  ca::test::kernel("test", source, FlagsBuilder(Language::cm).str());

  REQUIRE_THAT(out, Catch::Equals(ref));
}
