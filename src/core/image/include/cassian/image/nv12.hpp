/*
 * Copyright (C) 2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_IMAGE_NV12_HPP
#define CASSIAN_IMAGE_NV12_HPP

#include <stdexcept>
#include <vector>

#include "image.hpp"
#include "pixel/simple.hpp"

/**
 * Cassian namespace.
 */
namespace cassian {

struct Nv12Image {
  static constexpr auto type = ImageType::t_2d;
  static constexpr auto format = ImageFormat::unorm_int8;
  static constexpr auto order = ImageChannelOrder::nv12;

  using y_plane_type = uint8_t;
  using uv_plane_type = uint16_t;

  template <ImagePlane Plane>
  using plane_type =
      std::conditional_t<Plane == ImagePlane::y, y_plane_type, uv_plane_type>;

  Nv12Image(ImageDimensions dimensions)
      : y_plane(dimensions),
        uv_plane({dimensions.width / 2, dimensions.height / 2}) {
    if (dimensions.width % 2 != 0 || dimensions.height % 2 != 0) {
      throw std::logic_error(
          "NV12 image requires even width and height values");
    }
  }

  const auto &dimensions() const noexcept { return y_plane.dimensions(); }

  HostImage<uint8_t, type> y_plane;
  HostImage<uint16_t, type> uv_plane;
};

} // namespace cassian

#endif // CASSIAN_IMAGE_NV12_HPP
