/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_MAIN_FLAGS_BUILDER_HPP
#define CASSIAN_MAIN_FLAGS_BUILDER_HPP

#include <string>
#include <string_view>

namespace cassian {
namespace test {

enum class Language {
  opencl_c,
  cm,
};

enum class OptimizationLevel {
  none,
  full,
};

class FlagsBuilder {
public:
  explicit FlagsBuilder(Language lang) : lang_(lang) {
    switch (lang_) {
    case Language::opencl_c:
      flags_ = "";
      break;
    case Language::cm:
      flags_ = "-cmc";
      break;
    }
  }

  const std::string &str() noexcept { return flags_; }

  FlagsBuilder &append(std::string_view flag) {
    flags_ += ' ';
    flags_ += flag;
    return *this;
  }

  FlagsBuilder &define(std::string_view macro) {
    flags_ += " -D";
    flags_ += macro;
    return *this;
  }

  FlagsBuilder &define(std::string_view macro, std::string_view value) {
    define(macro);
    flags_ += "=\"";
    flags_ += value;
    flags_ += '"';
    return *this;
  }

  FlagsBuilder &include(std::string_view directory) {
    flags_ += " -I";
    flags_ += directory;
    return *this;
  }

  FlagsBuilder &optimize(OptimizationLevel level) {
    if (level == OptimizationLevel::full) {
      return *this;
    }

    switch (lang_) {
    case Language::opencl_c:
      return append("-cl-opt-disable");
    case Language::cm:
      return append("-O0");
    }
  }

  FlagsBuilder &debug() { return append("-g"); }

private:
  Language lang_;

  std::string flags_;
};

} // namespace test
} // namespace cassian

#endif
