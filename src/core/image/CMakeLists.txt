#
# Copyright (C) 2021-2022 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

list(
  APPEND
  PUBLIC_HEADERS
  "include/cassian/image/image.hpp"
  "include/cassian/image/nv12.hpp"
  "include/cassian/image/pixel/common.hpp"
  "include/cassian/image/pixel/normalized.hpp"
  "include/cassian/image/pixel/unnormalized.hpp")
list(APPEND PRIVATE_HEADERS)
list(APPEND SOURCES "src/image.cpp")

add_library(image ${PUBLIC_HEADERS} ${PRIVATE_HEADERS} ${SOURCES})
add_library(cassian::image ALIAS image)

target_link_libraries(
  image
  PUBLIC cassian::runtime cassian::random
  PRIVATE)

target_include_directories(
  image PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
               $<INSTALL_INTERFACE:include>)

set_target_properties(image PROPERTIES FOLDER core)
set_target_properties(image PROPERTIES PUBLIC_HEADER "${PUBLIC_HEADERS}")

cassian_install_target(image)
