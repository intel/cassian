/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/fp_types/half.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cstddef>
#include <cstdint>
#include <string>

namespace cassian {

Buffer::Buffer(std::uintptr_t id, size_t size) : id(id), size(size) {}

Image::Image(std::uintptr_t id, ImageDimensions dim) : id(id), dim(dim) {}

Kernel::Kernel(std::uintptr_t id) : id(id) {}

template <>
std::vector<Half> Runtime::read_buffer_to_vector(const Buffer &buffer) {
  const size_t elements = buffer.size / sizeof(uint16_t);
  std::vector<uint16_t> output_raw(elements);
  read_buffer(buffer, output_raw.data());
  std::vector<Half> output;
  output.reserve(elements);
  std::transform(std::begin(output_raw), std::end(output_raw),
                 std::back_inserter(output), Half::encode);
  return output;
}

template <>
void Runtime::write_buffer_from_vector(const Buffer &buffer,
                                       const std::vector<Half> &data) {
  std::vector<uint16_t> raw_data;
  raw_data.reserve(data.size());
  std::transform(std::begin(data), std::end(data), std::back_inserter(raw_data),
                 [](auto v) { return v.decode(); });
  write_buffer(buffer, raw_data.data());
}

Sampler::Sampler(std::uintptr_t id) : id(id) {}

// If you add new type to to_clc_function make sure to do following:
// * Add specialization to cpp file
// * If you added custom type (is_scalar returns false) mark it with:
//   using is_custom_type = int;
template <> std::string to_clc_string<int8_t>() { return "char"; }
template <> std::string to_clc_string<int16_t>() { return "short"; }
template <> std::string to_clc_string<int32_t>() { return "int"; }
template <> std::string to_clc_string<int64_t>() { return "long"; }
template <> std::string to_clc_string<uint8_t>() { return "uchar"; }
template <> std::string to_clc_string<uint16_t>() { return "ushort"; }
template <> std::string to_clc_string<uint32_t>() { return "uint"; }
template <> std::string to_clc_string<uint64_t>() { return "ulong"; }
template <> std::string to_clc_string<float>() { return "float"; }
template <> std::string to_clc_string<double>() { return "double"; }
template <> std::string to_clc_string<half>() { return "half"; }

template <> std::string to_cm_string<int8_t>() { return "char"; }
template <> std::string to_cm_string<int16_t>() { return "short"; }
template <> std::string to_cm_string<int32_t>() { return "int"; }
template <> std::string to_cm_string<int64_t>() { return "long long"; }
template <> std::string to_cm_string<uint8_t>() { return "unsigned char"; }
template <> std::string to_cm_string<uint16_t>() { return "unsigned short"; }
template <> std::string to_cm_string<uint32_t>() { return "unsigned int"; }
template <> std::string to_cm_string<uint64_t>() {
  return "unsigned long long";
}
template <> std::string to_cm_string<float>() { return "float"; }
template <> std::string to_cm_string<double>() { return "double"; }
template <> std::string to_cm_string<half>() { return "half"; }

} // namespace cassian
