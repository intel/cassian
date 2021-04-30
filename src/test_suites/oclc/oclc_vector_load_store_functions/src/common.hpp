/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_OCLC_VECTOR_LOAD_STORE_FUNCTIONS_COMMON_HPP
#define CASSIAN_OCLC_VECTOR_LOAD_STORE_FUNCTIONS_COMMON_HPP

#include <string>

template <typename T> std::string test_name() {
  return std::string(T::type_name);
}

enum class AddressSpace {
  clc_global,
  clc_local,
  clc_private,
  clc_constant,
  clc_generic
};

std::string to_string(AddressSpace address_space);

int suggest_work_size(const std::string &type);

#endif
