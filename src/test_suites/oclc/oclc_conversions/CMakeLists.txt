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
  "src/explicit_conversion.cpp"
  "src/utility.hpp"
  "src/union_reinterpretation.cpp"
  "src/as_type_reinterpretation.cpp")
list(
  APPEND
  KERNELS
  "kernels/oclc_conversions/implicit_conversion.cl"
  "kernels/oclc_conversions/explicit_cast.cl"
  "kernels/oclc_conversions/explicit_conversion.cl"
  "kernels/oclc_conversions/union_reinterpretation.cl"
  "kernels/oclc_conversions/as_type_reinterpretation.cl")

add_executable(oclc_conversions ${PUBLIC_HEADERS} ${PRIVATE_HEADERS} ${SOURCES})

target_include_directories(
  oclc_conversions PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

target_link_libraries(
  oclc_conversions
  PRIVATE Catch2::Catch2
          cassian::random
          cassian::runtime
          cassian::cli
          cassian::utility
          cassian::logging
          cassian::vector
          cassian::test_harness
          cassian::catch2_utils)

set_target_properties(oclc_conversions PROPERTIES FOLDER test_suites/oclc)

if(MSVC)
  set_source_files_properties(${SOURCES} COMPILE_FLAGS /fp:strict)
else()
  set_source_files_properties(${SOURCES} COMPILE_FLAGS -frounding-math)
endif()

cassian_install_target(oclc_conversions)

cassian_target_add_kernels(oclc_conversions ${KERNELS})
