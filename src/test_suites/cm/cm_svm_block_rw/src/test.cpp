/*
 * Copyright (C) 2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <catch2/catch.hpp>

#include <cassian/main/flags_builder.hpp>
#include <cassian/main/test_helper.hpp>
#include <cassian/random/random.hpp>
#include <cassian/test_harness/test_harness.hpp>
#include <cassian/utility/utility.hpp>

#include <iterator>

namespace ca = cassian;
using ca::half;
using ca::test::FlagsBuilder;
using ca::test::Language;

template <typename T> std::string dump_vector(std::vector<T> vec) {
  std::ostringstream oss;
  constexpr bool fp = cassian::is_floating_point_v<T>;
  constexpr bool uns = std::is_unsigned_v<T>;
  using out_t = std::conditional_t<fp, double,
                                   std::conditional_t<uns, uint64_t, int64_t>>;
  if (fp) {
    oss.precision(std::numeric_limits<out_t>::max_digits10);
  }
  std::ostream_iterator<out_t> out(oss, fp ? "," : uns ? "ull," : "ll,");
  std::vector<out_t> vec_d(vec.begin(), vec.end());
  std::copy(vec_d.begin(), vec_d.end(), out);
  return oss.str();
}

TEMPLATE_TEST_CASE("cm_write_svm_block", "[cm]", half, float, uint8_t, int8_t,
                   uint16_t, int16_t, uint32_t, int32_t, int64_t, uint64_t,
                   double) {
  constexpr unsigned buffer_size = 1024;

  ca::Requirements requirements;
  requirements.arithmetic_type<TestType>();
  if (ca::should_skip_test(requirements, *ca::test::runtime())) {
    return;
  }

  for (auto oword_number : {1, 2, 4, 8}) {
    const std::string source = cassian::load_text_file(
        cassian::get_asset("kernels/cm_svm_block_rw/write_svm_block_genx.cpp"));

    auto simd = oword_number * 16 / sizeof(TestType);

    auto data = ca::generate_vector<TestType>(simd, 0);

    auto ow_tt_ratio = 16 / sizeof(TestType);
    auto index = ca::generate_value<unsigned>(0, buffer_size - simd, 0) &
                 (~(ow_tt_ratio - 1));

    std::vector<TestType> ref(buffer_size, TestType(0));
    std::copy_n(data.begin(), simd, ref.begin() + index);

    decltype(data) out(buffer_size, TestType(0));

    ca::test::pass(index * sizeof(TestType));
    ca::test::input_output(out);

    ca::test::kernel("kernel", source,
                     FlagsBuilder(Language::cm)
                         .define("SIMD", std::to_string(simd))
                         .define("TYPE", ca::to_cm_string<TestType>())
                         .define("PASSED_VECTOR", dump_vector(data))
                         .str());

    REQUIRE_THAT(out, Catch::Equals(ref));
  }
}

template <typename TestType> void test_read_svm_block(bool is_unaligned) {
  constexpr unsigned buffer_size = 1024;

  ca::Requirements requirements;
  requirements.arithmetic_type<TestType>();
  if (ca::should_skip_test(requirements, *ca::test::runtime())) {
    return;
  }

  for (auto oword_number : {1, 2, 4, 8}) {
    const std::string source = cassian::load_text_file(cassian::get_asset(
        "kernels/cm_svm_block_rw/read_svm_block_aligned_genx.cpp"));

    auto simd = oword_number * 16 / sizeof(TestType);

    auto data = ca::generate_vector<TestType>(buffer_size, 0);

    auto index = ca::generate_value<unsigned>(0, buffer_size - simd, 0) &
                 (is_unaligned ? ~(4 / sizeof(TestType) - 1)
                               : ~(16 / sizeof(TestType) - 1));

    std::vector<TestType> ref(simd, TestType(0));
    std::copy_n(data.begin() + index, simd, ref.begin());

    decltype(ref) out(simd, TestType(0));

    ca::test::pass(index * sizeof(TestType));
    ca::test::input(data);
    ca::test::output(out);

    ca::test::kernel("kernel", source,
                     FlagsBuilder(Language::cm)
                         .define("SIMD", std::to_string(simd))
                         .define("TYPE", ca::to_cm_string<TestType>())
                         .define(is_unaligned ? "IS_UNALIGNED" : "IS_ALIGNED")
                         .str());

    REQUIRE_THAT(out, Catch::Equals(ref));
  }
}

TEMPLATE_TEST_CASE("cm_read_svm_block", "[cm]", half, float, double, uint8_t,
                   int8_t, uint16_t, int16_t, uint32_t, int32_t, int64_t,
                   uint64_t) {
  test_read_svm_block<TestType>(true);
  test_read_svm_block<TestType>(false);
}