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
  "src/explicit_conversion_vec1.cpp"
  "src/explicit_conversion_vec2.cpp"
  "src/explicit_conversion_vec3.cpp"
  "src/explicit_conversion_vec4.cpp"
  "src/explicit_conversion_vec8.cpp"
  "src/explicit_conversion_vec16.cpp"
  "src/utility.hpp"
  "src/union_reinterpretation_vec1.cpp"
  "src/union_reinterpretation_vec2.cpp"
  "src/union_reinterpretation_vec3.cpp"
  "src/union_reinterpretation_vec4.cpp"
  "src/union_reinterpretation_vec8.cpp"
  "src/union_reinterpretation_vec16.cpp"
  "src/as_type_reinterpretation_1b.cpp"
  "src/as_type_reinterpretation_2b.cpp"
  "src/as_type_reinterpretation_4b.cpp"
  "src/as_type_reinterpretation_8b.cpp"
  "src/pointer_cast_vec1.cpp"
  "src/pointer_cast_vec2.cpp"
  "src/pointer_cast_vec3.cpp"
  "src/pointer_cast_vec4.cpp"
  "src/pointer_cast_vec8.cpp"
  "src/pointer_cast_vec16.cpp")
list(
  APPEND
  KERNELS
  "kernels/oclc_conversions/implicit_conversion.cl"
  "kernels/oclc_conversions/explicit_cast.cl"
  "kernels/oclc_conversions/explicit_conversion.cl"
  "kernels/oclc_conversions/union_reinterpretation.cl"
  "kernels/oclc_conversions/as_type_reinterpretation.cl"
  "kernels/oclc_conversions/pointer_cast.cl")

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
