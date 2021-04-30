#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

list(APPEND PUBLIC_HEADERS "include/cassian/logging/logging.hpp")
list(APPEND PRIVATE_HEADERS)
list(APPEND SOURCES "src/logging.cpp")

add_library(logging ${PUBLIC_HEADERS} ${PRIVATE_HEADERS} ${SOURCES})
add_library(cassian::logging ALIAS logging)

target_include_directories(
  logging PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                 $<INSTALL_INTERFACE:include>)

set_target_properties(logging PROPERTIES FOLDER core)
set_target_properties(logging PROPERTIES PUBLIC_HEADER "${PUBLIC_HEADERS}")

cassian_install_target(logging)
