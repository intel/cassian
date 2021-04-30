#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

add_executable(test_random src/main.cpp src/random.cpp)

target_include_directories(
  test_random PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

target_link_libraries(test_random PRIVATE Catch2::Catch2 cassian::random
                                          cassian::utility)

set_target_properties(test_random PROPERTIES FOLDER tests/core)
cassian_install_target(test_random)

add_test(NAME test_random COMMAND test_random)
