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
  "src/isequal.cpp"
  "src/isnotequal.cpp"
  "src/select.cpp")

list(APPEND KERNELS "kernels/oclc_relational_functions/isequal.cl"
     "kernels/oclc_relational_functions/isnotequal.cl"
     "kernels/oclc_relational_functions/select.cl")

add_executable(oclc_relational_functions ${PUBLIC_HEADERS} ${PRIVATE_HEADERS}
                                         ${SOURCES})

target_include_directories(
  oclc_relational_functions
  PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

target_link_libraries(
  oclc_relational_functions
  PRIVATE Catch2::Catch2
          cassian::runtime
          cassian::cli
          cassian::utility
          cassian::logging
          cassian::random
          cassian::vector
          cassian::test_harness)

set_target_properties(oclc_relational_functions PROPERTIES FOLDER
                                                           test_suites/oclc)
cassian_install_target(oclc_relational_functions)

cassian_target_add_kernels(oclc_relational_functions ${KERNELS})
