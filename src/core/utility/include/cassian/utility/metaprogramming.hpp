/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_UTILITY_METAPROGRAMMING_HPP
#define CASSIAN_UTILITY_METAPROGRAMMING_HPP

#include <tuple>
#include <type_traits>

namespace cassian {

/**
 * Iterates over @c std::integer_sequence.
 * Example usage:
 * @code{.cpp}
 * static_for(std::index_sequence<2, 3, 4, 8, 16>{}, [&](auto i) {
 *   Vector<int, i.value> v();
 * })
 * @endcode
 *
 * @param[in] seq std::integer_sequence of numbers to iterate through
 * @param[in] func Lambda function containing loop body. It should take
 *                 one parameter. Compile-time constant integer is
 *                 accessible using std::integral_constant::value member.
 */
template <typename F, typename T, T... ints>
void static_for(std::integer_sequence<T, ints...> seq, F func) {
  (void)seq;
  (func(std::integral_constant<T, ints>{}), ...);
}

namespace detail {

/**
 * Helper struct for CartesianProduct.
 * Creates @c std::tuple containing tuples with first type = T, and second type
 * = (U1, U2, ...).
 *
 * @tparam T first type
 * @tparam U... types to use as second type
 */
template <typename T, typename... U> struct ProductRow {
  /**
   * Tuple with two-element tuples of (T, U...).
   */
  using type = std::tuple<std::tuple<T, U>...>;
};

/**
 * Specialization of ProductRow for @c std::tuple as first parameter.
 * Allows nesting CartesianProduct calls to produce permutations of more than
 * 2 sets of types.
 *
 * @tparam T... types wrapped in @c std::tuple
 * @tparam U... types to use as last type
 */
template <typename... T, typename... U>
struct ProductRow<std::tuple<T...>, U...> {
  using type = std::tuple<std::tuple<T..., U>...>;
};

/**
 * Helper struct for CartesianProduct.
 * Creates @c std::tuple containing elements of type V<T, (U1, U2, U3...)>.
 *
 * @tparam V<T, U> container type
 * @tparam T type
 * @tparam U... sizes
 */
template <template <typename, int N, int = N> typename V, typename T,
          size_t... U>
struct ContainerProductRow {
  /**
   * Tuple with elements of type T and sizes U.
   */
  using type = std::tuple<V<T, U>...>;
};

/**
 * Helper struct for CartesianProduct.
 * Creates @c std::tuple containing elements of type CLC_TYPE<(N1, N2, N3...)>.
 *
 * @tparam CLC_TYPE<N> type
 * @tparam N... sizes
 */
template <template <size_t> typename CLC_TYPE, size_t... N>
struct CLCTypeProductRow {
  using type = std::tuple<CLC_TYPE<N>...>;
};

} // namespace detail

/**
 * Collapses parameter types of multiple @c std::tuples into one.
 *
 * Example usage:
 * @code{.cpp}
 * using ScalarTypes = TupleConcat<
 *   std::tuple<int, float>, std::tuple<char, double>>::type;
 * // ScalarTypes = std::tuple<int, float, char, double>
 * @endcode
 *
 * @tparam T... @c std::tuple (one or more)
 */
template <typename... T> struct TupleConcat {
  /**
   * Combined @c std::tuple .
   */
  using type = std::tuple<T...>;
};

/**
 * Specialization of TupleConcat for odd @c std::tuple.
 */
template <typename... T> struct TupleConcat<std::tuple<T...>> {
  /**
   * Combined @c std::tuple .
   */
  using type = std::tuple<T...>;
};

/**
 * Specialization of TupleConcat for empty @c std::tuple at the end.
 */
template <typename... T> struct TupleConcat<std::tuple<T...>, std::tuple<>> {
  /**
   * Combined @c std::tuple .
   */
  using type = std::tuple<T...>;
};

/**
 * Specialization of TupleConcat for no parameters.
 */
template <> struct TupleConcat<> {
  /**
   * Combined @c std::tuple .
   */
  using type = std::tuple<>;
};

/**
 * Specialization of TupleConcat for more than two @c std::tuples.
 */
template <typename... T, typename... U, typename... Rest>
struct TupleConcat<std::tuple<T...>, std::tuple<U...>, Rest...> {
  /**
   * Combined @c std::tuple .
   */
  using type = typename TupleConcat<std::tuple<T..., U...>,
                                    typename TupleConcat<Rest...>::type>::type;
};

template <typename... T> struct CartesianProduct {};

/**
 * Creates an std::tuple containing dot product of types in input tuples, each
 * element represented as std::tuple with two parameters.
 *
 * Example usage:
 * @code{.cpp}
 * using ScalarPairs = CartesianProduct<
 *   std::tuple<int, float>, std::tuple<int, float>::type;
 * // ScalarPairs = std::tuple<
 * //   std::tuple<int, int>, std::tuple<int, float>,
 * //   std::tuple<float, int>, std::tuple<float, float>>
 * @endcode
 *
 * @tparam T... Types in first tuple
 * @tparam U... Types in second tuple
 */
template <typename... T, typename... U>
struct CartesianProduct<std::tuple<T...>, std::tuple<U...>> {
  /**
   * @c std::tuple containing two-element @c std::tuples of types.
   */
  using type =
      typename TupleConcat<typename detail::ProductRow<T, U...>::type...>::type;
};

/**
 * Creates an std::tuple containing dot product of types and integers, each
 * element represented as V (parameter of #type).
 *
 * Example usage:
 * @code{.cpp}
 * using VectorTypes = CartesianProduct<
 *   std::tuple<int, float>, std::index_sequence<2, 4>>::type<Vector>;
 * // VectorTypes = std::tuple<
 * //   Vector<int, 2>, Vector<int, 4>, Vector<float, 2>, Vector<float, 4>>
 * @endcode
 *
 * @tparam T... container types
 * @tparam U... container sizes
 */
template <typename... T, size_t... U>
struct CartesianProduct<std::tuple<T...>, std::index_sequence<U...>> {
  /**
   * Tuple containing containers of type V with types and sizes as parameters.
   *
   * @tparam V type of container (currently only compatible with Vector)
   */
  template <template <typename, int N, int = N> typename V>
  using type = typename TupleConcat<
      typename detail::ContainerProductRow<V, T, U...>::type...>::type;
};

/**
 * Creates an std::tuple containing dot product of integers and OpenCL C type.
 *
 * Example usage:
 * @code{.cpp}
 * using TypesFloat = CartesianProduct<std::index_sequence<2,
 * 4>>::type<OpenCLCFloat>;
 * // TypesFloat = std::tuple<OpenCLCFloat<2>, OpenCLCFloat<4>>
 * @endcode
 *
 * @tparam U... container sizes
 */
template <size_t... U> struct CartesianProduct<std::index_sequence<U...>> {
  /**
   * Tuple containing OpenCL C vector wrappers of type T.
   *
   * @tparam T OpenCL C type wrapper
   */
  template <template <size_t> typename T>
  using type = typename detail::CLCTypeProductRow<T, U...>::type;
};

template <typename... T> struct TupleZip {};

/**
 * Creates an std::tuple containing pairs of corresponding types in input
 * tuples, each element represented as std::tuple with two parameters.
 *
 * Example usage:
 * @code{.cpp}
 * using ScalarPairs = TupleZip<
 *   std::tuple<float, int>, std::tuple<short, char>::type;
 * // ScalarPairs = std::tuple<
 * //   std::tuple<float, short>, std::tuple<int, char>>
 * @endcode
 *
 * @tparam T... Types in first tuple
 * @tparam U... Types in second tuple
 */
template <typename... T, typename... U>
struct TupleZip<std::tuple<T...>, std::tuple<U...>> {
  /**
   * @c std::tuple containing two-element @c std::tuples of types.
   */
  using type = std::tuple<std::tuple<T, U>...>;
};

template <typename... T, typename... U, typename... V>
struct TupleZip<std::tuple<T...>, std::tuple<U...>, std::tuple<V...>> {
  /**
   * @c std::tuple containing three-element @c std::tuples of types.
   */
  using type = std::tuple<std::tuple<T, U, V>...>;
};

} // namespace cassian

#endif