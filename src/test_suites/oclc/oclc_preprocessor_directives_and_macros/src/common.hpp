/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_OCLC_PREPROCESSOR_DIRECTIVES_AND_MACROS_COMMON_HPP
#define CASSIAN_OCLC_PREPROCESSOR_DIRECTIVES_AND_MACROS_COMMON_HPP

#include <cassian/runtime/runtime.hpp>
#include <catch2/catch.hpp>

#include <string>
#include <utility>

int suggest_work_size(const std::string &type);

template <typename T>
std::vector<T> run_kernel(const cassian::Kernel &kernel,
                          const std::vector<T> &input, const size_t work_size,
                          cassian::Runtime *runtime) {
  cassian::Buffer output_buffer =
      runtime->create_buffer(sizeof(T) * input.size());

  runtime->set_kernel_argument(kernel, 0, output_buffer);

  runtime->run_kernel(kernel, work_size);

  std::vector<T> output = runtime->read_buffer_to_vector<T>(output_buffer);
  runtime->release_buffer(output_buffer);

  return output;
}

template <typename T>
class IsBetweenMatcher : public Catch::MatcherBase<std::vector<T>> {
  T m_begin, m_end;

public:
  IsBetweenMatcher(T begin, T end) : m_begin(begin), m_end(end) {}

  bool match(std::vector<T> const &input) const override {
    for (const auto &in : input) {
      if (in < m_begin && in > m_end) {
        return false;
      }
    }
    return true;
  }
  std::string describe() const override {
    std::ostringstream ss;
    ss << "each should be between " << m_begin << " and " << m_end;
    return ss.str();
  }
};

template <typename T>
class FileMacroMatcher : public Catch::MatcherBase<std::vector<T>> {
  std::string file_name;
  size_t work_size;

public:
  explicit FileMacroMatcher(std::string file_name, const size_t &work_size)
      : file_name(std::move(file_name)), work_size(work_size) {}

  bool match(std::vector<T> const &in) const override {
    for (size_t i = 0; i < work_size * (file_name.length() + 1);
         i = (i + 1) * (file_name.length() + 1)) {
      if (file_name.compare(0, file_name.length() + 1,
                            reinterpret_cast<const char *>(in.data()) + i) !=
          0) {
        return false;
      }
    }
    return true;
  }
  std::string describe() const override {
    std::ostringstream ss;
    ss << "should be equal to " << work_size << " strings of " << file_name;
    return ss.str();
  }
};

#endif
