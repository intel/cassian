#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

list(APPEND PUBLIC_HEADERS)
list(APPEND PRIVATE_HEADERS "src/common.hpp" "src/test_config.hpp")
list(APPEND SOURCES "src/main.cpp" "src/common.cpp" "src/test_config.cpp"
     "src/to_address_space.cpp")

list(
  APPEND KERNELS "kernels/oclc_address_space_qualifier_functions/to_global.cl"
  "kernels/oclc_address_space_qualifier_functions/to_local.cl"
  "kernels/oclc_address_space_qualifier_functions/to_private.cl")

add_executable(oclc_address_space_qualifier_functions
               ${PUBLIC_HEADERS} ${PRIVATE_HEADERS} ${SOURCES})

target_include_directories(
  oclc_address_space_qualifier_functions
  PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

target_link_libraries(
  oclc_address_space_qualifier_functions
  PRIVATE Catch2::Catch2
          cassian::runtime
          cassian::catch2_utils
          cassian::cli
          cassian::utility
          cassian::logging
          cassian::random
          cassian::vector
          cassian::test_harness)

set_target_properties(oclc_address_space_qualifier_functions
                      PROPERTIES FOLDER test_suites/oclc)
cassian_install_target(oclc_address_space_qualifier_functions)

cassian_target_add_kernels(oclc_address_space_qualifier_functions ${KERNELS})
