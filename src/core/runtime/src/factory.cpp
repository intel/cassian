/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/cli/cli.hpp>
#include <cassian/runtime/factory.hpp>
#include <cassian/runtime/runtime.hpp>
#include <memory>
#include <string>

#ifdef BUILD_OCL
#include <opencl_runtime.hpp>
#endif

#ifdef BUILD_L0
#include <level_zero_runtime.hpp>
#endif

namespace cassian {
std::unique_ptr<Runtime> create_runtime(const std::string &name) {
#ifdef BUILD_OCL
  if (name == "ocl") {
    return std::make_unique<OpenCLRuntime>();
  }
#endif

#ifdef BUILD_L0
  if (name == "l0") {
    return std::make_unique<LevelZeroRuntime>();
  }
#endif

  throw UnknownRuntimeException("Unknown runtime name: " + name);
}

void add_runtime_arguments(CommandLineParser *parser) {
#ifdef BUILD_OCL
  const std::string default_runtime = "ocl";
#elif !defined(BUILD_OCL) && defined(BUILD_L0)
  const std::string default_runtime = "l0";
#else
  const std::string default_runtime = "";
#endif

  parser->add_argument("--runtime", default_runtime);
  parser->add_argument("--program-type", "source");
}

} // namespace cassian
