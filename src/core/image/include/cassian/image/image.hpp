/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_IMAGE_IMAGE_HPP
#define CASSIAN_IMAGE_IMAGE_HPP

#include <stdexcept>
#include <vector>

#include <cassian/random/random.hpp>
#include <cassian/runtime/image_properties.hpp>

/**
 * Cassian namespace.
 */
namespace cassian {

template <typename Pixel, ImageType Img = ImageType::t_2d> class HostImage {
public:
  using pixel_type = Pixel;
  using reference = pixel_type &;
  using const_reference = const pixel_type &;
  using iterator = typename std::vector<Pixel>::iterator;
  using const_iterator = typename std::vector<Pixel>::const_iterator;
  using reverse_iterator = typename std::vector<Pixel>::reverse_iterator;
  using const_reverse_iterator =
      typename std::vector<Pixel>::const_reverse_iterator;

  static constexpr auto image_type = Img;

private:
  static constexpr bool is_1d = image_type == ImageType::t_1d;
  static constexpr bool is_2d =
      image_type == ImageType::t_1d_array || image_type == ImageType::t_2d;
  static constexpr bool is_3d =
      image_type == ImageType::t_2d_array || image_type == ImageType::t_3d;

public:
  class OutOfBounds : public std::out_of_range {
  public:
    OutOfBounds() : std::out_of_range("Out of bounds image access") {}
  };

  HostImage(ImageDimensions dimensions)
      : dimensions_(dimensions),
        data_(dimensions_.width * dimensions_.height * dimensions_.depth) {
    static_assert(is_1d || is_2d || is_3d);
  }

  const ImageDimensions &dimensions() const noexcept { return dimensions_; }

  iterator begin() noexcept { return data_.begin(); }
  const_iterator cbegin() const noexcept { return data_.cbegin(); }
  reverse_iterator rbegin() noexcept { return data_.rbegin(); }
  const_reverse_iterator crbegin() const noexcept { return data_.crbegin(); }

  iterator end() noexcept { return data_.end(); }
  const_iterator cend() const noexcept { return data_.cend(); }
  reverse_iterator rend() noexcept { return data_.rend(); }
  const_reverse_iterator crend() const noexcept { return data_.crend(); }

  const Pixel *data() const noexcept { return data_.data(); }

  template <bool cond = is_1d>
  std::enable_if_t<cond, reference> operator()(uint32_t x) {
    if (x >= dimensions_.width) {
      throw OutOfBounds();
    }
    return data_[x];
  }

  template <bool cond = is_1d>
  std::enable_if_t<cond, const_reference> operator()(uint32_t x) const {
    if (x >= dimensions_.width) {
      throw OutOfBounds();
    }
    return data_[x];
  }

  template <bool cond = is_2d>
  std::enable_if_t<cond, reference> operator()(uint32_t x, uint32_t y) {
    if (x >= dimensions_.width || y >= dimensions_.height) {
      throw OutOfBounds();
    }
    return data_[y * dimensions_.width + x];
  }

  template <bool cond = is_2d>
  std::enable_if_t<cond, const_reference> operator()(uint32_t x,
                                                     uint32_t y) const {
    if (x >= dimensions_.width || y >= dimensions_.height) {
      throw OutOfBounds();
    }
    return data_[y * dimensions_.width + x];
  }

  template <bool cond = is_3d>
  std::enable_if_t<cond, reference> operator()(uint32_t x, uint32_t y,
                                               uint32_t z) {
    if (x >= dimensions_.width || y >= dimensions_.height ||
        z >= dimensions_.depth) {
      throw OutOfBounds();
    }
    auto index =
        z * dimensions_.height * dimensions_.width + y * dimensions_.width + x;

    return data_[index];
  }

  template <bool cond = is_3d>
  std::enable_if_t<cond, const_reference> operator()(uint32_t x, uint32_t y,
                                                     uint32_t z) const {
    if (x >= dimensions_.width || y >= dimensions_.height ||
        z >= dimensions_.depth) {
      throw OutOfBounds();
    }
    auto index =
        z * dimensions_.height * dimensions_.width + y * dimensions_.width + x;

    return data_[index];
  }

private:
  const ImageDimensions dimensions_;
  std::vector<pixel_type> data_;
};

} // namespace cassian
#endif
