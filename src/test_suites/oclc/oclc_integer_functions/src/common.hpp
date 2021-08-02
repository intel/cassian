/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_OCLC_INTEGER_FUNCTIONS_COMMON_HPP
#define CASSIAN_OCLC_INTEGER_FUNCTIONS_COMMON_HPP

#include <cassian/runtime/openclc_types.hpp>
#include <cassian/vector/vector.hpp>
#include <stdexcept>
#include <string>
#include <type_traits>

int suggest_work_size(const std::string &type);

class UnknownFunctionException : public std::runtime_error {
  using std::runtime_error::runtime_error;
};

class UnknownTypeException : public std::runtime_error {
  using std::runtime_error::runtime_error;
};

template <typename TestType> auto test_name() {
  return std::string(TestType::type_name);
}

#endif
