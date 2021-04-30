/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/runtime/runtime.hpp>
#include <cassian/vector/vector.hpp>
#include <catch2/catch.hpp>

struct CustomType {
  using is_custom_type = int;
};

template <> std::string cassian::to_clc_string<CustomType>() {
  return "custom";
};

TEST_CASE("to_clc_string", "") {
  SECTION("input scalar") {
    SECTION("int") {
      REQUIRE(cassian::to_clc_string<int>() == std::string("int"));
    }
    SECTION("float") {
      REQUIRE(cassian::to_clc_string<float>() == std::string("float"));
    }
    SECTION("custom type") {
      REQUIRE(cassian::to_clc_string<CustomType>() == std::string("custom"));
    }
  }

  SECTION("input vector") {
    SECTION("int2") {
      REQUIRE(cassian::to_clc_string<cassian::Vector<int, 2>>() ==
              std::string("int2"));
    }
    SECTION("int3") {
      REQUIRE(cassian::to_clc_string<cassian::Vector<int, 3>>() ==
              std::string("int3"));
    }
    SECTION("int4") {
      REQUIRE(cassian::to_clc_string<cassian::Vector<int, 4>>() ==
              std::string("int4"));
    }
    SECTION("float4") {
      REQUIRE(cassian::to_clc_string<cassian::Vector<float, 4>>() ==
              std::string("float4"));
    }
    SECTION("custom type 4") {
      REQUIRE(cassian::to_clc_string<cassian::Vector<CustomType, 4>>() ==
              std::string("custom4"));
    }
  }
}

TEST_CASE("get_max_local_work_size", "") {
  SECTION("max work size is smaller than global_work_size, but does fit in "
          "size limits") {
    const size_t max_work_size = 1024;
    SECTION("in 1d scalar") {
      const size_t global_work_size = 2048;
      const size_t max_sizes = 2048;
      const size_t reference = 1024;
      REQUIRE(cassian::Runtime::get_max_local_work_size(
                  global_work_size, max_sizes, max_work_size) == reference);
    }
    SECTION("in 1d") {
      const std::array<size_t, 1> global_work_size = {2048};
      const std::array<size_t, 1> max_sizes = {2048};
      const std::array<size_t, 1> reference = {1024};
      REQUIRE(cassian::Runtime::get_max_local_work_size(
                  global_work_size, max_sizes, max_work_size) == reference);
    }
    SECTION("in 2d") {
      const std::array<size_t, 2> global_work_size = {1024, 1024};
      const std::array<size_t, 2> max_sizes = {64, 64};
      const std::array<size_t, 2> reference = {64, 16};
      REQUIRE(cassian::Runtime::get_max_local_work_size(
                  global_work_size, max_sizes, max_work_size) == reference);
    }
    SECTION("in 3d") {
      const std::array<size_t, 3> global_work_size = {1024, 1024, 1024};
      const std::array<size_t, 3> max_sizes = {64, 2, 16};
      const std::array<size_t, 3> reference = {64, 2, 8};
      REQUIRE(cassian::Runtime::get_max_local_work_size(
                  global_work_size, max_sizes, max_work_size) == reference);
    }
  }
  SECTION("max work size fits in global work size and size limits") {
    const size_t max_work_size = 512;
    SECTION("in 1d scalar") {
      const size_t global_work_size = 8;
      const size_t max_sizes = 512;
      const size_t reference = 8;
      REQUIRE(cassian::Runtime::get_max_local_work_size(
                  global_work_size, max_sizes, max_work_size) == reference);
    }
    SECTION("in 1d") {
      const std::array<size_t, 1> global_work_size = {8};
      const std::array<size_t, 1> max_sizes = {512};
      const std::array<size_t, 1> reference = {8};
      REQUIRE(cassian::Runtime::get_max_local_work_size(
                  global_work_size, max_sizes, max_work_size) == reference);
    }
    SECTION("in 2d") {
      const std::array<size_t, 2> global_work_size = {8, 8};
      const std::array<size_t, 2> max_sizes = {512, 256};
      const std::array<size_t, 2> reference = {8, 8};
      REQUIRE(cassian::Runtime::get_max_local_work_size(
                  global_work_size, max_sizes, max_work_size) == reference);
    }
    SECTION("in 3d") {
      const std::array<size_t, 3> global_work_size = {8, 8, 8};
      const std::array<size_t, 3> max_sizes = {512, 256, 64};
      const std::array<size_t, 3> reference = {8, 8, 8};
      REQUIRE(cassian::Runtime::get_max_local_work_size(
                  global_work_size, max_sizes, max_work_size) == reference);
    }
  }
}