/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/logging/logging.hpp>
#include <cassian/utility/version.hpp>
#include <sstream>
#include <string>
#include <version_config.hpp>

namespace cassian {
std::string get_version() {
  std::stringstream ss;
  ss << major_version << "." << minor_version << "." << patch_version;
  if (!git_revision.empty()) {
    ss << "+" << git_revision;
  }
  return ss.str();
}

void print_version() { logging::info() << "Cassian " << get_version() << '\n'; }

} // namespace cassian
