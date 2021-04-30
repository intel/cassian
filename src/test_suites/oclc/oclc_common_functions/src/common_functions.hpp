/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_OCLC_COMMON_FUNCTIONS_COMMON_FUNCTIONS_HPP
#define CASSIAN_OCLC_COMMON_FUNCTIONS_COMMON_FUNCTIONS_HPP

#include <algorithm>
#include <cassian/logging/logging.hpp>
#include <cassian/random/random.hpp>
#include <cassian/utility/utility.hpp>
#include <cassian/vector/vector.hpp>
#include <catch2/catch.hpp>
#include <cfloat>
#include <cmath>
#include <common.hpp>
#include <cstdlib>
#include <enum_definitions.hpp>
#include <limits>
#include <map>
#include <string>
#include <variant>
#include <vector>

constexpr std::initializer_list<Function> two_arg_functions = {
    Function::max, Function::min, Function::step};
constexpr std::initializer_list<Function> three_arg_functions = {
    Function::clamp, Function::mix, Function::smoothstep};

template <typename TEST_TYPE, typename INPUT_TYPE_1 = TEST_TYPE,
          typename INPUT_TYPE_2 = INPUT_TYPE_1,
          typename INPUT_TYPE_3 = INPUT_TYPE_2>
class OclcFunction {
private:
  Function function;
  size_t arg_num = 0;

public:
  size_t get_arg_num() const { return arg_num; }
  using gentype = typename TEST_TYPE::host_type;
  using input_type_1 = typename INPUT_TYPE_1::host_type;
  using input_type_2 = typename INPUT_TYPE_2::host_type;
  using input_type_3 = typename INPUT_TYPE_3::host_type;

  using scalar_type = typename TEST_TYPE::scalar_type::host_type;

  explicit OclcFunction(const Function &function) : function(function) {
    if (std::find(two_arg_functions.begin(), two_arg_functions.end(),
                  function) != two_arg_functions.end()) {
      arg_num = 2UL;
    } else if (std::find(three_arg_functions.begin(), three_arg_functions.end(),
                         function) != three_arg_functions.end()) {
      arg_num = 3UL;
    } else {
      arg_num = 1UL;
    }
  }
  auto get_function() const { return function; }
  std::string get_build_options(const std::string &function_string) const {
    std::stringstream ss;
    ss << "-DOUTPUT_TYPE=" << TEST_TYPE::device_type;
    if (arg_num == 2UL) {
      ss << " -DTWO_ARGS";
      ss << " -DINPUT_TYPE_2=" << INPUT_TYPE_2::device_type;
    } else if (arg_num == 3UL) {
      ss << " -DTHREE_ARGS";
      ss << " -DINPUT_TYPE_2=" << INPUT_TYPE_2::device_type;
      ss << " -DINPUT_TYPE_3=" << INPUT_TYPE_3::device_type;
    }
    ss << " -DINPUT_TYPE_1=" << INPUT_TYPE_1::device_type;
    ss << " -DFUNCTION=" << function_string;
    return ss.str();
  }
};

template <typename TEST_TYPE, typename INPUT_TYPE_1 = TEST_TYPE,
          typename INPUT_TYPE_2 = INPUT_TYPE_1,
          typename INPUT_TYPE_3 = INPUT_TYPE_2>
