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

TEMPLATE_TEST_CASE("cm_typed_write_1d", "[1d]", rgba_float, rgba_sint32,
                   rgba_uint32) {
  const std::string source = ca::load_text_file(
      ca::get_asset("kernels/cm_typed_surface_rw/write_typed_1d_genx.cpp"));

  constexpr unsigned buffer_size = 1024;

  for (auto mask : channel_masks) {
    auto smask = dump_cmask(mask);
    unsigned channels = smask.length();

    for (auto simd : {8, 16, 32}) {
      auto data =
          ca::generate_vector<typename TestType::read_type>(channels * simd, 0);

      std::vector<unsigned> u;
      do {
        auto index = ca::generate_value<unsigned>(0, buffer_size - 1, 0);

        if (std::find_if(std::begin(u), std::end(u), [index](auto x) {
              return x == index;
            }) == std::end(u)) {
          u.push_back(index);
        }
      } while (u.size() < simd);

      ca::HostImage<TestType, ca::ImageType::t_1d> ref({buffer_size, 1, 1});

      auto it = 0;
      if ((mask & CMask::r) != 0) {
        for (unsigned i = 0; i < simd; i++) {
          ref(u[i]).red_raw() = data[it++];
        }
      }
      if ((mask & CMask::g) != 0) {
        for (unsigned i = 0; i < simd; i++) {
          ref(u[i]).green_raw() = data[it++];
        }
      }
      if ((mask & CMask::b) != 0) {
        for (unsigned i = 0; i < simd; i++) {
          ref(u[i]).blue_raw() = data[it++];
        }
      }
      if ((mask & CMask::a) != 0) {
        for (unsigned i = 0; i < simd; i++) {
          ref(u[i]).alpha_raw() = data[it++];
        }
      }

      ca::HostImage<TestType, ca::ImageType::t_1d> out({buffer_size, 1, 1});

      ca::test::input_output(out);
      ca::test::input(u);

      ca::test::kernel(
          "kernel", source,
          FlagsBuilder(Language::cm)
              .define("SIMD", std::to_string(simd))
              .define("CHANNELS", std::to_string(channels))
              .define("TYPE", ca::to_cm_string<typename TestType::read_type>())
              .define("PASSED_VECTOR", dump_vector(data))
              .define("CHANNEL_MASK", "CM_" + smask + "_ENABLE")
              .str());

      REQUIRE_THAT(ref.storage(), Catch::Equals(out.storage()));
    }
  }
}

TEMPLATE_TEST_CASE("cm_typed_write_2d", "[2d]", rgba_float, rgba_sint32,
                   rgba_uint32) {
  const std::string source = ca::load_text_file(
      ca::get_asset("kernels/cm_typed_surface_rw/write_typed_2d_genx.cpp"));

  constexpr unsigned buffer_size = 256;

  for (auto mask : channel_masks) {
    auto smask = dump_cmask(mask);
    unsigned channels = smask.length();

    for (auto simd : {8, 16, 32}) {
      auto data =
          ca::generate_vector<typename TestType::read_type>(channels * simd, 0);

      std::vector<unsigned> u;
      do {
        auto index = ca::generate_value<unsigned>(0, buffer_size - 1, 0);

        if (std::find_if(std::begin(u), std::end(u), [index, simd](auto v) {
              return v == index;
            }) == std::end(u)) {
          u.push_back(index);
        }
      } while (u.size() < simd);

      std::vector<unsigned> v;
      do {
        auto index = ca::generate_value<unsigned>(0, buffer_size - 1, 0);

        if (std::find_if(std::begin(v), std::end(v), [index, simd](auto x) {
              return x == index;
            }) == std::end(v)) {
          v.push_back(index);
        }
      } while (v.size() < simd);

      ca::HostImage<TestType, ca::ImageType::t_2d> ref(
          {buffer_size, buffer_size, 1});

      auto it = 0;
      if ((mask & CMask::r) != 0) {
        for (unsigned i = 0; i < simd; i++) {
          ref(u[i], v[i]).red_raw() = data[it++];
        }
      }
      if ((mask & CMask::g) != 0) {
        for (unsigned i = 0; i < simd; i++) {
          ref(u[i], v[i]).green_raw() = data[it++];
        }
      }
      if ((mask & CMask::b) != 0) {
        for (unsigned i = 0; i < simd; i++) {
          ref(u[i], v[i]).blue_raw() = data[it++];
        }
      }
      if ((mask & CMask::a) != 0) {
        for (unsigned i = 0; i < simd; i++) {
          ref(u[i], v[i]).alpha_raw() = data[it++];
        }
      }

      ca::HostImage<TestType, ca::ImageType::t_2d> out(
          {buffer_size, buffer_size, 1});

      ca::test::input_output(out);
      ca::test::input(u);
      ca::test::input(v);

      ca::test::kernel(
          "kernel", source,
          FlagsBuilder(Language::cm)
              .define("SIMD", std::to_string(simd))
              .define("CHANNELS", std::to_string(channels))
              .define("TYPE", ca::to_cm_string<typename TestType::read_type>())
              .define("PASSED_VECTOR", dump_vector(data))
              .define("CHANNEL_MASK", "CM_" + smask + "_ENABLE")
              .str());

      REQUIRE_THAT(ref.storage(), Catch::Equals(out.storage()));
    }
  }
}

