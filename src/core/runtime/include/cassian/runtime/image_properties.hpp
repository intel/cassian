/*
 * Copyright (C) 2021-2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_RUNTIME_IMAGE_PROPERTIES_HPP
#define CASSIAN_RUNTIME_IMAGE_PROPERTIES_HPP
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>

/**
 * Cassian namespace.
 */
namespace cassian {

/**
 * Image Dimensions.
 */
struct ImageDimensions {
  /**
   * Image width
   */
  uint64_t width = 1;

  /**
   * Image height
   */
  uint32_t height = 1;

  /**
   * Image depth
   */
  uint32_t depth = 1;

  /**
   * Default constructor
   */
  constexpr ImageDimensions() = default;

  /**
   * Construct a new Image Dimensions object
   *
   * @param width
   */
  constexpr ImageDimensions(const uint64_t width) : width(width) {}

  /**
   * Construct a new Image Dimensions object
   *
   * @param width
   * @param height
   */
  constexpr ImageDimensions(const uint64_t width, const uint32_t height)
      : width(width), height(height){};

  /**
   * Construct a new Image Dimensions object
   *
   * @param width
   * @param height
   * @param depth
   */
  constexpr ImageDimensions(const uint64_t width, const uint32_t height,
                            const uint32_t depth)
      : width(width), height(height), depth(depth) {}

  /**
   * Assignment operator for 3-dimension array.
   */
  ImageDimensions &operator=(const std::array<size_t, 3> &rhs) {
    assert(rhs[1] <= UINT32_MAX);
    assert(rhs[2] <= UINT32_MAX);
    width = rhs[0];
    height = static_cast<uint32_t>(rhs[1]);
    depth = static_cast<uint32_t>(rhs[2]);
    return *this;
  }

  /**
   * Assignment operator for 2-dimension array.
   */
  ImageDimensions &operator=(const std::array<size_t, 2> &rhs) {
    assert(rhs[1] <= UINT32_MAX);
    width = rhs[0];
    height = static_cast<uint32_t>(rhs[1]);
    depth = 1;
    return *this;
  }
};

/**
 * Image Type.
 */
enum class ImageType {
  t_1d,
  t_1d_array,
  t_1d_buffer,
  t_2d,
  t_2d_array,
  t_2d_depth,
  t_2d_array_depth,
  t_3d
};

/**
 * Image Format.
 */
enum class ImageFormat {
  snorm_int8,
  snorm_int16,
  unorm_int8,
  unorm_int16,
  unorm_short_565,
  unorm_short_555,
  unorm_int_101010,
  unorm_int_101010_2,
  signed_int8,
  signed_int16,
  signed_int32,
  unsigned_int8,
  unsigned_int16,
  unsigned_int32,
  float16,
  float32
};

/**
 * Image Channel Order.
 */
enum class ImageChannelOrder {
  r,
  a,
  rg,
  ra,
  rgb,
  rgba,
  bgra,
  argb,
  abgr,
  nv12
};

/**
 * Image Plane.
 */
enum class ImagePlane { y, uv };

/**
 * Calculates image pixel size
 *
 * @param[in] format ImageFormat enum value
 * @param[in] order ImageChannelOrder enum value
 */
uint32_t get_pixel_size(const ImageFormat &format,
                        const ImageChannelOrder &order);

} // namespace cassian
#endif
