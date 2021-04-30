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
  "src/aligned.cpp"
  "src/packed.cpp"
  "src/unroll.cpp")
list(APPEND KERNELS "kernels/oclc_attribute_qualifiers/aligned.cl"
     "kernels/oclc_attribute_qualifiers/packed.cl"
     "kernels/oclc_attribute_qualifiers/unroll.cl")

add_executable(oclc_attribute_qualifiers ${PUBLIC_HEADERS} ${PRIVATE_HEADERS}
                                         ${SOURCES})

target_include_directories(
  oclc_attribute_qualifiers
  PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

target_link_libraries(
  oclc_attribute_qualifiers
  PRIVATE Catch2::Catch2
          cassian::runtime
          cassian::cli
          cassian::utility
          cassian::logging
          cassian::vector
          cassian::test_harness
          cassian::catch2_utils)

set_target_properties(oclc_attribute_qualifiers PROPERTIES FOLDER
                                                           test_suites/oclc)
cassian_install_target(oclc_attribute_qualifiers)

cassian_target_add_kernels(oclc_attribute_qualifiers ${KERNELS})
