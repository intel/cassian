/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_TESTS_FP_TYPES_COMMON_HPP
#define CASSIAN_TESTS_FP_TYPES_COMMON_HPP

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <tuple>
#include <vector>

static float uint32_to_float(uint32_t v) {
  float tmp = 0;
  std::memcpy(&tmp, &v, sizeof(uint32_t));
  return tmp;
}

template <typename In, typename Type, typename Out> Out cast(const In a) {
  Type b;

  if constexpr (std::is_same_v<In, typename Type::storage_t>) {
    b = Type::encode(a);
  } else {
    b = Type(a);
  }

  if constexpr (std::is_same_v<Out, typename Type::storage_t>) {
    return b.decode();
  } else {
    return static_cast<Out>(b);
  }
}

template <typename T, typename... Args>
std::vector<std::tuple<T, Args...>> zip(const std::vector<T> t,
                                        const std::vector<Args>... args) {
  std::vector<std::tuple<T, Args...>> temp(t.size());
  for (int i = 0; i < t.size(); ++i) {
    temp[i] = std::tie(t[i], args[i]...);
  }
  return temp;
}

#endif
