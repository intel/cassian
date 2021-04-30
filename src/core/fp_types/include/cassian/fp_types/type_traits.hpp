/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_FP_TYPES_TYPE_TRAITS_HPP
#define CASSIAN_FP_TYPES_TYPE_TRAITS_HPP

#include <type_traits>

namespace cassian {

/**
 * Checks whether T is a floating-point type
 */
template <typename T> struct is_floating_point : std::is_floating_point<T> {};

/**
 * Helper variable template for cassian::is_floating_point
 */
template <typename T>
inline constexpr bool is_floating_point_v = is_floating_point<T>::value;

} // namespace cassian

#endif // CASSIAN_FP_TYPES_TYPE_TRAITS_HPP
