#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

list(
  APPEND
  PUBLIC_HEADERS
  "include/cassian/fp_types/bfloat16.hpp"
  "include/cassian/fp_types/half.hpp"
  "include/cassian/fp_types/tfloat.hpp"
  "include/cassian/fp_types/type_traits.hpp"
  "include/cassian/fp_types/math.hpp")
list(APPEND PRIVATE_HEADERS)
list(APPEND SOURCES "src/bfloat16.cpp" "src/half.cpp" "src/tfloat.cpp")

add_library(fp_types ${PUBLIC_HEADERS} ${PRIVATE_HEADERS} ${SOURCES})
add_library(cassian::fp_types ALIAS fp_types)

target_include_directories(
  fp_types PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                  $<INSTALL_INTERFACE:include>)

set_target_properties(fp_types PROPERTIES FOLDER core)
set_target_properties(fp_types PROPERTIES PUBLIC_HEADER "${PUBLIC_HEADERS}")

cassian_install_target(fp_types)
