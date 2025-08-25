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
  "src/work_group_all.cpp"
  "src/work_group_any.cpp"
  "src/work_group_barrier.cpp"
  "src/work_group_broadcast.cpp"
  "src/work_group_reduce.cpp")

list(
  APPEND
  KERNELS
  "kernels/oclc_work_group_functions/work_group_all.cl"
  "kernels/oclc_work_group_functions/work_group_any.cl"
  "kernels/oclc_work_group_functions/work_group_barrier.cl"
  "kernels/oclc_work_group_functions/work_group_broadcast.cl"
  "kernels/oclc_work_group_functions/work_group_reduce.cl")

add_executable(oclc_work_group_functions ${PUBLIC_HEADERS} ${PRIVATE_HEADERS}
                                         ${SOURCES})

target_include_directories(
  oclc_work_group_functions
  PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

target_link_libraries(
  oclc_work_group_functions
  PRIVATE Catch2::Catch2
          cassian::runtime
          cassian::cli
          cassian::utility
          cassian::logging
          cassian::random
          cassian::vector
          cassian::test_harness)

set_target_properties(oclc_work_group_functions PROPERTIES FOLDER
                                                           test_suites/oclc)
cassian_install_target(oclc_work_group_functions)

cassian_target_add_kernels(oclc_work_group_functions ${KERNELS})
