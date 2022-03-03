/*
 * Copyright (C) 2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <common.hpp>

namespace ca = cassian;
using ca::half;
using ca::test::FlagsBuilder;
using ca::test::Language;

TEMPLATE_TEST_CASE("cm_typed_read_1d", "[1d]", rgba_float, rgba_sint32,
                   rgba_uint32) {
  const std::string source = ca::load_text_file(
      ca::get_asset("kernels/cm_typed_surface_rw/read_typed_1d_genx.cpp"));

  constexpr unsigned buffer_size = 1024;

  for (auto mask : channel_masks) {
    auto smask = dump_cmask(mask);
    unsigned channels = smask.length();

    for (auto simd : {8, 16, 32}) {
      ca::HostImage<TestType, ca::ImageType::t_1d> image({buffer_size, 1, 1});
      std::generate(image.begin(), image.end(),
                    []() { return ca::pixel::generate_value<TestType>(0); });

      std::vector<typename TestType::read_type> ref;
      ref.reserve(simd * 4);
      auto res = std::back_inserter(ref);

      const auto u = ca::generate_vector<unsigned>(simd, 0, buffer_size - 1, 0);

      if ((mask & r) != 0) {
        std::transform(std::begin(u), std::end(u), res,
                       [&image](auto x) { return image(x).red(); });
      }
      if ((mask & g) != 0) {
        std::transform(std::begin(u), std::end(u), res,
                       [&image](auto x) { return image(x).green(); });
      }
      if ((mask & b) != 0) {
        std::transform(std::begin(u), std::end(u), res,
                       [&image](auto x) { return image(x).blue(); });
      }
      if ((mask & a) != 0) {
        std::transform(std::begin(u), std::end(u), res,
                       [&image](auto x) { return image(x).alpha(); });
      }

      std::vector<typename TestType::read_type> out;

      ca::test::input(image);
      ca::test::input(u);
      ca::test::output(out, ref.size());

      ca::test::kernel(
          "kernel", source,
          FlagsBuilder(Language::cm)
              .define("SIMD", std::to_string(simd))
              .define("CHANNELS", std::to_string(channels))
              .define("TYPE", ca::to_cm_string<typename TestType::read_type>())
              .define("CHANNEL_MASK", "CM_" + smask + "_ENABLE")
              .str());

      REQUIRE_THAT(ref, Catch::Equals(out));
    }
  }
}

TEMPLATE_TEST_CASE("cm_typed_read_2d", "[2d]", rgba_float, rgba_sint32,
                   rgba_uint32) {
  const std::string source = ca::load_text_file(
      ca::get_asset("kernels/cm_typed_surface_rw/read_typed_2d_genx.cpp"));

  constexpr unsigned buffer_size = 256;

  for (auto mask : channel_masks) {
    auto smask = dump_cmask(mask);
    unsigned channels = smask.length();

    for (auto simd : {8, 16, 32}) {
      ca::HostImage<TestType, ca::ImageType::t_2d> image(
          {buffer_size, buffer_size, 1});
      std::generate(image.begin(), image.end(),
                    []() { return ca::pixel::generate_value<TestType>(0); });

      std::vector<typename TestType::read_type> ref;
      ref.reserve(simd * 4);
      auto res = std::back_inserter(ref);

      const auto u = ca::generate_vector<unsigned>(simd, 0, buffer_size - 1, 0);
      const auto v = ca::generate_vector<unsigned>(simd, 0, buffer_size - 1, 0);

      if ((mask & r) != 0) {
        std::transform(std::begin(u), std::end(u), std::begin(v), res,
                       [&image](auto x, auto y) { return image(x, y).red(); });
      }
      if ((mask & g) != 0) {
        std::transform(
            std::begin(u), std::end(u), std::begin(v), res,
            [&image](auto x, auto y) { return image(x, y).green(); });
      }
      if ((mask & b) != 0) {
        std::transform(std::begin(u), std::end(u), std::begin(v), res,
                       [&image](auto x, auto y) { return image(x, y).blue(); });
      }
      if ((mask & a) != 0) {
        std::transform(
            std::begin(u), std::end(u), std::begin(v), res,
            [&image](auto x, auto y) { return image(x, y).alpha(); });
      }

      std::vector<typename TestType::read_type> out;

      ca::test::input(image);
      ca::test::input(u);
      ca::test::input(v);
      ca::test::output(out, ref.size());

      ca::test::kernel(
          "kernel", source,
          FlagsBuilder(Language::cm)
              .define("SIMD", std::to_string(simd))
              .define("CHANNELS", std::to_string(channels))
              .define("TYPE", ca::to_cm_string<typename TestType::read_type>())
              .define("CHANNEL_MASK", "CM_" + smask + "_ENABLE")
              .str());

      REQUIRE_THAT(ref, Catch::Equals(out));
    }
  }
}

TEMPLATE_TEST_CASE("cm_typed_read_3d", "[3d]", rgba_float, rgba_sint32,
                   rgba_uint32) {
  const std::string source = ca::load_text_file(
      ca::get_asset("kernels/cm_typed_surface_rw/read_typed_3d_genx.cpp"));

  constexpr unsigned buffer_size = 128;

  for (auto mask : channel_masks) {
    auto smask = dump_cmask(mask);
    unsigned channels = smask.length();

    for (auto simd : {8, 16, 32}) {
      ca::HostImage<TestType, ca::ImageType::t_3d> image(
          {buffer_size, buffer_size, buffer_size});
      std::generate(image.begin(), image.end(),
                    []() { return ca::pixel::generate_value<TestType>(0); });

      std::vector<typename TestType::read_type> ref;
      ref.reserve(simd * 4);

      const auto u = ca::generate_vector<unsigned>(simd, 0, buffer_size - 1, 0);
      const auto v = ca::generate_vector<unsigned>(simd, 0, buffer_size - 1, 0);
      const auto w = ca::generate_vector<unsigned>(simd, 0, buffer_size - 1, 0);

      if ((mask & CMask::r) != 0) {
        for (unsigned i = 0; i < simd; i++) {
          ref.push_back(image(u[i], v[i], w[i]).red());
        }
      }
      if ((mask & CMask::g) != 0) {
        for (unsigned i = 0; i < simd; i++) {
          ref.push_back(image(u[i], v[i], w[i]).green());
        }
      }
      if ((mask & CMask::b) != 0) {
        for (unsigned i = 0; i < simd; i++) {
          ref.push_back(image(u[i], v[i], w[i]).blue());
        }
      }
      if ((mask & CMask::a) != 0) {
        for (unsigned i = 0; i < simd; i++) {
          ref.push_back(image(u[i], v[i], w[i]).alpha());
        }
      }

      std::vector<typename TestType::read_type> out;

      ca::test::input(image);
      ca::test::input(u);
      ca::test::input(v);
      ca::test::input(w);
      ca::test::output(out, ref.size());

      ca::test::kernel(
          "kernel", source,
          FlagsBuilder(Language::cm)
              .define("SIMD", std::to_string(simd))
              .define("CHANNELS", std::to_string(channels))
              .define("TYPE", ca::to_cm_string<typename TestType::read_type>())
              .define("CHANNEL_MASK", "CM_" + smask + "_ENABLE")
              .str());

      REQUIRE_THAT(ref, Catch::Equals(out));
    }
  }
}