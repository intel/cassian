/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_OFFLINE_COMPILER_OFFLINE_COMPILER_HPP
#define CASSIAN_OFFLINE_COMPILER_OFFLINE_COMPILER_HPP

#include <cstdint>
#include <string>
#include <vector>

/**
 * Cassian namespace.
 */
namespace cassian {
struct OclocProduct {
  std::string name;
  std::vector<uint8_t> data;
};
/**
 * Generate SPIR-V module from a file.
 *
 * @param[in] path path to input file.
 * @param[in] build_options build options to use during generation.
 * @returns path to output binary file with generated SPIR-V module.
 */
std::string generate_spirv(uint32_t device_id, int32_t device_revision,
                           const std::string &path,
                           const std::string &build_options = std::string(),
                           bool quiet = false);

/**
 * Generate SPIR-V module from source.
 *
 * @param[in] source source from which SPIR-V module will be generated.
 * @param[in] build_options build options to use during generation.
 * @returns SPIR-V module stored in raw bytes.
 */
std::vector<uint8_t> generate_spirv_from_source(
    uint32_t device_id, int32_t device_revision, const std::string &source,
    const std::string &build_options = std::string(), bool quiet = false);

/**
 * Generate ocloc products from different input types(source/spirv/binary data).
 *
 * @param[in] source_bytes input data for ocloc tool.
 * @param[in] build_options OpenCL build options used for compilation.
 * @param[in] ocloc_cmd ocloc command to use (compile/disasm).
 * @param[in] ocloc_options ocloc specific properties to add to command.
 * @param[in] program_type type of program for compilation.
 * @returns set of OclocProducts - files produced by ocloc.
 */
std::vector<OclocProduct> generate_offline_compiler_products(
    uint32_t device_id, int32_t device_revision,
    const std::vector<uint8_t> &source_bytes, const std::string &build_options,
    const std::string &ocloc_cmd,
    const std::vector<const char *> &ocloc_options,
    const std::string &program_type, bool quiet = false);
/**
 * Exception class used when an offline compiler encounters a fatal error.
 */
class OfflineCompilerException : public std::runtime_error {
  using std::runtime_error::runtime_error;
};

} // namespace cassian
#endif
