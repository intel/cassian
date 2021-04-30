/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_OCLC_INTEGER_FUNCTIONS_INTEGER_FUNCTIONS_HPP
#define CASSIAN_OCLC_INTEGER_FUNCTIONS_INTEGER_FUNCTIONS_HPP

#include <bitset>
#include <cassian/utility/utility.hpp>
#include <cassian/vector/vector.hpp>
#include <catch2/catch.hpp>
#include <cmath>
#include <common.hpp>
#include <cstdlib>
#include <enum_definitions.hpp>
#include <map>
#include <string>
#include <variant>
#include <vector>

CASSIAN_CATCH_REGISTER_INTEGER_FUNCTIONS;

enum class OutputType { gentype, ugentype };

template <typename TEST_TYPE, typename A, typename B> class OclcFunction {
private:
  Function function;
  std::map<int, A> input_map_1;
  std::map<int, B> input_map_2;
  OutputType output_type = OutputType::gentype;
  int version = 0;
  size_t get_arg_num() const { return input_map_1.size() + input_map_2.size(); }

public:
  using SGENTYPE = typename TEST_TYPE::scalar_type::host_type;
  OclcFunction(const Function &function, std::vector<A> primary_type_vector,
               std::vector<B> secondary_type_vector = std::vector<B>())
      : function(function) {
    int index = 0;
    for (auto i = 0; i < primary_type_vector.size(); i++) {
      input_map_1[index++] = primary_type_vector[i];
    }
    for (auto i = 0; i < secondary_type_vector.size(); i++) {
      input_map_2[index++] = secondary_type_vector[i];
    }
    if (function == Function::abs || function == Function::abs_diff) {
      output_type = OutputType::ugentype;
    }
  }
  OclcFunction(const int &version, const Function &function,
               std::vector<A> primary_type_vector,
               std::vector<B> secondary_type_vector = std::vector<B>())
      : OclcFunction(function, primary_type_vector, secondary_type_vector) {
    this->version = version;
  }
  auto get_input_map_1() const { return input_map_1; }
  auto get_input_map_2() const { return input_map_2; }
  auto get_version() const { return version; }
  auto get_output_type() const { return output_type; }
  auto get_function() const { return function; }
  template <typename T = A, cassian::EnableIfIsVector<T> = 0>
  auto get_input_1_value(const int &index) const {
    return input_map_1.at(index)[0];
  }
  template <typename T = A, cassian::EnableIfIsScalar<T> = 0>
  auto get_input_1_value(const int &index) const {
    return input_map_1.at(index);
  }
  template <typename T = B, cassian::EnableIfIsVector<T> = 0>
  auto get_input_2_value(const int &index) const {
    return input_map_2.at(index)[0];
  }
  template <typename T = B, cassian::EnableIfIsScalar<T> = 0>
  auto get_input_2_value(const int &index) const {
    return input_map_2.at(index);
  }

  std::string input_to_string() const {
    std::stringstream ss;
    ss << " {";
    for (const auto &input : input_map_1) {
      ss << cassian::to_string<A>(input.second) << ", ";
    }
    for (const auto &input : input_map_2) {
      ss << cassian::to_string<B>(input.second) << ", ";
    }
    ss.seekp(-2, std::ios_base::end);
    ss << "}";
    return ss.str();
  }
  virtual std::string
  get_build_options(const std::string &function_string) const {
    std::stringstream ss;
    ss << "-DOUTPUT_TYPE="
       << (output_type == OutputType::gentype
               ? TEST_TYPE::device_type
               : TEST_TYPE::unsigned_type::device_type);
    const auto arg_num = get_arg_num();
    if (arg_num == 2) {
      ss << " -DTWO_ARGS";
    } else if (arg_num == 3) {
      ss << " -DTHREE_ARGS";
    }
    ss << " -DFUNCTION=" << function_string;
    return ss.str();
  }
};

template <typename TEST_TYPE, typename GENTYPE, typename SGENTYPE>
class OclcGentypeFunction : public OclcFunction<TEST_TYPE, GENTYPE, SGENTYPE> {
private:
  using OclcFunc = OclcFunction<TEST_TYPE, GENTYPE, SGENTYPE>;
  auto get_gentype_value(const size_t &size) const {
    return OclcFunc::get_input_1_value(size);
  };
  auto get_gentype_map() const { return OclcFunc::get_input_map_1(); }
  auto get_sgentype_map() const { return OclcFunc::get_input_map_2(); }
  auto get_sgentype_value(const size_t &size) const {
    return OclcFunc::get_input_2_value(size);
  };

public:
  using OclcFunc::OclcFunction;
  std::string get_build_options(const std::string &function_string) const {
    std::stringstream ss;
    ss << OclcFunc::get_build_options(function_string);
    for (const auto &input : get_gentype_map()) {
      ss << " -DINPUT_TYPE_" << std::to_string(input.first + 1) << "="
         << TEST_TYPE::device_type;
    }
    for (const auto &input : get_sgentype_map()) {
      ss << " -DINPUT_TYPE_" << std::to_string(input.first + 1) << "="
         << TEST_TYPE::scalar_type::device_type;
    }
    return ss.str();
  }

