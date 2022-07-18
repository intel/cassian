/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_OCLC_GEOMETRIC_FUNCTIONS_GEOMETRIC_FUNCTIONS_HPP
#define CASSIAN_OCLC_GEOMETRIC_FUNCTIONS_GEOMETRIC_FUNCTIONS_HPP

#include "cassian/logging/logging.hpp"
#include <bitset>
#include <cassian/random/random.hpp>
#include <cassian/utility/math.hpp>
#include <cassian/utility/utility.hpp>
#include <cassian/vector/vector.hpp>
#include <catch2/catch.hpp>
#include <cfloat>
#include <common.hpp>
#include <cstdlib>
#include <enum_definitions.hpp>
#include <limits>
#include <map>
#include <string>
#include <variant>
#include <vector>

enum class OutputType { scalar, vector };
constexpr std::initializer_list<Function> two_arg_functions = {
    Function::cross, Function::dot, Function::distance,
    Function::fast_distance};
template <typename TEST_TYPE, typename INPUT_TYPE> class OclcFunction {
private:
  Function function;
  size_t arg_num = 0;

protected:
  OutputType output_type = OutputType::scalar;

public:
  size_t get_arg_num() const { return arg_num; }

  using SCALAR_TYPE = typename TEST_TYPE::scalar_type::host_type;
  explicit OclcFunction(const Function &function) : function(function) {
    if (std::find(two_arg_functions.begin(), two_arg_functions.end(),
                  function) != two_arg_functions.end()) {
      arg_num = 2UL;
    } else {
      arg_num = 1UL;
    }
  }
  auto get_function() const { return function; }
  std::string get_build_options(const std::string &function_string) const {
    std::stringstream ss;
    ss << "-DOUTPUT_TYPE="
       << (output_type == OutputType::scalar
               ? TEST_TYPE::scalar_type::device_type
               : TEST_TYPE::device_type);
    const auto arg_num = get_arg_num();
    if (arg_num == 2UL) {
      ss << " -DTWO_ARGS";
    }
    ss << " -DFUNCTION=" << function_string;
    ss << " -DINPUT_TYPE=" << TEST_TYPE::device_type;
    return ss.str();
  }

  std::vector<std::vector<INPUT_TYPE>> get_input_values() const {
    const auto min = -1.123456789;
    const auto max = 1.987654321;
    const auto seed = 0;
    switch (function) {
    case Function::dot:
      return {{cassian::generate_value<INPUT_TYPE>(min, max, seed),
               cassian::generate_value<INPUT_TYPE>(min, max, seed)}};
    case Function::cross:
      return {{cassian::generate_value<INPUT_TYPE>(min, max, seed),
               cassian::generate_value<INPUT_TYPE>(min, max, seed)}};
    case Function::distance:
      return {{cassian::generate_value<INPUT_TYPE>(min, max, seed),
               cassian::generate_value<INPUT_TYPE>(min, max, seed)}};
    case Function::fast_distance:
      return {{cassian::generate_value<INPUT_TYPE>(min, max, seed),
               cassian::generate_value<INPUT_TYPE>(min, max, seed)}};
    case Function::normalize:
      return {{INPUT_TYPE(min)}, {INPUT_TYPE(max)}};
    case Function::fast_normalize:
      return {{INPUT_TYPE(min)}, {INPUT_TYPE(max)}};
    case Function::length:
      return {{INPUT_TYPE(min)}, {INPUT_TYPE(max)}};
    case Function::fast_length:
      return {{INPUT_TYPE(min)}, {INPUT_TYPE(max)}};
    default:
      throw UnknownFunctionException(
          Catch::StringMaker<Function>::convert(function) + " uninitialized");
    }
  };

  std::vector<std::vector<std::vector<INPUT_TYPE>>>
  get_special_values_vectors(const int work_size) const {
    const auto min = -1.123456789;
    const auto max = 1.987654321;
    const auto seed = 0;
    constexpr auto inf = std::numeric_limits<SCALAR_TYPE>::infinity();
    switch (function) {
    case Function::dot:
      return {
          {std::vector<INPUT_TYPE>(work_size, INPUT_TYPE(inf)),
           std::vector<INPUT_TYPE>(work_size, INPUT_TYPE(inf))},
      };
    case Function::cross:
      return {{{std::vector<INPUT_TYPE>(work_size, INPUT_TYPE(inf)),
                std::vector<INPUT_TYPE>(work_size, INPUT_TYPE(inf))}}};
    case Function::normalize: {
      auto vector_with_nan =
          cassian::generate_vector<INPUT_TYPE>(work_size, min, max, seed);
      if constexpr (cassian::is_vector_v<INPUT_TYPE>) {
        for (int i = 0; i < work_size; ++i) {
          vector_with_nan[i][0] = std::numeric_limits<SCALAR_TYPE>::quiet_NaN();
        }
      } else {
        std::fill(vector_with_nan.begin(), vector_with_nan.end(),
                  INPUT_TYPE(0));
      }
      return {{std::vector<INPUT_TYPE>(work_size, INPUT_TYPE(inf))},
              {std::vector<INPUT_TYPE>(work_size, INPUT_TYPE(-inf))},
              {std::vector<INPUT_TYPE>(work_size, INPUT_TYPE(0))},
              {vector_with_nan}};
    }
    case Function::fast_normalize: {
      auto vector_with_nan =
          cassian::generate_vector<INPUT_TYPE>(work_size, min, max, seed);
      if constexpr (cassian::is_vector_v<INPUT_TYPE>) {
        for (int i = 0; i < work_size; ++i) {
          vector_with_nan[i][0] = std::numeric_limits<SCALAR_TYPE>::quiet_NaN();
        }
      } else {
        std::fill(vector_with_nan.begin(), vector_with_nan.end(),
                  INPUT_TYPE(0));
      }
      return {{std::vector<INPUT_TYPE>(work_size, INPUT_TYPE(inf))},
              {std::vector<INPUT_TYPE>(work_size, INPUT_TYPE(-inf))},
              {std::vector<INPUT_TYPE>(work_size, INPUT_TYPE(0))},
              {vector_with_nan}};
    }
    case Function::distance:
      return {{std::vector<INPUT_TYPE>(work_size, INPUT_TYPE(inf)),
               std::vector<INPUT_TYPE>(work_size, INPUT_TYPE(-inf))},
              {std::vector<INPUT_TYPE>(
                   work_size,
                   INPUT_TYPE(std::numeric_limits<SCALAR_TYPE>::max() + 1)),
               std::vector<INPUT_TYPE>(work_size, INPUT_TYPE(0))},
              {std::vector<INPUT_TYPE>(
                   work_size,
                   INPUT_TYPE(std::numeric_limits<SCALAR_TYPE>::min() - 1)),
               std::vector<INPUT_TYPE>(work_size, INPUT_TYPE(0))}};
    case Function::fast_distance:
      return {{std::vector<INPUT_TYPE>(work_size, INPUT_TYPE(inf)),
               std::vector<INPUT_TYPE>(work_size, INPUT_TYPE(-inf))}};
    case Function::length:
      return {{std::vector<INPUT_TYPE>(work_size, INPUT_TYPE(inf))},
              {std::vector<INPUT_TYPE>(work_size, INPUT_TYPE(-inf))}};
    case Function::fast_length:
      return {{std::vector<INPUT_TYPE>(work_size, INPUT_TYPE(inf))},
              {std::vector<INPUT_TYPE>(work_size, INPUT_TYPE(-inf))}};
    default:
      throw UnknownFunctionException(
          Catch::StringMaker<Function>::convert(function) + " uninitialized");
    }
  };
};

