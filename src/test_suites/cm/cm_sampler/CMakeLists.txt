#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

list(APPEND PUBLIC_HEADERS)
list(APPEND PRIVATE_HEADERS src/common.hpp)
list(APPEND SOURCES src/common.cpp src/load16.cpp src/sample16.cpp
     src/sample32.cpp)
list(
  APPEND
  KERNELS
  "kernels/cm_sampler/load16_1d_genx.cpp"
  "kernels/cm_sampler/load16_2d_genx.cpp"
  "kernels/cm_sampler/load16_3d_genx.cpp"
  "kernels/cm_sampler/sample16_1d_genx.cpp"
  "kernels/cm_sampler/sample16_2d_genx.cpp"
  "kernels/cm_sampler/sample16_3d_genx.cpp"
  "kernels/cm_sampler/sample32_2d_genx.cpp")

add_executable(cm_sampler ${PUBLIC_HEADERS} ${PRIVATE_HEADERS} ${SOURCES})

target_include_directories(
  cm_sampler PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

target_link_libraries(cm_sampler PRIVATE Catch2::Catch2 cassian::main
                                         cassian::image)

set_target_properties(cm_sampler PROPERTIES FOLDER test_suites/cm)
cassian_install_target(cm_sampler)

cassian_target_add_kernels(cm_sampler ${KERNELS})
