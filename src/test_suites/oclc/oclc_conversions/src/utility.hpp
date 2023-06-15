#ifndef CASSIAN_OCLC_CONVERSIONS_UTILITY_HPP
#define CASSIAN_OCLC_CONVERSIONS_UTILITY_HPP

#include <cassian/runtime/openclc_type_tuples.hpp>
#include <cassian/utility/metaprogramming.hpp>
#include <common.hpp>
#include <tuple>

namespace detail {

template <typename T> struct TupleToTestCase {};

template <typename T, typename U> struct TupleToTestCase<std::tuple<T, U>> {
  using type = TestCase<T, U>;
};

template <RoundingMode RND, typename... T> struct RoundingModeProductRow {};

template <RoundingMode DestRND, typename... T, typename... U,
          RoundingMode... RND, OverflowHandling... SAT, TestVariant... TV>
struct RoundingModeProductRow<DestRND,
                              std::tuple<TestCase<T, U, RND, SAT, TV>...>> {
  using type = std::tuple<TestCase<T, U, DestRND, SAT, TV>...>;
};

template <OverflowHandling SAT, typename... T>
struct OverflowHandlingProductRow {};

template <OverflowHandling DestSAT, typename... T, typename... U,
          RoundingMode... RND, OverflowHandling... SAT, TestVariant... TV>
struct OverflowHandlingProductRow<DestSAT,
                                  std::tuple<TestCase<T, U, RND, SAT, TV>...>> {
  using type = std::tuple<TestCase<T, U, RND, DestSAT, TV>...>;
};

template <TestVariant TV, typename... T> struct TestVariantProductRow {};

template <TestVariant DestTestVariant, typename... T, typename... U,
          RoundingMode... RND, OverflowHandling... SAT, TestVariant... TV>
struct TestVariantProductRow<DestTestVariant,
                             std::tuple<TestCase<T, U, RND, SAT, TV>...>> {
  using type = std::tuple<TestCase<T, U, RND, SAT, DestTestVariant>...>;
};

} // namespace detail

template <typename... T> struct TuplesToTestCases {};

template <typename... T> struct TuplesToTestCases<std::tuple<T...>> {
  using type = std::tuple<typename detail::TupleToTestCase<T>::type...>;
};
template <typename... T>
using tuples_to_test_cases_t = typename TuplesToTestCases<T...>::type;

template <typename T, RoundingMode... RND> struct RoundingModeProduct {
  using type = typename ca::TupleConcat<
      typename detail::RoundingModeProductRow<RND, T>::type...>::type;
};

template <typename T, RoundingMode... RND>
using rounding_mode_product_t = typename RoundingModeProduct<T, RND...>::type;

template <typename T, OverflowHandling... SAT> struct OverflowHandlingProduct {
  using type = typename ca::TupleConcat<
      typename detail::OverflowHandlingProductRow<SAT, T>::type...>::type;
};
template <typename T, OverflowHandling... SAT>
using overflow_handling_product_t =
    typename OverflowHandlingProduct<T, SAT...>::type;

template <typename T, TestVariant... TV> struct TestVariantProduct {
  using type = typename ca::TupleConcat<
      typename detail::TestVariantProductRow<TV, T>::type...>::type;
};
template <typename T, TestVariant... TV>
using test_variant_product_t = typename TestVariantProduct<T, TV...>::type;

