#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

list(APPEND PUBLIC_HEADERS "include/cassian/random/random.hpp")
list(APPEND PRIVATE_HEADERS)
list(APPEND SOURCES "src/random.cpp")

add_library(random ${PUBLIC_HEADERS} ${PRIVATE_HEADERS} ${SOURCES})
add_library(cassian::random ALIAS random)

target_include_directories(
  random
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
         $<INSTALL_INTERFACE:include>
  PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

target_link_libraries(
  random
  PUBLIC cassian::vector cassian::fp_types
  PRIVATE cassian::logging)

set_target_properties(random PROPERTIES FOLDER core)
set_target_properties(random PROPERTIES PUBLIC_HEADER "${PUBLIC_HEADERS}")

cassian_install_target(random)
