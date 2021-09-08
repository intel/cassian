#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

list(APPEND PUBLIC_HEADERS)
list(APPEND PRIVATE_HEADERS)
list(APPEND SOURCES "src/test.cpp")
list(APPEND KERNELS "kernels/cm_dp4a/dp4a_genx.cpp")

add_executable(cm_dp4a ${PUBLIC_HEADERS} ${PRIVATE_HEADERS} ${SOURCES})

target_include_directories(
  cm_dp4a PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

target_link_libraries(cm_dp4a PRIVATE Catch2::Catch2 cassian::main
                                      cassian::random cassian::reference)

set_target_properties(cm_dp4a PROPERTIES FOLDER test_suites/cm)
cassian_install_target(cm_dp4a)

cassian_target_add_kernels(cm_dp4a ${KERNELS})
