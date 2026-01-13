#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

list(APPEND PUBLIC_HEADERS "include/cassian/reference/dp4a.hpp"
     "include/cassian/reference/extended_bit_operations.hpp")
list(APPEND PRIVATE_HEADERS)
list(APPEND SOURCES "src/dp4a.cpp" "src/extended_bit_operations.cpp")

add_library(reference ${PUBLIC_HEADERS} ${PRIVATE_HEADERS} ${SOURCES})
add_library(cassian::reference ALIAS reference)

target_link_libraries(
  reference
  PUBLIC cassian::vector
  PRIVATE cassian::utility)

target_include_directories(
  reference PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                   $<INSTALL_INTERFACE:include>)

set_target_properties(reference PROPERTIES FOLDER core)
set_target_properties(reference PROPERTIES PUBLIC_HEADER "${PUBLIC_HEADERS}")

cassian_install_target(reference)
