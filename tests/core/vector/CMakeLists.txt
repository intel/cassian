#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

add_executable(test_vector src/main.cpp src/vector.cpp)

target_include_directories(
  test_vector PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

target_link_libraries(test_vector PRIVATE Catch2::Catch2 cassian::vector
                                          cassian::utility)

set_target_properties(test_vector PROPERTIES FOLDER tests/core)
cassian_install_target(test_vector)

add_test(NAME test_vector COMMAND test_vector)
