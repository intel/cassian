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
#include <cassian/utility/utility.hpp>

#include <iterator>

#include <common.hpp>

namespace ca = cassian;
using ca::half;
using ca::test::FlagsBuilder;
using ca::test::Language;

template <typename TestType> void test_oword_write() {
  constexpr unsigned buffer_size = 1024;

  for (auto oword_number : {1, 2, 4, 8}) {
    const std::string source = cassian::load_text_file(
        cassian::get_asset("kernels/cm_oword_rw/write_oword_genx.cpp"));

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

template <typename TestType> void test_oword_read(Alignment align) {
  constexpr unsigned buffer_size = 1024;

  for (auto oword_number : {1, 2, 3, 4, 5, 6, 7, 8}) {
    const std::string source = cassian::load_text_file(
        cassian::get_asset("kernels/cm_oword_rw/read_oword_genx.cpp"));

    auto simd = oword_number * 16 / sizeof(TestType);

    auto data = ca::generate_vector<TestType>(buffer_size, 0);

    auto index = ca::generate_value<unsigned>(0, buffer_size - simd, 0);
    index = index &
            ((align == Alignment::NOT_ALIGNED) ? ~(16 / sizeof(TestType) - 1)
                                               : ~(8 / sizeof(TestType) - 1));

    std::vector<TestType> ref(simd, TestType(0));
    std::copy_n(data.begin() + index, simd, ref.begin());

    decltype(data) out(simd, TestType(0));

    ca::test::pass(index * sizeof(TestType));
    ca::test::input(data);
    ca::test::output(out, simd);

    ca::test::kernel("kernel", source,
                     FlagsBuilder(Language::cm)
                         .define("SIMD", std::to_string(simd))
                         .define("TYPE", ca::to_cm_string<TestType>())
                         .define(dump_alignment(align))
                         .str());

    REQUIRE_THAT(out, Catch::Equals(ref));
  }
}

TEMPLATE_TEST_CASE("cm_write_oword", "", half, float, double, uint8_t, int8_t,
                   uint16_t, int16_t, uint32_t, int32_t, int64_t, uint64_t) {
  test_oword_write<TestType>();
}

TEMPLATE_TEST_CASE("cm_read_oword", "", half, float, double, uint8_t, int8_t,
                   uint16_t, int16_t, uint32_t, int32_t, int64_t, uint64_t) {
  test_oword_read<TestType>(Alignment::DWALIGNED);
  test_oword_read<TestType>(Alignment::MODIFIED_DWALIGNED);
  test_oword_read<TestType>(Alignment::NOT_ALIGNED);
}