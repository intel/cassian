/*
 * Copyright (C) 2026 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_REFERENCE_EXTENDED_BIT_OPERATIONS_HPP
#define CASSIAN_REFERENCE_EXTENDED_BIT_OPERATIONS_HPP

#include <cstdint>
#include <exception>
#include <string>
#include <vector>

/**
 * Cassian namespace.
 */
namespace cassian {

/**
 * Set value of bit in number
 *
 * @param[in] input_number number to be modified.
 * @param[in] offset  offset of bit to be set.
 * @param[in] bit value of bit to be set.
 * @returns modified input_number.
 */
uint64_t set_bit(uint64_t input_number, size_t offset, size_t bit);

/**
 * bitfield_extract_signed - returns extracted bit field from base, with sign
 * extension, that is: Bits of base numbered in [offset, offset + count - 1]
 * (inclusive), will be bits numbered in [0, count - 1] (inclusive) of result,
 * remaining bits will have value of bit numbered offset + count - 1 of base.
 * count is the number of bits to be extracted. If count equals 0, the result is
 * 0. If the count or offset or offset + count is greater than number of bits in
 * TYPE, the result is undefined. TYPE cannot be a floating point type.
 *
 * @tparam TYPE the type of base.
 * @param[in] base input value.
 * @param[in] base_offset offset of bits to be extracted.
 * @param[in] count  number of bits to be extracted.
 * @returns extracted bits.
 */
template <typename TYPE>
std::make_signed_t<TYPE>
bitfield_extract_signed(TYPE base, uint32_t base_offset, uint32_t count) {
  static_assert(!std::is_floating_point_v<TYPE>,
                "TYPE cannot be a floating point type.");
  auto binary_base = *reinterpret_cast<std::make_unsigned_t<TYPE> *>(&base);
  auto tmp_insert = static_cast<uint64_t>(base);
  auto tmp_base = static_cast<uint64_t>(base);
  const int data_size_in_bits = sizeof(TYPE) * 8;
  if (count == 0) {
    return 0;
  }
  for (size_t next_base_offset = base_offset;
       next_base_offset < base_offset + count; next_base_offset++) {
    size_t next_insert_offset = next_base_offset - base_offset;
    size_t take_bit_value =
        tmp_insert & (1LL << next_base_offset); // take from base from offset
    tmp_base =
        set_bit(tmp_base, next_insert_offset, take_bit_value > 0 ? 1 : 0);
  }
  size_t take_bit_value = tmp_insert & (1LL << (base_offset + count - 1));
  for (size_t next_base_offset = count; next_base_offset < data_size_in_bits;
       next_base_offset++) {
    tmp_base = set_bit(tmp_base, next_base_offset, take_bit_value > 0 ? 1 : 0);
  }
  binary_base = static_cast<std::make_unsigned_t<TYPE>>(tmp_base);
  return *reinterpret_cast<std::make_signed_t<TYPE> *>(&binary_base);
}
/**
 * bitfield_extract_unsigned - returns extracted bit field from base, without
 * sign extension, that is: Bits of base numbered in [offset, offset + count -
 * 1] (inclusive), will be bits numbered in [0, count - 1] (inclusive) of
 * result, remaining bits will be equal to 0. count is the number of bits to be
 * extracted. If count equals 0, the result is 0. If the count or offset or
 * offset + count is greater than number of bits in TYPE, the result is
 * undefined. TYPE cannot be a floating point type.
 *
 * @tparam TYPE the type of base.
 * @param[in] base input value.
 * @param[in] base_offset offset of bits to be extracted.
 * @param[in] count  number of bits to be extracted.
 * @returns extracted bits.
 */
template <typename TYPE>
std::make_unsigned_t<TYPE>
bitfield_extract_unsigned(TYPE base, uint32_t base_offset, uint32_t count) {
  static_assert(!std::is_floating_point_v<TYPE>,
                "TYPE cannot be a floating point type.");
  auto binary_base = *reinterpret_cast<std::make_unsigned_t<TYPE> *>(&base);
  uint64_t tmp_insert = static_cast<uint64_t>(binary_base);
  uint64_t tmp_base = static_cast<uint64_t>(binary_base);
  const int data_size_in_bits = sizeof(TYPE) * 8;
  if (count == 0) {
    return 0;
  }
  for (size_t next_base_offset = base_offset;
       next_base_offset < base_offset + count; next_base_offset++) {
    size_t next_insert_offset = next_base_offset - base_offset;
    size_t take_bit_value = tmp_insert & (1ULL << next_base_offset);
    tmp_base =
        set_bit(tmp_base, next_insert_offset, take_bit_value > 0 ? 1 : 0);
  }
  for (size_t next_base_offset = count; next_base_offset < data_size_in_bits;
       next_base_offset++) {
    tmp_base = set_bit(tmp_base, next_base_offset, 0);
  }
  return static_cast<std::make_unsigned_t<TYPE>>(tmp_base);
}
/**
 * bit_reverse - returns a base with reversed bits, that is:
 * Bit numbered n of result will have value of bit numbered width - n - 1 of
 * base, where width is number of bits of TYPE. TYPE cannot be a floating point
 * type.
 *
 * @tparam INPUT_TYPE the type of base.
 * @param[in] base input value.
 * @returns base with reversed bits.
 */
template <typename TYPE> TYPE bit_reverse(TYPE base) {
  static_assert(!std::is_floating_point_v<TYPE>,
                "TYPE cannot be a floating point type.");
  auto binary_base = *reinterpret_cast<std::make_unsigned_t<TYPE> *>(&base);
  auto tmp_insert = static_cast<uint64_t>(binary_base);
  auto tmp_base = static_cast<uint64_t>(binary_base);
  size_t count = sizeof(TYPE) * 8;
  for (size_t next_base_offset = 0; next_base_offset < count;
       next_base_offset++) {
    size_t next_insert_offset = count - next_base_offset - 1;
    size_t take_bit_value = tmp_insert & (1ULL << next_insert_offset);
    tmp_base = set_bit(tmp_base, next_base_offset, take_bit_value > 0 ? 1 : 0);
  }
  binary_base = static_cast<std::make_unsigned_t<TYPE>>(tmp_base);
  return *reinterpret_cast<TYPE *>(&binary_base);
}
/**
 * bitfield_insert - returns copy of base, with a modified bit field that comes
 * from insert, that is: Bits of result numbered outside [offset, offset + count
 * - 1] (inclusive), will come from the corresponding bits in base Bits of
 * result numbered in [offset, offset + count - 1] (inclusive) will come from
 * [0, count - 1] (inclusive) bits in insert count is the number of bits to be
 * modified. If count equals 0, the result will be equal to base. If count or
 * offset or offset + count is greater than number of bits in TYPE, the result
 * is undefined. TYPE cannot be a floating point type.
 *
 * @tparam TYPE the type of base.
 * @param[in] base input value.
 * @param[in] insert input bits to be inserted.
 * @param[in] base_offset offset where insert will be inserted.
 * @param[in] count  number of bits to be inserted.
 * @returns extracted bits.
 */
template <typename TYPE>
TYPE bitfield_insert(TYPE base, TYPE insert, uint32_t base_offset,
                     uint32_t count) {
  static_assert(!std::is_floating_point_v<TYPE>,
                "TYPE cannot be a floating point type.");
  auto binary_base = *reinterpret_cast<std::make_unsigned_t<TYPE> *>(&base);
  auto binary_insert = *reinterpret_cast<std::make_unsigned_t<TYPE> *>(&insert);
  auto tmp_insert = static_cast<uint64_t>(binary_insert);
  auto tmp_base = static_cast<uint64_t>(binary_base);
  for (size_t next_base_offset = base_offset;
       next_base_offset < base_offset + count; next_base_offset++) {
    size_t next_insert_offset = next_base_offset - base_offset;
    size_t take_bit_value = tmp_insert & (1ULL << next_insert_offset);
    tmp_base = set_bit(tmp_base, next_base_offset, take_bit_value > 0 ? 1 : 0);
  }
  binary_base = static_cast<std::make_unsigned_t<TYPE>>(tmp_base);
  return *reinterpret_cast<TYPE *>(&binary_base);
}

} // namespace cassian

#endif
