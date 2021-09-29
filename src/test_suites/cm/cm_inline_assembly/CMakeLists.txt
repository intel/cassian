#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

list(APPEND PUBLIC_HEADERS)
list(APPEND PRIVATE_HEADERS)
list(APPEND SOURCES "src/test.cpp")
list(
  APPEND
  KERNELS
  kernels/cm_inline_assembly/asm_a_genx.cpp
  kernels/cm_inline_assembly/asm_addr1_genx.cpp
  kernels/cm_inline_assembly/asm_addr2_genx.cpp
  kernels/cm_inline_assembly/asm_cr_docs_genx.cpp
  kernels/cm_inline_assembly/asm_cr_genx.cpp
  kernels/cm_inline_assembly/asm_cr_simd_genx.cpp
  kernels/cm_inline_assembly/asm_cr_src_genx.cpp
  kernels/cm_inline_assembly/asm_f_genx.cpp
  kernels/cm_inline_assembly/asm_format1_genx.cpp
  kernels/cm_inline_assembly/asm_global_genx.cpp
  kernels/cm_inline_assembly/asm_imm_docs_genx.cpp
  kernels/cm_inline_assembly/asm_immediate_i_genx.cpp
  kernels/cm_inline_assembly/asm_immediate_n_genx.cpp
  kernels/cm_inline_assembly/asm_matrix_genx.cpp
  kernels/cm_inline_assembly/asm_plus_mod_1_genx.cpp
  kernels/cm_inline_assembly/asm_plus_mod_genx.cpp
  kernels/cm_inline_assembly/asm_r_3_genx.cpp
  kernels/cm_inline_assembly/asm_r_4_genx.cpp
  kernels/cm_inline_assembly/asm_r_dst_genx.cpp
  kernels/cm_inline_assembly/asm_r_multiple_output_genx.cpp
  kernels/cm_inline_assembly/asm_rw_const_vector_genx.cpp
  kernels/cm_inline_assembly/asm_rw_genx.cpp
  kernels/cm_inline_assembly/asm_select_genx.cpp
  kernels/cm_inline_assembly/asm_struct_of_vec_phi_genx.cpp
  kernels/cm_inline_assembly/asm_struct_phi_genx.cpp
  kernels/cm_inline_assembly/asm_template_empty_genx.cpp
  kernels/cm_inline_assembly/asm_template_no_ops_genx.cpp
  kernels/cm_inline_assembly/asm_template_symbolic_names_genx.cpp
  kernels/cm_inline_assembly/asm_template_symbolic_names_mix_genx.cpp
  kernels/cm_inline_assembly/asm_two_kernels_genx.cpp
  kernels/cm_inline_assembly/asm_unroll_genx.cpp)

add_executable(cm_inline_assembly ${PUBLIC_HEADERS} ${PRIVATE_HEADERS}
                                  ${SOURCES})

target_include_directories(
  cm_inline_assembly PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

target_link_libraries(
  cm_inline_assembly PRIVATE Catch2::Catch2 cassian::main cassian::random
                             cassian::reference cassian::test_harness)

set_target_properties(cm_inline_assembly PROPERTIES FOLDER test_suites/cm)
cassian_install_target(cm_inline_assembly)

cassian_target_add_kernels(cm_inline_assembly ${KERNELS})
