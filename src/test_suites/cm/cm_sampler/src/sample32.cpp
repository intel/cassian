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

TEST_CASE("cm_sample32_2d", "[cm][image][sampler]") {
  constexpr auto simd = 16;
  constexpr auto width = 16;
  constexpr auto height = 16;

  const std::string source = ca::load_text_file(
      ca::get_asset("kernels/cm_sampler/sample32_2d_genx.cpp"));

  for (const auto bits : {8, 16}) {
    for (bool downsample : {false, true}) {
      for (const auto mask : channel_masks) {
        ca::HostImage<rgba_unorm8> image({width, height});
        std::generate(image.begin(), image.end(), []() {
          return ca::pixel::generate_value<rgba_unorm8>(0);
        });

        std::vector<uint16_t> ref;
        ref.reserve(simd * 4);

        const auto u = ca::generate_value<unsigned>(0, width - 9, 0);
        const auto v = ca::generate_value<unsigned>(0, height - 9, 0);
        const auto du = 1;
        const auto dv = 2;

        const auto shift = 8;

        int channels = 0;

        if ((mask & r) != 0) {
          for (auto y = 0; y < 4; y++) {
            for (auto x = 0; x < 8; x++) {
              ref.push_back(image(u + x * du, v + y * dv).red_raw() << shift);
            }
          }
          channels++;
        }
        if ((mask & g) != 0) {
          for (auto y = 0; y < 4; y++) {
            for (auto x = 0; x < 8; x++) {
              ref.push_back(image(u + x * du, v + y * dv).green_raw() << shift);
            }
          }
          channels++;
        }
        if ((mask & b) != 0) {
          for (auto y = 0; y < 4; y++) {
            for (auto x = 0; x < 8; x++) {
              ref.push_back(image(u + x * du, v + y * dv).blue_raw() << shift);
            }
          }
          channels++;
        }
        if ((mask & a) != 0) {
          for (auto y = 0; y < 4; y++) {
            for (auto x = 0; x < 8; x++) {
              ref.push_back(image(u + x * du, v + y * dv).alpha_raw() << shift);
            }
          }
          channels++;
        }

        std::vector<uint16_t> res;

        ca::test::output(res, ref.size());
        ca::test::input(image);
        ca::test::sampler(ca::SamplerCoordinates::normalized);
        ca::test::pass(static_cast<float>(u) / width);
        ca::test::pass(static_cast<float>(v) / height);
        ca::test::pass(static_cast<float>(du) / width);
        ca::test::pass(static_cast<float>(dv) / height);

        ca::test::kernel(
            "kernel", source,
            FlagsBuilder(Language::cm)
                .define("CHANNELS", std::to_string(channels))
                .define("CHANNEL_MASK", to_cm_string(mask))
                .define("FORMAT_CONTROL",
                        "CM_" + std::to_string(bits) +
                            (downsample ? "_DOWN_SAMPLE" : "_FULL"))
                .str());

        REQUIRE_THAT(ref, Catch::Equals(res));
      }
    }
  }
}
