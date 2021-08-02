/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_OCLC_GEOMETRIC_FUNCTIONS_COMMON_HPP
#define CASSIAN_OCLC_GEOMETRIC_FUNCTIONS_COMMON_HPP

#include <algorithm>
#include <cassian/runtime/openclc_types.hpp>
#include <cassian/vector/vector.hpp>
#include <catch2/catch.hpp>
#include <cfloat>
#include <cmath>
#include <cstring>
#include <enum_definitions.hpp>
#include <limits>
#include <stdexcept>
#include <string>
#include <type_traits>

CASSIAN_CATCH_REGISTER_GEOMETRIC_FUNCTIONS;

int suggest_work_size(const std::string &type);

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
std::string input_to_string(const std::vector<T> &input_a,
                            const std::vector<T> &input_b = std::vector<T>()) {
  std::stringstream ss;
  ss << "{";
  for (const auto &input : input_a) {
    ss << cassian::to_string<T>(input) << ", ";
  }
  ss.seekp(-2, std::ios_base::end);
  ss << "}";
  if (!input_b.empty()) {
    ss << ", {";
    for (const auto &input : input_b) {
      ss << cassian::to_string<T>(input) << ", ";
    }
    ss.seekp(-2, std::ios_base::end);
    ss << "}";
  }
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
auto get_max_element(const T &vector) {
  const auto it =
      std::max_element(vector.begin(), vector.begin() + T::vector_size);
  if (it == vector.begin() + T::vector_size) {
    throw EmptyVectorException("Vector is empty");
  }
  return *it;
}
template <typename T, cassian::EnableIfIsScalar<T> = 0>
auto get_max_element(const T &scalar) {
  return scalar;
}

template <typename T, cassian::EnableIfIsVector<T> = 0>
constexpr auto get_vector_width() {
  return T::vector_size;
}
template <typename T, cassian::EnableIfIsScalar<T> = 0>
constexpr auto get_vector_width() {
  return 1;
}

template <typename T>
std::vector<cassian::scalar_type_v<T>>
get_ulp_values(const std::vector<T> &input_a, const std::vector<T> &input_b,
               const Function &function) {
  using scalar_type = cassian::scalar_type_v<T>;
  constexpr scalar_type vector_width = get_vector_width<T>();
  constexpr auto epsilon = std::numeric_limits<scalar_type>::epsilon();
  const auto size = input_a.size();
  switch (function) {
  case Function::dot: {
    std::vector<scalar_type> ulp_values;
    for (auto i = 0; i < size; i++) {
      const auto a = get_max_element(input_a[i]);
      const auto b = get_max_element(input_b[i]);
      const auto max = std::max(a, b);
      const auto aet_value = max * max * (2 * vector_width - 1) * FLT_EPSILON;
      ulp_values.push_back(std::fabs(aet_value));
    }
    return ulp_values;
  }
  case Function::normalize: {
    constexpr auto size_of_type = sizeof(scalar_type);
    if constexpr (size_of_type == sizeof(double)) {
      const scalar_type ulp = (4.5 + vector_width) * epsilon;
      return std::vector<scalar_type>(size, ulp);
    } else {
      const scalar_type ulp = (2.0F + vector_width) * epsilon;
      return std::vector<scalar_type>(size, ulp);
    }
  }
  default:
    throw UnknownFunctionException(
        Catch::StringMaker<Function>::convert(function) + " uninitialized");
  }
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

std::string create_section_name(const std::string &function_string,
                                const std::string &section_type);

#endif
