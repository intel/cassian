/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/runtime/openclc_types.hpp>
#include <catch2/catch.hpp>

namespace ca = cassian;

TEST_CASE("device_type", "") {
  SECTION("char") {
    REQUIRE_THAT(ca::clc_char_t::device_type, Catch::Equals("char"));
  }
  SECTION("char2") {
    REQUIRE_THAT(ca::clc_char2_t::device_type, Catch::Equals("char2"));
  }
  SECTION("char3") {
    REQUIRE_THAT(ca::clc_char3_t::device_type, Catch::Equals("char3"));
  }
  SECTION("char4") {
    REQUIRE_THAT(ca::clc_char4_t::device_type, Catch::Equals("char4"));
  }
  SECTION("char8") {
    REQUIRE_THAT(ca::clc_char8_t::device_type, Catch::Equals("char8"));
  }
  SECTION("char16") {
    REQUIRE_THAT(ca::clc_char16_t::device_type, Catch::Equals("char16"));
  }

  SECTION("uchar") {
    REQUIRE_THAT(ca::clc_uchar_t::device_type, Catch::Equals("uchar"));
  }
  SECTION("uchar2") {
    REQUIRE_THAT(ca::clc_uchar2_t::device_type, Catch::Equals("uchar2"));
  }
  SECTION("uchar3") {
    REQUIRE_THAT(ca::clc_uchar3_t::device_type, Catch::Equals("uchar3"));
  }
  SECTION("uchar4") {
    REQUIRE_THAT(ca::clc_uchar4_t::device_type, Catch::Equals("uchar4"));
  }
  SECTION("uchar8") {
    REQUIRE_THAT(ca::clc_uchar8_t::device_type, Catch::Equals("uchar8"));
  }
  SECTION("uchar16") {
    REQUIRE_THAT(ca::clc_uchar16_t::device_type, Catch::Equals("uchar16"));
  }

  SECTION("short") {
    REQUIRE_THAT(ca::clc_short_t::device_type, Catch::Equals("short"));
  }
  SECTION("short2") {
    REQUIRE_THAT(ca::clc_short2_t::device_type, Catch::Equals("short2"));
  }
  SECTION("short3") {
    REQUIRE_THAT(ca::clc_short3_t::device_type, Catch::Equals("short3"));
  }
  SECTION("short4") {
    REQUIRE_THAT(ca::clc_short4_t::device_type, Catch::Equals("short4"));
  }
  SECTION("short8") {
    REQUIRE_THAT(ca::clc_short8_t::device_type, Catch::Equals("short8"));
  }
  SECTION("short16") {
    REQUIRE_THAT(ca::clc_short16_t::device_type, Catch::Equals("short16"));
  }

  SECTION("ushort") {
    REQUIRE_THAT(ca::clc_ushort_t::device_type, Catch::Equals("ushort"));
  }
  SECTION("ushort2") {
    REQUIRE_THAT(ca::clc_ushort2_t::device_type, Catch::Equals("ushort2"));
  }
  SECTION("ushort3") {
    REQUIRE_THAT(ca::clc_ushort3_t::device_type, Catch::Equals("ushort3"));
  }
  SECTION("ushort4") {
    REQUIRE_THAT(ca::clc_ushort4_t::device_type, Catch::Equals("ushort4"));
  }
  SECTION("ushort8") {
    REQUIRE_THAT(ca::clc_ushort8_t::device_type, Catch::Equals("ushort8"));
  }
  SECTION("ushort16") {
    REQUIRE_THAT(ca::clc_ushort16_t::device_type, Catch::Equals("ushort16"));
  }

  SECTION("int") {
    REQUIRE_THAT(ca::clc_int_t::device_type, Catch::Equals("int"));
  }
  SECTION("int2") {
    REQUIRE_THAT(ca::clc_int2_t::device_type, Catch::Equals("int2"));
  }
  SECTION("int3") {
    REQUIRE_THAT(ca::clc_int3_t::device_type, Catch::Equals("int3"));
  }
  SECTION("int4") {
    REQUIRE_THAT(ca::clc_int4_t::device_type, Catch::Equals("int4"));
  }
  SECTION("int8") {
    REQUIRE_THAT(ca::clc_int8_t::device_type, Catch::Equals("int8"));
  }
  SECTION("int16") {
    REQUIRE_THAT(ca::clc_int16_t::device_type, Catch::Equals("int16"));
  }

  SECTION("uint") {
    REQUIRE_THAT(ca::clc_uint_t::device_type, Catch::Equals("uint"));
  }
  SECTION("uint2") {
    REQUIRE_THAT(ca::clc_uint2_t::device_type, Catch::Equals("uint2"));
  }
  SECTION("uint3") {
    REQUIRE_THAT(ca::clc_uint3_t::device_type, Catch::Equals("uint3"));
  }
  SECTION("uint4") {
    REQUIRE_THAT(ca::clc_uint4_t::device_type, Catch::Equals("uint4"));
  }
  SECTION("uint8") {
    REQUIRE_THAT(ca::clc_uint8_t::device_type, Catch::Equals("uint8"));
  }
  SECTION("uint16") {
    REQUIRE_THAT(ca::clc_uint16_t::device_type, Catch::Equals("uint16"));
  }

  SECTION("long") {
    REQUIRE_THAT(ca::clc_long_t::device_type, Catch::Equals("long"));
  }
  SECTION("long2") {
    REQUIRE_THAT(ca::clc_long2_t::device_type, Catch::Equals("long2"));
  }
  SECTION("long3") {
    REQUIRE_THAT(ca::clc_long3_t::device_type, Catch::Equals("long3"));
  }
  SECTION("long4") {
    REQUIRE_THAT(ca::clc_long4_t::device_type, Catch::Equals("long4"));
  }
  SECTION("long8") {
    REQUIRE_THAT(ca::clc_long8_t::device_type, Catch::Equals("long8"));
  }
  SECTION("long16") {
    REQUIRE_THAT(ca::clc_long16_t::device_type, Catch::Equals("long16"));
  }

  SECTION("ulong") {
    REQUIRE_THAT(ca::clc_ulong_t::device_type, Catch::Equals("ulong"));
  }
  SECTION("ulong2") {
    REQUIRE_THAT(ca::clc_ulong2_t::device_type, Catch::Equals("ulong2"));
  }
  SECTION("ulong3") {
    REQUIRE_THAT(ca::clc_ulong3_t::device_type, Catch::Equals("ulong3"));
  }
  SECTION("ulong4") {
    REQUIRE_THAT(ca::clc_ulong4_t::device_type, Catch::Equals("ulong4"));
  }
  SECTION("ulong8") {
    REQUIRE_THAT(ca::clc_ulong8_t::device_type, Catch::Equals("ulong8"));
  }
  SECTION("ulong16") {
    REQUIRE_THAT(ca::clc_ulong16_t::device_type, Catch::Equals("ulong16"));
  }

  SECTION("float") {
    REQUIRE_THAT(ca::clc_float_t::device_type, Catch::Equals("float"));
  }
  SECTION("float2") {
    REQUIRE_THAT(ca::clc_float2_t::device_type, Catch::Equals("float2"));
  }
  SECTION("float3") {
    REQUIRE_THAT(ca::clc_float3_t::device_type, Catch::Equals("float3"));
  }
  SECTION("float4") {
    REQUIRE_THAT(ca::clc_float4_t::device_type, Catch::Equals("float4"));
  }
  SECTION("float8") {
    REQUIRE_THAT(ca::clc_float8_t::device_type, Catch::Equals("float8"));
  }
  SECTION("float16") {
    REQUIRE_THAT(ca::clc_float16_t::device_type, Catch::Equals("float16"));
  }

  SECTION("double") {
    REQUIRE_THAT(ca::clc_double_t::device_type, Catch::Equals("double"));
  }
  SECTION("double2") {
    REQUIRE_THAT(ca::clc_double2_t::device_type, Catch::Equals("double2"));
  }
  SECTION("double3") {
    REQUIRE_THAT(ca::clc_double3_t::device_type, Catch::Equals("double3"));
  }
  SECTION("double4") {
    REQUIRE_THAT(ca::clc_double4_t::device_type, Catch::Equals("double4"));
  }
  SECTION("double8") {
    REQUIRE_THAT(ca::clc_double8_t::device_type, Catch::Equals("double8"));
  }
  SECTION("double16") {
    REQUIRE_THAT(ca::clc_double16_t::device_type, Catch::Equals("double16"));
  }

  SECTION("half") {
    REQUIRE_THAT(ca::clc_half_t::device_type, Catch::Equals("half"));
  }
  SECTION("half2") {
    REQUIRE_THAT(ca::clc_half2_t::device_type, Catch::Equals("half2"));
  }
  SECTION("half3") {
    REQUIRE_THAT(ca::clc_half3_t::device_type, Catch::Equals("half3"));
  }
  SECTION("half4") {
    REQUIRE_THAT(ca::clc_half4_t::device_type, Catch::Equals("half4"));
  }
  SECTION("half8") {
    REQUIRE_THAT(ca::clc_half8_t::device_type, Catch::Equals("half8"));
  }
  SECTION("half16") {
    REQUIRE_THAT(ca::clc_half16_t::device_type, Catch::Equals("half16"));
  }

  SECTION("bool") {
    REQUIRE_THAT(ca::clc_bool_t::device_type, Catch::Equals("bool"));
  }
  SECTION("size_t") {
    REQUIRE_THAT(ca::clc_size_t::device_type, Catch::Equals("size_t"));
  }
  SECTION("ptrdiff_t") {
    REQUIRE_THAT(ca::clc_ptrdiff_t::device_type, Catch::Equals("ptrdiff_t"));
  }
  SECTION("intptr_t") {
    REQUIRE_THAT(ca::clc_intptr_t::device_type, Catch::Equals("intptr_t"));
  }
  SECTION("uintptr_t") {
    REQUIRE_THAT(ca::clc_uintptr_t::device_type, Catch::Equals("uintptr_t"));
  }
  SECTION("void") {
    REQUIRE_THAT(ca::clc_void_t::device_type, Catch::Equals("void"));
  }
}

