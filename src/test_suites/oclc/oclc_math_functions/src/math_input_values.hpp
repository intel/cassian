/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_OCLC_MATH_FUNCTIONS_MATH_INPUT_VALUES_HPP
#define CASSIAN_OCLC_MATH_FUNCTIONS_MATH_INPUT_VALUES_HPP

#include <bits/stdint-uintn.h>
#include <cassian/vector/vector.hpp>
#include <map>
#include <vector>

template <class T> struct Value {
  typename T::input_type_1 val_1;
  typename T::input_type_2 val_2;
  typename T::input_type_3 val_3;

public:
  Value() = default;
  auto get_val_1() const { return val_1; }
  auto get_val_2() const { return val_2; }
  auto get_val_3() const { return val_3; }
};
template <class T> struct MathInputValues {
  std::vector<Value<T>> edge_case_values = {};
  std::vector<Value<T>> denorm_values = {};
  std::vector<Value<T>> random_values = {};

public:
  MathInputValues() = default;
  template <typename INPUT> void add_random_case(const INPUT &a) {
    Value<T> val = {};
    val.val_1 = a;
    random_values.push_back(val);
  }
  template <typename INPUT_1, typename INPUT_2>
  void add_random_case(const INPUT_1 &a, const INPUT_2 &b) {
    Value<T> val = {};
    val.val_1 = a;
    val.val_2 = b;
    random_values.push_back(val);
  }
  template <typename INPUT_1, typename INPUT_2, typename INPUT_3>
  void add_random_case(const INPUT_1 &a, const INPUT_2 &b, const INPUT_3 &c) {
    Value<T> val = {};
    val.val_1 = a;
    val.val_2 = b;
    val.val_3 = b;
    random_values.push_back(val);
  }

  template <typename INPUT> void add_edge_case(const INPUT &a) {
    Value<T> val = {};
    val.val_1 = a;
    edge_case_values.push_back(val);
  }

  template <typename INPUT_1, typename INPUT_2>
  void add_edge_case(const INPUT_1 &a, const INPUT_2 &b) {
    Value<T> val = {};
    val.val_1 = a;
    val.val_2 = b;
    edge_case_values.push_back(val);
  }
};

#endif