/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/system/factory.hpp>
#include <cassian/system/library.hpp>
#include <catch2/catch.hpp>
#include <memory>

namespace ca = cassian;

#if defined(_WIN32)
constexpr char dummy_library_name[] = "cassian_dummy_library.dll";
#elif defined(__linux__)
constexpr char dummy_library_name[] = "libcassian_dummy_library.so";
#endif

TEST_CASE("library", "") {
  SECTION("library can be loaded") {
    REQUIRE_NOTHROW(ca::load_library(dummy_library_name));
  }

  SECTION("invalid library name results in exception") {
    REQUIRE_THROWS_MATCHES(
        ca::load_library("INVALID_LIBRARY"), ca::LibraryNotFoundException,
        Catch::Message("Failed to load library: INVALID_LIBRARY"));
  }
}

TEST_CASE("function", "") {
  std::unique_ptr<ca::Library> library = ca::load_library(dummy_library_name);

  SECTION("function can be loaded from library") {
    REQUIRE_NOTHROW(library->get_function("dummy_function"));
  }

  SECTION("invalid function name results in exception") {
    REQUIRE_THROWS_MATCHES(
        library->get_function("INVALID_FUNCTION"),
        ca::LibraryFunctionNotFoundException,
        Catch::Message("Failed to load library function: INVALID_FUNCTION"));
  }

  SECTION("function can be called") {
    typedef int (*pfn_dummy_function)(int);
    auto function = reinterpret_cast<pfn_dummy_function>(
        library->get_function("dummy_function"));
    REQUIRE(3 == function(3));
  }
}
