#
# Copyright (C) 2026 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

list(APPEND PUBLIC_HEADERS)
list(APPEND PRIVATE_HEADERS "src/common.hpp" "src/test_config.hpp")
list(APPEND SOURCES "src/main.cpp" "src/test_config.cpp" "src/test.cpp"
     "src/common.cpp")
list(
  APPEND
  KERNELS
  "kernels/oclc_extended_bit_operations/test_basic_bitfield_insert.cl"
  "kernels/oclc_extended_bit_operations/test_basic_bit_reverse.cl"
  "kernels/oclc_extended_bit_operations/test_basic_bitfield_extract_unsigned.cl"
  "kernels/oclc_extended_bit_operations/test_basic_bitfield_extract_signed.cl")

add_executable(oclc_extended_bit_operations ${PUBLIC_HEADERS}
                                            ${PRIVATE_HEADERS} ${SOURCES})

target_include_directories(
  oclc_extended_bit_operations
  PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

target_link_libraries(
  oclc_extended_bit_operations
  PRIVATE Catch2::Catch2
          cassian::runtime
          cassian::cli
          cassian::logging
          cassian::vector
          cassian::random
          cassian::reference
          cassian::test_harness
          cassian::utility
          cassian::catch2_utils)

set_target_properties(oclc_extended_bit_operations PROPERTIES FOLDER
                                                              test_suites/oclc)
cassian_install_target(oclc_extended_bit_operations)

cassian_target_add_kernels(oclc_extended_bit_operations ${KERNELS})
