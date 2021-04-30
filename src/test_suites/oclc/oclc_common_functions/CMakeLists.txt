#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

list(APPEND PUBLIC_HEADERS)
list(APPEND PRIVATE_HEADERS "src/common.hpp" "src/test_config.hpp"
     "src/enum_definitions.hpp" "src/common_functions.hpp")
list(APPEND SOURCES "src/main.cpp" "src/common.cpp" "src/test_config.cpp"
     "src/common_functions.cpp")

list(APPEND KERNELS "kernels/oclc_common_functions/common_functions.cl")

add_executable(oclc_common_functions ${PUBLIC_HEADERS} ${PRIVATE_HEADERS}
                                     ${SOURCES})

target_include_directories(
  oclc_common_functions
  PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

target_link_libraries(
  oclc_common_functions
  PRIVATE Catch2::Catch2
          cassian::runtime
          cassian::cli
          cassian::utility
          cassian::logging
          cassian::random
          cassian::vector
          cassian::test_harness
          cassian::catch2_utils)

set_target_properties(oclc_common_functions PROPERTIES FOLDER test_suites/oclc)
cassian_install_target(oclc_common_functions)

cassian_target_add_kernels(oclc_common_functions ${KERNELS})
