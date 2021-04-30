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
  "src/implicit_conversion.cpp"
  "src/explicit_cast.cpp"
  "src/explicit_conversion.cpp")
list(APPEND KERNELS "kernels/oclc_conversions/implicit_conversion.cl"
     "kernels/oclc_conversions/explicit_cast.cl"
     "kernels/oclc_conversions/explicit_conversion.cl")

add_executable(oclc_conversions ${PUBLIC_HEADERS} ${PRIVATE_HEADERS} ${SOURCES})

target_include_directories(
  oclc_conversions PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

target_link_libraries(
  oclc_conversions
  PRIVATE Catch2::Catch2
          cassian::runtime
          cassian::cli
          cassian::utility
          cassian::logging
          cassian::vector
          cassian::test_harness
          cassian::catch2_utils)

set_target_properties(oclc_conversions PROPERTIES FOLDER test_suites/oclc)
cassian_install_target(oclc_conversions)

cassian_target_add_kernels(oclc_conversions ${KERNELS})
