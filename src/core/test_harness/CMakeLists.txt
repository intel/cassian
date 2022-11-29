#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

list(APPEND PUBLIC_HEADERS "include/cassian/test_harness/test_harness.hpp"
     "include/cassian/test_harness/test_config.hpp")
list(APPEND PRIVATE_HEADERS)
list(APPEND SOURCES "src/test_harness.cpp" "src/test_config.cpp")

add_library(test_harness ${PUBLIC_HEADERS} ${PRIVATE_HEADERS} ${SOURCES})
add_library(cassian::test_harness ALIAS test_harness)

target_include_directories(
  test_harness PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                      $<INSTALL_INTERFACE:include>)

target_link_libraries(
  test_harness
  PUBLIC cassian::cli cassian::runtime cassian::fp_types
  PRIVATE cassian::logging)

set_target_properties(test_harness PROPERTIES FOLDER core)
set_target_properties(test_harness PROPERTIES PUBLIC_HEADER "${PUBLIC_HEADERS}")

cassian_install_target(test_harness)
