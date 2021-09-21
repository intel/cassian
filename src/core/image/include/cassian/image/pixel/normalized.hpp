/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_IMAGE_PIXEL_NORMALIZED_HPP
#define CASSIAN_IMAGE_PIXEL_NORMALIZED_HPP

#include <cassian/random/random.hpp>
#include <cassian/runtime/image_properties.hpp>

#include "common.hpp"

/**
 * Cassian namespace.
 */
namespace cassian {
namespace pixel {

template <typename T, ImageChannelOrder Order>
class Normalized : public Storage<T, Order> {
private:
  static constexpr auto get_image_format() {
    static_assert(std::is_integral_v<T> && sizeof(T) <= 2);

    if (std::is_signed_v<T>) {
      if (sizeof(T) == 1) {
        return ImageFormat::snorm_int8;
      }
      return ImageFormat::snorm_int16;
    }
    if (sizeof(T) == 1) {
      return ImageFormat::unorm_int8;
    }
    return ImageFormat::unorm_int16;
  }

  using base_t = Storage<T, Order>;

public:
  using storage_type = typename base_t::storage_type;
  using base_t::channel_number;
  using base_t::channel_order;

  using read_type = float;

  static constexpr auto image_format = get_image_format();

  using base_t::has_alpha;
  using base_t::has_blue;
  using base_t::has_green;
  using base_t::has_red;

  using base_t::alpha_raw;
  using base_t::blue_raw;
  using base_t::green_raw;
  using base_t::red_raw;

private:
  static read_type normalize(storage_type value) {
    auto result = static_cast<read_type>(value) /
                  std::numeric_limits<storage_type>::max();
    return (result < -1) ? read_type(-1) : result;
  }

public:
  std::enable_if_t<base_t::has_red, read_type> red() const {
    return normalize(red_raw());
  }
  std::enable_if_t<base_t::has_green, read_type> green() const {
    return normalize(green_raw());
  }
  std::enable_if_t<base_t::has_blue, read_type> blue() const {
    return normalize(blue_raw());
  }
  std::enable_if_t<base_t::has_alpha, read_type> alpha() const {
    return normalize(alpha_raw());
  }
};
} // namespace pixel
} // namespace cassian
#endif
