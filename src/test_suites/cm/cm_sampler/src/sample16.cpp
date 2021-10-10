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

TEMPLATE_TEST_CASE("cm_sample16_1d", "[cm][image][sampler]", rgba_float,
                   rgba_unorm16, rgba_snorm16, rgba_unorm8, rgba_snorm8,
                   rgba_sint8, rgba_sint16, rgba_sint32, rgba_uint8,
                   rgba_uint16, rgba_uint32) {
  constexpr auto simd = 16;
  constexpr auto width = 16;

  const std::string source = ca::load_text_file(
      ca::get_asset("kernels/cm_sampler/sample16_1d_genx.cpp"));

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

    std::vector<float> u_norm;
    u_norm.reserve(u.size());
    std::transform(std::begin(u), std::end(u), std::back_inserter(u_norm),
                   [width](float value) { return value / width; });

    ca::test::output(res, ref.size());
    ca::test::input(image);
    ca::test::sampler(ca::SamplerCoordinates::normalized);
    ca::test::input(u_norm);

    ca::test::kernel(
        "kernel", source,
        FlagsBuilder(Language::cm)
            .define("CHANNELS", std::to_string(channels))
            .define("CHANNEL_MASK", to_cm_string(mask))
            .define("READ_TYPE",
                    ca::to_cm_string<typename TestType::read_type>())
            .str());

    REQUIRE_THAT(ref, Catch::Equals(res));
  }
}

TEMPLATE_TEST_CASE("cm_sample16_2d", "[cm][image][sampler]", rgba_float,
                   rgba_unorm16, rgba_snorm16, rgba_unorm8, rgba_snorm8,
                   rgba_sint8, rgba_sint16, rgba_sint32, rgba_uint8,
                   rgba_uint16, rgba_uint32) {
  constexpr auto simd = 16;
  constexpr auto width = 16;
  constexpr auto height = 16;

  const std::string source = ca::load_text_file(
      ca::get_asset("kernels/cm_sampler/sample16_2d_genx.cpp"));

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

    std::vector<float> u_norm;
    u_norm.reserve(u.size());
    std::transform(std::begin(u), std::end(u), std::back_inserter(u_norm),
                   [width](float value) { return value / width; });

    std::vector<float> v_norm;
    v_norm.reserve(v.size());
    std::transform(std::begin(v), std::end(v), std::back_inserter(v_norm),
                   [height](float value) { return value / height; });

    ca::test::output(res, ref.size());
    ca::test::input(image);
    ca::test::sampler(ca::SamplerCoordinates::normalized);
    ca::test::input(u_norm);
    ca::test::input(v_norm);

    ca::test::kernel(
        "kernel", source,
        FlagsBuilder(Language::cm)
            .define("CHANNELS", std::to_string(channels))
            .define("CHANNEL_MASK", to_cm_string(mask))
            .define("READ_TYPE",
                    ca::to_cm_string<typename TestType::read_type>())
            .str());

    REQUIRE_THAT(ref, Catch::Equals(res));
  }
}

TEMPLATE_TEST_CASE("cm_sample16_3d", "[cm][image][sampler]", rgba_float,
                   rgba_unorm16, rgba_snorm16, rgba_unorm8, rgba_snorm8,
                   rgba_sint8, rgba_sint16, rgba_sint32, rgba_uint8,
                   rgba_uint16, rgba_uint32) {
  constexpr auto simd = 16;
  constexpr auto width = 16;
  constexpr auto height = 16;
  constexpr auto depth = 16;

  const std::string source = ca::load_text_file(
      ca::get_asset("kernels/cm_sampler/sample16_3d_genx.cpp"));

  for (const auto mask : channel_masks) {
    ca::HostImage<TestType, ca::ImageType::t_3d> image({width, height, depth});
    std::generate(image.begin(), image.end(),
                  []() { return ca::pixel::generate_value<TestType>(0); });

    std::vector<typename TestType::read_type> ref;
    ref.reserve(simd * 4);
    auto out = std::back_inserter(ref);

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

    std::vector<float> u_norm;
    u_norm.reserve(u.size());
    std::transform(std::begin(u), std::end(u), std::back_inserter(u_norm),
                   [width](float value) { return value / width; });

    std::vector<float> v_norm;
    v_norm.reserve(v.size());
    std::transform(std::begin(v), std::end(v), std::back_inserter(v_norm),
                   [height](float value) { return value / height; });

    std::vector<float> z_norm;
    z_norm.reserve(z.size());
    std::transform(std::begin(z), std::end(z), std::back_inserter(z_norm),
                   [depth](float value) { return value / depth; });

    ca::test::output(res, ref.size());
    ca::test::input(image);
    ca::test::sampler(ca::SamplerCoordinates::normalized);
    ca::test::input(u_norm);
    ca::test::input(v_norm);
    ca::test::input(z_norm);

    ca::test::kernel(
        "kernel", source,
        FlagsBuilder(Language::cm)
            .define("CHANNELS", std::to_string(channels))
            .define("CHANNEL_MASK", to_cm_string(mask))
            .define("READ_TYPE",
                    ca::to_cm_string<typename TestType::read_type>())
            .str());

    REQUIRE_THAT(ref, Catch::Equals(res));
  }
}