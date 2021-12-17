#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

list(APPEND PUBLIC_HEADERS)
list(APPEND PRIVATE_HEADERS)
list(APPEND SOURCES "src/half.cpp")
list(APPEND KERNELS
     "kernels/cm_stochastic_rounding/stochastic_rounding_genx.cpp")

add_executable(cm_stochastic_rounding ${PUBLIC_HEADERS} ${PRIVATE_HEADERS}
                                      ${SOURCES})

target_include_directories(
  cm_stochastic_rounding
  PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

target_link_libraries(
  cm_stochastic_rounding PRIVATE Catch2::Catch2 cassian::main cassian::random
                                 cassian::fp_types)

set_target_properties(cm_stochastic_rounding PROPERTIES FOLDER test_suites/cm)
cassian_install_target(cm_stochastic_rounding)

cassian_target_add_kernels(cm_stochastic_rounding ${KERNELS})