class GentypeXFunction
    : public OclcFunction<TEST_TYPE, INPUT_TYPE_1, INPUT_TYPE_2, INPUT_TYPE_3> {
  using OclcFunc =
      OclcFunction<TEST_TYPE, INPUT_TYPE_1, INPUT_TYPE_2, INPUT_TYPE_3>;

public:
  using scalar_type = typename TEST_TYPE::scalar_type::host_type;
  using gentype = typename TEST_TYPE::host_type;

  explicit GentypeXFunction(const Function &function) : OclcFunc(function) {}
  std::vector<std::map<uint32_t, gentype>> get_gentype_values() const {
    constexpr auto seed = 0;
    const auto x = cassian::generate_value<scalar_type>(-0.1234567891011F,
                                                        0.98765432111F, seed);
    switch (this->get_function()) {
    case Function::clamp: {
      return {{{0, gentype(x)}}};
    }
    case Function::max: {
      return {{{0, gentype(x)}}};
    }
    case Function::min: {
      return {{{0, gentype(x)}}};
    }
    case Function::mix: {
      const auto y = cassian::generate_value<scalar_type>(-1.1234567891011F,
                                                          2.98765432111F, seed);
      return {{{0, gentype(x)}, {1, gentype(y)}}};
    }
    case Function::smoothstep: {
      return {{{2, gentype(x)}}};
    }
    case Function::step: {
      return {{{1, gentype(x)}}};
    }
    default:
      throw;
    }
  }

  std::vector<std::map<uint32_t, scalar_type>>
  get_gentype_scalar_values() const {
    constexpr auto seed = 0;
    const auto x = cassian::generate_value<scalar_type>(-0.1234567891011F,
                                                        0.98765432111F, 0);
    switch (this->get_function()) {
    case Function::clamp: {
      const scalar_type minval = cassian::generate_value<scalar_type>(
          -0.1234567891011F, 0.98765432111F, seed);
      const scalar_type maxval = cassian::generate_value<scalar_type>(
          1.1234567891011F, 3.98765432111F, seed);
      assert(minval <= maxval);
      return {{{1, minval}, {2, maxval}}};
    }
    case Function::max: {
      const scalar_type y = cassian::generate_value<scalar_type>(
          1.1234567891011F, 3.98765432111F, seed);
      return {{{1, y}}};
    }
    case Function::min: {
      const scalar_type y = cassian::generate_value<scalar_type>(
          -3.1234567891011F, -1.98765432111F, seed);
      return {{{1, y}}};
    }
    case Function::mix: {
      const scalar_type a =
          cassian::generate_value<scalar_type>(0.0F, 1.0F, seed);
      return {{{2, a}}};
    }
    case Function::step: {
      const scalar_type edge = cassian::generate_value<scalar_type>(
          1.1234567891011F, 3.98765432111F, seed);
      return {{{0, edge}}};
    }
    case Function::smoothstep: {
      const scalar_type edge_0 = cassian::generate_value<scalar_type>(
          -0.1234567891011F, 0.98765432111F, seed);
      const scalar_type edge_1 = cassian::generate_value<scalar_type>(
          1.1234567891011F, 3.98765432111F, seed);
      return {{{0, edge_0}, {1, edge_1}}};
    }
    default:
      throw;
    }
  }
};

template <typename TEST_TYPE, typename INPUT_TYPE_1 = TEST_TYPE,
          typename INPUT_TYPE_2 = INPUT_TYPE_1,
          typename INPUT_TYPE_3 = INPUT_TYPE_2>
