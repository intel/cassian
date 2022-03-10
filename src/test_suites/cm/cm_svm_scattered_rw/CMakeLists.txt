#
# Copyright (C) 2022 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

list(APPEND PUBLIC_HEADERS)
list(APPEND SOURCES src/test.cpp)
list(APPEND KERNELS "kernels/cm_svm_scattered_rw/read_svm_scattered_genx.cpp"
     "kernels/cm_svm_scattered_rw/write_svm_scattered_genx.cpp")

add_executable(cm_svm_scattered_rw ${PUBLIC_HEADERS} ${PRIVATE_HEADERS}
                                   ${SOURCES})

target_include_directories(
  cm_svm_scattered_rw
  PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

target_link_libraries(
  cm_svm_scattered_rw PRIVATE Catch2::Catch2 cassian::main cassian::random
                              cassian::reference cassian::test_harness)

set_target_properties(cm_svm_scattered_rw PROPERTIES FOLDER test_suites/cm)
cassian_install_target(cm_svm_scattered_rw)

cassian_target_add_kernels(cm_svm_scattered_rw ${KERNELS})
