#
# Copyright (C) 2021-2023 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

list(APPEND PUBLIC_HEADERS)
list(APPEND PRIVATE_HEADERS "src/common.hpp" "src/test_config.hpp")
list(
  APPEND
  SOURCES
  "src/main.cpp"
  "src/common.cpp"
  "src/test_config.cpp"
  "src/arguments.cpp"
  "src/excess_arguments.cpp"
  "src/text.cpp")
list(APPEND KERNELS "kernels/oclc_printf/arguments.cl"
     "kernels/oclc_printf/excess_arguments.cl" "kernels/oclc_printf/text.cl")

add_executable(oclc_printf ${PUBLIC_HEADERS} ${PRIVATE_HEADERS} ${SOURCES})

target_include_directories(
  oclc_printf PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

target_link_libraries(
  oclc_printf
  PRIVATE Catch2::Catch2
          cassian::runtime
          cassian::cli
          cassian::utility
          cassian::logging
          cassian::vector
          cassian::test_harness
          cassian::catch2_utils)

set_target_properties(oclc_printf PROPERTIES FOLDER test_suites/oclc)
cassian_install_target(oclc_printf)

cassian_target_add_kernels(oclc_printf ${KERNELS})
