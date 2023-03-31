#
# Copyright (C) 2021-2023 Intel Corporation
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
  "src/arithmetic_operators.cpp"
  "src/unary_operators.cpp"
  "src/pre_post_operators.cpp"
  "src/relational_operators.cpp"
  "src/equality_operators.cpp"
  "src/bitwise_operators.cpp"
  "src/logical_operators.cpp"
  "src/unary_logical_operator.cpp"
  "src/ternary_selection_operator.cpp"
  "src/shift_operators.cpp"
  "src/sizeof_operator.cpp"
  "src/comma_operator.cpp"
  "src/indirection_operator.cpp"
  "src/address_operator.cpp"
  "src/assignment_operator.cpp")

list(
  APPEND
  KERNELS
  "kernels/oclc_operators/arithmetic_operators.cl"
  "kernels/oclc_operators/unary_operators.cl"
  "kernels/oclc_operators/pre_post_operators.cl"
  "kernels/oclc_operators/relational_operators.cl"
  "kernels/oclc_operators/equality_operators.cl"
  "kernels/oclc_operators/bitwise_operators.cl"
  "kernels/oclc_operators/logical_operators.cl"
  "kernels/oclc_operators/unary_logical_operator.cl"
  "kernels/oclc_operators/ternary_selection_operator.cl"
  "kernels/oclc_operators/shift_operators.cl"
  "kernels/oclc_operators/sizeof_operator.cl"
  "kernels/oclc_operators/comma_operator.cl"
  "kernels/oclc_operators/indirection_operator.cl"
  "kernels/oclc_operators/address_operator.cl"
  "kernels/oclc_operators/assignment_operator.cl")

add_executable(oclc_operators ${PUBLIC_HEADERS} ${PRIVATE_HEADERS} ${SOURCES})

target_include_directories(
  oclc_operators PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

target_link_libraries(
  oclc_operators
  PRIVATE Catch2::Catch2
          cassian::runtime
          cassian::catch2_utils
          cassian::cli
          cassian::utility
          cassian::logging
          cassian::fp_types
          cassian::random
          cassian::vector
          cassian::test_harness)

set_target_properties(oclc_operators PROPERTIES FOLDER test_suites/oclc)
cassian_install_target(oclc_operators)

cassian_target_add_kernels(oclc_operators ${KERNELS})
