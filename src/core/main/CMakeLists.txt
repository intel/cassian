#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

list(APPEND PUBLIC_HEADERS "include/cassian/main/config.hpp"
     "include/cassian/main/flags_builder.hpp"
     "include/cassian/main/test_helper.hpp")
list(APPEND PRIVATE_HEADERS)
list(APPEND SOURCES "src/main.cpp" "src/test_helper.cpp" "src/config.cpp")

add_library(main STATIC ${PUBLIC_HEADERS} ${PRIVATE_HEADERS} ${SOURCES})
add_library(cassian::main ALIAS main)

target_include_directories(
  main PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
              $<INSTALL_INTERFACE:include>)

target_link_libraries(
  main
  PUBLIC Catch2::Catch2 cassian::fp_types cassian::runtime cassian::image
         cassian::test_harness
  PRIVATE cassian::cli cassian::utility)

set_target_properties(main PROPERTIES FOLDER core)
set_target_properties(main PROPERTIES PUBLIC_HEADER "${PUBLIC_HEADERS}")

cassian_install_target(main)
