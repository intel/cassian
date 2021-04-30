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
  "src/vload.cpp"
  "src/vstore.cpp")

list(
  APPEND
  KERNELS
  "kernels/oclc_vector_load_store_functions/vload_global.cl"
  "kernels/oclc_vector_load_store_functions/vload_local.cl"
  "kernels/oclc_vector_load_store_functions/vload_private.cl"
  "kernels/oclc_vector_load_store_functions/vload_constant.cl"
  "kernels/oclc_vector_load_store_functions/vload_generic.cl"
  "kernels/oclc_vector_load_store_functions/vstore_global.cl"
  "kernels/oclc_vector_load_store_functions/vstore_local.cl"
  "kernels/oclc_vector_load_store_functions/vstore_private.cl"
  "kernels/oclc_vector_load_store_functions/vstore_generic.cl")

add_executable(oclc_vector_load_store_functions ${PUBLIC_HEADERS}
                                                ${PRIVATE_HEADERS} ${SOURCES})

target_include_directories(
  oclc_vector_load_store_functions
  PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

target_link_libraries(
  oclc_vector_load_store_functions
  PRIVATE Catch2::Catch2
          cassian::runtime
          cassian::cli
          cassian::utility
          cassian::logging
          cassian::random
          cassian::vector
          cassian::catch2_utils
          cassian::test_harness)

set_target_properties(oclc_vector_load_store_functions
                      PROPERTIES FOLDER test_suites/oclc)
cassian_install_target(oclc_vector_load_store_functions)

cassian_target_add_kernels(oclc_vector_load_store_functions ${KERNELS})
