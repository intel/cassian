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
#include <numeric>

namespace ca = cassian;
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

template <typename TestType>
void test_scattered_and_scaled_read_vector(bool if_scaled) {
  constexpr unsigned buffer_size = 1024;

  for (auto simd : {1, 2, 4, 8, 16, 32}) {
    const std::string source = cassian::load_text_file(cassian::get_asset(
        "kernels/cm_scaled_scattered_rw/read_scaled_scattered_genx.cpp"));

    auto data = ca::generate_vector<TestType>(buffer_size, 0);

    std::vector<unsigned> indices =
        ca::generate_vector<unsigned>(simd, 0, buffer_size - 1, 0);
    auto global_offset = ca::generate_value<unsigned>(
        0, *std::min_element(indices.begin(), indices.end()), 0);
    std::for_each(indices.begin(), indices.end(),
                  [global_offset](auto &x) { x = x - global_offset; });

    std::vector<TestType> ref(simd, TestType(0));
    for (int i = 0; i < simd; ++i) {
      ref[i] = data[global_offset + indices[i]];
    }

    if (if_scaled) {
      std::for_each(indices.begin(), indices.end(),
                    [](auto &x) { x *= sizeof(TestType); });
      global_offset *= sizeof(TestType);
    }

    decltype(data) out(simd, TestType(0));

    std::vector<unsigned> out_offset(simd);
    std::iota(out_offset.begin(), out_offset.end(), 0);

    ca::test::pass(global_offset);
    ca::test::input(indices);
    ca::test::input(data);
    ca::test::input(out_offset);
    ca::test::output(out, simd);

    ca::test::kernel("kernel", source,
                     FlagsBuilder(Language::cm)
                         .define("SIMD", std::to_string(simd))
                         .define("TYPE", ca::to_cm_string<TestType>())
                         .define(if_scaled ? "SCALED_READ" : "READ")
                         .str());

    CHECK_THAT(out, Catch::Equals(ref));
  }
}

template <typename TestType>
void test_scattered_and_scaled_write_vector(bool if_scaled) {
  constexpr unsigned buffer_size = 1024;

  for (auto simd : {1, 2, 4, 8, 16, 32}) {
    const std::string source = cassian::load_text_file(cassian::get_asset(
        "kernels/cm_scaled_scattered_rw/write_scaled_scattered_genx.cpp"));

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

    auto global_offset = ca::generate_value<unsigned>(
        0, *std::min_element(indices.begin(), indices.end()), 0);
    std::for_each(indices.begin(), indices.end(),
                  [global_offset](auto &x) { x = x - global_offset; });

    std::vector<TestType> ref(buffer_size, TestType(0));
    for (int i = 0; i < simd; ++i) {
      ref[global_offset + indices[i]] = data[i];
    }

    if (if_scaled) {
      std::for_each(indices.begin(), indices.end(),
                    [](auto &x) { x *= sizeof(TestType); });
      global_offset *= sizeof(TestType);
    }

    decltype(data) out(buffer_size, TestType(0));

    ca::test::input(indices);
    ca::test::pass(global_offset);
    ca::test::input_output(out);

    ca::test::kernel("kernel", source,
                     FlagsBuilder(Language::cm)
                         .define("VECTOR_INPUT")
                         .define("SIMD", std::to_string(simd))
                         .define("TYPE", ca::to_cm_string<TestType>())
                         .define("PASSED_VECTOR", dump_vector(data))
                         .define(if_scaled ? "SCALED_WRITE" : "WRITE")
                         .str());

    REQUIRE_THAT(out, Catch::Equals(ref));
  }
}

template <typename TestType> void test_scattered_write_value() {
  constexpr unsigned buffer_size = 1024;

  const std::string source = cassian::load_text_file(cassian::get_asset(
      "kernels/cm_scaled_scattered_rw/write_scaled_scattered_genx.cpp"));

  auto data = ca::generate_value<TestType>(0);
  std::vector<unsigned> indices;

  auto index = ca::generate_value<unsigned>(0, buffer_size - 1, 0);
  auto global_offset = ca::generate_value<unsigned>(0, index, 0);
  index = index - global_offset;

  std::vector<TestType> ref(buffer_size, TestType(0));
  ref[global_offset + index] = data;

  decltype(ref) out(buffer_size, TestType(0));

  ca::test::pass(index);
  ca::test::pass(global_offset);
  ca::test::input_output(out);

  ca::test::kernel("kernel", source,
                   FlagsBuilder(Language::cm)
                       .define("VALUE_INPUT")
                       .define("TYPE", ca::to_cm_string<TestType>())
                       .define("PASSED_VALUE", std::to_string(data))
                       .define("WRITE")
                       .str());

  REQUIRE_THAT(out, Catch::Equals(ref));
}

TEMPLATE_TEST_CASE("cm_read_scaled_scattered", "", float, uint8_t, int8_t,
                   uint16_t, int16_t, uint32_t, int32_t) {
  test_scattered_and_scaled_read_vector<TestType>(false);
  test_scattered_and_scaled_read_vector<TestType>(true);
}

TEMPLATE_TEST_CASE("cm_write_scaled_scattered", "", float, uint8_t, int8_t,
                   uint16_t, int16_t, uint32_t, int32_t) {
  test_scattered_and_scaled_write_vector<TestType>(false);
  test_scattered_write_value<TestType>();
  test_scattered_and_scaled_write_vector<TestType>(true);
}