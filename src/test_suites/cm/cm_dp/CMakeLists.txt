#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

list(APPEND PUBLIC_HEADERS)
list(APPEND PRIVATE_HEADERS "src/common.hpp")
list(APPEND SOURCES "src/common.cpp" "src/dp2.cpp" "src/dp3.cpp" "src/dp4.cpp")
list(APPEND KERNELS "kernels/cm_dp/dp_genx.cpp")

add_executable(cm_dp ${PUBLIC_HEADERS} ${PRIVATE_HEADERS} ${SOURCES})

target_include_directories(
  cm_dp PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

target_link_libraries(cm_dp PRIVATE Catch2::Catch2 cassian::main
                                    cassian::random cassian::reference)

set_target_properties(cm_dp PROPERTIES FOLDER test_suites/cm)
cassian_install_target(cm_dp)

cassian_target_add_kernels(cm_dp ${KERNELS})
