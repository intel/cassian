/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/runtime/openclc_types.hpp>
#include <cassian/utility/utility.hpp>
#include <cassian/vector/vector.hpp>
#include <catch2/catch.hpp>
#include <cstdint>
#include <vector>

namespace ca = cassian;

namespace {
TEST_CASE("combine_bytes", "") {
  SECTION("input uint8_t output uint32_t") {
    const std::vector<uint8_t> input = {1, 2, 3, 4};
    const uint32_t reference = uint8_t(input[3]) << 24 |
                               uint8_t(input[2]) << 16 |
                               uint8_t(input[1]) << 8 | uint8_t(input[0]);
    const auto output = ca::combine_bytes<uint32_t>(input);
    REQUIRE(output == reference);
  }

  SECTION("input int8_t output int32_t") {
    const std::vector<int8_t> input = {-1, -2, -3, -4};
    const int32_t reference = uint8_t(input[3]) << 24 |
                              uint8_t(input[2]) << 16 | uint8_t(input[1]) << 8 |
                              uint8_t(input[0]);
    const auto output = ca::combine_bytes<int32_t>(input);
    REQUIRE(output == reference);
  }
}

TEST_CASE("split_bytes", "") {
  SECTION("input uint32_t output uint8_t") {
    const uint32_t input = 0x04030201;
    const std::vector<uint8_t> reference = {1, 2, 3, 4};
    const auto output = ca::split_bytes<uint8_t>(input);
    REQUIRE(output == reference);
  }

  SECTION("input int32_t output int8_t") {
    const int32_t input = 0xFCFDFEFF;
    const std::vector<int8_t> reference = {-1, -2, -3, -4};
    const auto output = ca::split_bytes<int8_t>(input);
    REQUIRE(output == reference);
  }
}

TEST_CASE("pack_vector", "") {

  SECTION("input 8 bits output 8 bits step 1") {
    const std::vector<uint8_t> input = {0x00, 0x01, 0x02, 0x03};
    const std::vector<uint8_t> expected = {0x00, 0x01, 0x02, 0x03};
    const std::vector<uint8_t> output = ca::pack_vector<uint8_t>(input, 1);
    REQUIRE(expected == output);
  }

  SECTION("input 8 bits output 16 bits step 1") {
    const std::vector<uint8_t> input = {0x00, 0x01, 0x02, 0x03,
                                        0x04, 0x05, 0x06, 0x07};
    const std::vector<uint16_t> expected = {0x0100, 0x0302, 0x0504, 0x0706};
    const std::vector<uint16_t> output = ca::pack_vector<uint16_t>(input, 1);
    REQUIRE(expected == output);
  }

  SECTION("input 8 bits output 32 bits step 1") {
    const std::vector<uint8_t> input = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
                                        0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B,
                                        0x0C, 0x0D, 0x0E, 0x0F};
    const std::vector<uint32_t> expected = {0x03020100, 0x07060504, 0x0B0A0908,
                                            0x0F0E0D0C};
    const std::vector<uint32_t> output = ca::pack_vector<uint32_t>(input, 1);
    REQUIRE(expected == output);
  }

  SECTION("input 16 bits output 16 bits step 1") {
    const std::vector<uint16_t> input = {0x0100, 0x0302, 0x0504, 0x0706};
    const std::vector<uint16_t> expected = {0x0100, 0x0302, 0x0504, 0x0706};
    const std::vector<uint16_t> output = ca::pack_vector<uint16_t>(input, 1);
    REQUIRE(expected == output);
  }

  SECTION("input 16 bits output 32 bits step 1") {
    const std::vector<uint16_t> input = {0x0100, 0x0302, 0x0504, 0x0706,
                                         0x0908, 0x0B0A, 0x0D0C, 0x0F0E};
    const std::vector<uint32_t> expected = {0x03020100, 0x07060504, 0x0B0A0908,
                                            0x0F0E0D0C};
    const std::vector<uint32_t> output = ca::pack_vector<uint32_t>(input, 1);
    REQUIRE(expected == output);
  }

  SECTION("input 32 bits output 32 bits step 1") {
    const std::vector<uint32_t> input = {0x03020100, 0x07060504, 0x0B0A0908,
                                         0x0F0E0D0C};
    const std::vector<uint32_t> expected = {0x03020100, 0x07060504, 0x0B0A0908,
                                            0x0F0E0D0C};
    const std::vector<uint32_t> output = ca::pack_vector<uint32_t>(input, 1);
    REQUIRE(expected == output);
  }

