/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include <cassian/logging/logging.hpp>
#include <cassian/offline_compiler/offline_compiler.hpp>
#include <cassian/system/factory.hpp>
#include <cassian/system/library.hpp>
#include <cassian/utility/utility.hpp>

namespace cassian {
#if defined(_WIN32)
#define SIGNATURE __cdecl
constexpr char ocloc[] = "ocloc64.dll";
#else
#define SIGNATURE
constexpr char ocloc[] = "libocloc.so";
#endif

class Ocloc {
private:
  // The argument names do not match cassian style guide.
  // This is copy-paste from ocloc_api.h which is a part of ocloc package.
  using oclocInvoke_t = int
  SIGNATURE(unsigned int numArgs, const char *argv[],                 // NOLINT
            const uint32_t numSources,                                // NOLINT
            const uint8_t **dataSources, const uint64_t *lenSources,  // NOLINT
            const char **nameSources, const uint32_t numInputHeaders, // NOLINT
            const uint8_t **dataInputHeaders,                         // NOLINT
            const uint64_t *lenInputHeaders,                          // NOLINT
            const char **nameInputHeaders, uint32_t *numOutputs,      // NOLINT
            uint8_t ***dataOutputs, uint64_t **lenOutputs,            // NOLINT
            char ***nameOutputs);                                     // NOLINT

  using oclocFreeOutput_t = int SIGNATURE(uint32_t *numOutputs,   // NOLINT
                                          uint8_t ***dataOutputs, // NOLINT
                                          uint64_t **lenOutputs,  // NOLINT
                                          char ***nameOutputs);   // NOLINT

  using p_oclocInvoke_t = oclocInvoke_t *;
  using p_oclocFreeOutput_t = oclocFreeOutput_t *;

  std::unique_ptr<Library> library_;

public:
  const p_oclocInvoke_t invoke;   // NOLINT
  const p_oclocFreeOutput_t free; // NOLINT

  Ocloc()
      : library_(load_library(ocloc)),
        invoke(reinterpret_cast<p_oclocInvoke_t>(
            library_->get_function("oclocInvoke"))),
        free(reinterpret_cast<p_oclocFreeOutput_t>(
            library_->get_function("oclocFreeOutput"))) {}
};

std::string generate_spirv(uint32_t device_id, int32_t device_revision,
                           const std::string &path,
                           const std::string &build_options, bool quiet) {
  static const std::string spv_file = "kernel.spv";

  auto source = load_text_file(path);
  auto spirv = generate_spirv_from_source(device_id, device_revision, source,
                                          build_options, quiet);
  save_binary_file(spirv, spv_file);

  return spv_file;
}

std::vector<uint8_t>
generate_spirv_from_source(uint32_t device_id, int32_t device_revision,
                           const std::string &source,
                           const std::string &build_options, bool quiet) {
  static Ocloc ocloc;

  static const std::string src_file = "source.cl";
  static const std::string spv_file = "kernel";
  static const std::string log_file = "stdout.log";

  std::string device;
  std::string revision;

  std::vector<const char *> args = {
      "ocloc",     "compile",
      "-file",     src_file.c_str(),
      "-options",  build_options.c_str(),
      "-output",   spv_file.c_str(),
      "-spv_only", "-output_no_suffix",
  };

  if (device_id != 0) {
    std::ostringstream out;
    out << "0x" << std::hex << std::setw(4) << std::setfill('0')
        << std::noshowbase << device_id;
    device = out.str();

    args.push_back("-device");
    args.push_back(device.c_str());
  }

  if (device_revision >= 0) {
    revision = std::to_string(device_revision);

    args.push_back("-revision_id");
    args.push_back(revision.c_str());
  }

  if (quiet) {
    args.push_back("-q");
  }

  const uint32_t num_sources = 1;
  const uint8_t *data_sources[] = {
      reinterpret_cast<const uint8_t *>(source.c_str())};
  const uint64_t len_sources[] = {source.length() + 1};
  const char *name_sources[] = {src_file.c_str()};

  const uint32_t num_includes = 0;
  const uint8_t **data_includes = nullptr;
  const uint64_t *len_includes = nullptr;
  const char **name_includes = nullptr;

  uint32_t num_outputs = 0;
  uint8_t **data_outputs = nullptr;
  uint64_t *len_outputs = nullptr;
  char **name_outputs = nullptr;

  auto _ = finally([&]() mutable {
    ocloc.free(&num_outputs, &data_outputs, &len_outputs, &name_outputs);
  });

  int status = ocloc.invoke(
      args.size(), args.data(),                                  // ocloc args
      num_sources, data_sources, len_sources, name_sources,      // source code
      num_includes, data_includes, len_includes, name_includes,  // includes
      &num_outputs, &data_outputs, &len_outputs, &name_outputs); // outputs

  if (status != 0) {
    if (!quiet) {
      auto *logp =
          std::find_if(name_outputs, name_outputs + num_outputs,
                       [&](const char *name) { return name == log_file; });

      assert(logp != name_outputs + num_outputs);
      const auto log_index = logp - name_outputs;
      std::string_view log(reinterpret_cast<char *>(data_outputs[log_index]),
                           len_outputs[log_index]);

      logging::error() << "Build log:\n" << log << '\n';
    }

    throw OfflineCompilerException("Offline compiler failed");
  }

  auto *spvp =
      std::find_if(name_outputs, name_outputs + num_outputs,
                   [&](const char *name) { return name == spv_file + ".spv"; });

  assert(spvp != name_outputs + num_outputs);
  const auto spv_index = spvp - name_outputs;
  const uint8_t *spv = data_outputs[spv_index];
  const auto spv_length = len_outputs[spv_index];

  return {spv, spv + spv_length};
}

} // namespace cassian
