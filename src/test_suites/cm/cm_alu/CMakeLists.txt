#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

list(APPEND PUBLIC_HEADERS)
list(APPEND PRIVATE_HEADERS "src/common.hpp" "src/test_config.hpp")
list(APPEND SOURCES "src/main.cpp" "src/common.cpp" "src/test_config.cpp")
list(APPEND KERNELS kernels/cm_alu/binary_genx.cpp
     kernels/cm_alu/binary_intrinsic_genx.cpp kernels/cm_alu/unary_genx.cpp
     kernels/cm_alu/unary_intrinsic_genx.cpp)

add_library(cm_alu STATIC ${PUBLIC_HEADERS} ${PRIVATE_HEADERS} ${SOURCES})

target_include_directories(
  cm_alu PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)
target_link_libraries(
  cm_alu
  PUBLIC Catch2::Catch2
         cassian::runtime
         cassian::cli
         cassian::utility
         cassian::logging
         cassian::test_harness
         cassian::random)

cassian_target_add_kernels(cm_alu ${KERNELS})

macro(operator _name)
  add_executable(cm_alu_operator_${_name} "src/operators/${_name}.cpp")
  target_link_libraries(cm_alu_operator_${_name} PRIVATE cm_alu)
  set_target_properties(cm_alu_operator_${_name} PROPERTIES FOLDER
                                                            test_suites/cm)
  cassian_install_target(cm_alu_operator_${_name})
endmacro(operator)

macro(intrinsic _name)
  add_executable(cm_alu_intrinsic_${_name} "src/intrinsics/${_name}.cpp")
  target_link_libraries(cm_alu_intrinsic_${_name} PRIVATE cm_alu)
  set_target_properties(cm_alu_intrinsic_${_name} PROPERTIES FOLDER
                                                             test_suites/cm)
  cassian_install_target(cm_alu_intrinsic_${_name})
endmacro(intrinsic)

# Unary operators
operator(plus)
operator(minus)
operator(bitwise_not)
operator(logical_not)

# Binary arithmetic operators
operator(add)
operator(sub)
operator(mul)
operator(div)
operator(mod)

# Binary bitwise operators
operator(bitwise_and)
operator(bitwise_or)
operator(bitwise_xor)

# Bitwise shift operators
operator(shl)
operator(shr)

intrinsic(abs)
intrinsic(add)
intrinsic(mul)
intrinsic(shr)