  SECTION("input 8 bits output 24 bits step 1") {
    struct CustomType {
      uint8_t s[3];
    };
    const std::vector<uint8_t> input = {0x00, 0x01, 0x02, 0x03, 0x04,
                                        0x05, 0x06, 0x07, 0x08};
    const std::vector<CustomType> expected = {
        {0x00, 0x01, 0x02}, {0x03, 0x04, 0x05}, {0x06, 0x07, 0x08}};
    const std::vector<CustomType> output =
        ca::pack_vector<CustomType>(input, 1);
    for (size_t i = 0; i < output.size(); ++i) {
      REQUIRE(expected[i].s[0] == output[i].s[0]);
      REQUIRE(expected[i].s[1] == output[i].s[1]);
      REQUIRE(expected[i].s[2] == output[i].s[2]);
    }
  }

  SECTION("input 8 bits output 8 bits step 2") {
    const std::vector<uint8_t> input = {0x00, 0x01, 0x02, 0x03};
    const std::vector<uint8_t> expected = {0x00, 0x01, 0x02, 0x03};
    const std::vector<uint8_t> output = ca::pack_vector<uint8_t>(input, 2);
    REQUIRE(expected == output);
  }

  SECTION("input 8 bits output 16 bits step 2") {
    const std::vector<uint8_t> input = {0x00, 0x01, 0x02, 0x03,
                                        0x04, 0x05, 0x06, 0x07};
    const std::vector<uint16_t> expected = {0x0200, 0x0301, 0x0604, 0x0705};
    const std::vector<uint16_t> output = ca::pack_vector<uint16_t>(input, 2);
    REQUIRE(expected == output);
  }

  SECTION("input 8 bits output 32 bits step 2") {
    const std::vector<uint8_t> input = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
                                        0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B,
                                        0x0C, 0x0D, 0x0E, 0x0F};
    const std::vector<uint32_t> expected = {0x06040200, 0x07050301, 0x0E0C0A08,
                                            0x0F0D0B09};
    const std::vector<uint32_t> output = ca::pack_vector<uint32_t>(input, 2);
    REQUIRE(expected == output);
  }

  SECTION("input 8 bits output 16 bits step 3") {
    const std::vector<uint8_t> input = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
                                        0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B};
    const std::vector<uint16_t> expected = {0x0300, 0x0401, 0x0502,
                                            0x0906, 0x0A07, 0xB08};
    const std::vector<uint16_t> output = ca::pack_vector<uint16_t>(input, 3);
    REQUIRE(expected == output);
  }

  SECTION("input output int cassian::Vector<int, 3> step 1") {
    const std::vector<int> input = {1, 2, 3, 4, 5, 6};
    const std::vector<ca::Vector<int, 3>> expected = {{1, 2, 3}, {4, 5, 6}};

    const std::vector<ca::Vector<int, 3>> output =
        ca::pack_vector<ca::Vector<int, 3>>(input, 1);
    REQUIRE(expected == output);
  }

  SECTION("input cassian::Vector output int step 2") {
    const std::vector<int> input = {1, 4, 2, 5, 3, 6};
    const std::vector<ca::Vector<int, 3>> expected = {{1, 2, 3}, {4, 5, 6}};

    const std::vector<ca::Vector<int, 3>> output =
        ca::pack_vector<ca::Vector<int, 3>>(input, 2);
    REQUIRE(expected == output);
  }

  SECTION("input int output cassian::Vector<int, 3, 4> step 1") {
    const std::vector<int> input = {1, 2, 3, 4, 5, 6};
    const std::vector<ca::Vector<int, 3, 4>> expected = {{1, 2, 3}, {4, 5, 6}};

    const std::vector<ca::Vector<int, 3, 4>> output =
        ca::pack_vector<ca::Vector<int, 3, 4>>(input, 1);
    REQUIRE(expected == output);
  }

  SECTION("input int output cassian::Vector<int, 3, 4> step 2") {
    const std::vector<int> input = {1, 4, 2, 5, 3, 6};
    const std::vector<ca::Vector<int, 3, 4>> expected = {{1, 2, 3}, {4, 5, 6}};

    const std::vector<ca::Vector<int, 3, 4>> output =
        ca::pack_vector<ca::Vector<int, 3, 4>>(input, 2);
    REQUIRE(expected == output);
  }
}

