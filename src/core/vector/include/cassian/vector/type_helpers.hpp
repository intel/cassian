/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_VECTOR_TYPE_HELPERS_HPP
#define CASSIAN_VECTOR_TYPE_HELPERS_HPP

#include <cassian/vector/vector.hpp>

/**
 * Cassian namespace.
 */
namespace cassian {

/**
 * Detail namespace.
 */
namespace detail {
/**
 * Constants for numbers
 */
constexpr int v2 = 2;   /**< 2 */
constexpr int v3 = 3;   /**< 3 */
constexpr int v4 = 4;   /**< 4 */
constexpr int v8 = 8;   /**< 8 */
constexpr int v16 = 16; /**< 16 */
} // namespace detail

/**
 * Type aliasses for cassian::Vector specializations
 */

using int8_2_t = Vector<int8_t, detail::v2>; /**< Vector<int8_t, 2> */
using int8_3_t =
    Vector<int8_t, detail::v3, detail::v4>;    /**< Vector<int8_t, 3, 4> */
using int8_4_t = Vector<int8_t, detail::v4>;   /**< Vector<int8_t, 4> */
using int8_8_t = Vector<int8_t, detail::v8>;   /**< Vector<int8_t, 8> */
using int8_16_t = Vector<int8_t, detail::v16>; /**< Vector<int8_t, 16> */

using uint8_2_t = Vector<uint8_t, detail::v2>; /**< Vector<uint8_t, 2> */
using uint8_3_t =
    Vector<uint8_t, detail::v3, detail::v4>;     /**< Vector<uint8_t, 3, 4> */
using uint8_4_t = Vector<uint8_t, detail::v4>;   /**< Vector<uint8_t, 4> */
using uint8_8_t = Vector<uint8_t, detail::v8>;   /**< Vector<uint8_t, 8> */
using uint8_16_t = Vector<uint8_t, detail::v16>; /**< Vector<uint8_t, 16> */

using int16_2_t = Vector<int16_t, detail::v2>; /**< Vector<int16_t, 2> */
using int16_3_t =
    Vector<int16_t, detail::v3, detail::v4>;     /**< Vector<int16_t, 3, 4> */
using int16_4_t = Vector<int16_t, detail::v4>;   /**< Vector<int16_t, 4> */
using int16_8_t = Vector<int16_t, detail::v8>;   /**< Vector<int16_t, 8> */
using int16_16_t = Vector<int16_t, detail::v16>; /**< Vector<int16_t, 16> */

using uint16_2_t = Vector<uint16_t, detail::v2>; /**< Vector<uint16_t, 2> */
using uint16_3_t =
    Vector<uint16_t, detail::v3, detail::v4>;    /**< Vector<uint16_t, 3, 4> */
using uint16_4_t = Vector<uint16_t, detail::v4>; /**< Vector<uint16_t, 4> */
using uint16_8_t = Vector<uint16_t, detail::v8>; /**< Vector<uint16_t, 8> */
using uint16_16_t = Vector<uint16_t, detail::v16>; /**< Vector<uint16_t, 16> */

using int32_2_t = Vector<int32_t, detail::v2>; /**< Vector<int32_t, 2> */
using int32_3_t =
    Vector<int32_t, detail::v3, detail::v4>;     /**< Vector<int32_t, 3, 4> */
using int32_4_t = Vector<int32_t, detail::v4>;   /**< Vector<int32_t, 4> */
using int32_8_t = Vector<int32_t, detail::v8>;   /**< Vector<int32_t, 8> */
using int32_16_t = Vector<int32_t, detail::v16>; /**< Vector<int32_t, 16> */

using uint32_2_t = Vector<uint32_t, detail::v2>; /**< Vector<uint32_t, 2> */
using uint32_3_t =
    Vector<uint32_t, detail::v3, detail::v4>;    /**< Vector<uint32_t, 3, 4> */
using uint32_4_t = Vector<uint32_t, detail::v4>; /**< Vector<uint32_t, 4> */
using uint32_8_t = Vector<uint32_t, detail::v8>; /**< Vector<uint32_t, 8> */
using uint32_16_t = Vector<uint32_t, detail::v16>; /**< Vector<uint32_t, 16> */

using int64_2_t = Vector<int64_t, detail::v2>; /**< Vector<int64_t, 2> */
using int64_3_t =
    Vector<int64_t, detail::v3, detail::v4>;     /**< Vector<int64_t, 3, 4> */
using int64_4_t = Vector<int64_t, detail::v4>;   /**< Vector<int64_t, 4> */
using int64_8_t = Vector<int64_t, detail::v8>;   /**< Vector<int64_t, 8> */
using int64_16_t = Vector<int64_t, detail::v16>; /**< Vector<int64_t, 16> */

using uint64_2_t = Vector<uint64_t, detail::v2>; /**< Vector<uint64_t, 2> */
using uint64_3_t =
    Vector<uint64_t, detail::v3, detail::v4>;    /**< Vector<uint64_t, 3, 4> */
using uint64_4_t = Vector<uint64_t, detail::v4>; /**< Vector<uint64_t, 4> */
using uint64_8_t = Vector<uint64_t, detail::v8>; /**< Vector<uint64_t, 8> */
using uint64_16_t = Vector<uint64_t, detail::v16>; /**< Vector<uint64_t, 16> */

using float2_t = Vector<float, detail::v2>; /**< Vector<float, 2> */
using float3_t =
    Vector<float, detail::v3, detail::v4>;    /**< Vector<float, 3, 4> */
using float4_t = Vector<float, detail::v4>;   /**< Vector<float, 4> */
using float8_t = Vector<float, detail::v8>;   /**< Vector<float, 8> */
using float16_t = Vector<float, detail::v16>; /**< Vector<float, 16> */

using double2_t = Vector<double, detail::v2>; /**< Vector<double, 2> */
using double3_t =
    Vector<double, detail::v3, detail::v4>;     /**< Vector<double, 3, 4> */
using double4_t = Vector<double, detail::v4>;   /**< Vector<double, 4> */
using double8_t = Vector<double, detail::v8>;   /**< Vector<double, 8> */
using double16_t = Vector<double, detail::v16>; /**< Vector<double, 16> */

} // namespace cassian

#endif
