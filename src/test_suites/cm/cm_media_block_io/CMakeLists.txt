#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

list(APPEND PUBLIC_HEADERS)
list(APPEND PRIVATE_HEADERS)
list(APPEND SOURCES "src/read.cpp" "src/write.cpp")
list(APPEND KERNELS "kernels/cm_media_block_io/media_block_read_genx.cpp"
     "kernels/cm_media_block_io/media_block_write_genx.cpp")

add_executable(cm_media_block_io ${PUBLIC_HEADERS} ${PRIVATE_HEADERS}
                                 ${SOURCES})

target_include_directories(
  cm_media_block_io PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

target_link_libraries(cm_media_block_io PRIVATE Catch2::Catch2 cassian::main
                                                cassian::test_harness)

set_target_properties(cm_media_block_io PROPERTIES FOLDER test_suites/cm)
cassian_install_target(cm_media_block_io)

cassian_target_add_kernels(cm_media_block_io ${KERNELS})