TEST_CASE("unpack_vector", "") {

  SECTION("input 8 bits output 8 bits step 1") {
    const std::vector<uint8_t> input = {0x00, 0x01, 0x02, 0x03};
    const std::vector<uint8_t> expected = {0x00, 0x01, 0x02, 0x03};

    const std::vector<uint8_t> output = ca::unpack_vector<uint8_t>(input, 1);
    REQUIRE(expected == output);
  }

  SECTION("input 16 bits output 8 bits step 1") {
    const std::vector<uint16_t> input = {0x0100, 0x0302, 0x0504, 0x0706};
    const std::vector<uint8_t> expected = {0x00, 0x01, 0x02, 0x03,
                                           0x04, 0x05, 0x06, 0x07};

    const std::vector<uint8_t> output = ca::unpack_vector<uint8_t>(input, 1);
    REQUIRE(expected == output);
  }

  SECTION("input 32 bits output 8 bits step 1") {
    const std::vector<uint32_t> input = {0x03020100, 0x07060504, 0x0B0A0908,
                                         0x0F0E0D0C};
    const std::vector<uint8_t> expected = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
                                           0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B,
                                           0x0C, 0x0D, 0x0E, 0x0F};

    const std::vector<uint8_t> output = ca::unpack_vector<uint8_t>(input, 1);
    REQUIRE(expected == output);
  }

  SECTION("input 16 bits output 16 bits step 1") {
    const std::vector<uint16_t> input = {0x0100, 0x0302, 0x0504, 0x0706};
    const std::vector<uint16_t> expected = {0x0100, 0x0302, 0x0504, 0x0706};

    const std::vector<uint16_t> output = ca::unpack_vector<uint16_t>(input, 1);
    REQUIRE(expected == output);
  }

  SECTION("input 32 bits output 16 bits step 1") {
    const std::vector<uint32_t> input = {0x03020100, 0x07060504, 0x0B0A0908,
                                         0x0F0E0D0C};
    const std::vector<uint16_t> expected = {0x0100, 0x0302, 0x0504, 0x0706,
                                            0x0908, 0x0B0A, 0x0D0C, 0x0F0E};

    const std::vector<uint16_t> output = ca::unpack_vector<uint16_t>(input, 1);
    REQUIRE(expected == output);
  }

  SECTION("input 32 bits output 32 bits step 1") {
    const std::vector<uint32_t> input = {0x03020100, 0x07060504, 0x0B0A0908,
                                         0x0F0E0D0C};
    const std::vector<uint32_t> expected = {0x03020100, 0x07060504, 0x0B0A0908,
                                            0x0F0E0D0C};
    const std::vector<uint32_t> output = ca::unpack_vector<uint32_t>(input, 1);
    REQUIRE(expected == output);
  }

  SECTION("input 24 bits output 8 bits step 1") {
    struct CustomType {
      uint8_t s[3];
    };
    const std::vector<CustomType> input = {
        {0x00, 0x01, 0x02}, {0x03, 0x04, 0x05}, {0x06, 0x07, 0x08}};
    const std::vector<uint8_t> expected = {0x00, 0x01, 0x02, 0x03, 0x04,
                                           0x05, 0x06, 0x07, 0x08};

    const std::vector<uint8_t> output = ca::unpack_vector<uint8_t>(input, 1);
    REQUIRE(expected == output);
  }

  SECTION("input 8 bits output 8 bits step 2") {
    const std::vector<uint8_t> input = {0x00, 0x01, 0x02, 0x03};
    const std::vector<uint8_t> expected = {0x00, 0x01, 0x02, 0x03};

    const std::vector<uint8_t> output = ca::unpack_vector<uint8_t>(input, 2);
    REQUIRE(expected == output);
  }

  SECTION("input 16 bits output 8 bits step 2") {
    const std::vector<uint16_t> input = {0x0200, 0x0301, 0x0604, 0x0705};
    const std::vector<uint8_t> expected = {0x00, 0x01, 0x02, 0x03,
                                           0x04, 0x05, 0x06, 0x07};

    const std::vector<uint8_t> output = ca::unpack_vector<uint8_t>(input, 2);
    REQUIRE(expected == output);
  }

  SECTION("input 32 bits output 8 bits step 2") {
    const std::vector<uint32_t> input = {0x06040200, 0x07050301, 0x0E0C0A08,
                                         0x0F0D0B09};
    const std::vector<uint8_t> expected = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
                                           0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B,
                                           0x0C, 0x0D, 0x0E, 0x0F};

    const std::vector<uint8_t> output = ca::unpack_vector<uint8_t>(input, 2);
    REQUIRE(expected == output);
  }

  SECTION("input 16 bits output 8 bits step 3") {
    const std::vector<uint16_t> input = {0x0300, 0x0401, 0x0502,
                                         0x0906, 0x0A07, 0xB08};
    const std::vector<uint8_t> expected = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
                                           0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B};

    const std::vector<uint8_t> output = ca::unpack_vector<uint8_t>(input, 3);
    REQUIRE(expected == output);
  }

  SECTION("input cassian::Vector<int, 3> output int step 1") {
    const std::vector<ca::Vector<int, 3>> input = {{1, 2, 3}, {4, 5, 6}};
    const std::vector<int> expected = {1, 2, 3, 4, 5, 6};

    const std::vector<int> output = ca::unpack_vector<int>(input, 1);
    REQUIRE(expected == output);
  }

  SECTION("input cassian::Vector output int step 2") {
    const std::vector<ca::Vector<int, 3>> input = {{1, 2, 3}, {4, 5, 6}};
    const std::vector<int> expected = {1, 4, 2, 5, 3, 6};

    const std::vector<int> output = ca::unpack_vector<int>(input, 2);
    REQUIRE(expected == output);
  }

  SECTION("input cassian::Vector<int, 3, 4> output int step 1") {
    const std::vector<ca::Vector<int, 3, 4>> input = {{1, 2, 3}, {4, 5, 6}};
    const std::vector<int> expected = {1, 2, 3, 4, 5, 6};

    const std::vector<int> output = ca::unpack_vector<int>(input, 1);
    REQUIRE(expected == output);
  }

  SECTION("input cassian::Vector<int, 3, 4> output int step 2") {
    const std::vector<ca::Vector<int, 3, 4>> input = {{1, 2, 3}, {4, 5, 6}};
    const std::vector<int> expected = {1, 4, 2, 5, 3, 6};

    const std::vector<int> output = ca::unpack_vector<int>(input, 2);
    REQUIRE(expected == output);
  }
}

