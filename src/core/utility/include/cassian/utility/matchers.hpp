/*
 * Copyright (C) 2025 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_UTILITY_MATCHERS_HPP
#define CASSIAN_UTILITY_MATCHERS_HPP

#include <catch2/catch.hpp>
#include <sstream>
#include <string>

namespace cassian {

template <typename T> bool nan_sensitive_eq(const T lhs, const T rhs) {
  return lhs.nan_sensitive_eq(rhs);
}
template<> bool nan_sensitive_eq<float>(const float lhs, const float rhs);
template<> bool nan_sensitive_eq<double>(const double lhs, const double rhs);

template <typename T>
class NanSensitiveEqMatcher : public Catch::MatcherBase<T> {
  T value;

public:
  explicit NanSensitiveEqMatcher(const T v) : value(v) {}

  bool match(const T &v) const override { return nan_sensitive_eq(v, value); }

  std::string describe() const override {
    std::ostringstream ss;
    ss << "is NaN sensitively equal to " << value;
    return ss.str();
  }
};

template <typename T>
inline NanSensitiveEqMatcher<T> nan_sensitive_equal(const T value) {
  return NanSensitiveEqMatcher<T>(value);
}

}

#endif