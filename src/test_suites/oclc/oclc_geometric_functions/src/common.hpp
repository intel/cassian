/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_OCLC_GEOMETRIC_FUNCTIONS_COMMON_HPP
#define CASSIAN_OCLC_GEOMETRIC_FUNCTIONS_COMMON_HPP

#include <algorithm>
#include <cassian/fp_types/math.hpp>
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
namespace ca = cassian;

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
    ss << ca::to_string<T>(input) << ", ";
  }
  ss.seekp(-2, std::ios_base::end);
  ss << "}";
  if (!input_b.empty()) {
    ss << ", {";
    for (const auto &input : input_b) {
      ss << ca::to_string<T>(input) << ", ";
    }
    ss.seekp(-2, std::ios_base::end);
    ss << "}";
  }
  return ss.str();
}

template <typename T> T ulp_distance(const T &a, const T &b) {
  using std::isinf;
  using std::isnan;
  if (a == b) {
    return 0;
  }
  if (isnan(a) || isnan(b)) {
    return std::numeric_limits<T>::quiet_NaN();
  }
  if (isinf(a) || isinf(b)) {
    return std::numeric_limits<T>::infinity();
  }
  return ((b >= a) ? b - a : a - b);
}

template <typename T, ca::EnableIfIsVector<T> = 0>
auto get_max_element(const T &vector) {
  const auto it =
      std::max_element(vector.begin(), vector.begin() + T::vector_size);
  if (it == vector.begin() + T::vector_size) {
    throw EmptyVectorException("Vector is empty");
  }
  return *it;
}
template <typename T, ca::EnableIfIsScalar<T> = 0>
auto get_max_element(const T &scalar) {
  return scalar;
}

template <typename T, ca::EnableIfIsVector<T> = 0>
constexpr auto get_vector_width() {
  return T::vector_size;
}
template <typename T, ca::EnableIfIsScalar<T> = 0>
constexpr auto get_vector_width() {
  return 1;
}

template <typename T>
std::vector<ca::scalar_type_v<T>> get_ulp_values(const std::vector<T> &input_a,
                                                 const std::vector<T> &input_b,
                                                 const Function &function) {
  using scalar_type = ca::scalar_type_v<T>;
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
      const auto aet_value = max * max * (2 * vector_width - 1) *
                             std::numeric_limits<float>::epsilon();
      ulp_values.push_back(std::fabs(aet_value));
    }
    return ulp_values;
  }
  case Function::cross: {
    std::vector<scalar_type> ulp_values;
    for (auto i = 0; i < size; i++) {
      const auto a = get_max_element(input_a[i]);
      const auto b = get_max_element(input_b[i]);
      const auto max = std::max(a, b);
      const auto aet_value =
          max * max * 3 * std::numeric_limits<float>::epsilon();
      ulp_values.push_back(std::fabs(aet_value));
    }
    return ulp_values;
  }
  case Function::normalize: {
    if constexpr (std::is_same_v<scalar_type, double>) {
      const scalar_type ulp = (4.5 + vector_width) * epsilon;
      return std::vector<scalar_type>(size, ulp);
    } else {
      const scalar_type ulp = (2.0F + vector_width) * epsilon;
      return std::vector<scalar_type>(size, ulp);
    }
  }
  case Function::distance: {
    if constexpr (std::is_same_v<scalar_type, double>) {
      const scalar_type ulp = (5.5 + 2 * vector_width) * epsilon;
      return std::vector<scalar_type>(size, ulp);
    } else {
      const scalar_type ulp = (2.5F + 2 * vector_width) * epsilon;
      return std::vector<scalar_type>(size, ulp);
    }
  }
  case Function::length: {
    if constexpr (std::is_same_v<scalar_type, double>) {
      const scalar_type ulp = (5.5 + vector_width) * epsilon;
      return std::vector<scalar_type>(size, ulp);
    } else {
      const scalar_type ulp = (2.75F + 0.5F * vector_width) * epsilon;
      return std::vector<scalar_type>(size, ulp);
    }
  }
  case Function::fast_distance: {
    const scalar_type ulp = (8191.5F + 2 * vector_width) * epsilon;
    return std::vector<scalar_type>(size, ulp);
  }
  case Function::fast_length: {
    const scalar_type ulp = (8191.5F + vector_width) * epsilon;
    return std::vector<scalar_type>(size, ulp);
  }
  case Function::fast_normalize: {
    const scalar_type ulp = (8192.0F + vector_width) * epsilon;
    return std::vector<scalar_type>(size, ulp);
  }
  default:
    throw UnknownFunctionException(
        Catch::StringMaker<Function>::convert(function) + " uninitialized");
  }
}

template <typename T, ca::EnableIfIsVector<T> = 0>
auto randomize_input(const std::vector<T> &input) {
  auto randomized_input = input;
  for (auto j = 1; j < input.size(); j++) {
    for (auto k = 0; k < T::vector_size; k++) {
      using std::nextafter;
      randomized_input[j][k] = nextafter(
          input[j - 1][k], std::numeric_limits<ca::scalar_type_v<T>>::max());
    }
  }
  return randomized_input;
}

template <typename T, ca::EnableIfIsScalar<T> = 0>
auto randomize_input(const std::vector<T> &input) {
  auto randomized_input = input;
  for (auto j = 1; j < input.size(); j++) {
    using std::nextafter;
    randomized_input[j] = nextafter(
        input[j - 1], std::numeric_limits<ca::scalar_type_v<T>>::max());
  }
  return randomized_input;
}

std::string create_section_name(const std::string &function_string,
                                const std::string &section_type);

#endif
