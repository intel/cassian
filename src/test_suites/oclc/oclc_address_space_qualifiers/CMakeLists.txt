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
  "src/local.cpp"
  "src/constant.cpp"
  "src/global.cpp"
  "src/test_config.cpp")

list(APPEND KERNELS "kernels/oclc_address_space_qualifiers/local.cl"
     "kernels/oclc_address_space_qualifiers/constant.cl"
     "kernels/oclc_address_space_qualifiers/global.cl")

add_executable(oclc_address_space_qualifiers ${PUBLIC_HEADERS}
                                             ${PRIVATE_HEADERS} ${SOURCES})

target_include_directories(
  oclc_address_space_qualifiers
  PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

target_link_libraries(
  oclc_address_space_qualifiers
  PRIVATE Catch2::Catch2 cassian::runtime cassian::cli cassian::logging
          cassian::utility cassian::test_harness)

set_target_properties(oclc_address_space_qualifiers PROPERTIES FOLDER
                                                               test_suites/oclc)

cassian_install_target(oclc_address_space_qualifiers)

cassian_target_add_kernels(oclc_address_space_qualifiers ${KERNELS})
