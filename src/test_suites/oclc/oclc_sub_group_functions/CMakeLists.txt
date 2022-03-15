#
# Copyright (C) 2022 Intel Corporation
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
  "src/sub_group_broadcast.cpp"
  "src/sub_group_size.cpp")

list(APPEND KERNELS "kernels/oclc_sub_group_functions/sub_group_broadcast.cl"
     "kernels/oclc_sub_group_functions/sub_group_size.cl")

add_executable(oclc_sub_group_functions ${PUBLIC_HEADERS} ${PRIVATE_HEADERS}
                                        ${SOURCES})

target_include_directories(
  oclc_sub_group_functions
  PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

target_link_libraries(
  oclc_sub_group_functions
  PRIVATE Catch2::Catch2
          cassian::runtime
          cassian::cli
          cassian::utility
          cassian::logging
          cassian::catch2_utils
          cassian::random
          cassian::vector
          cassian::test_harness)

set_target_properties(oclc_sub_group_functions PROPERTIES FOLDER
                                                          test_suites/oclc)
cassian_install_target(oclc_sub_group_functions)

cassian_target_add_kernels(oclc_sub_group_functions ${KERNELS})
