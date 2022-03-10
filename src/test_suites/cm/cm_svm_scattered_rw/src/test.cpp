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
#include <numeric>

namespace ca = cassian;
using ca::half;
using ca::test::FlagsBuilder;
using ca::test::Language;

template <typename T> std::string dump_vector(std::vector<T> vec) {
  std::ostringstream oss;
  constexpr bool fp = ca::is_floating_point_v<T>;
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

template <typename TestType> void test_svm_scattered_write() {
  constexpr unsigned buffer_size = 1024;

  ca::Requirements requirements;
  requirements.arithmetic_type<TestType>();
  if (ca::should_skip_test(requirements, *ca::test::runtime())) {
    return;
  }

  for (auto simd : {8, 16, 32}) {
    const std::string source = cassian::load_text_file(cassian::get_asset(
        "kernels/cm_svm_scattered_rw/write_svm_scattered_genx.cpp"));

    auto data = ca::generate_vector<TestType>(simd, 0);
    std::vector<unsigned> indices;
    do {
      auto index = ca::generate_value<unsigned>(0, buffer_size - 1, 0);

      if (std::find_if(std::begin(indices), std::end(indices),
                       [index, simd](auto v) { return v == index; }) ==
          std::end(indices)) {
        indices.push_back(index);
      }
    } while (indices.size() < simd);

    std::vector<TestType> ref(buffer_size, TestType(0));
    for (int i = 0; i < simd; ++i) {
      ref[indices[i]] = data[i];
    }

    std::for_each(indices.begin(), indices.end(),
                  [](auto &x) { x *= sizeof(TestType); });

    decltype(data) out(buffer_size, TestType(0));

    ca::test::input(indices);
    ca::test::input_output(out);

    ca::test::kernel("kernel", source,
                     FlagsBuilder(Language::cm)
                         .define("VECTOR_INPUT")
                         .define("SIMD", std::to_string(simd))
                         .define("TYPE", ca::to_cm_string<TestType>())
                         .define("PASSED_VECTOR", dump_vector(data))
                         .str());

    REQUIRE_THAT(out, Catch::Equals(ref));
  }
}

template <typename TestType> void test_svm_scattered_read() {
  constexpr unsigned buffer_size = 1024;

  ca::Requirements requirements;
  requirements.arithmetic_type<TestType>();
  if (ca::should_skip_test(requirements, *ca::test::runtime())) {
    return;
  }

  for (auto simd : {8, 16, 32}) {
    const std::string source = cassian::load_text_file(cassian::get_asset(
        "kernels/cm_svm_scattered_rw/read_svm_scattered_genx.cpp"));

    auto data = ca::generate_vector<TestType>(buffer_size, 0);

    std::vector<unsigned> indices =
        ca::generate_vector<unsigned>(simd, 0, buffer_size - 1, 0);

    std::vector<TestType> ref(simd, TestType(0));
    for (int i = 0; i < simd; ++i) {
      ref[i] = data[indices[i]];
    }

    std::for_each(indices.begin(), indices.end(),
                  [](auto &x) { x *= sizeof(TestType); });

    decltype(data) out(simd, TestType(0));

    std::vector<unsigned> out_offset(simd);
    std::iota(out_offset.begin(), out_offset.end(), 0);

    ca::test::input(indices);
    ca::test::input(data);
    ca::test::output(out, simd);

    ca::test::kernel("kernel", source,
                     FlagsBuilder(Language::cm)
                         .define("SIMD", std::to_string(simd))
                         .define("TYPE", ca::to_cm_string<TestType>())
                         .str());

    CHECK_THAT(out, Catch::Equals(ref));
  }
}

TEMPLATE_TEST_CASE("cm_write_svm_scattered", "", half, float, uint8_t, int8_t,
                   uint16_t, int16_t, uint32_t, int32_t, int64_t, uint64_t,
                   double) {
  test_svm_scattered_write<TestType>();
}

TEMPLATE_TEST_CASE("cm_read_svm_scattered", "", half, float, uint8_t, int8_t,
                   uint16_t, int16_t, uint32_t, int32_t, int64_t, uint64_t,
                   double) {
  test_svm_scattered_read<TestType>();
}