#
# Copyright (C) 2022 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

list(APPEND PUBLIC_HEADERS)
list(APPEND PRIVATE_HEADERS src/common.hpp)
list(APPEND SOURCES src/read_typed.cpp src/write_typed.cpp src/common.cpp)
list(
  APPEND
  KERNELS
  "kernels/cm_typed_surface_rw/write_typed_1d_genx.cpp"
  "kernels/cm_typed_surface_rw/write_typed_2d_genx.cpp"
  "kernels/cm_typed_surface_rw/write_typed_3d_genx.cpp"
  "kernels/cm_typed_surface_rw/read_typed_1d_genx.cpp"
  "kernels/cm_typed_surface_rw/read_typed_2d_genx.cpp"
  "kernels/cm_typed_surface_rw/read_typed_3d_genx.cpp")

add_executable(cm_typed_surface_rw ${PUBLIC_HEADERS} ${PRIVATE_HEADERS}
                                   ${SOURCES})

target_include_directories(
  cm_typed_surface_rw
  PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

target_link_libraries(
  cm_typed_surface_rw PRIVATE Catch2::Catch2 cassian::main cassian::random
                              cassian::reference)

set_target_properties(cm_typed_surface_rw PROPERTIES FOLDER test_suites/cm)
cassian_install_target(cm_typed_surface_rw)

cassian_target_add_kernels(cm_typed_surface_rw ${KERNELS})