  SGENTYPE get_reference_value() const {
    switch (OclcFunc::get_function()) {
    case Function::abs:
      if constexpr (std::is_unsigned<SGENTYPE>::value) {
        return get_gentype_value(0);
      } else {
        return std::abs(get_gentype_value(0));
      }
    case Function::abs_diff:
      if constexpr (std::is_unsigned<SGENTYPE>::value) {
        return get_gentype_value(0) - get_gentype_value(1);
      } else {
        return std::abs(get_gentype_value(0) - get_gentype_value(1));
      }
    case Function::clamp:
      if (OclcFunc::get_version() == 1) {
        return std::clamp(get_gentype_value(0), get_gentype_value(1),
                          get_gentype_value(2));
      } else {
        return std::clamp(get_gentype_value(0), get_sgentype_value(1),
                          get_sgentype_value(2));
      }
    case Function::add_sat: {
      const auto x = get_gentype_value(0);
      const auto y = get_gentype_value(1);
      const auto result = x + y;
      if constexpr (std::is_unsigned<SGENTYPE>::value) {
        if (result < x || result < y) {
          return std::numeric_limits<SGENTYPE>::max();
        }
        return result;
      } else {
        if (y > 0) {
          if (result < x) {
            return std::numeric_limits<SGENTYPE>::max();
          }
        } else {
          if (result > x) {
            return std::numeric_limits<SGENTYPE>::min();
          }
        }
        return result;
      }
    }
    case Function::hadd: {
      const auto x = get_gentype_value(0);
      const auto y = get_gentype_value(1);
      return (x >> SGENTYPE(1)) + (y >> SGENTYPE(1)) + (x & y & SGENTYPE(1));
    }
    case Function::rhadd: {
      const auto x = get_gentype_value(0);
      const auto y = get_gentype_value(1);
      return (x >> SGENTYPE(1)) + (y >> SGENTYPE(1)) + ((x | y) & SGENTYPE(1));
    }
    case Function::clz: {
      SGENTYPE count = 0;
      const auto x = get_gentype_value(0);
      constexpr auto size = sizeof(SGENTYPE) * 8;
      const std::bitset<size> bits(x);
      for (;; count++) {
        if (!bits.test(size - count - 1)) {
          continue;
        }
        break;
      }
      return count;
    }
    default:
      throw UnknownFunctionException(
          "get_reference_value for Function: " +
          Catch::StringMaker<Function>::convert(OclcFunc::get_function()) +
          " is not implemented");
    }
  }
};

template <typename TEST_TYPE, typename REGRESSED_TYPE, typename UREGRESSED_TYPE>
class SpecialRegressedFunction
    : public OclcFunction<TEST_TYPE, REGRESSED_TYPE, UREGRESSED_TYPE> {
  using OclcFunc = OclcFunction<TEST_TYPE, REGRESSED_TYPE, UREGRESSED_TYPE>;
  auto get_regressed_value(const int &index) const {
    return OclcFunc::get_input_1_value(index);
  };
  auto get_uregressed_value(const int &index) const {
    return OclcFunc::get_input_2_value(index);
  };
  auto get_regressed_map() const { return OclcFunc::get_input_map_1(); }
  auto get_uregressed_map() const { return OclcFunc::get_input_map_2(); }

public:
  using GENTYPE = typename TEST_TYPE::host_type;
  using SGENTYPE = typename TEST_TYPE::scalar_type::host_type;
  using OclcFunc::OclcFunction;
  std::string get_build_options(const std::string &function_string) const {
    std::stringstream ss;
    ss << OclcFunc::get_build_options(function_string);
    for (const auto &input : get_regressed_map()) {
      ss << " -DINPUT_TYPE_" << std::to_string(input.first + 1) << "="
         << TEST_TYPE::regressed_type::device_type;
    }
    for (const auto &input : get_uregressed_map()) {
      ss << " -DINPUT_TYPE_" << std::to_string(input.first + 1) << "="
         << TEST_TYPE::regressed_type::unsigned_type::device_type;
    }
    return ss.str();
  }

  SGENTYPE get_reference_value() const {
    if constexpr (std::is_unsigned<SGENTYPE>::value) {
      const auto a = get_uregressed_value(0);
      return (static_cast<SGENTYPE>(a) << (sizeof(a) * 8)) |
             get_uregressed_value(1);
    } else {
      const auto a = get_regressed_value(0);
      return (static_cast<SGENTYPE>(a) << (sizeof(a) * 8)) |
             get_uregressed_value(1);
    }
  }
};

#endif