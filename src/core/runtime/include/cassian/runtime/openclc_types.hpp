/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_RUNTIME_OPENCLC_TYPES_HPP
#define CASSIAN_RUNTIME_OPENCLC_TYPES_HPP

#include <cassian/fp_types/bfloat.hpp>
#include <cassian/fp_types/half.hpp>
#include <cassian/fp_types/tfloat.hpp>
#include <cassian/vector/vector.hpp>

/**
 * Cassian namespace.
 */
namespace cassian {

namespace detail {

template <size_t N> struct OpenCLCChar {};
template <size_t N> struct OpenCLCUchar {};
template <size_t N> struct OpenCLCShort {};
template <size_t N> struct OpenCLCUshort {};
template <size_t N> struct OpenCLCInt {};
template <size_t N> struct OpenCLCUint {};
template <size_t N> struct OpenCLCLong {};
template <size_t N> struct OpenCLCUlong {};
template <size_t N> struct OpenCLCHalf {};
template <size_t N> struct OpenCLCFloat {};
template <size_t N> struct OpenCLCDouble {};
template <size_t N> struct OpenCLCBfloat16 {};
template <size_t N> struct OpenCLCTfloat {};

template <> struct OpenCLCUchar<1> {
  using host_type = uint8_t;
  using underlying_type = uint8_t;
  using scalar_type = OpenCLCUchar<1>;
  using logical_type = OpenCLCChar<1>;
  using unsigned_type = OpenCLCUchar<1>;
  using signed_type = OpenCLCChar<1>;
  static constexpr char device_type[] = "uchar";
  static constexpr char type_name[] = "uchar";
  static constexpr char type_abbr[] = "u8";
};

template <> struct OpenCLCUchar<2> {
  using host_type = Vector<uint8_t, 2>;
  using underlying_type = uint8_t;
  using scalar_type = OpenCLCUchar<1>;
  using logical_type = OpenCLCChar<2>;
  using unsigned_type = OpenCLCUchar<2>;
  using signed_type = OpenCLCChar<2>;
  static constexpr char device_type[] = "uchar2";
  static constexpr char type_name[] = "uchar2";
  static constexpr char type_abbr[] = "u8";
};

template <> struct OpenCLCUchar<3> {
  using host_type = Vector<uint8_t, 3, 4>;
  using underlying_type = uint8_t;
  using scalar_type = OpenCLCUchar<1>;
  using logical_type = OpenCLCChar<3>;
  using unsigned_type = OpenCLCUchar<3>;
  using signed_type = OpenCLCChar<3>;
  static constexpr char device_type[] = "uchar3";
  static constexpr char type_name[] = "uchar3";
  static constexpr char type_abbr[] = "u8";
};

template <> struct OpenCLCUchar<4> {
  using host_type = Vector<uint8_t, 4>;
  using underlying_type = uint8_t;
  using scalar_type = OpenCLCUchar<1>;
  using logical_type = OpenCLCChar<4>;
  using unsigned_type = OpenCLCUchar<4>;
  using signed_type = OpenCLCChar<4>;
  static constexpr char device_type[] = "uchar4";
  static constexpr char type_name[] = "uchar4";
  static constexpr char type_abbr[] = "u8";
};

template <> struct OpenCLCUchar<8> {
  using host_type = Vector<uint8_t, 8>;
  using underlying_type = uint8_t;
  using scalar_type = OpenCLCUchar<1>;
  using logical_type = OpenCLCChar<8>;
  using unsigned_type = OpenCLCUchar<8>;
  using signed_type = OpenCLCChar<8>;
  static constexpr char device_type[] = "uchar8";
  static constexpr char type_name[] = "uchar8";
  static constexpr char type_abbr[] = "u8";
};

template <> struct OpenCLCUchar<16> {
  using host_type = Vector<uint8_t, 16>;
  using underlying_type = uint8_t;
  using scalar_type = OpenCLCUchar<1>;
  using logical_type = OpenCLCChar<16>;
  using unsigned_type = OpenCLCUchar<16>;
  using signed_type = OpenCLCChar<16>;
  static constexpr char device_type[] = "uchar16";
  static constexpr char type_name[] = "uchar16";
  static constexpr char type_abbr[] = "u8";
};

template <> struct OpenCLCChar<1> {
  using host_type = int8_t;
  using underlying_type = int8_t;
  using scalar_type = OpenCLCChar<1>;
  using logical_type = OpenCLCChar<1>;
  using unsigned_type = OpenCLCUchar<1>;
  using signed_type = OpenCLCChar<1>;
  static constexpr char device_type[] = "char";
  static constexpr char type_name[] = "char";
  static constexpr char type_abbr[] = "i8";
};

template <> struct OpenCLCChar<2> {
  using host_type = Vector<int8_t, 2>;
  using underlying_type = int8_t;
  using scalar_type = OpenCLCChar<1>;
  using logical_type = OpenCLCChar<2>;
  using unsigned_type = OpenCLCUchar<2>;
  using signed_type = OpenCLCChar<2>;
  static constexpr char device_type[] = "char2";
  static constexpr char type_name[] = "char2";
  static constexpr char type_abbr[] = "i8";
};

template <> struct OpenCLCChar<3> {
  using host_type = Vector<int8_t, 3, 4>;
  using underlying_type = int8_t;
  using scalar_type = OpenCLCChar<1>;
  using logical_type = OpenCLCChar<3>;
  using unsigned_type = OpenCLCUchar<3>;
  using signed_type = OpenCLCChar<3>;
  static constexpr char device_type[] = "char3";
  static constexpr char type_name[] = "char3";
  static constexpr char type_abbr[] = "i8";
};

template <> struct OpenCLCChar<4> {
  using host_type = Vector<int8_t, 4>;
  using underlying_type = int8_t;
  using scalar_type = OpenCLCChar<1>;
  using logical_type = OpenCLCChar<4>;
  using unsigned_type = OpenCLCUchar<4>;
  using signed_type = OpenCLCChar<4>;
  static constexpr char device_type[] = "char4";
  static constexpr char type_name[] = "char4";
  static constexpr char type_abbr[] = "i8";
};

template <> struct OpenCLCChar<8> {
  using host_type = Vector<int8_t, 8>;
  using underlying_type = int8_t;
  using scalar_type = OpenCLCChar<1>;
  using logical_type = OpenCLCChar<8>;
  using unsigned_type = OpenCLCUchar<8>;
  using signed_type = OpenCLCChar<8>;
  static constexpr char device_type[] = "char8";
  static constexpr char type_name[] = "char8";
  static constexpr char type_abbr[] = "i8";
};

template <> struct OpenCLCChar<16> {
  using host_type = Vector<int8_t, 16>;
  using underlying_type = int8_t;
  using scalar_type = OpenCLCChar<1>;
  using logical_type = OpenCLCChar<16>;
  using unsigned_type = OpenCLCUchar<16>;
  using signed_type = OpenCLCChar<16>;
  static constexpr char device_type[] = "char16";
  static constexpr char type_name[] = "char16";
  static constexpr char type_abbr[] = "i8";
};

template <> struct OpenCLCUshort<1> {
  using host_type = uint16_t;
  using underlying_type = uint16_t;
  using scalar_type = OpenCLCUshort<1>;
  using logical_type = OpenCLCShort<1>;
  using unsigned_type = OpenCLCUshort<1>;
  using signed_type = OpenCLCShort<1>;
  using regressed_type = OpenCLCChar<1>;
  static constexpr char device_type[] = "ushort";
  static constexpr char type_name[] = "ushort";
  static constexpr char type_abbr[] = "u16";
};

template <> struct OpenCLCUshort<2> {
  using host_type = Vector<uint16_t, 2>;
  using underlying_type = uint16_t;
  using scalar_type = OpenCLCUshort<1>;
  using logical_type = OpenCLCShort<2>;
  using unsigned_type = OpenCLCUshort<2>;
  using signed_type = OpenCLCShort<2>;
  using regressed_type = OpenCLCChar<2>;
  static constexpr char device_type[] = "ushort2";
  static constexpr char type_name[] = "ushort2";
  static constexpr char type_abbr[] = "u16";
};

template <> struct OpenCLCUshort<3> {
  using host_type = Vector<uint16_t, 3, 4>;
  using underlying_type = uint16_t;
  using scalar_type = OpenCLCUshort<1>;
  using logical_type = OpenCLCShort<3>;
  using unsigned_type = OpenCLCUshort<3>;
  using signed_type = OpenCLCShort<3>;
  using regressed_type = OpenCLCChar<3>;
  static constexpr char device_type[] = "ushort3";
  static constexpr char type_name[] = "ushort3";
  static constexpr char type_abbr[] = "u16";
};

template <> struct OpenCLCUshort<4> {
  using host_type = Vector<uint16_t, 4>;
  using underlying_type = uint16_t;
  using scalar_type = OpenCLCUshort<1>;
  using logical_type = OpenCLCShort<4>;
  using unsigned_type = OpenCLCUshort<4>;
  using signed_type = OpenCLCShort<4>;
  using regressed_type = OpenCLCChar<4>;
  static constexpr char device_type[] = "ushort4";
  static constexpr char type_name[] = "ushort4";
  static constexpr char type_abbr[] = "u16";
};

template <> struct OpenCLCUshort<8> {
  using host_type = Vector<uint16_t, 8>;
  using underlying_type = uint16_t;
  using scalar_type = OpenCLCUshort<1>;
  using logical_type = OpenCLCShort<8>;
  using unsigned_type = OpenCLCUshort<8>;
  using signed_type = OpenCLCShort<8>;
  using regressed_type = OpenCLCChar<8>;
  static constexpr char device_type[] = "ushort8";
  static constexpr char type_name[] = "ushort8";
  static constexpr char type_abbr[] = "u16";
};

template <> struct OpenCLCUshort<16> {
  using host_type = Vector<uint16_t, 16>;
  using underlying_type = uint16_t;
  using scalar_type = OpenCLCUshort<1>;
  using logical_type = OpenCLCShort<16>;
  using unsigned_type = OpenCLCUshort<16>;
  using signed_type = OpenCLCShort<16>;
  using regressed_type = OpenCLCChar<16>;
  static constexpr char device_type[] = "ushort16";
  static constexpr char type_name[] = "ushort16";
  static constexpr char type_abbr[] = "u16";
};

template <> struct OpenCLCShort<1> {
  using host_type = int16_t;
  using underlying_type = int16_t;
  using scalar_type = OpenCLCShort<1>;
  using logical_type = OpenCLCShort<1>;
  using unsigned_type = OpenCLCUshort<1>;
  using signed_type = OpenCLCShort<1>;
  using regressed_type = OpenCLCChar<1>;
  static constexpr char device_type[] = "short";
  static constexpr char type_name[] = "short";
  static constexpr char type_abbr[] = "i16";
};

template <> struct OpenCLCShort<2> {
  using host_type = Vector<int16_t, 2>;
  using underlying_type = int16_t;
  using scalar_type = OpenCLCShort<1>;
  using logical_type = OpenCLCShort<2>;
  using unsigned_type = OpenCLCUshort<2>;
  using signed_type = OpenCLCShort<2>;
  using regressed_type = OpenCLCChar<2>;
  static constexpr char device_type[] = "short2";
  static constexpr char type_name[] = "short2";
  static constexpr char type_abbr[] = "i16";
};

template <> struct OpenCLCShort<3> {
  using host_type = Vector<int16_t, 3, 4>;
  using underlying_type = int16_t;
  using scalar_type = OpenCLCShort<1>;
  using logical_type = OpenCLCShort<3>;
  using unsigned_type = OpenCLCUshort<3>;
  using signed_type = OpenCLCShort<3>;
  using regressed_type = OpenCLCChar<3>;
  static constexpr char device_type[] = "short3";
  static constexpr char type_name[] = "short3";
  static constexpr char type_abbr[] = "i16";
};

template <> struct OpenCLCShort<4> {
  using host_type = Vector<int16_t, 4>;
  using underlying_type = int16_t;
  using scalar_type = OpenCLCShort<1>;
  using logical_type = OpenCLCShort<4>;
  using unsigned_type = OpenCLCUshort<4>;
  using signed_type = OpenCLCShort<4>;
  using regressed_type = OpenCLCChar<4>;
  static constexpr char device_type[] = "short4";
  static constexpr char type_name[] = "short4";
  static constexpr char type_abbr[] = "i16";
};

template <> struct OpenCLCShort<8> {
  using host_type = Vector<int16_t, 8>;
  using underlying_type = int16_t;
  using scalar_type = OpenCLCShort<1>;
  using logical_type = OpenCLCShort<8>;
  using unsigned_type = OpenCLCUshort<8>;
  using signed_type = OpenCLCShort<8>;
  using regressed_type = OpenCLCChar<8>;
  static constexpr char device_type[] = "short8";
  static constexpr char type_name[] = "short8";
  static constexpr char type_abbr[] = "i16";
};

template <> struct OpenCLCShort<16> {
  using host_type = Vector<int16_t, 16>;
  using underlying_type = int16_t;
  using scalar_type = OpenCLCShort<1>;
  using logical_type = OpenCLCShort<16>;
  using unsigned_type = OpenCLCUshort<16>;
  using signed_type = OpenCLCShort<16>;
  using regressed_type = OpenCLCChar<16>;
  static constexpr char device_type[] = "short16";
  static constexpr char type_name[] = "short16";
  static constexpr char type_abbr[] = "i16";
};

template <> struct OpenCLCUint<1> {
  using host_type = uint32_t;
  using scalar_type = OpenCLCUint<1>;
  using underlying_type = uint32_t;
  using logical_type = OpenCLCInt<1>;
  using unsigned_type = OpenCLCUint<1>;
  using signed_type = OpenCLCInt<1>;
  using regressed_type = OpenCLCShort<1>;
  static constexpr char device_type[] = "uint";
  static constexpr char type_name[] = "uint";
  static constexpr char type_abbr[] = "u32";
  static constexpr char device_atomic_type[] = "atomic_uint";
};

template <> struct OpenCLCUint<2> {
  using host_type = Vector<uint32_t, 2>;
  using underlying_type = uint32_t;
  using scalar_type = OpenCLCUint<1>;
  using logical_type = OpenCLCInt<2>;
  using unsigned_type = OpenCLCUint<2>;
  using signed_type = OpenCLCInt<2>;
  using regressed_type = OpenCLCShort<2>;
  static constexpr char device_type[] = "uint2";
  static constexpr char type_name[] = "uint2";
  static constexpr char type_abbr[] = "u32";
};

template <> struct OpenCLCUint<3> {
  using host_type = Vector<uint32_t, 3, 4>;
  using underlying_type = uint32_t;
  using scalar_type = OpenCLCUint<1>;
  using logical_type = OpenCLCInt<3>;
  using unsigned_type = OpenCLCUint<3>;
  using signed_type = OpenCLCInt<3>;
  using regressed_type = OpenCLCShort<3>;
  static constexpr char device_type[] = "uint3";
  static constexpr char type_name[] = "uint3";
  static constexpr char type_abbr[] = "u32";
};

template <> struct OpenCLCUint<4> {
  using host_type = Vector<uint32_t, 4>;
  using underlying_type = uint32_t;
  using scalar_type = OpenCLCUint<1>;
  using logical_type = OpenCLCInt<4>;
  using unsigned_type = OpenCLCUint<4>;
  using signed_type = OpenCLCInt<4>;
  using regressed_type = OpenCLCShort<4>;
  static constexpr char device_type[] = "uint4";
  static constexpr char type_name[] = "uint4";
  static constexpr char type_abbr[] = "u32";
};

template <> struct OpenCLCUint<8> {
  using host_type = Vector<uint32_t, 8>;
  using underlying_type = uint32_t;
  using scalar_type = OpenCLCUint<1>;
  using logical_type = OpenCLCInt<8>;
  using unsigned_type = OpenCLCUint<8>;
  using signed_type = OpenCLCInt<8>;
  using regressed_type = OpenCLCShort<8>;
  static constexpr char device_type[] = "uint8";
  static constexpr char type_name[] = "uint8";
  static constexpr char type_abbr[] = "u32";
};

template <> struct OpenCLCUint<16> {
  using host_type = Vector<uint32_t, 16>;
  using underlying_type = uint32_t;
  using scalar_type = OpenCLCUint<1>;
  using logical_type = OpenCLCInt<16>;
  using unsigned_type = OpenCLCUint<16>;
  using signed_type = OpenCLCInt<16>;
  using regressed_type = OpenCLCShort<16>;
  static constexpr char device_type[] = "uint16";
  static constexpr char type_name[] = "uint16";
  static constexpr char type_abbr[] = "u32";
};

template <> struct OpenCLCInt<1> {
  using host_type = int32_t;
  using underlying_type = int32_t;
  using scalar_type = OpenCLCInt<1>;
  using logical_type = OpenCLCInt<1>;
  using unsigned_type = OpenCLCUint<1>;
  using signed_type = OpenCLCInt<1>;
  using regressed_type = OpenCLCShort<1>;
  static constexpr char device_type[] = "int";
  static constexpr char type_name[] = "int";
  static constexpr char type_abbr[] = "i32";
  static constexpr char device_atomic_type[] = "atomic_int";
};

template <> struct OpenCLCInt<2> {
  using host_type = Vector<int32_t, 2>;
  using underlying_type = int32_t;
  using scalar_type = OpenCLCInt<1>;
  using logical_type = OpenCLCInt<2>;
  using unsigned_type = OpenCLCUint<2>;
  using signed_type = OpenCLCInt<2>;
  using regressed_type = OpenCLCShort<2>;
  static constexpr char device_type[] = "int2";
  static constexpr char type_name[] = "int2";
  static constexpr char type_abbr[] = "i32";
};

template <> struct OpenCLCInt<3> {
  using host_type = Vector<int32_t, 3, 4>;
  using underlying_type = int32_t;
  using scalar_type = OpenCLCInt<1>;
  using logical_type = OpenCLCInt<3>;
  using unsigned_type = OpenCLCUint<3>;
  using signed_type = OpenCLCInt<3>;
  using regressed_type = OpenCLCShort<3>;
  static constexpr char device_type[] = "int3";
  static constexpr char type_name[] = "int3";
  static constexpr char type_abbr[] = "i32";
};

template <> struct OpenCLCInt<4> {
  using host_type = Vector<int32_t, 4>;
  using underlying_type = int32_t;
  using scalar_type = OpenCLCInt<1>;
  using logical_type = OpenCLCInt<4>;
  using unsigned_type = OpenCLCUint<4>;
  using signed_type = OpenCLCInt<4>;
  using regressed_type = OpenCLCShort<4>;
  static constexpr char device_type[] = "int4";
  static constexpr char type_name[] = "int4";
  static constexpr char type_abbr[] = "i32";
};

template <> struct OpenCLCInt<8> {
  using host_type = Vector<int32_t, 8>;
  using underlying_type = int32_t;
  using scalar_type = OpenCLCInt<1>;
  using logical_type = OpenCLCInt<8>;
  using unsigned_type = OpenCLCUint<8>;
  using signed_type = OpenCLCInt<8>;
  using regressed_type = OpenCLCShort<8>;
  static constexpr char device_type[] = "int8";
  static constexpr char type_name[] = "int8";
  static constexpr char type_abbr[] = "i32";
};

template <> struct OpenCLCInt<16> {
  using host_type = Vector<int32_t, 16>;
  using underlying_type = int32_t;
  using scalar_type = OpenCLCInt<1>;
  using logical_type = OpenCLCInt<16>;
  using unsigned_type = OpenCLCUint<16>;
  using signed_type = OpenCLCInt<16>;
  using regressed_type = OpenCLCShort<16>;
  static constexpr char device_type[] = "int16";
  static constexpr char type_name[] = "int16";
  static constexpr char type_abbr[] = "i32";
};

template <> struct OpenCLCUlong<1> {
  using host_type = uint64_t;
  using underlying_type = uint64_t;
  using scalar_type = OpenCLCUlong<1>;
  using logical_type = OpenCLCLong<1>;
  using unsigned_type = OpenCLCUlong<1>;
  using signed_type = OpenCLCLong<1>;
  using regressed_type = OpenCLCInt<1>;
  static constexpr char device_type[] = "ulong";
  static constexpr char type_name[] = "ulong";
  static constexpr char type_abbr[] = "u64";
  static constexpr char device_atomic_type[] = "atomic_ulong";
};

template <> struct OpenCLCUlong<2> {
  using host_type = Vector<uint64_t, 2>;
  using underlying_type = uint64_t;
  using scalar_type = OpenCLCUlong<1>;
  using logical_type = OpenCLCLong<2>;
  using unsigned_type = OpenCLCUlong<2>;
  using signed_type = OpenCLCLong<2>;
  using regressed_type = OpenCLCInt<2>;
  static constexpr char device_type[] = "ulong2";
  static constexpr char type_name[] = "ulong2";
  static constexpr char type_abbr[] = "u64";
};

template <> struct OpenCLCUlong<3> {
  using host_type = Vector<uint64_t, 3, 4>;
  using underlying_type = uint64_t;
  using scalar_type = OpenCLCUlong<1>;
  using logical_type = OpenCLCLong<3>;
  using unsigned_type = OpenCLCUlong<3>;
  using signed_type = OpenCLCLong<3>;
  using regressed_type = OpenCLCInt<3>;
  static constexpr char device_type[] = "ulong3";
  static constexpr char type_name[] = "ulong3";
  static constexpr char type_abbr[] = "u64";
};

template <> struct OpenCLCUlong<4> {
  using host_type = Vector<uint64_t, 4>;
  using underlying_type = uint64_t;
  using scalar_type = OpenCLCUlong<1>;
  using logical_type = OpenCLCLong<4>;
  using unsigned_type = OpenCLCUlong<4>;
  using signed_type = OpenCLCLong<4>;
  using regressed_type = OpenCLCInt<4>;
  static constexpr char device_type[] = "ulong4";
  static constexpr char type_name[] = "ulong4";
  static constexpr char type_abbr[] = "u64";
};

template <> struct OpenCLCUlong<8> {
  using host_type = Vector<uint64_t, 8>;
  using underlying_type = uint64_t;
  using scalar_type = OpenCLCUlong<1>;
  using logical_type = OpenCLCLong<8>;
  using unsigned_type = OpenCLCUlong<8>;
  using signed_type = OpenCLCLong<8>;
  using regressed_type = OpenCLCInt<8>;
  static constexpr char device_type[] = "ulong8";
  static constexpr char type_name[] = "ulong8";
  static constexpr char type_abbr[] = "u64";
};

template <> struct OpenCLCUlong<16> {
  using host_type = Vector<uint64_t, 16>;
  using underlying_type = uint64_t;
  using scalar_type = OpenCLCUlong<1>;
  using logical_type = OpenCLCLong<16>;
  using unsigned_type = OpenCLCUlong<16>;
  using signed_type = OpenCLCLong<16>;
  using regressed_type = OpenCLCInt<16>;
  static constexpr char device_type[] = "ulong16";
  static constexpr char type_name[] = "ulong16";
  static constexpr char type_abbr[] = "u64";
};

template <> struct OpenCLCLong<1> {
  using host_type = int64_t;
  using underlying_type = int64_t;
  using scalar_type = OpenCLCLong<1>;
  using logical_type = OpenCLCLong<1>;
  using unsigned_type = OpenCLCUlong<1>;
  using signed_type = OpenCLCLong<1>;
  using regressed_type = OpenCLCInt<1>;
  static constexpr char device_type[] = "long";
  static constexpr char type_name[] = "long";
  static constexpr char type_abbr[] = "i64";
  static constexpr char device_atomic_type[] = "atomic_long";
};

template <> struct OpenCLCLong<2> {
  using host_type = Vector<int64_t, 2>;
  using underlying_type = int64_t;
  using scalar_type = OpenCLCLong<1>;
  using logical_type = OpenCLCLong<2>;
  using unsigned_type = OpenCLCUlong<2>;
  using signed_type = OpenCLCLong<2>;
  using regressed_type = OpenCLCInt<2>;
  static constexpr char device_type[] = "long2";
  static constexpr char type_name[] = "long2";
  static constexpr char type_abbr[] = "i64";
};

template <> struct OpenCLCLong<3> {
  using host_type = Vector<int64_t, 3, 4>;
  using underlying_type = int64_t;
  using scalar_type = OpenCLCLong<1>;
  using logical_type = OpenCLCLong<3>;
  using unsigned_type = OpenCLCUlong<3>;
  using signed_type = OpenCLCLong<3>;
  using regressed_type = OpenCLCInt<3>;
  static constexpr char device_type[] = "long3";
  static constexpr char type_name[] = "long3";
  static constexpr char type_abbr[] = "i64";
};

template <> struct OpenCLCLong<4> {
  using host_type = Vector<int64_t, 4>;
  using underlying_type = int64_t;
  using scalar_type = OpenCLCLong<1>;
  using logical_type = OpenCLCLong<4>;
  using unsigned_type = OpenCLCUlong<4>;
  using signed_type = OpenCLCLong<4>;
  using regressed_type = OpenCLCInt<4>;
  static constexpr char device_type[] = "long4";
  static constexpr char type_name[] = "long4";
  static constexpr char type_abbr[] = "i64";
};

template <> struct OpenCLCLong<8> {
  using host_type = Vector<int64_t, 8>;
  using underlying_type = int64_t;
  using scalar_type = OpenCLCLong<1>;
  using logical_type = OpenCLCLong<8>;
  using unsigned_type = OpenCLCUlong<8>;
  using signed_type = OpenCLCLong<8>;
  using regressed_type = OpenCLCInt<8>;
  static constexpr char device_type[] = "long8";
  static constexpr char type_name[] = "long8";
  static constexpr char type_abbr[] = "i64";
};

template <> struct OpenCLCLong<16> {
  using host_type = Vector<int64_t, 16>;
  using underlying_type = int64_t;
  using scalar_type = OpenCLCLong<1>;
  using logical_type = OpenCLCLong<16>;
  using unsigned_type = OpenCLCUlong<16>;
  using signed_type = OpenCLCLong<16>;
  using regressed_type = OpenCLCInt<16>;
  static constexpr char device_type[] = "long16";
  static constexpr char type_name[] = "long16";
  static constexpr char type_abbr[] = "i64";
};

template <> struct OpenCLCFloat<1> {
  using host_type = float;
  using underlying_type = float;
  using scalar_type = OpenCLCFloat<1>;
  using logical_type = OpenCLCInt<1>;
  static constexpr char device_type[] = "float";
  static constexpr char type_name[] = "float";
  static constexpr char type_abbr[] = "f32";
  static constexpr char device_atomic_type[] = "atomic_float";
};

template <> struct OpenCLCFloat<2> {
  using host_type = Vector<float, 2>;
  using underlying_type = float;
  using scalar_type = OpenCLCFloat<1>;
  using logical_type = OpenCLCInt<2>;
  static constexpr char device_type[] = "float2";
  static constexpr char type_name[] = "float2";
  static constexpr char type_abbr[] = "f32";
};

template <> struct OpenCLCFloat<3> {
  using host_type = Vector<float, 3, 4>;
  using underlying_type = float;
  using scalar_type = OpenCLCFloat<1>;
  using logical_type = OpenCLCInt<3>;
  static constexpr char device_type[] = "float3";
  static constexpr char type_name[] = "float3";
  static constexpr char type_abbr[] = "f32";
};

template <> struct OpenCLCFloat<4> {
  using host_type = Vector<float, 4>;
  using underlying_type = float;
  using scalar_type = OpenCLCFloat<1>;
  using logical_type = OpenCLCInt<4>;
  static constexpr char device_type[] = "float4";
  static constexpr char type_name[] = "float4";
  static constexpr char type_abbr[] = "f32";
};

template <> struct OpenCLCFloat<8> {
  using host_type = Vector<float, 8>;
  using underlying_type = float;
  using scalar_type = OpenCLCFloat<1>;
  using logical_type = OpenCLCInt<8>;
  static constexpr char device_type[] = "float8";
  static constexpr char type_name[] = "float8";
  static constexpr char type_abbr[] = "f32";
};

template <> struct OpenCLCFloat<16> {
  using host_type = Vector<float, 16>;
  using underlying_type = float;
  using scalar_type = OpenCLCFloat<1>;
  using logical_type = OpenCLCInt<16>;
  static constexpr char device_type[] = "float16";
  static constexpr char type_name[] = "float16";
  static constexpr char type_abbr[] = "f32";
};

template <> struct OpenCLCDouble<1> {
  using host_type = double;
  using underlying_type = double;
  using scalar_type = OpenCLCDouble<1>;
  using logical_type = OpenCLCLong<1>;
  static constexpr char device_type[] = "double";
  static constexpr char type_name[] = "double";
  static constexpr char type_abbr[] = "f64";
  static constexpr char device_atomic_type[] = "atomic_double";
};

template <> struct OpenCLCDouble<2> {
  using host_type = Vector<double, 2>;
  using underlying_type = double;
  using scalar_type = OpenCLCDouble<1>;
  using logical_type = OpenCLCLong<2>;
  static constexpr char device_type[] = "double2";
  static constexpr char type_name[] = "double2";
  static constexpr char type_abbr[] = "f64";
};

template <> struct OpenCLCDouble<3> {
  using host_type = Vector<double, 3, 4>;
  using underlying_type = double;
  using scalar_type = OpenCLCDouble<1>;
  using logical_type = OpenCLCLong<3>;
  static constexpr char device_type[] = "double3";
  static constexpr char type_name[] = "double3";
  static constexpr char type_abbr[] = "f64";
};

template <> struct OpenCLCDouble<4> {
  using host_type = Vector<double, 4>;
  using underlying_type = double;
  using scalar_type = OpenCLCDouble<1>;
  using logical_type = OpenCLCLong<4>;
  static constexpr char device_type[] = "double4";
  static constexpr char type_name[] = "double4";
  static constexpr char type_abbr[] = "f64";
};

template <> struct OpenCLCDouble<8> {
  using host_type = Vector<double, 8>;
  using underlying_type = double;
  using scalar_type = OpenCLCDouble<1>;
  using logical_type = OpenCLCLong<8>;
  static constexpr char device_type[] = "double8";
  static constexpr char type_name[] = "double8";
  static constexpr char type_abbr[] = "f64";
};

template <> struct OpenCLCDouble<16> {
  using host_type = Vector<double, 16>;
  using underlying_type = double;
  using scalar_type = OpenCLCDouble<1>;
  using logical_type = OpenCLCLong<16>;
  static constexpr char device_type[] = "double16";
  static constexpr char type_name[] = "double16";
  static constexpr char type_abbr[] = "f64";
};

template <> struct OpenCLCHalf<1> {
  using host_type = Half;
  using underlying_type = uint16_t;
  using scalar_type = OpenCLCHalf<1>;
  using logical_type = OpenCLCShort<1>;
  static constexpr char device_type[] = "half";
  static constexpr char type_name[] = "half";
  static constexpr char type_abbr[] = "f16";
};

template <> struct OpenCLCHalf<2> {
  using host_type = Vector<Half, 2>;
  using underlying_type = uint16_t;
  using scalar_type = OpenCLCHalf<1>;
  using logical_type = OpenCLCShort<2>;
  static constexpr char device_type[] = "half2";
  static constexpr char type_name[] = "half2";
  static constexpr char type_abbr[] = "f16";
};

template <> struct OpenCLCHalf<3> {
  using host_type = Vector<Half, 3, 4>;
  using underlying_type = uint16_t;
  using scalar_type = OpenCLCHalf<1>;
  using logical_type = OpenCLCShort<3>;
  static constexpr char device_type[] = "half3";
  static constexpr char type_name[] = "half3";
  static constexpr char type_abbr[] = "f16";
};

template <> struct OpenCLCHalf<4> {
  using host_type = Vector<Half, 4>;
  using underlying_type = uint16_t;
  using scalar_type = OpenCLCHalf<1>;
  using logical_type = OpenCLCShort<4>;
  static constexpr char device_type[] = "half4";
  static constexpr char type_name[] = "half4";
  static constexpr char type_abbr[] = "f16";
};

template <> struct OpenCLCHalf<8> {
  using host_type = Vector<Half, 8>;
  using underlying_type = uint16_t;
  using scalar_type = OpenCLCHalf<1>;
  using logical_type = OpenCLCShort<8>;
  static constexpr char device_type[] = "half8";
  static constexpr char type_name[] = "half8";
  static constexpr char type_abbr[] = "f16";
};

template <> struct OpenCLCHalf<16> {
  using host_type = Vector<Half, 16>;
  using underlying_type = uint16_t;
  using scalar_type = OpenCLCHalf<1>;
  using logical_type = OpenCLCShort<16>;
  static constexpr char device_type[] = "half16";
  static constexpr char type_name[] = "half16";
  static constexpr char type_abbr[] = "f16";
};

template <> struct OpenCLCBfloat16<1> {
  using host_type = Bfloat;
  using scalar_type = OpenCLCBfloat16<1>;
  using underlying_type = float;
  static constexpr char device_type[] = "short";
  static constexpr char type_name[] = "bfloat";
  static constexpr char type_abbr[] = "bf16";
};

template <> struct OpenCLCBfloat16<2> {
  using host_type = Vector<Bfloat, 2>;
  using scalar_type = OpenCLCBfloat16<1>;
  using underlying_type = float;
  static constexpr char device_type[] = "short2";
  static constexpr char type_name[] = "bfloat2";
  static constexpr char type_abbr[] = "bf16";
};

template <> struct OpenCLCBfloat16<3> {
  using host_type = Vector<Bfloat, 3, 4>;
  using scalar_type = OpenCLCBfloat16<1>;
  using underlying_type = float;
  static constexpr char device_type[] = "short3";
  static constexpr char type_name[] = "bfloat3";
  static constexpr char type_abbr[] = "bf16";
};

template <> struct OpenCLCBfloat16<4> {
  using host_type = Vector<Bfloat, 4>;
  using scalar_type = OpenCLCBfloat16<1>;
  using underlying_type = float;
  static constexpr char device_type[] = "short4";
  static constexpr char type_name[] = "bfloat4";
  static constexpr char type_abbr[] = "bf16";
};

template <> struct OpenCLCBfloat16<8> {
  using host_type = Vector<Bfloat, 8>;
  using scalar_type = OpenCLCBfloat16<1>;
  using underlying_type = float;
  static constexpr char device_type[] = "short8";
  static constexpr char type_name[] = "bfloat8";
  static constexpr char type_abbr[] = "bf16";
};

template <> struct OpenCLCBfloat16<16> {
  using host_type = Vector<Bfloat, 16>;
  using scalar_type = OpenCLCBfloat16<1>;
  using underlying_type = float;
  static constexpr char device_type[] = "short16";
  static constexpr char type_name[] = "bfloat16";
  static constexpr char type_abbr[] = "bf16";
};

template <> struct OpenCLCTfloat<1> {
  using host_type = Tfloat;
  using scalar_type = OpenCLCTfloat<1>;
  using underlying_type = float;
  static constexpr char device_type[] = "int";
  static constexpr char type_name[] = "tfloat";
  static constexpr char type_abbr[] = "tf32";
};

template <> struct OpenCLCTfloat<2> {
  using host_type = Vector<Tfloat, 2>;
  using scalar_type = OpenCLCTfloat<1>;
  using underlying_type = float;
  static constexpr char device_type[] = "int2";
  static constexpr char type_name[] = "tfloat2";
  static constexpr char type_abbr[] = "tf32";
};

template <> struct OpenCLCTfloat<3> {
  using host_type = Vector<Tfloat, 3, 4>;
  using scalar_type = OpenCLCTfloat<1>;
  using underlying_type = float;
  static constexpr char device_type[] = "int3";
  static constexpr char type_name[] = "tfloat3";
  static constexpr char type_abbr[] = "tf32";
};

template <> struct OpenCLCTfloat<4> {
  using host_type = Vector<Tfloat, 4>;
  using scalar_type = OpenCLCTfloat<1>;
  using underlying_type = float;
  static constexpr char device_type[] = "int4";
  static constexpr char type_name[] = "tfloat4";
  static constexpr char type_abbr[] = "tf32";
};

template <> struct OpenCLCTfloat<8> {
  using host_type = Vector<Tfloat, 8>;
  using scalar_type = OpenCLCTfloat<1>;
  using underlying_type = float;
  static constexpr char device_type[] = "int8";
  static constexpr char type_name[] = "tfloat8";
  static constexpr char type_abbr[] = "tf32";
};

template <> struct OpenCLCTfloat<16> {
  using host_type = Vector<Tfloat, 16>;
  using scalar_type = OpenCLCTfloat<1>;
  using underlying_type = float;
  static constexpr char device_type[] = "int16";
  static constexpr char type_name[] = "tfloat16";
  static constexpr char type_abbr[] = "tf32";
};

struct OpenCLCBool {
  using host_type = bool;
  using underlying_type = bool;
  static constexpr char device_type[] = "bool";
  static constexpr char type_name[] = "bool";
};

struct OpenCLCSizeT {
  using host_type = size_t;
  using underlying_type = size_t;
  static constexpr char device_type[] = "size_t";
  static constexpr char type_name[] = "size_t";
  static constexpr char device_atomic_type[] = "atomic_size_t";
};

struct OpenCLCPtrdiffT {
  using host_type = ptrdiff_t;
  using underlying_type = ptrdiff_t;
  static constexpr char device_type[] = "ptrdiff_t";
  static constexpr char type_name[] = "ptrdiff_t";
  static constexpr char device_atomic_type[] = "atomic_ptrdiff_t";
};

struct OpenCLCIntptrT {
  using host_type = intptr_t;
  using underlying_type = intptr_t;
  static constexpr char device_type[] = "intptr_t";
  static constexpr char type_name[] = "intptr_t";
  static constexpr char device_atomic_type[] = "atomic_intptr_t";
};

struct OpenCLCUintptrT {
  using host_type = uintptr_t;
  using underlying_type = uintptr_t;
  static constexpr char device_type[] = "uintptr_t";
  static constexpr char type_name[] = "uintptr_t";
  static constexpr char device_atomic_type[] = "atomic_uintptr_t";
};

struct OpenCLCVoid {
  using host_type = void;
  using underlying_type = void;
  static constexpr char device_type[] = "void";
  static constexpr char type_name[] = "void";
};
} // namespace detail

/**
 * Wrapper for OpenCL C char data type.
 */
using clc_char_t = detail::OpenCLCChar<1>;
/**
 * Wrapper for OpenCL C char2 data type.
 */
using clc_char2_t = detail::OpenCLCChar<2>;
/**
 * Wrapper for OpenCL C char3 data type.
 */
using clc_char3_t = detail::OpenCLCChar<3>;
/**
 * Wrapper for OpenCL C char4 data type.
 */
using clc_char4_t = detail::OpenCLCChar<4>;
/**
 * Wrapper for OpenCL C char8 data type.
 */
using clc_char8_t = detail::OpenCLCChar<8>;
/**
 * Wrapper for OpenCL C char16 data type.
 */
using clc_char16_t = detail::OpenCLCChar<16>;

/**
 * Wrapper for OpenCL C uchar data type.
 */
using clc_uchar_t = detail::OpenCLCUchar<1>;
/**
 * Wrapper for OpenCL C uchar2 data type.
 */
using clc_uchar2_t = detail::OpenCLCUchar<2>;
/**
 * Wrapper for OpenCL C uchar3 data type.
 */
using clc_uchar3_t = detail::OpenCLCUchar<3>;
/**
 * Wrapper for OpenCL C uchar4 data type.
 */
using clc_uchar4_t = detail::OpenCLCUchar<4>;
/**
 * Wrapper for OpenCL C uchar8 data type.
 */
using clc_uchar8_t = detail::OpenCLCUchar<8>;
/**
 * Wrapper for OpenCL C uchar8 data type.
 */
using clc_uchar16_t = detail::OpenCLCUchar<16>;

/**
 * Wrapper for OpenCL C short data type.
 */
using clc_short_t = detail::OpenCLCShort<1>;
/**
 * Wrapper for OpenCL C short2 data type.
 */
using clc_short2_t = detail::OpenCLCShort<2>;
/**
 * Wrapper for OpenCL C short3 data type.
 */
using clc_short3_t = detail::OpenCLCShort<3>;
/**
 * Wrapper for OpenCL C short4 data type.
 */
using clc_short4_t = detail::OpenCLCShort<4>;
/**
 * Wrapper for OpenCL C short8 data type.
 */
using clc_short8_t = detail::OpenCLCShort<8>;
/**
 * Wrapper for OpenCL C short16 data type.
 */
using clc_short16_t = detail::OpenCLCShort<16>;

/**
 * Wrapper for OpenCL C ushort data type.
 */
using clc_ushort_t = detail::OpenCLCUshort<1>;
/**
 * Wrapper for OpenCL C ushort2 data type.
 */
using clc_ushort2_t = detail::OpenCLCUshort<2>;
/**
 * Wrapper for OpenCL C ushort3 data type.
 */
using clc_ushort3_t = detail::OpenCLCUshort<3>;
/**
 * Wrapper for OpenCL C ushort4 data type.
 */
using clc_ushort4_t = detail::OpenCLCUshort<4>;
/**
 * Wrapper for OpenCL C ushort8 data type.
 */
using clc_ushort8_t = detail::OpenCLCUshort<8>;
/**
 * Wrapper for OpenCL C ushort16 data type.
 */
using clc_ushort16_t = detail::OpenCLCUshort<16>;

/**
 * Wrapper for OpenCL C int data type.
 */
using clc_int_t = detail::OpenCLCInt<1>;
/**
 * Wrapper for OpenCL C int2 data type.
 */
using clc_int2_t = detail::OpenCLCInt<2>;
/**
 * Wrapper for OpenCL C int3 data type.
 */
using clc_int3_t = detail::OpenCLCInt<3>;
/**
 * Wrapper for OpenCL C int4 data type.
 */
using clc_int4_t = detail::OpenCLCInt<4>;
/**
 * Wrapper for OpenCL C int8 data type.
 */
using clc_int8_t = detail::OpenCLCInt<8>;
/**
 * Wrapper for OpenCL C int16 data type.
 */
using clc_int16_t = detail::OpenCLCInt<16>;

/**
 * Wrapper for OpenCL C uint data type.
 */
using clc_uint_t = detail::OpenCLCUint<1>;
/**
 * Wrapper for OpenCL C uint2 data type.
 */
using clc_uint2_t = detail::OpenCLCUint<2>;
/**
 * Wrapper for OpenCL C uint3 data type.
 */
using clc_uint3_t = detail::OpenCLCUint<3>;
/**
 * Wrapper for OpenCL C uint4 data type.
 */
using clc_uint4_t = detail::OpenCLCUint<4>;
/**
 * Wrapper for OpenCL C uint8 data type.
 */
using clc_uint8_t = detail::OpenCLCUint<8>;
/**
 * Wrapper for OpenCL C uint16 data type.
 */
using clc_uint16_t = detail::OpenCLCUint<16>;

/**
 * Wrapper for OpenCL C long data type.
 */
using clc_long_t = detail::OpenCLCLong<1>;
/**
 * Wrapper for OpenCL C long2 data type.
 */
using clc_long2_t = detail::OpenCLCLong<2>;
/**
 * Wrapper for OpenCL C long3 data type.
 */
using clc_long3_t = detail::OpenCLCLong<3>;
/**
 * Wrapper for OpenCL C long4 data type.
 */
using clc_long4_t = detail::OpenCLCLong<4>;
/**
 * Wrapper for OpenCL C long8 data type.
 */
using clc_long8_t = detail::OpenCLCLong<8>;
/**
 * Wrapper for OpenCL C long16 data type.
 */
using clc_long16_t = detail::OpenCLCLong<16>;

/**
 * Wrapper for OpenCL C ulong data type.
 */
using clc_ulong_t = detail::OpenCLCUlong<1>;
/**
 * Wrapper for OpenCL C ulong2 data type.
 */
using clc_ulong2_t = detail::OpenCLCUlong<2>;
/**
 * Wrapper for OpenCL C ulong3 data type.
 */
using clc_ulong3_t = detail::OpenCLCUlong<3>;
/**
 * Wrapper for OpenCL C ulong4 data type.
 */
using clc_ulong4_t = detail::OpenCLCUlong<4>;
/**
 * Wrapper for OpenCL C ulong8 data type.
 */
using clc_ulong8_t = detail::OpenCLCUlong<8>;
/**
 * Wrapper for OpenCL C ulong16 data type.
 */
using clc_ulong16_t = detail::OpenCLCUlong<16>;

/**
 * Wrapper for OpenCL C float data type.
 */
using clc_float_t = detail::OpenCLCFloat<1>;
/**
 * Wrapper for OpenCL C float2 data type.
 */
using clc_float2_t = detail::OpenCLCFloat<2>;
/**
 * Wrapper for OpenCL C float3 data type.
 */
using clc_float3_t = detail::OpenCLCFloat<3>;
/**
 * Wrapper for OpenCL C float4 data type.
 */
using clc_float4_t = detail::OpenCLCFloat<4>;
/**
 * Wrapper for OpenCL C float8 data type.
 */
using clc_float8_t = detail::OpenCLCFloat<8>;
/**
 * Wrapper for OpenCL C float16 data type.
 */
using clc_float16_t = detail::OpenCLCFloat<16>;

/**
 * Wrapper for OpenCL C double data type.
 */
using clc_double_t = detail::OpenCLCDouble<1>;
/**
 * Wrapper for OpenCL C double2 data type.
 */
using clc_double2_t = detail::OpenCLCDouble<2>;
/**
 * Wrapper for OpenCL C double3 data type.
 */
using clc_double3_t = detail::OpenCLCDouble<3>;
/**
 * Wrapper for OpenCL C double4 data type.
 */
using clc_double4_t = detail::OpenCLCDouble<4>;
/**
 * Wrapper for OpenCL C double8 data type.
 */
using clc_double8_t = detail::OpenCLCDouble<8>;
/**
 * Wrapper for OpenCL C double16 data type.
 */
using clc_double16_t = detail::OpenCLCDouble<16>;

/**
 * Wrapper for OpenCL C half data type.
 */
using clc_half_t = detail::OpenCLCHalf<1>;
/**
 * Wrapper for OpenCL C half2 data type.
 */
using clc_half2_t = detail::OpenCLCHalf<2>;
/**
 * Wrapper for OpenCL C half3 data type.
 */
using clc_half3_t = detail::OpenCLCHalf<3>;
/**
 * Wrapper for OpenCL C half4 data type.
 */
using clc_half4_t = detail::OpenCLCHalf<4>;
/**
 * Wrapper for OpenCL C half8 data type.
 */
using clc_half8_t = detail::OpenCLCHalf<8>;
/**
 * Wrapper for OpenCL C half16 data type.
 */
using clc_half16_t = detail::OpenCLCHalf<16>;

/**
 * Wrapper for OpenCL C bfloat16 data type.
 */
using clc_bfloat_t = detail::OpenCLCBfloat16<1>;
/**
 * Wrapper for OpenCL C bfloat16_2 data type.
 */
using clc_bfloat2_t = detail::OpenCLCBfloat16<2>;
/**
 * Wrapper for OpenCL C bfloat16_4 data type.
 */
using clc_bfloat4_t = detail::OpenCLCBfloat16<4>;
/**
 * Wrapper for OpenCL C bfloat16_8 data type.
 */
using clc_bfloat8_t = detail::OpenCLCBfloat16<8>;

/**
 * Wrapper for OpenCL C tfloat data type.
 */
using clc_tfloat_t = detail::OpenCLCTfloat<1>;

/**
 * Wrapper for OpenCL C bool data type.
 */
using clc_bool_t = detail::OpenCLCBool;
/**
 * Wrapper for OpenCL C size_t data type.
 */
using clc_size_t = detail::OpenCLCSizeT;
/**
 * Wrapper for OpenCL C ptrdiff_t data type.
 */
using clc_ptrdiff_t = detail::OpenCLCPtrdiffT;
/**
 * Wrapper for OpenCL C intptr_t data type.
 */
using clc_intptr_t = detail::OpenCLCIntptrT;
/**
 * Wrapper for OpenCL C uintptr_t data type.
 */
using clc_uintptr_t = detail::OpenCLCUintptrT;
/**
 * Wrapper for OpenCL C void data type.
 */
using clc_void_t = detail::OpenCLCVoid;

} // namespace cassian

#endif