TEST_CASE("to_string") {
  SECTION("std::array - zero elements") {
    const std::array<size_t, 0> input = {};
    const std::string reference = "{}";
    const std::string output = ca::to_string(input);
    REQUIRE(output == reference);
  }
  SECTION("std::array - one element") {
    const std::array<size_t, 1> input = {1};
    const std::string reference = "{1}";
    const std::string output = ca::to_string(input);
    REQUIRE(output == reference);
  }
  SECTION("std::array - two elements") {
    const std::array<size_t, 2> input = {1, 2};
    const std::string reference = "{1, 2}";
    const std::string output = ca::to_string(input);
    REQUIRE(output == reference);
  }
  SECTION("std::array - multiple elements") {
    const std::array<size_t, 5> input = {1, 2, 3, 4, 5};
    const std::string reference = "{1, 2, 3, 4, 5}";
    const std::string output = ca::to_string(input);
    REQUIRE(output == reference);
  }
}

TEMPLATE_TEST_CASE("to_string_switch", "", ca::clc_int_t, ca::clc_int4_t) {
  using host_type = typename TestType::host_type;
  const auto input = host_type(1);
  const std::string reference =
      ca::is_vector_v<host_type> ? "{1, 1, 1, 1}" : "1";
  const std::string output = ca::to_string(input);
  REQUIRE(output == reference);
}

TEST_CASE("convert_to_forward_slashes") {
  SECTION("string with back slashes") {
    const std::string input = R"(C:\path\to\something)";
    const std::string reference = "C:/path/to/something";
    const std::string output = ca::convert_to_forward_slashes(input);
    REQUIRE(output == reference);
  }
  SECTION("string with forward slashes") {
    const std::string input = "c/path/to/something";
    const std::string reference = "c/path/to/something";
    const std::string output = ca::convert_to_forward_slashes(input);
    REQUIRE(output == reference);
  }
  SECTION("string with mixed slashes") {
    const std::string input = "C:\\path/to\\something";
    const std::string reference = "C:/path/to/something";
    const std::string output = ca::convert_to_forward_slashes(input);
    REQUIRE(output == reference);
  }
}

} // namespace
