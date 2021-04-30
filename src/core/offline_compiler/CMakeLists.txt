#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

list(APPEND PUBLIC_HEADERS
     "include/cassian/offline_compiler/offline_compiler.hpp")
list(APPEND PRIVATE_HEADERS)
list(APPEND SOURCES "src/offline_compiler.cpp")

add_library(offline_compiler ${PUBLIC_HEADERS} ${PRIVATE_HEADERS} ${SOURCES})
add_library(cassian::offline_compiler ALIAS offline_compiler)

target_include_directories(
  offline_compiler PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                          $<INSTALL_INTERFACE:include>)

target_link_libraries(offline_compiler PRIVATE cassian::utility
                                               cassian::logging cassian::system)

set_target_properties(offline_compiler PROPERTIES FOLDER core)
set_target_properties(offline_compiler PROPERTIES PUBLIC_HEADER
                                                  "${PUBLIC_HEADERS}")

cassian_install_target(offline_compiler)
