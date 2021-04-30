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
  "src/declaration.cpp"
  "src/vector_literals.cpp"
  "src/vector_components.cpp"
  "src/test_config.cpp")
list(APPEND KERNELS "kernels/oclc_data_types/declaration.cl"
     "kernels/oclc_data_types/vector_literals.cl"
     "kernels/oclc_data_types/vector_components.cl")

add_executable(oclc_data_types ${PUBLIC_HEADERS} ${PRIVATE_HEADERS} ${SOURCES})

target_include_directories(
  oclc_data_types PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

target_link_libraries(
  oclc_data_types
  PRIVATE Catch2::Catch2
          cassian::runtime
          cassian::catch2_utils
          cassian::cli
          cassian::utility
          cassian::logging
          cassian::vector
          cassian::test_harness)

set_target_properties(oclc_data_types PROPERTIES FOLDER test_suites/oclc)
cassian_install_target(oclc_data_types)

cassian_target_add_kernels(oclc_data_types ${KERNELS})
