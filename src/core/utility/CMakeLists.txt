#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

list(
  APPEND
  PUBLIC_HEADERS
  "include/cassian/utility/utility.hpp"
  "include/cassian/utility/version.hpp"
  "include/cassian/utility/metaprogramming.hpp"
  "include/cassian/utility/math.hpp"
  "include/cassian/utility/matchers.hpp"
  "include/cassian/utility/comparators.hpp")
list(APPEND PRIVATE_HEADERS "src/version_config.hpp")
list(APPEND SOURCES "src/utility.cpp" "src/version.cpp" "src/matchers.cpp"
     "${PROJECT_BINARY_DIR}/generated/version_config.cpp")

configure_file(src/version_config.cpp.in
               ${PROJECT_BINARY_DIR}/generated/version_config.cpp)

add_library(utility ${PUBLIC_HEADERS} ${PRIVATE_HEADERS} ${SOURCES})
add_library(cassian::utility ALIAS utility)

target_include_directories(
  utility
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
         $<INSTALL_INTERFACE:include>
  PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

target_link_libraries(
  utility
  PUBLIC cassian::vector cassian::logging
  PRIVATE Catch2::Catch2)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION
                                                 VERSION_LESS 9.1)
  target_link_libraries(utility PRIVATE -lstdc++fs)
endif()

set_target_properties(utility PROPERTIES FOLDER core)
set_target_properties(utility PROPERTIES PUBLIC_HEADER "${PUBLIC_HEADERS}")

cassian_install_target(utility)
