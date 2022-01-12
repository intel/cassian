#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

list(APPEND PUBLIC_HEADERS)
list(APPEND PRIVATE_HEADERS)
list(APPEND SOURCES "src/test.cpp")
list(APPEND KERNELS "kernels/cm_bfn/bfn_genx.cpp")

add_executable(cm_bfn ${PUBLIC_HEADERS} ${PRIVATE_HEADERS} ${SOURCES})

target_include_directories(
  cm_bfn PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

target_link_libraries(cm_bfn PRIVATE Catch2::Catch2 cassian::main
                                     cassian::random cassian::reference)

set_target_properties(cm_bfn PROPERTIES FOLDER test_suites/cm)
cassian_install_target(cm_bfn)

cassian_target_add_kernels(cm_bfn ${KERNELS})
