/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/runtime/feature.hpp>
#include <catch2/catch.hpp>

namespace ca = cassian;

TEST_CASE("to_string") {
  SECTION("fp16") {
    const ca::Feature input = ca::Feature::fp16;
    const std::string output = cassian::to_string(input);
    const std::string expected = "fp16";
    REQUIRE(output == expected);
  }
  SECTION("fp64") {
    const ca::Feature input = ca::Feature::fp64;
    const std::string output = cassian::to_string(input);
    const std::string expected = "fp64";
    REQUIRE(output == expected);
  }
  SECTION("image") {
    const ca::Feature input = ca::Feature::image;
    const std::string output = cassian::to_string(input);
    const std::string expected = "image";
    REQUIRE(output == expected);
  }
  SECTION("image2d") {
    const ca::Feature input = ca::Feature::image2d;
    const std::string output = cassian::to_string(input);
    const std::string expected = "image2d";
    REQUIRE(output == expected);
  }
  SECTION("sampling") {
    const ca::Feature input = ca::Feature::sampling;
    const std::string output = cassian::to_string(input);
    const std::string expected = "sampling";
  }
  SECTION("read_write_images") {
    const ca::Feature input = ca::Feature::read_write_images;
    const std::string output = cassian::to_string(input);
    const std::string expected = "read_write_images";
    REQUIRE(output == expected);
  }
  SECTION("int64_atomics") {
    const ca::Feature input = ca::Feature::int64_atomics;
    const std::string output = cassian::to_string(input);
    const std::string expected = "int64_atomics";
    REQUIRE(output == expected);
  }
  SECTION("global_offsets") {
    const ca::Feature input = ca::Feature::global_offsets;
    const std::string output = cassian::to_string(input);
    const std::string expected = "global_offsets";
    REQUIRE(output == expected);
  }
}
