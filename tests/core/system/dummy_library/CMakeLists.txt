#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

add_library(dummy_library SHARED include/dummy_library.hpp
                                 src/dummy_library.cpp)

target_include_directories(
  dummy_library PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                        $<INSTALL_INTERFACE:include>)

set_target_properties(dummy_library PROPERTIES FOLDER tests/core/system)

cassian_install_target(dummy_library)
