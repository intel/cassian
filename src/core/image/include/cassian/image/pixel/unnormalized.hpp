/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_IMAGE_PIXEL_UNNORMALIZED_HPP
#define CASSIAN_IMAGE_PIXEL_UNNORMALIZED_HPP

#include <cassian/fp_types/half.hpp>
#include <cassian/random/random.hpp>
#include <cassian/runtime/image_properties.hpp>

/**
 * Cassian namespace.
 */
namespace cassian {
namespace pixel {

template <typename T, ImageChannelOrder Order>
class Unnormalized : public Storage<T, Order> {
private:
  static constexpr auto get_image_format() {
    static_assert((std::is_integral_v<T> && sizeof(T) <= 4) ||
                  std::is_same_v<T, float> || std::is_same_v<T, half>);
    if (std::is_same_v<T, int8_t>) {
      return ImageFormat::signed_int8;
    }
    if (std::is_same_v<T, int16_t>) {
      return ImageFormat::signed_int16;
    }
    if (std::is_same_v<T, int32_t>) {
      return ImageFormat::signed_int32;
    }
    if (std::is_same_v<T, uint8_t>) {
      return ImageFormat::unsigned_int8;
    }
    if (std::is_same_v<T, uint16_t>) {
      return ImageFormat::unsigned_int16;
    }
    if (std::is_same_v<T, uint32_t>) {
      return ImageFormat::unsigned_int32;
    }
    if (std::is_same_v<T, half>) {
      return ImageFormat::float16;
    }
    if (std::is_same_v<T, float>) {
      return ImageFormat::float32;
    }
  }

  using base_t = Storage<T, Order>;

public:
  using storage_type = typename base_t::storage_type;
  using base_t::channel_number;
  using base_t::channel_order;

  using read_type = std::conditional_t<
      cassian::is_floating_point_v<storage_type>, float,
      std::conditional_t<std::is_signed_v<storage_type>, int32_t, uint32_t>>;

  static constexpr auto image_format = get_image_format();

  using base_t::has_alpha;
  using base_t::has_blue;
  using base_t::has_green;
  using base_t::has_red;

  using base_t::alpha_raw;
  using base_t::blue_raw;
  using base_t::green_raw;
  using base_t::red_raw;

  std::enable_if_t<base_t::has_red, read_type> red() const {
    return static_cast<read_type>(red_raw());
  }
  std::enable_if_t<base_t::has_green, read_type> green() const {
    return static_cast<read_type>(green_raw());
  }
  std::enable_if_t<base_t::has_blue, read_type> blue() const {
    return static_cast<read_type>(blue_raw());
  }
  std::enable_if_t<base_t::has_alpha, read_type> alpha() const {
    return static_cast<read_type>(alpha_raw());
  }
};
} // namespace pixel
} // namespace cassian
#endif
