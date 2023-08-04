/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_RUNTIME_OPENCLC_TYPE_TUPLES_HPP
#define CASSIAN_RUNTIME_OPENCLC_TYPE_TUPLES_HPP

#include <cassian/fp_types/half.hpp>
#include <cassian/runtime/openclc_types.hpp>
#include <cassian/utility/metaprogramming.hpp>
#include <cassian/vector/vector.hpp>

/**
 * Cassian namespace.
 */
namespace cassian {

namespace detail {
using VectorSizes = std::index_sequence<1, 2, 3, 4, 8, 16>;

template <size_t N>
using BasicTypes =
    std::tuple<OpenCLCChar<N>, OpenCLCShort<N>, OpenCLCInt<N>, OpenCLCLong<N>,
               OpenCLCUchar<N>, OpenCLCUshort<N>, OpenCLCUint<N>,
               OpenCLCUlong<N>, OpenCLCFloat<N>, OpenCLCDouble<N>>;

template <size_t N>
using IntegerTypes =
    std::tuple<OpenCLCChar<N>, OpenCLCShort<N>, OpenCLCInt<N>, OpenCLCLong<N>,
               OpenCLCUchar<N>, OpenCLCUshort<N>, OpenCLCUint<N>,
               OpenCLCUlong<N>>;

template <size_t N>
using FloatingPointTypes =
    std::tuple<OpenCLCHalf<N>, OpenCLCFloat<N>, OpenCLCDouble<N>>;
} // namespace detail

/**
 * Tuple containing OpenCL C scalar types.
 */
using ScalarTypes = detail::BasicTypes<1>;

/**
 * Tuple containing OpenCL C integer scalar types.
 */
using IntegerScalarTypes = detail::IntegerTypes<1>;

/**
 * Tuple containing OpenCL C floating point scalar types.
 */
using FloatingPointScalarTypes = detail::FloatingPointTypes<1>;

/**
 * Tuple containing OpenCL C 2-element vector types.
 */
using Vector2Types = detail::BasicTypes<2>;

/**
 * Tuple containing OpenCL C 2-element vector integer types.
 */
using Vector2IntegerTypes = detail::IntegerTypes<2>;

/**
 * Tuple containing OpenCL C 2-element vector floating point types.
 */
using Vector2FloatingPointTypes = detail::FloatingPointTypes<2>;

/**
 * Tuple containing OpenCL C 3-element vector types.
 */
using Vector3Types = detail::BasicTypes<3>;

/**
 * Tuple containing OpenCL C 3-element vector integer types.
 */
using Vector3IntegerTypes = detail::IntegerTypes<3>;

/**
 * Tuple containing OpenCL C 3-element vector floating point types.
 */
using Vector3FloatingPointTypes = detail::FloatingPointTypes<3>;

/**
 * Tuple containing OpenCL C 4-element vector types.
 */
using Vector4Types = detail::BasicTypes<4>;

/**
 * Tuple containing OpenCL C 4-element vector integer types.
 */
using Vector4IntegerTypes = detail::IntegerTypes<4>;

/**
 * Tuple containing OpenCL C 4-element vector floating point types.
 */
using Vector4FloatingPointTypes = detail::FloatingPointTypes<4>;

/**
 * Tuple containing OpenCL C 8-element vector types.
 */
using Vector8Types = detail::BasicTypes<8>;

/**
 * Tuple containing OpenCL C 8-element vector integer types.
 */
using Vector8IntegerTypes = detail::IntegerTypes<8>;

/**
 * Tuple containing OpenCL C 8-element vector floating point types.
 */
using Vector8FloatingPointTypes = detail::FloatingPointTypes<8>;

/**
 * Tuple containing OpenCL C 16-element vector types.
 */
using Vector16Types = detail::BasicTypes<16>;

/**
 * Tuple containing OpenCL C 16-element vector integer types.
 */
using Vector16IntegerTypes = detail::IntegerTypes<16>;

/**
 * Tuple containing OpenCL C 16-element vector floating point types.
 */
using Vector16FloatingPointTypes = detail::FloatingPointTypes<16>;

/**
 * Tuple containing OpenCL C vector types.
 */
using VectorTypes = TupleConcat<Vector2Types, Vector3Types, Vector4Types,
                                Vector8Types, Vector16Types>::type;

/**
 * Tuple containing OpenCL C char scalar and vector types.
 */
using TypesChar =
    CartesianProduct<detail::VectorSizes>::type<detail::OpenCLCChar>;

/**
 * Tuple containing OpenCL C short scalar and vector types.
 */
using TypesShort =
    CartesianProduct<detail::VectorSizes>::type<detail::OpenCLCShort>;

/**
 * Tuple containing OpenCL C int scalar and vector types.
 */
using TypesInt =
    CartesianProduct<detail::VectorSizes>::type<detail::OpenCLCInt>;

/**
 * Tuple containing OpenCL C long scalar and vector types.
 */
using TypesLong =
    CartesianProduct<detail::VectorSizes>::type<detail::OpenCLCLong>;

/**
 * Tuple containing OpenCL C uchar scalar and vector types.
 */
using TypesUchar =
    CartesianProduct<detail::VectorSizes>::type<detail::OpenCLCUchar>;

/**
 * Tuple containing OpenCL C ushort scalar and vector types.
 */
using TypesUshort =
    CartesianProduct<detail::VectorSizes>::type<detail::OpenCLCUshort>;

/**
 * Tuple containing OpenCL C uint scalar and vector types.
 */
using TypesUint =
    CartesianProduct<detail::VectorSizes>::type<detail::OpenCLCUint>;

/**
 * Tuple containing OpenCL C ulong scalar and vector types.
 */
using TypesUlong =
    CartesianProduct<detail::VectorSizes>::type<detail::OpenCLCUlong>;

/**
 * Tuple containing OpenCL C half scalar and vector types.
 */
using TypesHalf =
    CartesianProduct<detail::VectorSizes>::type<detail::OpenCLCHalf>;
/**
 * Tuple containing OpenCL C float scalar and vector types.
 */
using TypesFloat =
    CartesianProduct<detail::VectorSizes>::type<detail::OpenCLCFloat>;

/**
 * Tuple containing OpenCL C double scalar and vector types.
 */
using TypesDouble =
    CartesianProduct<detail::VectorSizes>::type<detail::OpenCLCDouble>;

// Dot products below need to be separate to avoid hitting template recursion
// depth limit in MSVC

/**
 * Tuple containing 2-element tuples of OpenCL C scalar types.
 */
using ScalarToScalar = CartesianProduct<ScalarTypes, ScalarTypes>::type;

/**
 * Tuple containing 2-element tuples of OpenCL C integer scalar types.
 */
using IntegerScalarToIntegerScalar =
    CartesianProduct<IntegerScalarTypes, IntegerScalarTypes>::type;

/**
 * Tuple containing 2-element tuples of OpenCL C floating point scalar types.
 */
using FloatingPointScalarToFloatingPointScalar =
    CartesianProduct<FloatingPointScalarTypes, FloatingPointScalarTypes>::type;

/**
 * Tuple containing 2-element tuples of OpenCL C scalar and 2-element vector
 * types.
 */
using ScalarToVector2 = CartesianProduct<ScalarTypes, Vector2Types>::type;

/**
 * Tuple containing 2-element tuples of OpenCL C scalar and 3-element vector
 * types.
 */
using ScalarToVector3 = CartesianProduct<ScalarTypes, Vector3Types>::type;

/**
 * Tuple containing 2-element tuples of OpenCL C scalar and 4-element vector
 * types.
 */
using ScalarToVector4 = CartesianProduct<ScalarTypes, Vector4Types>::type;

/**
 * Tuple containing 2-element tuples of OpenCL C scalar and 8-element vector
 * types.
 */
using ScalarToVector8 = CartesianProduct<ScalarTypes, Vector8Types>::type;

/**
 * Tuple containing 2-element tuples of OpenCL C scalar and 16-element vector
 * types.
 */
using ScalarToVector16 = CartesianProduct<ScalarTypes, Vector16Types>::type;

/**
 * Tuple containing 2-element tuples of OpenCL C 2-element vector and scalar
 * types.
 */
using Vector2ToScalar = CartesianProduct<Vector2Types, ScalarTypes>::type;

/**
 * Tuple containing 2-element tuples of OpenCL C 2-element vector types.
 */
using Vector2ToVector2 = CartesianProduct<Vector2Types, Vector2Types>::type;

/**
 * Tuple containing 2-element tuples of OpenCL C 3-element vector types.
 */
using Vector2ToVector3 = CartesianProduct<Vector2Types, Vector3Types>::type;

/**
 * Tuple containing 2-element tuples of OpenCL C 2-element vector and 4-element
 * vector types.
 */
using Vector2ToVector4 = CartesianProduct<Vector2Types, Vector4Types>::type;

/**
 * Tuple containing 2-element tuples of OpenCL C 2-element vector and 8-element
 * vector types.
 */
using Vector2ToVector8 = CartesianProduct<Vector2Types, Vector8Types>::type;

/**
 * Tuple containing 2-element tuples of OpenCL C 2-element vector and 16-element
 * vector types.
 */
using Vector2ToVector16 = CartesianProduct<Vector2Types, Vector16Types>::type;

/**
 * Tuple containing 2-element tuples of OpenCL C 2-element integer vector types.
 */
using Vector2IntegerToVector2Integer =
    CartesianProduct<Vector2IntegerTypes, Vector2IntegerTypes>::type;

/**
 * Tuple containing 2-element tuples of OpenCL C 2-element floating point vector
 * types.
 */
using Vector2FloatingPointToVector2FloatingPoint =
    CartesianProduct<Vector2FloatingPointTypes,
                     Vector2FloatingPointTypes>::type;

/**
 * Tuple containing 2-element tuples of OpenCL C 3-element vector and scalar
 * types.
 */
using Vector3ToScalar = CartesianProduct<Vector3Types, ScalarTypes>::type;

/**
 * Tuple containing 2-element tuples of OpenCL C 3-element vector and 2-element
 * vector types.
 */
using Vector3ToVector2 = CartesianProduct<Vector3Types, Vector2Types>::type;

/**
 * Tuple containing 2-element tuples of OpenCL C 3-element vector types.
 */
using Vector3ToVector3 = CartesianProduct<Vector3Types, Vector3Types>::type;

/**
 * Tuple containing 2-element tuples of OpenCL C 3-element vector and 4-element
 * vector types.
 */
using Vector3ToVector4 = CartesianProduct<Vector3Types, Vector4Types>::type;

/**
 * Tuple containing 2-element tuples of OpenCL C 3-element vector and 8-element
 * vector types.
 */
using Vector3ToVector8 = CartesianProduct<Vector3Types, Vector8Types>::type;

/**
 * Tuple containing 2-element tuples of OpenCL C 3-element vector and 16-element
 * vector types.
 */
using Vector3ToVector16 = CartesianProduct<Vector3Types, Vector16Types>::type;

/**
 * Tuple containing 2-element tuples of OpenCL C 3-element integer vector types.
 */
using Vector3IntegerToVector3Integer =
    CartesianProduct<Vector3IntegerTypes, Vector3IntegerTypes>::type;

/**
 * Tuple containing 2-element tuples of OpenCL C 3-element floating point vector
 * types.
 */
using Vector3FloatingPointToVector3FloatingPoint =
    CartesianProduct<Vector3FloatingPointTypes,
                     Vector3FloatingPointTypes>::type;

/**
 * Tuple containing 2-element tuples of OpenCL C 4-element vector and scalar
 * types.
 */
using Vector4ToScalar = CartesianProduct<Vector4Types, ScalarTypes>::type;

/**
 * Tuple containing 2-element tuples of OpenCL C 4-element vector and 2-element
 * vector types.
 */
using Vector4ToVector2 = CartesianProduct<Vector4Types, Vector2Types>::type;

/**
 * Tuple containing 2-element tuples of OpenCL C 4-element vector and 3-element
 * vector types.
 */
using Vector4ToVector3 = CartesianProduct<Vector4Types, Vector3Types>::type;

/**
 * Tuple containing 2-element tuples of OpenCL C 4-element vector types.
 */
using Vector4ToVector4 = CartesianProduct<Vector4Types, Vector4Types>::type;

/**
 * Tuple containing 2-element tuples of OpenCL C 4-element vector and 8-element
 * vector types.
 */
using Vector4ToVector8 = CartesianProduct<Vector4Types, Vector8Types>::type;

/**
 * Tuple containing 2-element tuples of OpenCL C 4-element vector and 16-element
 * vector types.
 */
using Vector4ToVector16 = CartesianProduct<Vector4Types, Vector16Types>::type;

/**
 * Tuple containing 2-element tuples of OpenCL C 4-element integer vector types.
 */
using Vector4IntegerToVector4Integer =
    CartesianProduct<Vector4IntegerTypes, Vector4IntegerTypes>::type;

/**
 * Tuple containing 2-element tuples of OpenCL C 4-element floating point vector
 * types.
 */
using Vector4FloatingPointToVector4FloatingPoint =
    CartesianProduct<Vector4FloatingPointTypes,
                     Vector4FloatingPointTypes>::type;
/**
 * Tuple containing 2-element tuples of OpenCL C 8-element vector and scalar
 * types.
 */
using Vector8ToScalar = CartesianProduct<Vector8Types, ScalarTypes>::type;

/**
 * Tuple containing 2-element tuples of OpenCL C 8-element vector and 2-element
 * vector types.
 */
using Vector8ToVector2 = CartesianProduct<Vector8Types, Vector2Types>::type;

/**
 * Tuple containing 2-element tuples of OpenCL C 8-element vector and 3-element
 * vector types.
 */
using Vector8ToVector3 = CartesianProduct<Vector8Types, Vector3Types>::type;

/**
 * Tuple containing 2-element tuples of OpenCL C 8-element vector and 4-element
 * vector types.
 */
using Vector8ToVector4 = CartesianProduct<Vector8Types, Vector4Types>::type;

/**
 * Tuple containing 2-element tuples of OpenCL C 8-element vector types.
 */
using Vector8ToVector8 = CartesianProduct<Vector8Types, Vector8Types>::type;

/**
 * Tuple containing 2-element tuples of OpenCL C 8-element vector and 16-element
 * vector types.
 */
using Vector8ToVector16 = CartesianProduct<Vector8Types, Vector16Types>::type;

/**
 * Tuple containing 2-element tuples of OpenCL C 8-element integer vector types.
 */
using Vector8IntegerToVector8Integer =
    CartesianProduct<Vector8IntegerTypes, Vector8IntegerTypes>::type;

/**
 * Tuple containing 2-element tuples of OpenCL C 8-element floating point vector
 * types.
 */
using Vector8FloatingPointToVector8FloatingPoint =
    CartesianProduct<Vector8FloatingPointTypes,
                     Vector8FloatingPointTypes>::type;

/**
 * Tuple containing 2-element tuples of OpenCL C 16-element vector and scalar
 * types.
 */
using Vector16ToScalar = CartesianProduct<Vector16Types, ScalarTypes>::type;

/**
 * Tuple containing 2-element tuples of OpenCL C 16element vector and 2-element
 * vector types.
 */
using Vector16ToVector2 = CartesianProduct<Vector16Types, Vector2Types>::type;

/**
 * Tuple containing 2-element tuples of OpenCL C 16element vector and 3-element
 * vector types.
 */
using Vector16ToVector3 = CartesianProduct<Vector16Types, Vector3Types>::type;

/**
 * Tuple containing 2-element tuples of OpenCL C 16-element vector and 4-element
 * vector types.
 */
using Vector16ToVector4 = CartesianProduct<Vector16Types, Vector4Types>::type;

/**
 * Tuple containing 2-element tuples of OpenCL C 16-element vector and 8-element
 * vector types.
 */
using Vector16ToVector8 = CartesianProduct<Vector16Types, Vector8Types>::type;

/**
 * Tuple containing 2-element tuples of OpenCL C 16-element vector types.
 */
using Vector16ToVector16 = CartesianProduct<Vector16Types, Vector16Types>::type;

/**
 * Tuple containing 2-element tuples of OpenCL C 16-element integer vector
 * types.
 */
using Vector16IntegerToVector16Integer =
    CartesianProduct<Vector16IntegerTypes, Vector16IntegerTypes>::type;

/**
 * Tuple containing 2-element tuples of OpenCL C 16-element floating point
 * vector types.
 */
using Vector16FloatingPointToVector16FloatingPoint =
    CartesianProduct<Vector16FloatingPointTypes,
                     Vector16FloatingPointTypes>::type;

/**
 * Tuple containing OpenCL C unsigned integer scalar and vector types.
 */
using UnsignedTypes =
    TupleConcat<TypesUchar, TypesUshort, TypesUint, TypesUlong>::type;

/**
 * Tuple containing OpenCL C signed integer scalar and vector types.
 */
using SignedTypes =
    TupleConcat<TypesChar, TypesShort, TypesInt, TypesLong>::type;

/**
 * Tuple containing OpenCL C integer scalar and vector types.
 */
using IntegerTypes = TupleConcat<UnsignedTypes, SignedTypes>::type;

} // namespace cassian

#endif