TEMPLATE_TEST_CASE("cm_typed_write_3d", "[3d]", rgba_float, rgba_sint32,
                   rgba_uint32) {
  const std::string source = ca::load_text_file(
      ca::get_asset("kernels/cm_typed_surface_rw/write_typed_3d_genx.cpp"));

  constexpr unsigned buffer_size = 128;

  for (auto mask : channel_masks) {
    auto smask = dump_cmask(mask);
    unsigned channels = smask.length();

    for (auto simd : {8, 16, 32}) {
      auto data =
          ca::generate_vector<typename TestType::read_type>(channels * simd, 0);

      std::vector<unsigned> u;
      do {
        auto index = ca::generate_value<unsigned>(0, buffer_size - 1, 0);

        if (std::find_if(std::begin(u), std::end(u), [index](auto v) {
              return v == index;
            }) == std::end(u)) {
          u.push_back(index);
        }
      } while (u.size() < simd);

      std::vector<unsigned> v;
      do {
        auto index = ca::generate_value<unsigned>(0, buffer_size - 1, 0);

        if (std::find_if(std::begin(v), std::end(v), [index](auto x) {
              return x == index;
            }) == std::end(v)) {
          v.push_back(index);
        }
      } while (v.size() < simd);

      std::vector<unsigned> w;
      do {
        auto index = ca::generate_value<unsigned>(0, buffer_size - 1, 0);

        if (std::find_if(std::begin(w), std::end(w), [index](auto x) {
              return x == index;
            }) == std::end(w)) {
          w.push_back(index);
        }
      } while (w.size() < simd);

      ca::HostImage<TestType, ca::ImageType::t_3d> ref(
          {buffer_size, buffer_size, buffer_size});

      auto it = 0;
      if ((mask & CMask::r) != 0) {
        for (unsigned i = 0; i < simd; i++) {
          ref(u[i], v[i], w[i]).red_raw() = data[it++];
        }
      }
      if ((mask & CMask::g) != 0) {
        for (unsigned i = 0; i < simd; i++) {
          ref(u[i], v[i], w[i]).green_raw() = data[it++];
        }
      }
      if ((mask & CMask::b) != 0) {
        for (unsigned i = 0; i < simd; i++) {
          ref(u[i], v[i], w[i]).blue_raw() = data[it++];
        }
      }
      if ((mask & CMask::a) != 0) {
        for (unsigned i = 0; i < simd; i++) {
          ref(u[i], v[i], w[i]).alpha_raw() = data[it++];
        }
      }

      ca::HostImage<TestType, ca::ImageType::t_3d> out(
          {buffer_size, buffer_size, buffer_size});

      ca::test::input_output(out);
      ca::test::input(u);
      ca::test::input(v);
      ca::test::input(w);

      ca::test::kernel(
          "kernel", source,
          FlagsBuilder(Language::cm)
              .define("SIMD", std::to_string(simd))
              .define("CHANNELS", std::to_string(channels))
              .define("TYPE", ca::to_cm_string<typename TestType::read_type>())
              .define("PASSED_VECTOR", dump_vector(data))
              .define("CHANNEL_MASK", "CM_" + smask + "_ENABLE")
              .str());

      REQUIRE_THAT(ref.storage(), Catch::Equals(out.storage()));
    }
  }
}