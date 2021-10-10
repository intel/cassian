/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <algorithm>

#include <catch2/catch.hpp>

#include <cassian/image/image.hpp>
#include <cassian/main/flags_builder.hpp>
#include <cassian/main/test_helper.hpp>
#include <cassian/utility/utility.hpp>

#include "common.hpp"

namespace ca = cassian;
using ca::test::FlagsBuilder;
using ca::test::Language;

TEMPLATE_TEST_CASE("cm_load16_1d", "[cm][image][sampler]", rgba_float,
                   rgba_unorm16, rgba_snorm16, rgba_unorm8, rgba_snorm8) {
  constexpr auto simd = 16;
  constexpr auto width = 16;

  const std::string source = ca::load_text_file(
      ca::get_asset("kernels/cm_sampler/load16_1d_genx.cpp"));

  for (const auto mask : channel_masks) {
    ca::HostImage<TestType, ca::ImageType::t_1d> image({width, 1, 1});
    std::generate(image.begin(), image.end(),
                  []() { return ca::pixel::generate_value<TestType>(0); });

    std::vector<typename TestType::read_type> ref;
    ref.reserve(simd * 4);
    auto out = std::back_inserter(ref);

    const auto u = ca::generate_vector<unsigned>(simd, 0, width - 1, 0);

    int channels = 0;

    if ((mask & r) != 0) {
      std::transform(std::begin(u), std::end(u), out,
                     [&image](auto x) { return image(x).red(); });
      channels++;
    }
    if ((mask & g) != 0) {
      std::transform(std::begin(u), std::end(u), out,
                     [&image](auto x) { return image(x).green(); });
      channels++;
    }
    if ((mask & b) != 0) {
      std::transform(std::begin(u), std::end(u), out,
                     [&image](auto x) { return image(x).blue(); });
      channels++;
    }
    if ((mask & a) != 0) {
      std::transform(std::begin(u), std::end(u), out,
                     [&image](auto x) { return image(x).alpha(); });
      channels++;
    }

    std::vector<typename TestType::read_type> res;

    ca::test::output(res, ref.size());
    ca::test::input(image);
    ca::test::input(u);

    ca::test::kernel("kernel", source,
                     FlagsBuilder(Language::cm)
                         .define("CHANNELS", std::to_string(channels))
                         .define("CHANNEL_MASK", to_cm_string(mask))
                         .str());

    REQUIRE_THAT(ref, Catch::Equals(res));
  }
}

TEMPLATE_TEST_CASE("cm_load16_2d", "[cm][image][sampler]", rgba_float,
                   rgba_unorm16, rgba_snorm16, rgba_unorm8, rgba_snorm8) {
  constexpr auto simd = 16;
  constexpr auto width = 16;
  constexpr auto height = 16;

  const std::string source = ca::load_text_file(
      ca::get_asset("kernels/cm_sampler/load16_2d_genx.cpp"));

  for (const auto mask : channel_masks) {
    ca::HostImage<TestType, ca::ImageType::t_2d> image({width, height});
    std::generate(image.begin(), image.end(),
                  []() { return ca::pixel::generate_value<TestType>(0); });

    std::vector<typename TestType::read_type> ref;
    ref.reserve(simd * 4);
    auto out = std::back_inserter(ref);

    const auto u = ca::generate_vector<unsigned>(simd, 0, width - 1, 0);
    const auto v = ca::generate_vector<unsigned>(simd, 0, height - 1, 0);

    int channels = 0;

    if ((mask & r) != 0) {
      std::transform(std::begin(u), std::end(u), std::begin(v), out,
                     [&image](auto x, auto y) { return image(x, y).red(); });
      channels++;
    }
    if ((mask & g) != 0) {
      std::transform(std::begin(u), std::end(u), std::begin(v), out,
                     [&image](auto x, auto y) { return image(x, y).green(); });
      channels++;
    }
    if ((mask & b) != 0) {
      std::transform(std::begin(u), std::end(u), std::begin(v), out,
                     [&image](auto x, auto y) { return image(x, y).blue(); });
      channels++;
    }
    if ((mask & a) != 0) {
      std::transform(std::begin(u), std::end(u), std::begin(v), out,
                     [&image](auto x, auto y) { return image(x, y).alpha(); });
      channels++;
    }

    std::vector<typename TestType::read_type> res;

    ca::test::output(res, ref.size());
    ca::test::input(image);
    ca::test::input(u);
    ca::test::input(v);

    ca::test::kernel("kernel", source,
                     FlagsBuilder(Language::cm)
                         .define("CHANNELS", std::to_string(channels))
                         .define("CHANNEL_MASK", to_cm_string(mask))
                         .str());

    REQUIRE_THAT(ref, Catch::Equals(res));
  }
}

TEMPLATE_TEST_CASE("cm_load16_3d", "[cm][image][sampler]", rgba_float,
                   rgba_unorm16, rgba_snorm16, rgba_unorm8, rgba_snorm8) {
  constexpr auto simd = 16;
  constexpr auto width = 16;
  constexpr auto height = 16;
  constexpr auto depth = 16;

  const std::string source = ca::load_text_file(
      ca::get_asset("kernels/cm_sampler/load16_3d_genx.cpp"));

  for (const auto mask : channel_masks) {
    ca::HostImage<TestType, ca::ImageType::t_3d> image({width, height, depth});
    std::generate(image.begin(), image.end(),
                  []() { return ca::pixel::generate_value<TestType>(0); });

    std::vector<typename TestType::read_type> ref;
    ref.reserve(simd * 4);

    const auto u = ca::generate_vector<unsigned>(simd, 0, width - 1, 0);
    const auto v = ca::generate_vector<unsigned>(simd, 0, height - 1, 0);
    const auto z = ca::generate_vector<unsigned>(simd, 0, depth - 1, 0);

    int channels = 0;

    if ((mask & r) != 0) {
      for (unsigned i = 0; i < simd; i++) {
        ref.push_back(image(u[i], v[i], z[i]).red());
      }
      channels++;
    }
    if ((mask & g) != 0) {
      for (unsigned i = 0; i < simd; i++) {
        ref.push_back(image(u[i], v[i], z[i]).green());
      }
      channels++;
    }
    if ((mask & b) != 0) {
      for (unsigned i = 0; i < simd; i++) {
        ref.push_back(image(u[i], v[i], z[i]).blue());
      }
      channels++;
    }
    if ((mask & a) != 0) {
      for (unsigned i = 0; i < simd; i++) {
        ref.push_back(image(u[i], v[i], z[i]).alpha());
      }
      channels++;
    }

    std::vector<typename TestType::read_type> res;

    ca::test::output(res, ref.size());
    ca::test::input(image);
    ca::test::input(u);
    ca::test::input(v);
    ca::test::input(z);

    ca::test::kernel("kernel", source,
                     FlagsBuilder(Language::cm)
                         .define("CHANNELS", std::to_string(channels))
                         .define("CHANNEL_MASK", to_cm_string(mask))
                         .str());

    REQUIRE_THAT(ref, Catch::Equals(res));
  }
}