#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

list(APPEND PUBLIC_HEADERS)
list(APPEND PRIVATE_HEADERS)
list(APPEND SOURCES "src/bfloat.cpp")
list(APPEND KERNELS "kernels/cm_convert/convert_genx.cpp")

add_executable(cm_convert ${PUBLIC_HEADERS} ${PRIVATE_HEADERS}
                                 ${SOURCES})

target_include_directories(
  cm_convert PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

target_link_libraries(
  cm_convert PRIVATE Catch2::Catch2 cassian::main cassian::random
                            cassian::fp_types)

set_target_properties(cm_convert PROPERTIES FOLDER test_suites/cm)
cassian_install_target(cm_convert)

cassian_target_add_kernels(cm_convert ${KERNELS})
