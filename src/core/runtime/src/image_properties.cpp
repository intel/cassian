/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/runtime/image_properties.hpp>

namespace ca = cassian;

uint32_t ca::get_pixel_size(const ca::ImageFormat &format,
                            const ca::ImageChannelOrder &order) {
  auto channel_size = 0U;
  auto channels_num = 0U;
  switch (format) {
  case ca::ImageFormat::snorm_int8:
  case ca::ImageFormat::unorm_int8:
  case ca::ImageFormat::signed_int8:
  case ca::ImageFormat::unsigned_int8:
    channel_size = 1;
    break;
  case ca::ImageFormat::unorm_int16:
  case ca::ImageFormat::snorm_int16:
  case ca::ImageFormat::signed_int16:
  case ca::ImageFormat::unsigned_int16:
  case ca::ImageFormat::float16:
  case ca::ImageFormat::unorm_short_565:
  case ca::ImageFormat::unorm_short_555:
    channel_size = 2;
    break;
  case ca::ImageFormat::unorm_int_101010:
  case ca::ImageFormat::unorm_int_101010_2:
  case ca::ImageFormat::signed_int32:
  case ca::ImageFormat::unsigned_int32:
  case ca::ImageFormat::float32:
    channel_size = 4;
    break;
  }
  switch (order) {
  case ca::ImageChannelOrder::r:
  case ca::ImageChannelOrder::a:
    channels_num = 1;
    break;
  case ca::ImageChannelOrder::rg:
  case ca::ImageChannelOrder::ra:
    channels_num = 2;
    break;
  case ca::ImageChannelOrder::rgb:
    channels_num = 3;
    break;
  case ca::ImageChannelOrder::rgba:
  case ca::ImageChannelOrder::bgra:
  case ca::ImageChannelOrder::argb:
  case ca::ImageChannelOrder::abgr:
    channels_num = 4;
    break;
  }
  return channels_num * channel_size;
}