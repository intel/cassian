/*
 * Copyright (C) 2021-2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <algorithm>

#include <catch2/catch.hpp>

#include <cassian/image/image.hpp>
#include <cassian/image/nv12.hpp>
#include <cassian/main/flags_builder.hpp>
#include <cassian/main/test_helper.hpp>
#include <cassian/utility/utility.hpp>

#include "common.hpp"

namespace ca = cassian;
using ca::test::FlagsBuilder;
using ca::test::Language;

TEST_CASE("cm_sample32_2d", "[cm][image][sampler]") {
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

TEST_CASE("cm_sample32_nv12", "[cm][image][sampler][nv12]") {
  constexpr auto width = 16;
  constexpr auto height = 16;
  constexpr auto bits = 16;
  constexpr auto downsample = false;

  const std::string source = ca::load_text_file(
      ca::get_asset("kernels/cm_sampler/sample32_2d_genx.cpp"));

  for (const auto mask : channel_masks) {
    if ((mask & a) != 0) {
      continue;
    }

    ca::Nv12Image image({width, height});

    uint16_t data = 0;
    std::generate(image.y_plane.begin(), image.y_plane.end(), []() {
      return ca::generate_value<ca::Nv12Image::y_plane_type>(0);
    });
    std::generate(image.uv_plane.begin(), image.uv_plane.end(), [&data]() {
      return ca::generate_value<ca::Nv12Image::uv_plane_type>(0);
    });

    std::vector<uint16_t> ref;
    std::vector<uint16_t> res;

    const auto u = ca::generate_value<unsigned>(0, width - 9, 0);
    const auto v = ca::generate_value<unsigned>(0, height - 9, 0);
    const auto du = 1;
    const auto dv = 1;

    const auto shift = 8;

    int channels = 0;

    if ((mask & r) != 0) {
      constexpr uint16_t v_mask = 0xff00;
      for (auto y = 0; y < 4; y++) {
        for (auto x = 0; x < 8; x++) {
          auto i = (u + x * du) / 2;
          auto j = (v + y * dv) / 2;
          ref.push_back(image.uv_plane(i, j) & v_mask);
        }
      }
      channels++;
    }
    if ((mask & g) != 0) {
      for (auto y = 0; y < 4; y++) {
        for (auto x = 0; x < 8; x++) {
          ref.push_back(image.y_plane(u + x * du, v + y * dv) << shift);
        }
      }
      channels++;
    }
    if ((mask & b) != 0) {
      constexpr uint16_t u_mask = 0xff;
      for (auto y = 0; y < 4; y++) {
        for (auto x = 0; x < 8; x++) {
          auto i = (u + x * du) / 2;
          auto j = (v + y * dv) / 2;
          ref.push_back((image.uv_plane(i, j) & u_mask) << shift);
        }
      }
      channels++;
    }

    ca::test::output(res, ref.size());
    ca::test::input(image);
    ca::test::sampler(ca::SamplerCoordinates::normalized);
    ca::test::pass(static_cast<float>(u) / width);
    ca::test::pass(static_cast<float>(v) / height);
    ca::test::pass(static_cast<float>(du) / width);
    ca::test::pass(static_cast<float>(dv) / height);

    ca::test::kernel("kernel", source,
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
