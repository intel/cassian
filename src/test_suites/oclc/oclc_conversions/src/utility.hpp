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
          RoundingMode... RND, OverflowHandling... SAT>
struct RoundingModeProductRow<DestRND,
                              std::tuple<TestCase<T, U, RND, SAT>...>> {
  using type = std::tuple<TestCase<T, U, DestRND, SAT>...>;
};

template <OverflowHandling SAT, typename... T>
struct OverflowHandlingProductRow {};

template <OverflowHandling DestSAT, typename... T, typename... U,
          RoundingMode... RND, OverflowHandling... SAT>
struct OverflowHandlingProductRow<DestSAT,
                                  std::tuple<TestCase<T, U, RND, SAT>...>> {
  using type = std::tuple<TestCase<T, U, RND, DestSAT>...>;
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

namespace cassian {
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
