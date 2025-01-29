/*
 * Copyright (C) 2025 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <array>
#include <cassian/runtime/openclc_types.hpp>
#include <cassian/utility/metaprogramming.hpp>
#include <catch2/catch.hpp>
#include <type_traits>

namespace {
namespace ca = cassian;
namespace cad = cassian::detail;

enum class A0 {};
enum class A1 {};
enum class A2 {};
enum class B0 {};
enum class B1 {};
enum class B2 {};
enum class C0 {};
enum class C1 {};
enum class C2 {};
enum class C3 {};
enum class D0 {};
enum class D1 {};
enum class D2 {};
enum class P0 {};
enum class P1 {};

#define TYPE_SET_A A0, A1, A2     // NOLINT
#define TYPE_SET_B B0, B1, B2     // NOLINT
#define TYPE_SET_C C0, C1, C2, C3 // NOLINT
#define TYPE_SET_D D0, D1, D2     // NOLINT

using tupleA = std::tuple<TYPE_SET_A>;
using tupleB = std::tuple<TYPE_SET_B>;
using tupleC = std::tuple<TYPE_SET_C>;
using tupleD = std::tuple<TYPE_SET_D>;

TEST_CASE("StaticFor", "[metaprogramming]") {
  SECTION("Trivial sequence") {
    constexpr std::tuple index_tuple = {0, 1, 2, 3};
    ca::static_for(
        std::make_index_sequence<std::tuple_size_v<decltype(index_tuple)>>{},
        [&index_tuple](auto seqElement) {
          REQUIRE(seqElement.value == std::get<seqElement.value>(index_tuple));
        });
  }
}

TEST_CASE("TupleZip", "[metaprogramming]") {

  SECTION("Zip two tuples") {
    REQUIRE(std::is_same_v<ca::tuple_zip_t<tupleA, tupleB>,
                           std::tuple<std::tuple<A0, B0>, std::tuple<A1, B1>,
                                      std::tuple<A2, B2>>>);
    REQUIRE(!std::is_same_v<ca::tuple_zip_t<tupleB, tupleA>,
                            ca::tuple_zip_t<tupleA, tupleB>>);
  }

  SECTION("Zip three tuples") {
    REQUIRE(std::is_same_v<
            ca::tuple_zip_t<tupleA, tupleB, tupleD>,
            std::tuple<std::tuple<A0, B0, D0>, std::tuple<A1, B1, D1>,
                       std::tuple<A2, B2, D2>>>);
  }
}

TEST_CASE("TupleConcat", "[metaprogramming]") {
  SECTION("Concat empty") {
    REQUIRE(std::is_same_v<ca::tuple_concat_t<>, std::tuple<>>);
    REQUIRE(std::is_same_v<ca::tuple_concat_t<std::tuple<>>, std::tuple<>>);
    REQUIRE(std::is_same_v<ca::tuple_concat_t<std::tuple<>, std::tuple<>>,
                           std::tuple<>>);
  }

  SECTION("Concat 1") {
    REQUIRE(std::is_same_v<ca::tuple_concat_t<tupleA>, tupleA>);
  }

  SECTION("Concat 2") {
    REQUIRE(std::is_same_v<ca::tuple_concat_t<tupleA, tupleB>,
                           std::tuple<TYPE_SET_A, TYPE_SET_B>>);
    REQUIRE(std::is_same_v<ca::tuple_concat_t<tupleB, tupleA>,
                           std::tuple<TYPE_SET_B, TYPE_SET_A>>);
    REQUIRE(!std::is_same_v<ca::tuple_concat_t<tupleB, tupleA>,
                            ca::tuple_concat_t<tupleA, tupleB>>);
  }

  SECTION("Concat 3") {
    REQUIRE(std::is_same_v<ca::tuple_concat_t<tupleA, tupleB, tupleD>,
                           std::tuple<TYPE_SET_A, TYPE_SET_B, TYPE_SET_D>>);
  }

  SECTION("Concat with empty") {
    REQUIRE(std::is_same_v<ca::tuple_concat_t<tupleA, std::tuple<>>, tupleA>);
    REQUIRE(std::is_same_v<ca::tuple_concat_t<std::tuple<>, tupleA>, tupleA>);
    REQUIRE(
        std::is_same_v<ca::tuple_concat_t<tupleA, std::tuple<>, tupleB, tupleD>,
                       std::tuple<TYPE_SET_A, TYPE_SET_B, TYPE_SET_D>>);
  }

  SECTION("Concat scalar") {
    REQUIRE(std::is_same_v<ca::tuple_concat_t<TYPE_SET_A>, tupleA>);
  }

  SECTION("Concat tuple with scalar") {
    REQUIRE(std::is_same_v<ca::tuple_concat_t<tupleA, tupleB, TYPE_SET_C>,
                           std::tuple<TYPE_SET_A, TYPE_SET_B, TYPE_SET_C>>);
  }
}

TEST_CASE("ProductRow", "[metaprogramming]") {
  SECTION("Basic product row") {
    using RowT = cad::ProductRow<P0, TYPE_SET_A>::type;
    constexpr size_t expected_row_size = std::tuple_size_v<tupleA>;

    // (P0, A0), (P0, A1) .., (P0, AN)
    ca::static_for(
        std::make_index_sequence<expected_row_size>{}, [](auto index) {
          REQUIRE(std::is_same_v<
                  std::tuple_element_t<index.value, RowT>,
                  std::tuple<P0, std::tuple_element_t<index.value, tupleA>>>);
        });
  }

  SECTION("Product row by tuple") {
    using PredicateT = std::tuple<P0, P1>;
    using RowT = cad::ProductRow<PredicateT, TYPE_SET_A>::type;
    constexpr size_t expected_row_size = std::tuple_size_v<tupleA>;

    REQUIRE(expected_row_size == std::tuple_size_v<RowT>);
    ca::static_for(
        std::make_index_sequence<expected_row_size>{}, [](auto index) {
          REQUIRE(std::is_same_v<
                  std::tuple_element_t<index.value, RowT>,
                  std::tuple<P0, P1,
                             std::tuple_element_t<index.value, tupleA>>>);
        });
  }
}

TEST_CASE("CartesianProduct", "[metaprogramming]") {
  using ProductAC = ca::cartesian_product_t<tupleA, tupleC>;
  using ProductCA = ca::cartesian_product_t<tupleC, tupleA>;

  constexpr size_t product_size =
      std::tuple_size_v<tupleA> * std::tuple_size_v<tupleC>;

  SECTION("Product dimentions") {
    REQUIRE(std::tuple_size_v<ProductAC> == product_size);
    REQUIRE(std::tuple_size_v<std::tuple_element_t<0, ProductAC>> == 2);

    REQUIRE(std::tuple_size_v<ProductCA> == product_size);
    REQUIRE(std::tuple_size_v<std::tuple_element_t<0, ProductCA>> == 2);

    REQUIRE(!std::is_same_v<ProductAC, ProductCA>);
  }

  // (A0, C0), (A0, C1) .., (A0, CN)
  // ...
  // (AM, C0), (AM, C1) .., (AM, CN)
  SECTION("Product type check") {
    REQUIRE(!std::is_same_v<ProductAC, ProductCA>);
    ca::static_for(std::make_index_sequence<product_size>{}, [](auto index) {
      constexpr size_t row_len = std::tuple_size_v<tupleC>;
      REQUIRE(std::is_same_v<
              std::tuple_element_t<index.value, ProductAC>,
              std::tuple<std::tuple_element_t<index.value / row_len, tupleA>,
                         std::tuple_element_t<index.value % row_len, tupleC>>>);
    });
  }
}

template <typename T, int N, int = N>
using ContainerT = typename std::array<T, N>;

TEST_CASE("ProductRow for containers", "[metaprogramming]") {
  SECTION("Generate tuple of arrays") {
    using ArrayTupleT = cad::ContainerProductRow<ContainerT, A0, 0, 1, 2>::type;
    ca::static_for(std::make_index_sequence<3>{}, [](auto index) {
      REQUIRE(std::is_same_v<std::tuple_element_t<index.value, ArrayTupleT>,
                             std::array<A0, index.value>>);
    });
  }

  SECTION("Generate tuple CLC containers") {
    using ClcVector = cad::CLCTypeProductRow<cad::OpenCLCChar, 0, 1, 2>::type;
    ca::static_for(std::make_index_sequence<3>{}, [](auto index) {
      REQUIRE(std::is_same_v<std::tuple_element_t<index.value, ClcVector>,
                             cad::OpenCLCChar<index.value>>);
    });
  }
}

TEST_CASE("Cartesian product for containers", "[metaprogramming]") {
  using LenList = std::index_sequence<0, 1, 2>;

  SECTION("Empty tuple of arrays") {
    using ArrayTypesA =
        ca::CartesianProduct<std::tuple<>, LenList>::type<ContainerT>;
    REQUIRE(std::tuple_size_v<ArrayTypesA> == 0);
  }

  SECTION("Tuple of arrays") {
    using ArrayTypes = ca::CartesianProduct<tupleA, LenList>::type<ContainerT>;
    constexpr size_t product_size = std::tuple_size_v<tupleA> * LenList::size();
    REQUIRE(product_size == std::tuple_size_v<ArrayTypes>);

    // CT<A0, 0>, ... , CT<A0, 2>
    // ...
    // CT<A2, 0>, ... , CT<A2, 2>
    ca::static_for(std::make_index_sequence<product_size>{}, [](auto index) {
      constexpr size_t row_len = std::tuple_size_v<tupleA>;
      constexpr auto product_size = index.value % row_len;

      REQUIRE(std::is_same_v<
              std::tuple_element_t<index.value, ArrayTypes>,
              ContainerT<std::tuple_element_t<index.value / row_len, tupleA>,
                         product_size>>);
    });
  }

  SECTION("Tuple of CLC containers") {
    using ArrayTypes = ca::CartesianProduct<LenList>::type<cad::OpenCLCChar>;
    REQUIRE(LenList::size() == std::tuple_size_v<ArrayTypes>);

    // OpenCLCChar<0>, ... , OpenCLCChar<2>
    ca::static_for(std::make_index_sequence<LenList::size()>{}, [](auto index) {
      REQUIRE(std::is_same_v<std::tuple_element_t<index.value, ArrayTypes>,
                             cad::OpenCLCChar<index.value>>);
    });
  }
}
} // namespace
