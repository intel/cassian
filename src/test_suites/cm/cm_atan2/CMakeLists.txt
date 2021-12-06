#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

list(APPEND PUBLIC_HEADERS)
list(APPEND PRIVATE_HEADERS "src/common.hpp")
list(APPEND SOURCES "src/atan2.cpp" "src/atan2_fast.cpp")
list(APPEND KERNELS "kernels/cm_atan2/atan2_genx.cpp")

add_executable(cm_atan2 ${PUBLIC_HEADERS} ${PRIVATE_HEADERS} ${SOURCES})

target_include_directories(
  cm_atan2 PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

target_link_libraries(cm_atan2 PRIVATE Catch2::Catch2 cassian::main
                                       cassian::random cassian::reference)

set_target_properties(cm_atan2 PROPERTIES FOLDER test_suites/cm)
cassian_install_target(cm_atan2)

cassian_target_add_kernels(cm_atan2 ${KERNELS})
