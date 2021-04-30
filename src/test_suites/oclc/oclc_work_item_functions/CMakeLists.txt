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
  "src/get_work_dim.cpp"
  "src/get_global_id.cpp"
  "src/get_local_id.cpp")

list(APPEND KERNELS "kernels/oclc_work_item_functions/get_work_dim.cl"
     "kernels/oclc_work_item_functions/get_global_id.cl"
     "kernels/oclc_work_item_functions/get_local_id.cl")

add_executable(oclc_work_item_functions ${PUBLIC_HEADERS} ${PRIVATE_HEADERS}
                                        ${SOURCES})

target_include_directories(
  oclc_work_item_functions
  PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

target_link_libraries(
  oclc_work_item_functions
  PRIVATE Catch2::Catch2
          cassian::runtime
          cassian::cli
          cassian::utility
          cassian::logging
          cassian::random
          cassian::vector
          cassian::test_harness)

set_target_properties(oclc_work_item_functions PROPERTIES FOLDER
                                                          test_suites/oclc)
cassian_install_target(oclc_work_item_functions)

cassian_target_add_kernels(oclc_work_item_functions ${KERNELS})
