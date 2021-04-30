#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

list(APPEND PUBLIC_HEADERS "include/cassian/cli/cli.hpp")
list(APPEND PRIVATE_HEADERS)
list(APPEND SOURCES "src/cli.cpp")

add_library(cli ${PUBLIC_HEADERS} ${PRIVATE_HEADERS} ${SOURCES})
add_library(cassian::cli ALIAS cli)

target_include_directories(
  cli PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
             $<INSTALL_INTERFACE:include>)

set_target_properties(cli PROPERTIES FOLDER core)
set_target_properties(cli PROPERTIES PUBLIC_HEADER "${PUBLIC_HEADERS}")

cassian_install_target(cli)
