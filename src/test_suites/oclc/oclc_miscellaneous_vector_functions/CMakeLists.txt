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
  "src/vec_step.cpp"
  "src/shuffle.cpp")

list(APPEND KERNELS "kernels/oclc_miscellaneous_vector_functions/vec_step.cl"
     "kernels/oclc_miscellaneous_vector_functions/shuffle.cl")

add_executable(oclc_miscellaneous_vector_functions
               ${PUBLIC_HEADERS} ${PRIVATE_HEADERS} ${SOURCES})

target_include_directories(
  oclc_miscellaneous_vector_functions
  PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

target_link_libraries(
  oclc_miscellaneous_vector_functions
  PRIVATE Catch2::Catch2
          cassian::runtime
          cassian::catch2_utils
          cassian::cli
          cassian::utility
          cassian::logging
          cassian::random
          cassian::vector
          cassian::test_harness)

set_target_properties(oclc_miscellaneous_vector_functions
                      PROPERTIES FOLDER test_suites/oclc)
cassian_install_target(oclc_miscellaneous_vector_functions)

cassian_target_add_kernels(oclc_miscellaneous_vector_functions ${KERNELS})