namespace cassian {

namespace detail {

using VectorSizes = std::index_sequence<1, 2, 3, 4, 8, 16>;

template <size_t N>
using OneByteTypes = std::tuple<OpenCLCChar<N>, OpenCLCUchar<N>>;

template <size_t N>
using TwoByteTypes = std::tuple<OpenCLCShort<N>, OpenCLCUshort<N>>;

template <size_t N>
using FourByteTypes =
    std::tuple<OpenCLCInt<N>, OpenCLCUint<N>, OpenCLCFloat<N>>;

template <size_t N>
using EightByteTypes =
    std::tuple<OpenCLCLong<N>, OpenCLCUlong<N>, OpenCLCDouble<N>>;
} // namespace detail

using ScalarOneByteTypesToScalarOneByteTypes =
    cassian::CartesianProduct<detail::OneByteTypes<1>,
                              detail::OneByteTypes<1>>::type;

using Vector2OneByteTypesToVector2OneByteTypes =
    cassian::CartesianProduct<detail::OneByteTypes<2>,
                              detail::OneByteTypes<2>>::type;

using Vector3OneByteTypesToVector3OneByteTypes =
    cassian::CartesianProduct<detail::OneByteTypes<3>,
                              detail::OneByteTypes<3>>::type;

using Vector4OneByteTypesToVector4OneByteTypes =
    cassian::CartesianProduct<detail::OneByteTypes<4>,
                              detail::OneByteTypes<4>>::type;

using Vector4OneByteTypesToVector3OneByteTypes =
    cassian::CartesianProduct<detail::OneByteTypes<4>,
                              detail::OneByteTypes<3>>::type;

using Vector8OneByteTypesToVector8OneByteTypes =
    cassian::CartesianProduct<detail::OneByteTypes<8>,
                              detail::OneByteTypes<8>>::type;

using Vector16OneByteTypesToVector16OneByteTypes =
    cassian::CartesianProduct<detail::OneByteTypes<16>,
                              detail::OneByteTypes<16>>::type;

using ScalarTwoByteTypesToScalarTwoByteTypes =
    cassian::CartesianProduct<detail::TwoByteTypes<1>,
                              detail::TwoByteTypes<1>>::type;

using Vector2TwoByteTypesToVector2TwoByteTypes =
    cassian::CartesianProduct<detail::TwoByteTypes<2>,
                              detail::TwoByteTypes<2>>::type;

using Vector3TwoByteTypesToVector3TwoByteTypes =
    cassian::CartesianProduct<detail::TwoByteTypes<3>,
                              detail::TwoByteTypes<3>>::type;

using Vector4TwoByteTypesToVector4TwoByteTypes =
    cassian::CartesianProduct<detail::TwoByteTypes<4>,
                              detail::TwoByteTypes<4>>::type;

using Vector4TwoByteTypesToVector3TwoByteTypes =
    cassian::CartesianProduct<detail::TwoByteTypes<4>,
                              detail::TwoByteTypes<3>>::type;

using Vector8TwoByteTypesToVector8TwoByteTypes =
    cassian::CartesianProduct<detail::TwoByteTypes<8>,
                              detail::TwoByteTypes<8>>::type;

using Vector16TwoByteTypesToVector16TwoByteTypes =
    cassian::CartesianProduct<detail::TwoByteTypes<16>,
                              detail::TwoByteTypes<16>>::type;

using ScalarFourByteTypesToScalarFourByteTypes =
    cassian::CartesianProduct<detail::FourByteTypes<1>,
                              detail::FourByteTypes<1>>::type;

using Vector2FourByteTypesToVector2FourByteTypes =
    cassian::CartesianProduct<detail::FourByteTypes<2>,
                              detail::FourByteTypes<2>>::type;

using Vector3FourByteTypesToVector3FourByteTypes =
    cassian::CartesianProduct<detail::FourByteTypes<3>,
                              detail::FourByteTypes<3>>::type;

using Vector4FourByteTypesToVector4FourByteTypes =
    cassian::CartesianProduct<detail::FourByteTypes<4>,
                              detail::FourByteTypes<4>>::type;

using Vector4FourByteTypesToVector3FourByteTypes =
    cassian::CartesianProduct<detail::FourByteTypes<4>,
                              detail::FourByteTypes<3>>::type;

using Vector8FourByteTypesToVector8FourByteTypes =
    cassian::CartesianProduct<detail::FourByteTypes<8>,
                              detail::FourByteTypes<8>>::type;

using Vector16FourByteTypesToVector16FourByteTypes =
    cassian::CartesianProduct<detail::FourByteTypes<16>,
                              detail::FourByteTypes<16>>::type;

using ScalarEightByteTypesToScalarEightByteTypes =
    cassian::CartesianProduct<detail::EightByteTypes<1>,
                              detail::EightByteTypes<1>>::type;

using Vector2EightByteTypesToVector2EightByteTypes =
    cassian::CartesianProduct<detail::EightByteTypes<2>,
                              detail::EightByteTypes<2>>::type;

using Vector3EightByteTypesToVector3EightByteTypes =
    cassian::CartesianProduct<detail::EightByteTypes<3>,
                              detail::EightByteTypes<3>>::type;

using Vector4EightByteTypesToVector4EightByteTypes =
    cassian::CartesianProduct<detail::EightByteTypes<4>,
                              detail::EightByteTypes<4>>::type;

using Vector4EightByteTypesToVector3EightByteTypes =
    cassian::CartesianProduct<detail::EightByteTypes<4>,
                              detail::EightByteTypes<3>>::type;

using Vector8EightByteTypesToVector8EightByteTypes =
    cassian::CartesianProduct<detail::EightByteTypes<8>,
                              detail::EightByteTypes<8>>::type;

using Vector16EightByteTypesToVector16EightByteTypes =
    cassian::CartesianProduct<detail::EightByteTypes<16>,
                              detail::EightByteTypes<16>>::type;

/**
 * Tuple containing cartesian product of OpenCL C integer scalar types and
 * OpenCL C floating point scalar types.
 */
using IntegerScalarToFloatingPointScalar =
    cassian::CartesianProduct<cassian::IntegerScalarTypes,
                              cassian::FloatingPointScalarTypes>::type;

/**
 * Tuple containing cartesian product of OpenCL C floating point scalar types
 * and OpenCL C integer scalar types.
 */
using FloatingPointScalarToIntegerScalar =
    cassian::CartesianProduct<cassian::FloatingPointScalarTypes,
                              cassian::IntegerScalarTypes>::type;

/**
 * Tuple containing cartesian product of OpenCL C 2-element integer vector types
 * and OpenCL C 2-element floating point vector types.
 */
using Vector2IntegerToVector2FloatingPoint =
    cassian::CartesianProduct<cassian::Vector2IntegerTypes,
                              cassian::Vector2FloatingPointTypes>::type;

/**
 * Tuple containing cartesian product of OpenCL C 2-element floating point
 * vector types and OpenCL C 2-element integer vector types.
 */
using Vector2FloatingPointToVector2Integer =
    cassian::CartesianProduct<cassian::Vector2FloatingPointTypes,
                              cassian::Vector2IntegerTypes>::type;

/**
 * Tuple containing cartesian product of OpenCL C 3-element integer vector types
 * and OpenCL C 3-element floating point vector types.
 */
using Vector3IntegerToVector3FloatingPoint =
    cassian::CartesianProduct<cassian::Vector3IntegerTypes,
                              cassian::Vector3FloatingPointTypes>::type;

/**
 * Tuple containing cartesian product of OpenCL C 3-element floating point
 * vector types and OpenCL C 3-element integer vector types.
 */
using Vector3FloatingPointToVector3Integer =
    cassian::CartesianProduct<cassian::Vector3FloatingPointTypes,
                              cassian::Vector3IntegerTypes>::type;

/**
 * Tuple containing cartesian product of OpenCL C 4-element integer vector types
 * and OpenCL C 4-element floating point vector types.
 */
using Vector4IntegerToVector4FloatingPoint =
    cassian::CartesianProduct<cassian::Vector4IntegerTypes,
                              cassian::Vector4FloatingPointTypes>::type;

/**
 * Tuple containing cartesian product of OpenCL C 4-element floating point
 * vector types and OpenCL C 4-element integer vector types.
 */
using Vector4FloatingPointToVector4Integer =
    cassian::CartesianProduct<cassian::Vector4FloatingPointTypes,
                              cassian::Vector4IntegerTypes>::type;

/**
 * Tuple containing cartesian product of OpenCL C 8-element integer vector types
 * and OpenCL C 8-element floating point vector types.
 */
using Vector8IntegerToVector8FloatingPoint =
    cassian::CartesianProduct<cassian::Vector8IntegerTypes,
                              cassian::Vector8FloatingPointTypes>::type;

/**
 * Tuple containing cartesian product of OpenCL C 8-element floating point
 * vector types and OpenCL C 8-element integer vector types.
 */
using Vector8FloatingPointToVector8Integer =
    cassian::CartesianProduct<cassian::Vector8FloatingPointTypes,
                              cassian::Vector8IntegerTypes>::type;

/**
 * Tuple containing cartesian product of OpenCL C 16-element integer vector
 * types and OpenCL C 16-element floating point vector types.
 */
using Vector16IntegerToVector16FloatingPoint =
    cassian::CartesianProduct<cassian::Vector16IntegerTypes,
                              cassian::Vector16FloatingPointTypes>::type;

/**
 * Tuple containing cartesian product of OpenCL C 16-element floating point
 * vector types and OpenCL C 16-element integer vector types.
 */
using Vector16FloatingPointToVector16Integer =
    cassian::CartesianProduct<cassian::Vector16FloatingPointTypes,
                              cassian::Vector16IntegerTypes>::type;

} // namespace cassian

#endif
