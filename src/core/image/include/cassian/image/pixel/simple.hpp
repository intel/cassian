/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_IMAGE_PIXEL_SIMPLE_HPP
#define CASSIAN_IMAGE_PIXEL_SIMPLE_HPP

#include <cassian/fp_types/half.hpp>

#include "common.hpp"

namespace cassian {
namespace pixel {

template <> struct PixelTraits<int8_t> {
  using storage_type = int8_t;
  static constexpr auto image_format = ImageFormat::signed_int8;
  static constexpr auto channel_order = ImageChannelOrder::r;
  static constexpr int channel_number = 1;
};

template <> struct PixelTraits<int16_t> {
  using storage_type = int16_t;
  static constexpr auto image_format = ImageFormat::signed_int16;
  static constexpr auto channel_order = ImageChannelOrder::r;
  static constexpr int channel_number = 1;
};

template <> struct PixelTraits<int32_t> {
  using storage_type = int32_t;
  static constexpr auto image_format = ImageFormat::signed_int32;
  static constexpr auto channel_order = ImageChannelOrder::r;
  static constexpr int channel_number = 1;
};

template <> struct PixelTraits<uint8_t> {
  using storage_type = uint8_t;
  static constexpr auto image_format = ImageFormat::unsigned_int8;
  static constexpr auto channel_order = ImageChannelOrder::r;
  static constexpr int channel_number = 1;
};

template <> struct PixelTraits<uint16_t> {
  using storage_type = uint16_t;
  static constexpr auto image_format = ImageFormat::unsigned_int16;
  static constexpr auto channel_order = ImageChannelOrder::r;
  static constexpr int channel_number = 1;
};

template <> struct PixelTraits<uint32_t> {
  using storage_type = uint32_t;
  static constexpr auto image_format = ImageFormat::unsigned_int32;
  static constexpr auto channel_order = ImageChannelOrder::r;
  static constexpr int channel_number = 1;
};

template <> struct PixelTraits<cassian::half> {
  using storage_type = cassian::half;
  static constexpr auto image_format = ImageFormat::float16;
  static constexpr auto channel_order = ImageChannelOrder::r;
  static constexpr int channel_number = 1;
};

template <> struct PixelTraits<float> {
  using storage_type = float;
  static constexpr auto image_format = ImageFormat::float32;
  static constexpr auto channel_order = ImageChannelOrder::r;
  static constexpr int channel_number = 1;
};
} // namespace pixel
} // namespace cassian
#endif
