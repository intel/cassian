#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

add_executable(test_utility src/main.cpp src/utility.cpp src/math.cpp
                            src/comparators.cpp)

target_include_directories(
  test_utility PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

target_link_libraries(test_utility PRIVATE Catch2::Catch2 cassian::utility
                                           cassian::runtime)

set_target_properties(test_utility PROPERTIES FOLDER tests/core)
cassian_install_target(test_utility)

add_test(NAME test_utility COMMAND test_utility)
