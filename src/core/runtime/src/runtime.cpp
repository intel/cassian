/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cstddef>
#include <cstdint>
#include <string>

#include <cassian/fp_types/bfloat16.hpp>
#include <cassian/fp_types/half.hpp>
#include <cassian/runtime/runtime.hpp>

namespace cassian {

Buffer::Buffer(int device, std::uintptr_t id, size_t size)
    : device(device), id(id), size(size) {}

Buffer::Buffer(std::uintptr_t id, size_t size) : id(id), size(size) {}

Image::Image(std::uintptr_t id, ImageDimensions dim) : id(id), dim(dim) {}

Kernel::Kernel(std::uintptr_t id) : id(id) {}

template <>
std::vector<Bfloat16> Runtime::read_buffer_to_vector(const Buffer &buffer) {
  const size_t elements = buffer.size / sizeof(uint16_t);
  std::vector<uint16_t> output_raw(elements);
  read_buffer(buffer, output_raw.data());
  std::vector<Bfloat16> output;
  output.reserve(elements);
  std::transform(std::begin(output_raw), std::end(output_raw),
                 std::back_inserter(output), Bfloat16::encode);
  return output;
}

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
std::vector<Tfloat> Runtime::read_buffer_to_vector(const Buffer &buffer) {
  const size_t elements = buffer.size / sizeof(uint32_t);
  std::vector<uint32_t> output_raw(elements);
  read_buffer(buffer, output_raw.data());
  std::vector<Tfloat> output;
  output.reserve(elements);
  std::transform(std::begin(output_raw), std::end(output_raw),
                 std::back_inserter(output), Tfloat::encode);
  return output;
}

template <>
void Runtime::write_buffer_from_vector(const Buffer &buffer,
                                       const std::vector<Bfloat16> &data) {
  std::vector<uint16_t> raw_data;
  raw_data.reserve(data.size());
  std::transform(std::begin(data), std::end(data), std::back_inserter(raw_data),
                 [](auto v) { return v.decode(); });
  write_buffer(buffer, raw_data.data());
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

template <>
void Runtime::write_buffer_from_vector(const Buffer &buffer,
                                       const std::vector<Tfloat> &data) {
  std::vector<uint32_t> raw_data;
  raw_data.reserve(data.size());
  std::transform(std::begin(data), std::end(data), std::back_inserter(raw_data),
                 [](auto v) { return v.decode(); });
  write_buffer(buffer, raw_data.data());
}

Sampler::Sampler(std::uintptr_t id) : id(id) {}

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
template <> std::string to_cm_string<Bfloat16>() { return "bfloat16"; }
template <> std::string to_cm_string<Tfloat>() { return "float"; }

} // namespace cassian
