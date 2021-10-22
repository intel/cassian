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

TEMPLATE_TEST_CASE("cm_3d_sample_2d", "[cm][image][sampler]", rgba_float,
                   rgba_unorm16, rgba_snorm16, rgba_unorm8, rgba_snorm8,
                   rgba_sint8, rgba_sint16, rgba_sint32, rgba_uint8,
                   rgba_uint16, rgba_uint32) {
  constexpr auto width = 16;
  constexpr auto height = 16;

  const std::string source = ca::load_text_file(
      ca::get_asset("kernels/cm_sampler/cm_3d_sample_genx.cpp"));

  const auto hwsimd =
      ca::test::runtime()->get_device_property(ca::DeviceProperty::simd_width);

  for (const auto simd : {hwsimd, hwsimd * 2}) {
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
        std::transform(
            std::begin(u), std::end(u), std::begin(v), out,
            [&image](auto x, auto y) { return image(x, y).green(); });
        channels++;
      }
      if ((mask & b) != 0) {
        std::transform(std::begin(u), std::end(u), std::begin(v), out,
                       [&image](auto x, auto y) { return image(x, y).blue(); });
        channels++;
      }
      if ((mask & a) != 0) {
        std::transform(
            std::begin(u), std::end(u), std::begin(v), out,
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
              .define("SIMD", std::to_string(simd))
              .define("CHANNELS", std::to_string(channels))
              .define("CHANNEL_MASK", to_cm_string(mask))
              .define("READ_TYPE",
                      ca::to_cm_string<typename TestType::read_type>())
              .str());

      REQUIRE_THAT(ref, Catch::Equals(res));
    }
  }
}
