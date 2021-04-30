/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_RUNTIME_PROGRAM_DESCRIPTOR_HPP
#define CASSIAN_RUNTIME_PROGRAM_DESCRIPTOR_HPP

#include <string>
#include <vector>
/**
 * Cassian namespace.
 */
namespace cassian {

/**
 * Program descriptor.
 */
struct ProgramDescriptor {
  /**
   * Program source code.
   */
  std::string source;

  /**
   * Program's compile options.
   */
  std::string compiler_options;

  /**
   * Program type.
   */
  std::string program_type;

  /**
   * Default constructor.
   */
  ProgramDescriptor() = default;

  /**
   * Construct a ProgramDescriptor object.
   * @param[in] source program source.
   * @param[in] compiler_options program compilation options.
   * @param[in] program_type program type to use during kernel construction.
   */
  ProgramDescriptor(const std::string &source,
                    const std::string &compiler_options,
                    const std::string &program_type)
      : source(source), compiler_options(compiler_options),
        program_type(program_type) {}
};
} // namespace cassian
#endif