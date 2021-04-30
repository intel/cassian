/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_OCLC_COMMON_FUNCTIONS_COMMON_HPP
#define CASSIAN_OCLC_COMMON_FUNCTIONS_COMMON_HPP

#include <algorithm>
#include <cassian/runtime/openclc_types.hpp>
#include <cassian/vector/type_helpers.hpp>
#include <cassian/vector/vector.hpp>
#include <catch2/catch.hpp>
#include <cfloat>
#include <cstring>
#include <enum_definitions.hpp>
#include <limits>
#include <stdexcept>
#include <string>
#include <type_traits>

CASSIAN_CATCH_REGISTER_COMMON_FUNCTIONS;

int suggest_work_size(const std::string &type);

std::string create_section_name(const std::string &function_string,
                                const size_t &section_type);

class UnknownFunctionException : public std::runtime_error {
  using std::runtime_error::runtime_error;
};

class UnknownTypeException : public std::runtime_error {
  using std::runtime_error::runtime_error;
};

class EmptyVectorException : public std::runtime_error {
  using std::runtime_error::runtime_error;
};

template <typename TestType> auto test_name() {
  return std::string(TestType::type_name);
}

template <typename T>
std::string input_to_string(const std::vector<T> &input_a) {
  std::stringstream ss;
  ss << "{";
  for (const auto &input : input_a) {
    ss << cassian::to_string<T>(input) << ", ";
  }
  ss.seekp(-2, std::ios_base::end);
  ss << "}";
  return ss.str();
}

template <typename T> T ulp_distance(const T &a, const T &b) {
  if (a == b) {
    return 0;
  }
  if (std::isnan(a) || std::isnan(b)) {
    return std::numeric_limits<T>::quiet_NaN();
  }
  if (std::isinf(a) || std::isinf(b)) {
    return std::numeric_limits<T>::infinity();
  }
  return ((b >= a) ? b - a : a - b);
}

template <typename T, cassian::EnableIfIsVector<T> = 0>
auto randomize_input(const std::vector<T> &input) {
  auto randomized_input = input;
  for (auto j = 1; j < input.size(); j++) {
    for (auto k = 0; k < T::vector_size; k++) {
      randomized_input[j][k] =
          std::nextafter(input[j - 1][k],
                         std::numeric_limits<cassian::scalar_type_v<T>>::max());
    }
  }
  return randomized_input;
}

template <typename T, cassian::EnableIfIsScalar<T> = 0>
auto randomize_input(const std::vector<T> &input) {
  auto randomized_input = input;
  for (auto j = 1; j < input.size(); j++) {
    randomized_input[j] = std::nextafter(
        input[j - 1], std::numeric_limits<cassian::scalar_type_v<T>>::max());
  }
  return randomized_input;
}

#endif
