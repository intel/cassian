/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_IMAGE_PIXEL_COMMON_HPP
#define CASSIAN_IMAGE_PIXEL_COMMON_HPP

#include <cassian/runtime/image_properties.hpp>

/**
 * Cassian namespace.
 */
namespace cassian {
namespace pixel {

constexpr int get_channel_number(ImageChannelOrder order) {
  switch (order) {
  case ImageChannelOrder::r:
  case ImageChannelOrder::a:
    return 1;
  case ImageChannelOrder::rg:
  case ImageChannelOrder::ra:
    return 2;
  case ImageChannelOrder::rgb:
    return 3;
  case ImageChannelOrder::rgba:
  case ImageChannelOrder::bgra:
  case ImageChannelOrder::argb:
  case ImageChannelOrder::abgr:
    return 4;
  }
}

constexpr int get_red_index(ImageChannelOrder order) {
  switch (order) {
  case ImageChannelOrder::r:
  case ImageChannelOrder::rg:
  case ImageChannelOrder::rgb:
  case ImageChannelOrder::rgba:
    return 0;
  case ImageChannelOrder::bgra:
    return 2;
  case ImageChannelOrder::argb:
    return 1;
  case ImageChannelOrder::abgr:
    return 3;
  default:
    return -1;
  }
}

constexpr int get_green_index(ImageChannelOrder order) {
  switch (order) {
  case ImageChannelOrder::rg:
  case ImageChannelOrder::rgb:
  case ImageChannelOrder::rgba:
  case ImageChannelOrder::bgra:
    return 1;
  case ImageChannelOrder::argb:
  case ImageChannelOrder::abgr:
    return 2;
  default:
    return -1;
  }
}

constexpr int get_blue_index(ImageChannelOrder order) {
  switch (order) {
  case ImageChannelOrder::rgb:
  case ImageChannelOrder::rgba:
    return 2;
  case ImageChannelOrder::bgra:
    return 0;
  case ImageChannelOrder::argb:
    return 3;
  case ImageChannelOrder::abgr:
    return 1;
  default:
    return -1;
  }
}

constexpr int get_alpha_index(ImageChannelOrder order) {
  switch (order) {
  case ImageChannelOrder::a:
  case ImageChannelOrder::argb:
  case ImageChannelOrder::abgr:
    return 0;
  case ImageChannelOrder::ra:
    return 1;
  case ImageChannelOrder::rgba:
  case ImageChannelOrder::bgra:
    return 3;
  default:
    return -1;
  }
}

template <typename T, ImageChannelOrder Order> class Storage {
public:
  using storage_type = T;
  static constexpr auto channel_order = Order;
  static constexpr int channel_number = get_channel_number(channel_order);

protected:
  static constexpr auto red_index = get_red_index(channel_order);
  static constexpr auto green_index = get_green_index(channel_order);
  static constexpr auto blue_index = get_blue_index(channel_order);
  static constexpr auto alpha_index = get_alpha_index(channel_order);

public:
  static constexpr bool has_red = red_index >= 0;
  static constexpr bool has_green = green_index >= 0;
  static constexpr bool has_blue = blue_index >= 0;
  static constexpr bool has_alpha = alpha_index >= 0;

  std::enable_if_t<has_red, storage_type> red_raw() const {
    return data_[red_index];
  }
  std::enable_if_t<has_green, storage_type> green_raw() const {
    return data_[green_index];
  }
  std::enable_if_t<has_blue, storage_type> blue_raw() const {
    return data_[blue_index];
  }
  std::enable_if_t<has_alpha, storage_type> alpha_raw() const {
    return data_[alpha_index];
  }

  std::enable_if_t<has_red, storage_type &> red_raw() {
    return data_[red_index];
  }
  std::enable_if_t<has_green, storage_type &> green_raw() {
    return data_[green_index];
  }
  std::enable_if_t<has_blue, storage_type &> blue_raw() {
    return data_[blue_index];
  }
  std::enable_if_t<has_alpha, storage_type &> alpha_raw() {
    return data_[alpha_index];
  }

private:
  storage_type data_[channel_number];
};

template <typename T> T generate_value(const int seed) {
  using storage_type = typename T::storage_type;
  T value;

  if constexpr (T::has_red) {
    value.red_raw() = cassian::generate_value<storage_type>(seed);
  }

  if constexpr (T::has_green) {
    value.green_raw() = cassian::generate_value<storage_type>(seed);
  }

  if constexpr (T::has_blue) {
    value.blue_raw() = cassian::generate_value<storage_type>(seed);
  }

  if constexpr (T::has_alpha) {
    value.alpha_raw() = cassian::generate_value<storage_type>(seed);
  }

  return value;
}
} // namespace pixel
} // namespace cassian
#endif
