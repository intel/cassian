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
  "src/program_sampler.cpp"
  "src/arg_sampler.cpp"
  "src/sampler_less.cpp"
  "src/test_config.cpp")
list(
  APPEND KERNELS "kernels/oclc_image_read_write_functions/program_sampler.cl"
  "kernels/oclc_image_read_write_functions/arg_sampler.cl"
  "kernels/oclc_image_read_write_functions/sampler_less.cl")

add_executable(oclc_image_read_write_functions ${PUBLIC_HEADERS}
                                               ${PRIVATE_HEADERS} ${SOURCES})

target_include_directories(
  oclc_image_read_write_functions
  PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

target_link_libraries(
  oclc_image_read_write_functions
  PRIVATE Catch2::Catch2
          cassian::runtime
          cassian::cli
          cassian::utility
          cassian::logging
          cassian::vector
          cassian::test_harness)

set_target_properties(oclc_image_read_write_functions
                      PROPERTIES FOLDER test_suites/oclc)
cassian_install_target(oclc_image_read_write_functions)

cassian_target_add_kernels(oclc_image_read_write_functions ${KERNELS})
