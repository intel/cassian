#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

list(APPEND PUBLIC_HEADERS)
list(APPEND PRIVATE_HEADERS "src/common.hpp" "src/test_config.hpp")

list(APPEND SOURCES "src/main.cpp" "src/common.cpp" "src/program_scope.cpp"
     "src/test_config.cpp")

list(APPEND KERNELS "kernels/oclc_storage_class_specifiers/program_scope.cl")

add_executable(oclc_storage_class_specifiers ${PUBLIC_HEADERS}
                                             ${PRIVATE_HEADERS} ${SOURCES})

target_include_directories(
  oclc_storage_class_specifiers
  PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

target_link_libraries(
  oclc_storage_class_specifiers
  PRIVATE Catch2::Catch2
          cassian::runtime
          cassian::cli
          cassian::utility
          cassian::logging
          cassian::vector
          cassian::test_harness)

set_target_properties(oclc_storage_class_specifiers PROPERTIES FOLDER
                                                               test_suites/oclc)

cassian_install_target(oclc_storage_class_specifiers)

cassian_target_add_kernels(oclc_storage_class_specifiers ${KERNELS})
