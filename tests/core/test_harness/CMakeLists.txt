#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

add_executable(test_test_harness src/main.cpp src/test_harness.cpp)

target_include_directories(
  test_test_harness
  PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

target_link_libraries(
  test_test_harness PRIVATE Catch2::Catch2 cassian::test_harness
                            cassian::utility cassian::vector cassian::fp_types)

set_target_properties(test_test_harness PROPERTIES FOLDER tests/core)
cassian_install_target(test_test_harness)

add_test(NAME test_test_harness COMMAND test_test_harness)