class GentypeFunction
    : public OclcFunction<TEST_TYPE, INPUT_TYPE_1, INPUT_TYPE_2, INPUT_TYPE_3> {
  using OclcFunc =
      OclcFunction<TEST_TYPE, INPUT_TYPE_1, INPUT_TYPE_2, INPUT_TYPE_3>;

public:
  using scalar_type = typename TEST_TYPE::scalar_type::host_type;

  using gentype = typename TEST_TYPE::host_type;
  explicit GentypeFunction(const Function &function) : OclcFunc(function) {}
  std::vector<std::map<uint32_t, gentype>> get_gentype_values() const {
    constexpr auto inf = std::numeric_limits<scalar_type>::infinity();
    constexpr auto seed = 0;
    switch (this->get_function()) {
    case Function::clamp: {
      const auto minval = cassian::generate_value<scalar_type>(
          -0.1234567891011F, 0.98765432111F, seed);
      const auto maxval = cassian::generate_value<scalar_type>(
          1.1234567891011F, 3.98765432111F, seed);
      const auto x = cassian::generate_value<scalar_type>(-0.3234567891011F,
                                                          6.98765432111F, seed);
      return {{{0, gentype(x)}, {1, gentype(minval)}, {2, gentype(maxval)}},
              {{0, gentype(inf)}, {1, gentype(-inf)}, {2, gentype(inf)}}};
    }
    case Function::max: {
      const auto x = cassian::generate_value<scalar_type>(-0.1234567891011F,
                                                          0.98765432111F, seed);
      const auto y = cassian::generate_value<scalar_type>(1.1234567891011F,
                                                          3.98765432111F, seed);
      return {{{0, gentype(x)}, {1, gentype(y)}},
              {{0, gentype(inf)}, {1, gentype(-inf)}}};
    }
    case Function::min: {
      const auto x = cassian::generate_value<scalar_type>(-0.1234567891011F,
                                                          0.98765432111F, seed);
      const auto y = cassian::generate_value<scalar_type>(1.1234567891011F,
                                                          3.98765432111F, seed);
      return {{{0, gentype(x)}, {1, gentype(y)}},
              {{0, gentype(inf)}, {1, gentype(-inf)}}};
    }
    case Function::mix: {
      const auto x = cassian::generate_value<scalar_type>(-0.1234567891011F,
                                                          0.98765432111F, seed);
      const auto y = cassian::generate_value<scalar_type>(1.1234567891011F,
                                                          3.98765432111F, seed);
      const auto a = cassian::generate_value<scalar_type>(0.0F, 1.0F, seed);
      return {{{0, gentype(x)}, {1, gentype(y)}, {2, gentype(a)}},
              {{0, gentype(inf)}, {1, gentype(-inf)}, {2, gentype(a)}}};
    }
    case Function::smoothstep: {
      const auto edge0 = cassian::generate_value<scalar_type>(
          -0.1234567891011F, 0.98765432111F, seed);
      const auto edge1 = cassian::generate_value<scalar_type>(
          1.1234567891011F, 3.98765432111F, seed);
      const auto x = cassian::generate_value<scalar_type>(-0.5F, 1.0F, seed);
      return {{{0, gentype(edge0)}, {1, gentype(edge1)}, {2, gentype(x)}}};
    }
    case Function::step: {
      const auto x = cassian::generate_value<scalar_type>(-0.1234567891011F,
                                                          0.98765432111F, seed);
      const auto y = cassian::generate_value<scalar_type>(1.1234567891011F,
                                                          3.98765432111F, seed);
      return {{{0, gentype(x)}, {1, gentype(y)}},
              {{0, gentype(inf)}, {1, gentype(-inf)}}};
    }
    case Function::sign: {
      const auto x = std::nextafter(0.0F, -1.0F);
      return {{{0, gentype(x)}}};
    }
    case Function::radians: {
      const auto x =
          cassian::generate_value<scalar_type>(-180.0F, 180.0F, seed);
      return {{{0, gentype(x)}}, {{0, gentype(inf)}}, {{0, gentype(-inf)}}};
    }
    case Function::degrees: {
      const auto x = cassian::generate_value<scalar_type>(-3.14F, 3.14F, seed);
      return {{{0, gentype(x)}}, {{0, gentype(inf)}}, {{0, gentype(-inf)}}};
    }
    default:
      throw;
    }
  }

  std::vector<std::map<uint32_t, scalar_type>>
  get_gentype_scalar_values() const {
    return std::vector<std::map<uint32_t, scalar_type>>();
  }
};

template <typename T>
std::vector<cassian::scalar_type_v<T>> get_ulp_values(const Function &function,
                                                      const size_t &work_size) {
  using scalar_type = cassian::scalar_type_v<T>;
  constexpr auto epsilon = std::numeric_limits<scalar_type>::epsilon();
  switch (function) {
  case Function::max:
  case Function::min:
  case Function::step:
  case Function::sign:
  case Function::clamp: {
    constexpr scalar_type ulp = 0.0F;
    std::vector<scalar_type> ulp_values(work_size, ulp);
    return ulp_values;
  }
  case Function::mix: {
    constexpr scalar_type aet = 1e-3F;
    std::vector<scalar_type> ulp_values(work_size, aet);
    return ulp_values;
  }
  case Function::smoothstep: {
    constexpr scalar_type aet = 1e-5F;
    std::vector<scalar_type> ulp_values(work_size, aet);
    return ulp_values;
  }
  case Function::degrees:
  case Function::radians: {
    constexpr scalar_type ulp = 2.0F * epsilon;
    std::vector<scalar_type> ulp_values(work_size, ulp);
    return ulp_values;
  }
  default:
    throw UnknownFunctionException(
        Catch::StringMaker<Function>::convert(function) + " uninitialized");
  }
}

template <typename OUTPUT_TYPE, typename INPUT_TYPE_1, typename INPUT_TYPE_2,
          typename INPUT_TYPE_3>