template <typename TEST_TYPE, typename INPUT_TYPE>
class VectorOclcFunction : public OclcFunction<TEST_TYPE, INPUT_TYPE> {
  using OclcFunc = OclcFunction<TEST_TYPE, INPUT_TYPE>;

public:
  using vector_type = typename TEST_TYPE::host_type;
  explicit VectorOclcFunction(const Function &function) : OclcFunc(function) {
    this->output_type = OutputType::vector;
  }

  vector_type
  calculate_reference(const INPUT_TYPE &input_value_a,
                      const INPUT_TYPE &input_value_b = INPUT_TYPE(0)) const {
    switch (this->get_function()) {
    case Function::normalize:
      return cassian::normalize(input_value_a);
    case Function::fast_normalize:
      return cassian::normalize(input_value_a);
    case Function::cross:
      return cassian::cross_product(input_value_a, input_value_b);
    default:
      throw UnknownFunctionException(
          Catch::StringMaker<Function>::convert(this->get_function()) +
          " uninitialized");
    }
  }
};

template <typename TEST_TYPE, typename INPUT_TYPE>
class ScalarOclcFunction : public OclcFunction<TEST_TYPE, INPUT_TYPE> {
private:
  using OclcFunc = OclcFunction<TEST_TYPE, INPUT_TYPE>;

public:
  using scalar_type = typename TEST_TYPE::scalar_type::host_type;
  using OclcFunc::OclcFunction;

