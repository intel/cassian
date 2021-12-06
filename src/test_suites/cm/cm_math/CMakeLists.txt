#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

list(APPEND PUBLIC_HEADERS)
list(APPEND PRIVATE_HEADERS "src/common.hpp")
list(
  APPEND
  SOURCES
  "src/functions/inv.cpp"
  "src/functions/log.cpp"
  "src/functions/exp.cpp"
  "src/functions/sqrt.cpp"
  "src/functions/rsqrt.cpp"
  "src/functions/sin.cpp"
  "src/functions/cos.cpp")
list(APPEND KERNELS "kernels/cm_math/math_genx.cpp")

add_executable(cm_math ${PUBLIC_HEADERS} ${PRIVATE_HEADERS} ${SOURCES})

target_include_directories(
  cm_math PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

target_link_libraries(cm_math PRIVATE Catch2::Catch2 cassian::main
                                      cassian::random cassian::reference)

set_target_properties(cm_math PROPERTIES FOLDER test_suites/cm)
cassian_install_target(cm_math)

cassian_target_add_kernels(cm_math ${KERNELS})
