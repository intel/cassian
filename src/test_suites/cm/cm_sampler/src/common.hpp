/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/image/pixel/normalized.hpp>
#include <cassian/image/pixel/unnormalized.hpp>

using rgba_float =
    cassian::pixel::Unnormalized<float, cassian::ImageChannelOrder::rgba>;
using rgba_snorm8 =
    cassian::pixel::Normalized<int8_t, cassian::ImageChannelOrder::rgba>;
using rgba_snorm16 =
    cassian::pixel::Normalized<int16_t, cassian::ImageChannelOrder::rgba>;
using rgba_unorm8 =
    cassian::pixel::Normalized<uint8_t, cassian::ImageChannelOrder::rgba>;
using rgba_unorm16 =
    cassian::pixel::Normalized<uint16_t, cassian::ImageChannelOrder::rgba>;

using rgba_sint8 =
    cassian::pixel::Unnormalized<int8_t, cassian::ImageChannelOrder::rgba>;
using rgba_sint16 =
    cassian::pixel::Unnormalized<int16_t, cassian::ImageChannelOrder::rgba>;
using rgba_sint32 =
    cassian::pixel::Unnormalized<int32_t, cassian::ImageChannelOrder::rgba>;
using rgba_uint8 =
    cassian::pixel::Unnormalized<uint8_t, cassian::ImageChannelOrder::rgba>;
using rgba_uint16 =
    cassian::pixel::Unnormalized<uint16_t, cassian::ImageChannelOrder::rgba>;
using rgba_uint32 =
    cassian::pixel::Unnormalized<uint32_t, cassian::ImageChannelOrder::rgba>;

enum ChannelMask {
  none = 0,
  r = 1,
  g = 2,
  b = 4,
  a = 8,
  gr = r | g,
  br = r | b,
  ar = r | a,
  bg = g | b,
  ag = g | a,
  ab = b | a,
  bgr = r | g | b,
  agr = r | g | a,
  abr = r | b | a,
  abgr = r | g | b | a,
};

std::string to_cm_string(ChannelMask cmask);

extern const ChannelMask channel_masks[14];
