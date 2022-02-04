/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/logging/logging.hpp>
#include <cassian/offline_compiler/offline_compiler.hpp>
#include <cassian/runtime/cm_utils.hpp>
#include <cassian/runtime/openclc_utils.hpp>

namespace cassian {
namespace {
bool check_kernel_compilation(Runtime *runtime, const std::string &kernel_name,
                              const std::string &source,
                              const std::string &compiler_flags,
                              const std::string &program_type) {
  try {
    bool quiet = true;
    auto k = runtime->create_kernel(kernel_name, source, compiler_flags,
                                    program_type, std::nullopt, quiet);
    runtime->release_kernel(k);
  } catch (const RuntimeException &e) {
    return false;
  } catch (const OfflineCompilerException &e) {
    return false;
  }
  return true;
}
} // namespace

bool check_optional_openclc_feature_support(Runtime *runtime,
                                            const std::string &program_type,
                                            const std::string &feature_macro) {
  std::string source = "#ifndef " + feature_macro +
                       "\n"
                       "#error \"Feature unsupported\"\n"
                       "#endif \n"
                       "void kernel test() {} ";

  bool is_supported = check_kernel_compilation(runtime, "test", source,
                                               " -cl-std=CL3.0", program_type);

  if (!is_supported) {
    logging::info() << feature_macro << " unsupported\n";
  }

  return is_supported;
}

bool check_optional_cm_feature_support(Runtime *runtime,
                                       const std::string &program_type,
                                       const std::string &feature_macro) {
  std::string source = "#include <cm/cm.h>\n"
                       "#ifndef " +
                       feature_macro +
                       "\n"
                       "#error \"Feature unsupported\"\n"
                       "#endif \n"
                       "extern \"C\" _GENX_MAIN_ void test() {} ";

  bool is_supported =
      check_kernel_compilation(runtime, "test", source, "-cmc", program_type);

  if (!is_supported) {
    logging::info() << feature_macro << " unsupported\n";
  }

  return is_supported;
}

} // namespace cassian
