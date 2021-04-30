#
# Copyright (C) 2021 Intel Corporation
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
  "src/reqd_work_group_size.cpp"
  "src/multiple_kernels.cpp")
list(APPEND KERNELS "kernels/oclc_function_qualifiers/reqd_work_group_size.cl"
     "kernels/oclc_function_qualifiers/multiple_kernels.cl")

add_executable(oclc_function_qualifiers ${PUBLIC_HEADERS} ${PRIVATE_HEADERS}
                                        ${SOURCES})

target_include_directories(
  oclc_function_qualifiers
  PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

target_link_libraries(
  oclc_function_qualifiers
  PRIVATE Catch2::Catch2
          cassian::runtime
          cassian::cli
          cassian::utility
          cassian::logging
          cassian::vector
          cassian::test_harness
          cassian::catch2_utils)

set_target_properties(oclc_function_qualifiers PROPERTIES FOLDER
                                                          test_suites/oclc)
cassian_install_target(oclc_function_qualifiers)

cassian_target_add_kernels(oclc_function_qualifiers ${KERNELS})