  scalar_type
  calculate_reference(const INPUT_TYPE &input_value_a,
                      const INPUT_TYPE &input_value_b = INPUT_TYPE(0)) const {
    switch (this->get_function()) {
    case Function::dot: {
      return cassian::dot_product(input_value_a, input_value_b);
    case Function::distance:
      return cassian::distance(input_value_a, input_value_b);
    case Function::fast_distance:
      return cassian::distance(input_value_a, input_value_b);
    case Function::length:
      return cassian::length(input_value_a);
    case Function::fast_length:
      return cassian::length(input_value_a);
    default:
      throw UnknownFunctionException(
          Catch::StringMaker<Function>::convert(this->get_function()) +
          " uninitialized");
    }
    }
  }
};

template <typename T, typename cassian::EnableIfIsVector<T> = 0>
bool match_results(const T &result, const T &reference,
                   const cassian::scalar_type_v<T> &ulp_value,
                   const Function &function) {
  for (auto i = 0; i < result.size(); i++) {
    if (!match_results(result[i], reference[i], ulp_value, function)) {
      return false;
    }
  }
  return true;
}

template <typename T, typename cassian::EnableIfIsScalar<T> = 0>
bool match_results(const T &result, const T &reference, const T &ulp_value,
                   const Function &function) {
  if (std::isnan(result) && std::isnan(reference)) {
    return true;
  }
  if (function == Function::dot || function == Function::cross) {
    // use absolute error tolerance comparison
    if (result == reference) {
      return true;
    }
    const auto max_error =
        std::max(std::numeric_limits<T>::epsilon(), ulp_value);
    const auto delta = std::fabs(reference - result) / reference;
    return delta <= max_error;
  }
  const auto ulp_dist = ulp_distance(result, reference);
  return ulp_dist <= ulp_value;
}

template <typename OUTPUT_TYPE, typename INPUT_TYPE, typename SCALAR_TYPE>
class UlpComparator : public Catch::MatcherBase<std::vector<OUTPUT_TYPE>> {
  std::vector<OUTPUT_TYPE> reference;
  Function function;
  std::vector<SCALAR_TYPE> ulp_values;

public:
  UlpComparator<OUTPUT_TYPE, INPUT_TYPE, SCALAR_TYPE>(
      const std::vector<OUTPUT_TYPE> &reference,
      const std::vector<INPUT_TYPE> &input_a,
      const std::vector<INPUT_TYPE> &input_b, const Function &function)
      : reference(reference), function(function) {
    ulp_values = get_ulp_values(input_a, input_b, function);
  }

  bool match(std::vector<OUTPUT_TYPE> const &result) const override {
    for (auto i = 0; i < result.size(); i++) {
      if (!match_results(result[i], reference[i], ulp_values[i], function)) {
        return false;
      }
    }
    return true;
  }
  std::string describe() const override {
    return "\nreference: " + input_to_string(reference);
  }
};

#endif