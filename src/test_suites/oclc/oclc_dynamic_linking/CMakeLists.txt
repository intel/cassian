#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

list(APPEND PUBLIC_HEADERS)
list(APPEND PRIVATE_HEADERS)
list(APPEND SOURCES "src/test.cpp")
list(
  APPEND
  KERNELS
  "kernels/oclc_dynamic_linking/circular_dependencies_callee.cl"
  "kernels/oclc_dynamic_linking/circular_dependencies_kernel.cl"
  "kernels/oclc_dynamic_linking/extern_function_callee.cl"
  "kernels/oclc_dynamic_linking/extern_function_kernel.cl"
  "kernels/oclc_dynamic_linking/single.cl")

add_executable(oclc_dynamic_linking ${PUBLIC_HEADERS} ${PRIVATE_HEADERS}
                                    ${SOURCES})

target_include_directories(
  oclc_dynamic_linking
  PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

target_link_libraries(
  oclc_dynamic_linking PRIVATE Catch2::Catch2 cassian::main cassian::random
                               cassian::reference)

set_target_properties(oclc_dynamic_linking PROPERTIES FOLDER test_suites/oclc)
cassian_install_target(oclc_dynamic_linking)

cassian_target_add_kernels(oclc_dynamic_linking ${KERNELS})
