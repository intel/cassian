/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <algorithm>

#include <catch2/catch.hpp>

#include <cassian/image/image.hpp>
#include <cassian/image/pixel/simple.hpp>
#include <cassian/main/flags_builder.hpp>
#include <cassian/main/test_helper.hpp>
#include <cassian/test_harness/test_harness.hpp>
#include <cassian/utility/utility.hpp>

namespace ca = cassian;
using ca::half;
using ca::test::FlagsBuilder;
using ca::test::Language;

template <typename T> void test_read(unsigned width, unsigned height) {
  const std::string source = ca::load_text_file(
      ca::get_asset("kernels/cm_media_block_io/media_block_read_genx.cpp"));

  constexpr unsigned image_width = 128;
  constexpr unsigned image_height = 128;
  ca::HostImage<T, ca::ImageType::t_2d> image({image_width, image_height});

  std::generate(image.begin(), image.end(),
                []() { return ca::generate_value<T>(0); });

  auto x = ca::generate_value<unsigned>(0, image_width - width - 1, 0);
  auto y = ca::generate_value<unsigned>(0, image_height - height - 1, 0);

  unsigned mask = 0;

  if constexpr (sizeof(T) == 1) {
    mask = 3;
  } else if constexpr (sizeof(T) == 2) {
    mask = 1;
  }

  x &= ~mask;

  std::vector<T> out;
  std::vector<T> ref;
  ref.reserve(width * height);

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      ref.emplace_back(image(x + j, y + i));
    }
  }

  ca::test::output(out, ref.size());
  ca::test::input(image);
  ca::test::pass(x);
  ca::test::pass(y);

  ca::test::kernel("kernel", source,
                   FlagsBuilder(Language::cm)
                       .define("TYPE", ca::to_cm_string<T>())
                       .define("WIDTH", std::to_string(width))
                       .define("HEIGHT", std::to_string(height))
                       .str());

  REQUIRE_THAT(out, Catch::Equals(ref));
}

TEMPLATE_TEST_CASE("read", "[cm][mediablock][read][common]", int8_t, int16_t,
                   int32_t, uint8_t, uint16_t, uint32_t, half, float) {
  ca::Requirements requirements;
  requirements.arithmetic_type<TestType>();
  if (ca::test::should_skip_test(requirements)) {
    return;
  }

  constexpr unsigned max_bytes = 256;
  constexpr unsigned max_elements = max_bytes / sizeof(TestType);

  unsigned min_width = sizeof(uint32_t) / sizeof(TestType);
  if (min_width == 0) {
    min_width = 1;
  }

  const unsigned max_height = max_elements / min_width;

  constexpr unsigned max_width = 32 / sizeof(TestType);
  constexpr unsigned height_for_max_width = max_elements / max_width;

  test_read<TestType>(4, 4);
  test_read<TestType>(8, 5);

  test_read<TestType>(min_width, 1);
  test_read<TestType>(min_width, max_height);
  test_read<TestType>(max_width, 1);
  test_read<TestType>(max_width, height_for_max_width);
}

TEMPLATE_TEST_CASE("read_extra_width", "[cm][mediablock][read][extra]", int8_t,
                   int16_t, int32_t, uint8_t, uint16_t, uint32_t, half, float) {
  ca::Requirements requirements;
  requirements.arithmetic_type<TestType>();
  if (ca::test::should_skip_test(requirements)) {
    return;
  }

  constexpr unsigned max_bytes = 256;
  constexpr unsigned max_elements = max_bytes / sizeof(TestType);

  constexpr unsigned extra_width = 64 / sizeof(TestType);
  constexpr unsigned height_for_extra_width = max_elements / extra_width;

  test_read<TestType>(extra_width, 1);
  test_read<TestType>(extra_width, height_for_extra_width);
}

TEMPLATE_TEST_CASE("read_padding", "[cm][mediablock][read][padding]", int8_t,
                   int16_t, int32_t, uint8_t, uint16_t, uint32_t, half, float) {
  ca::Requirements requirements;
  requirements.arithmetic_type<TestType>();
  if (ca::test::should_skip_test(requirements)) {
    return;
  }

  test_read<TestType>(5, 3);
}
