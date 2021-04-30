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
  "src/common.cpp"
  "src/main.cpp"
  "src/test_config.cpp"
  "src/macros.cpp"
  "src/extensions.cpp")

list(APPEND KERNELS
     "kernels/oclc_preprocessor_directives_and_macros/test_macros.cl"
     "kernels/oclc_preprocessor_directives_and_macros/test_extensions.cl")

add_executable(oclc_preprocessor_directives_and_macros
               ${PUBLIC_HEADERS} ${PRIVATE_HEADERS} ${SOURCES})

target_include_directories(
  oclc_preprocessor_directives_and_macros
  PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

target_link_libraries(
  oclc_preprocessor_directives_and_macros
  PRIVATE Catch2::Catch2
          cassian::runtime
          cassian::cli
          cassian::utility
          cassian::logging
          cassian::random
          cassian::vector
          cassian::test_harness)

set_target_properties(oclc_preprocessor_directives_and_macros
                      PROPERTIES FOLDER test_suites/oclc)
cassian_install_target(oclc_preprocessor_directives_and_macros)

cassian_target_add_kernels(oclc_preprocessor_directives_and_macros ${KERNELS})
