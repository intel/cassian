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
  "src/async_work_group_copy.cpp"
  "src/async_work_group_strided_copy.cpp"
  "src/prefetch.cpp"
  "src/wait_group_events.cpp")

list(
  APPEND
  KERNELS
  "kernels/oclc_async_copy_functions/async_work_group_copy.cl"
  "kernels/oclc_async_copy_functions/async_work_group_strided_copy.cl"
  "kernels/oclc_async_copy_functions/wait_group_events.cl"
  "kernels/oclc_async_copy_functions/prefetch.cl")

add_executable(oclc_async_copy_functions ${PUBLIC_HEADERS} ${PRIVATE_HEADERS}
                                         ${SOURCES})

target_include_directories(
  oclc_async_copy_functions
  PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

target_link_libraries(
  oclc_async_copy_functions
  PRIVATE Catch2::Catch2
          cassian::runtime
          cassian::catch2_utils
          cassian::cli
          cassian::utility
          cassian::logging
          cassian::random
          cassian::vector
          cassian::test_harness)

set_target_properties(oclc_async_copy_functions PROPERTIES FOLDER
                                                           test_suites/oclc)
cassian_install_target(oclc_async_copy_functions)

cassian_target_add_kernels(oclc_async_copy_functions ${KERNELS})
