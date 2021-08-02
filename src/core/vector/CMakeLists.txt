#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

list(APPEND PUBLIC_HEADERS "include/cassian/vector/vector.hpp")
list(APPEND PRIVATE_HEADERS)
list(APPEND SOURCES "src/vector.cpp")

add_library(vector ${PUBLIC_HEADERS} ${PRIVATE_HEADERS} ${SOURCES})
add_library(cassian::vector ALIAS vector)

target_include_directories(
  vector PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                $<INSTALL_INTERFACE:include>)

set_target_properties(vector PROPERTIES FOLDER core)
set_target_properties(vector PROPERTIES PUBLIC_HEADER "${PUBLIC_HEADERS}")

cassian_install_target(vector)
