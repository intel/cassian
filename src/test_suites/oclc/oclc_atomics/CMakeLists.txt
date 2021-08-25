#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

list(APPEND PUBLIC_HEADERS)
list(APPEND PRIVATE_HEADERS "src/common.hpp" "src/test_config.hpp")
list(
  APPEND
  SOURCES
  "src/main.cpp"
  "src/common.cpp"
  "src/test_config.cpp"
  "src/atomic_init.cpp"
  "src/atomic_load.cpp"
  "src/atomic_store.cpp"
  "src/atomic_fetch.cpp"
  "src/atomic_exchange.cpp"
  "src/atomic_var_init.cpp"
  "src/atomic_work_item_fence.cpp"
  "src/atomic_compare_exchange.cpp"
  "src/atomic_flag_init.cpp"
  "src/atomic_flag_test_and_set.cpp"
  "src/atomic_flag_clear.cpp")

list(
  APPEND
  KERNELS
  "kernels/oclc_atomics/atomic_init_global.cl"
  "kernels/oclc_atomics/atomic_init_local.cl"
  "kernels/oclc_atomics/atomic_store_global.cl"
  "kernels/oclc_atomics/atomic_store_local.cl"
  "kernels/oclc_atomics/atomic_load_global.cl"
  "kernels/oclc_atomics/atomic_load_local.cl"
  "kernels/oclc_atomics/atomic_fetch_global.cl"
  "kernels/oclc_atomics/atomic_fetch_local.cl"
  "kernels/oclc_atomics/atomic_exchange_global.cl"
  "kernels/oclc_atomics/atomic_exchange_local.cl"
  "kernels/oclc_atomics/atomic_var_init.cl"
  "kernels/oclc_atomics/atomic_work_item_fence.cl"
  "kernels/oclc_atomics/atomic_compare_exchange_global.cl"
  "kernels/oclc_atomics/atomic_compare_exchange_local.cl"
  "kernels/oclc_atomics/atomic_flag_init.cl"
  "kernels/oclc_atomics/atomic_flag_test_and_set_global.cl"
  "kernels/oclc_atomics/atomic_flag_test_and_set_local.cl"
  "kernels/oclc_atomics/atomic_flag_clear_global.cl"
  "kernels/oclc_atomics/atomic_flag_clear_local.cl")

add_executable(oclc_atomics ${PUBLIC_HEADERS} ${PRIVATE_HEADERS} ${SOURCES})

target_include_directories(
  oclc_atomics PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

target_link_libraries(
  oclc_atomics
  PRIVATE Catch2::Catch2
          cassian::runtime
          cassian::cli
          cassian::utility
          cassian::logging
          cassian::random
          cassian::vector
          cassian::test_harness)

set_target_properties(oclc_atomics PROPERTIES FOLDER test_suites/oclc)
cassian_install_target(oclc_atomics)

cassian_target_add_kernels(oclc_atomics ${KERNELS})
