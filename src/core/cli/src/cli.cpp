/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/cli/cli.hpp>
#include <string>
#include <utility>

namespace cassian {
void CommandLineParser::add_argument(const std::string &name,
                                     const std::string &default_value) {
  arguments_[name] = default_value;
}

void CommandLineParser::parse(int *argc, char **argv) {
  for (int i = 1; i < *argc; ++i) {
    const std::string str = argv[i];
    int tokens_to_remove = 0;

    if (str.rfind("--list", 0) == 0 || str == "-l" || str == "-t") {
      list_requested_ = true;
    }

    for (const auto &kv : arguments_) {
      const auto key = kv.first;
      if (str == key) {
        tokens_to_remove++;

        // TODO: Check for out-of-bound access
        arguments_[key] = argv[i + 1];
        tokens_to_remove++;

        break;
      }
    }

    for (int j = i; j < *argc - tokens_to_remove; ++j) {
      argv[j] = argv[j + tokens_to_remove];
    }
    *argc -= tokens_to_remove;
    i -= tokens_to_remove;
  }
}

bool CommandLineParser::list_requested() const { return list_requested_; }

} // namespace cassian
