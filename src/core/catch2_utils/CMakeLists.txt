#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

list(APPEND PUBLIC_HEADERS "include/cassian/catch2_utils/catch2_utils.hpp")
list(APPEND PRIVATE_HEADERS)
list(APPEND SOURCES)

add_library(catch2_utils INTERFACE)
add_library(cassian::catch2_utils ALIAS catch2_utils)

target_link_libraries(catch2_utils INTERFACE Catch2::Catch2)

target_include_directories(
  catch2_utils INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                         $<INSTALL_INTERFACE:include>)

cassian_install_target(catch2_utils)