OUTPUT_TYPE calculate_reference(const INPUT_TYPE_1 &input_value_a,
                                const INPUT_TYPE_2 &input_value_b,
                                const INPUT_TYPE_3 &input_value_c,
                                const Function &function) {
  constexpr auto is_vector_a = cassian::is_vector_v<INPUT_TYPE_1>;
  constexpr auto is_vector_b = cassian::is_vector_v<INPUT_TYPE_2>;
  constexpr auto is_vector_c = cassian::is_vector_v<INPUT_TYPE_3>;
  constexpr auto is_vector_output = cassian::is_vector_v<OUTPUT_TYPE>;
  switch (function) {
  case Function::clamp: {
    if constexpr (is_vector_output) {
      OUTPUT_TYPE result(0.0F);
      for (auto i = 0; i < OUTPUT_TYPE::vector_size; i++) {
        if constexpr (is_vector_a && is_vector_b && is_vector_c) {
          result[i] = std::fmin(std::fmax(input_value_a[i], input_value_b[i]),
                                input_value_c[i]);
        } else if constexpr (is_vector_a && !is_vector_c && !is_vector_b) {
          result[i] = std::fmin(std::fmax(input_value_a[i], input_value_b),
                                input_value_c);
        }
      }
      return result;
    } else {
      return std::fmin(std::fmax(input_value_a, input_value_b), input_value_c);
    }
  }
  case Function::max: {
    if constexpr (is_vector_output) {
      OUTPUT_TYPE result(0.0F);
      for (auto i = 0; i < OUTPUT_TYPE::vector_size; i++) {
        if constexpr (is_vector_b && is_vector_a) {
          result[i] = std::max(input_value_a[i], input_value_b[i]);
        } else if constexpr (is_vector_a && !is_vector_b) {
          result[i] = std::max(input_value_a[i], input_value_b);
        }
      }
      return result;
    } else {
      return std::max(input_value_a, input_value_b);
    }
  }
  case Function::min: {
    if constexpr (cassian::is_vector_v<OUTPUT_TYPE>) {
      OUTPUT_TYPE result(0.0F);
      for (auto i = 0; i < OUTPUT_TYPE::vector_size; i++) {
        if constexpr (is_vector_b && is_vector_a) {
          result[i] = std::min(input_value_a[i], input_value_b[i]);
        } else if constexpr (is_vector_a && !is_vector_b) {
          result[i] = std::min(input_value_a[i], input_value_b);
        }
      }
      return result;
    } else {
      return std::min(input_value_a, input_value_b);
    }
  }
  case Function::mix: {
    if constexpr (is_vector_output) {
      OUTPUT_TYPE result(0.0F);
      for (auto i = 0; i < OUTPUT_TYPE::vector_size; i++) {
        if constexpr (is_vector_a && is_vector_b && is_vector_c) {
          result[i] = input_value_a[i] +
                      (input_value_b[i] - input_value_a[i]) * input_value_c[i];
        } else if constexpr (is_vector_a && is_vector_b && !is_vector_c) {
          result[i] = input_value_a[i] +
                      (input_value_b[i] - input_value_a[i]) * input_value_c;
        }
      }
      return result;
    } else {
      return input_value_a + (input_value_b - input_value_a) * input_value_c;
    }
  }
  case Function::step: {
    if constexpr (is_vector_output) {
      OUTPUT_TYPE result(0.0F);
      for (auto i = 0; i < OUTPUT_TYPE::vector_size; i++) {
        if constexpr (is_vector_b) {
          result[i] = input_value_b[i] < input_value_a ? 0.0F : 1.0F;
        } else {
          result[i] = input_value_b < input_value_a ? 0.0F : 1.0F;
        }
      }
      return result;
    } else {
      return input_value_b < input_value_a ? OUTPUT_TYPE(0.0F)
                                           : OUTPUT_TYPE(1.0F);
    }
  }
  case Function::smoothstep: {
    OUTPUT_TYPE result(0.0F);
    constexpr cassian::scalar_type_v<OUTPUT_TYPE> lo = 0.0F;
    constexpr cassian::scalar_type_v<OUTPUT_TYPE> hi = 1.0F;
    if constexpr (is_vector_output) {
      for (auto i = 0; i < OUTPUT_TYPE::vector_size; i++) {
        if constexpr (is_vector_c && is_vector_a && is_vector_b) {
          result[i] = std::clamp((input_value_c[i] - input_value_a[i]) /
                                     (input_value_b[i] - input_value_a[i]),
                                 lo, hi);
        } else if constexpr (is_vector_c && !is_vector_a && !is_vector_b) {
          result[i] = std::clamp((input_value_c[i] - input_value_a) /
                                     (input_value_b - input_value_a),
                                 lo, hi);
        }
        result[i] = result[i] * result[i] * (3.0F - 2.0F * result[i]);
      }
      return result;
    } else {
      result = std::clamp((input_value_c - input_value_a) /
                              (input_value_b - input_value_a),
                          lo, hi);
      return result * result * (3.0F - 2.0F * result);
    }
  }
  case Function::degrees: {
    using scalar_type = cassian::scalar_type_v<OUTPUT_TYPE>;
    OUTPUT_TYPE result(0.0F);
    constexpr scalar_type pi_const = 180.0F / M_PI;
    if constexpr (is_vector_a) {
      OUTPUT_TYPE result(0.0F);
      for (auto i = 0; i < OUTPUT_TYPE::vector_size; i++) {
        result[i] = pi_const * input_value_a[i];
      }
      return result;
    } else if constexpr (!is_vector_a && !is_vector_output) {
      return pi_const * input_value_a;
    }
  }
  case Function::radians: {
    using scalar_type = cassian::scalar_type_v<OUTPUT_TYPE>;
    OUTPUT_TYPE result(0.0F);
    constexpr scalar_type pi_const = scalar_type(M_PI) / 180.0F;
    if constexpr (is_vector_a && is_vector_output) {
      OUTPUT_TYPE result(0.0F);
      for (auto i = 0; i < OUTPUT_TYPE::vector_size; i++) {
        result[i] = pi_const * input_value_a[i];
      }
      return result;
    } else if constexpr (!is_vector_a && !is_vector_output) {
      return pi_const * input_value_a;
    }
  }
  case Function::sign: {
    OUTPUT_TYPE result(0.0F);
    if constexpr (is_vector_a && is_vector_output) {
      OUTPUT_TYPE result(0.0F);
      for (auto i = 0; i < OUTPUT_TYPE::vector_size; i++) {
        if (std::isnan(input_value_a[i])) {
          result[i] = 0.0F;
        }
        if (input_value_a[i] > 0.0F) {
          result[i] = 1.0F;
        }
        if (input_value_a[i] == -0.0F) {
          result[i] = -0.0F;
        }
        if (input_value_a[i] == +0.0F) {
          result[i] = +0.0F;
        }
        if (input_value_a[i] < 0.0F) {
          result[i] = -1.0F;
        }
      }
      return result;
    } else if constexpr (!is_vector_a && !is_vector_output) {
      if (std::isnan(input_value_a)) {
        return 0.0F;
      }
      if (input_value_a > 0.0F) {
        return 1.0F;
      }
      if (input_value_a == -0.0F) {
        return -0.0F;
      }
      if (input_value_a == +0.0F) {
        return +0.0F;
      }
      if (input_value_a < 0.0F) {
        return -1.0F;
      }
    }
  }
  default:
    throw UnknownFunctionException(
        Catch::StringMaker<Function>::convert(function) +
        " uninitialized calculate_reference");
  }
}

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
  if (function == Function::mix || function == Function::smoothstep) {
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

template <typename OUTPUT_TYPE>
class UlpComparator : public Catch::MatcherBase<std::vector<OUTPUT_TYPE>> {
  std::vector<OUTPUT_TYPE> reference;
  Function function;
  std::vector<cassian::scalar_type_v<OUTPUT_TYPE>> ulp_values;

public:
  UlpComparator<OUTPUT_TYPE>(const std::vector<OUTPUT_TYPE> &reference,
                             const size_t &work_size, const Function &function)
      : reference(reference), function(function) {
    ulp_values = get_ulp_values<OUTPUT_TYPE>(function, work_size);
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
    return "\nreference: " + input_to_string<OUTPUT_TYPE>(reference);
  }
};
#endif