TEST_CASE("atomic_type", "") {
  SECTION("int") {
    REQUIRE_THAT(ca::clc_int_t::device_atomic_type,
                 Catch::Equals("atomic_int"));
  }
  SECTION("uint") {
    REQUIRE_THAT(ca::clc_uint_t::device_atomic_type,
                 Catch::Equals("atomic_uint"));
  }

  SECTION("long") {
    REQUIRE_THAT(ca::clc_long_t::device_atomic_type,
                 Catch::Equals("atomic_long"));
  }
  SECTION("ulong") {
    REQUIRE_THAT(ca::clc_ulong_t::device_atomic_type,
                 Catch::Equals("atomic_ulong"));
  }

  SECTION("float") {
    REQUIRE_THAT(ca::clc_float_t::device_atomic_type,
                 Catch::Equals("atomic_float"));
  }
  SECTION("double") {
    REQUIRE_THAT(ca::clc_double_t::device_atomic_type,
                 Catch::Equals("atomic_double"));
  }

  SECTION("size_t") {
    REQUIRE_THAT(ca::clc_size_t::device_atomic_type,
                 Catch::Equals("atomic_size_t"));
  }
  SECTION("ptrdiff_t") {
    REQUIRE_THAT(ca::clc_ptrdiff_t::device_atomic_type,
                 Catch::Equals("atomic_ptrdiff_t"));
  }
  SECTION("intptr_t") {
    REQUIRE_THAT(ca::clc_intptr_t::device_atomic_type,
                 Catch::Equals("atomic_intptr_t"));
  }
  SECTION("uintptr_t") {
    REQUIRE_THAT(ca::clc_uintptr_t::device_atomic_type,
                 Catch::Equals("atomic_uintptr_t"));
  }
}
