/*
 * Copyright (C) 2026 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/reference/extended_bit_operations.hpp>

/**
 * Cassian namespace.
 */
namespace cassian {

uint64_t set_bit(uint64_t input_number, size_t offset, size_t bit) {
  uint64_t mask = 1ULL << offset;
  uint64_t result = input_number;
  result = (input_number & ~mask) | ((bit << offset) & mask);
  return result;
}

} // namespace cassian
