/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_LEVEL_ZERO_RUNTIME_UTILS_HPP
#define CASSIAN_LEVEL_ZERO_RUNTIME_UTILS_HPP

#include <cassian/runtime/image_properties.hpp>
#include <cassian/runtime/runtime.hpp>
#include <ze_api.h>

/**
 * Cassian namespace.
 */
namespace cassian {
namespace {
/**
 * Describes the number of components and their corresponding 8-bit widths
 *
 * @param[in] number_of_components
 * @throws cassian::RuntimeException Thrown if layout is unsupported for the
 * number of components
 */
ze_image_format_layout_t get_image_layout_8(const int &number_of_components) {
  if (number_of_components == 1) {
    return ZE_IMAGE_FORMAT_LAYOUT_8;
  } else if (number_of_components == 2) {
    return ZE_IMAGE_FORMAT_LAYOUT_8_8;
  } else if (number_of_components == 4) {
    return ZE_IMAGE_FORMAT_LAYOUT_8_8_8_8;
  }
  throw RuntimeException(
      "Unsupported number of components of the image format layout: " +
      std::to_string(number_of_components));
}

/**
 * Describes the number of components and their corresponding 16-bit widths
 *
 * @param[in] number_of_components
 * @throws cassian::RuntimeException Thrown if layout is unsupported for the
 * number of components
 */
ze_image_format_layout_t get_image_layout_16(const int &number_of_components) {
  if (number_of_components == 1) {
    return ZE_IMAGE_FORMAT_LAYOUT_16;
  } else if (number_of_components == 2) {
    return ZE_IMAGE_FORMAT_LAYOUT_16_16;
  } else if (number_of_components == 4) {
    return ZE_IMAGE_FORMAT_LAYOUT_16_16_16_16;
  } else {
    throw RuntimeException(
        "Unsupported number of components of the image format layout: " +
        std::to_string(number_of_components));
  }
}

/**
 * Describes the number of components and their corresponding 32-bit widths
 *
 * @param[in] number_of_components
 * @throws cassian::RuntimeException Thrown if layout is unsupported for the
 * number of components
 */
ze_image_format_layout_t get_image_layout_32(const int &number_of_components) {
  if (number_of_components == 1) {
    return ZE_IMAGE_FORMAT_LAYOUT_32;
  } else if (number_of_components == 2) {
    return ZE_IMAGE_FORMAT_LAYOUT_32_32;
  } else if (number_of_components == 4) {
    return ZE_IMAGE_FORMAT_LAYOUT_32_32_32_32;
  } else {
    throw RuntimeException(
        "Unsupported number of components of the image format layout: " +
        std::to_string(number_of_components));
  }
}
} // namespace

/**
 * Gets Level Zero image type
 *
 * @param[in] type ImageType
 * @throws cassian::RuntimeException Thrown if type is unsupported by Level Zero
 */
ze_image_type_t ze_get_image_type(const ImageType type) {
  switch (type) {
  case ImageType::t_1d:
    return ZE_IMAGE_TYPE_1D;
  case ImageType::t_1d_array:
    return ZE_IMAGE_TYPE_1DARRAY;
  case ImageType::t_2d:
    return ZE_IMAGE_TYPE_2D;
  case ImageType::t_2d_array:
    return ZE_IMAGE_TYPE_2DARRAY;
  case ImageType::t_3d:
    return ZE_IMAGE_TYPE_3D;
  case ImageType::t_1d_buffer:
    return ZE_IMAGE_TYPE_BUFFER;
  default:
    throw RuntimeException("Unsupported image type for level zero");
  }
}

/**
 * Creates Level Zero image format
 *
 * @param[in] format ImageFormat
 * @param[in] order ImageChannelOrder
 * @throws cassian::RuntimeException Thrown if format is unsupported
 */
ze_image_format_t ze_create_image_format(const ImageFormat format,
                                         const ImageChannelOrder order) {
  ze_image_format_t f = {};
  auto number_of_components = 1;
  switch (order) {
  case ImageChannelOrder::r:
    f.x = ZE_IMAGE_FORMAT_SWIZZLE_R;
    f.y = ZE_IMAGE_FORMAT_SWIZZLE_0;
    f.z = ZE_IMAGE_FORMAT_SWIZZLE_0;
    f.w = ZE_IMAGE_FORMAT_SWIZZLE_1;
    break;
  case ImageChannelOrder::a:
    f.x = ZE_IMAGE_FORMAT_SWIZZLE_0;
    f.y = ZE_IMAGE_FORMAT_SWIZZLE_0;
    f.z = ZE_IMAGE_FORMAT_SWIZZLE_0;
    f.w = ZE_IMAGE_FORMAT_SWIZZLE_A;
    break;
  case ImageChannelOrder::rg:
    f.x = ZE_IMAGE_FORMAT_SWIZZLE_R;
    f.y = ZE_IMAGE_FORMAT_SWIZZLE_G;
    f.z = ZE_IMAGE_FORMAT_SWIZZLE_0;
    f.w = ZE_IMAGE_FORMAT_SWIZZLE_1;
    number_of_components = 2;
    break;
  case ImageChannelOrder::ra:
    f.x = ZE_IMAGE_FORMAT_SWIZZLE_R;
    f.y = ZE_IMAGE_FORMAT_SWIZZLE_0;
    f.z = ZE_IMAGE_FORMAT_SWIZZLE_0;
    f.w = ZE_IMAGE_FORMAT_SWIZZLE_A;
    number_of_components = 2;
    break;
  case ImageChannelOrder::rgb:
    f.x = ZE_IMAGE_FORMAT_SWIZZLE_R;
    f.y = ZE_IMAGE_FORMAT_SWIZZLE_G;
    f.z = ZE_IMAGE_FORMAT_SWIZZLE_B;
    f.w = ZE_IMAGE_FORMAT_SWIZZLE_1;
    number_of_components = 3;
    break;
  case ImageChannelOrder::rgba:
    f.x = ZE_IMAGE_FORMAT_SWIZZLE_R;
    f.y = ZE_IMAGE_FORMAT_SWIZZLE_G;
    f.z = ZE_IMAGE_FORMAT_SWIZZLE_B;
    f.w = ZE_IMAGE_FORMAT_SWIZZLE_A;
    number_of_components = 4;
    break;
  case ImageChannelOrder::bgra:
    f.x = ZE_IMAGE_FORMAT_SWIZZLE_B;
    f.y = ZE_IMAGE_FORMAT_SWIZZLE_G;
    f.z = ZE_IMAGE_FORMAT_SWIZZLE_R;
    f.w = ZE_IMAGE_FORMAT_SWIZZLE_A;
    number_of_components = 4;
    break;
  case ImageChannelOrder::argb:
    f.x = ZE_IMAGE_FORMAT_SWIZZLE_A;
    f.y = ZE_IMAGE_FORMAT_SWIZZLE_R;
    f.z = ZE_IMAGE_FORMAT_SWIZZLE_G;
    f.w = ZE_IMAGE_FORMAT_SWIZZLE_B;
    number_of_components = 4;
    break;
  case ImageChannelOrder::abgr:
    f.x = ZE_IMAGE_FORMAT_SWIZZLE_A;
    f.y = ZE_IMAGE_FORMAT_SWIZZLE_B;
    f.z = ZE_IMAGE_FORMAT_SWIZZLE_G;
    f.w = ZE_IMAGE_FORMAT_SWIZZLE_R;
    number_of_components = 4;
    break;
  case ImageChannelOrder::nv12:
    f.layout = ZE_IMAGE_FORMAT_LAYOUT_NV12;
    return f;
  default:
    throw RuntimeException("Unsupported channel order for level zero");
  }
  switch (format) {
  case ImageFormat::snorm_int8:
    f.layout = get_image_layout_8(number_of_components);
    f.type = ZE_IMAGE_FORMAT_TYPE_SNORM;
    break;
  case ImageFormat::snorm_int16:
    f.layout = get_image_layout_16(number_of_components);
    f.type = ZE_IMAGE_FORMAT_TYPE_SNORM;
    break;
  case ImageFormat::unorm_int8:
    f.layout = get_image_layout_8(number_of_components);
    f.type = ZE_IMAGE_FORMAT_TYPE_UNORM;
    break;
  case ImageFormat::unorm_int16:
    f.layout = get_image_layout_16(number_of_components);
    f.type = ZE_IMAGE_FORMAT_TYPE_UNORM;
    break;
  case ImageFormat::unorm_short_565:
    f.layout = ZE_IMAGE_FORMAT_LAYOUT_5_6_5;
    f.type = ZE_IMAGE_FORMAT_TYPE_UNORM;
    break;
  case ImageFormat::unorm_short_555:
    f.layout = ZE_IMAGE_FORMAT_LAYOUT_5_5_5_1;
    f.type = ZE_IMAGE_FORMAT_TYPE_UNORM;
    break;
  case ImageFormat::unorm_int_101010_2:
    f.layout = ZE_IMAGE_FORMAT_LAYOUT_10_10_10_2;
    f.type = ZE_IMAGE_FORMAT_TYPE_UNORM;
    break;
  case ImageFormat::signed_int8:
    f.layout = get_image_layout_8(number_of_components);
    f.type = ZE_IMAGE_FORMAT_TYPE_SINT;
    break;
  case ImageFormat::signed_int16:
    f.layout = get_image_layout_16(number_of_components);
    f.type = ZE_IMAGE_FORMAT_TYPE_SINT;
    break;
  case ImageFormat::signed_int32:
    f.layout = get_image_layout_32(number_of_components);
    f.type = ZE_IMAGE_FORMAT_TYPE_SINT;
    break;
  case ImageFormat::unsigned_int8:
    f.layout = get_image_layout_8(number_of_components);
    f.type = ZE_IMAGE_FORMAT_TYPE_UINT;
    break;
  case ImageFormat::unsigned_int16:
    f.layout = get_image_layout_16(number_of_components);
    f.type = ZE_IMAGE_FORMAT_TYPE_UINT;
    break;
  case ImageFormat::unsigned_int32:
    f.layout = get_image_layout_32(number_of_components);
    f.type = ZE_IMAGE_FORMAT_TYPE_UINT;
    break;
  case ImageFormat::float16:
    f.layout = get_image_layout_16(number_of_components);
    // there is no corresponding level zero type for half float
    throw RuntimeException("Half float image type not supported");
  case ImageFormat::float32:
    f.layout = get_image_layout_32(number_of_components);
    f.type = ZE_IMAGE_FORMAT_TYPE_FLOAT;
    break;
  case ImageFormat::unorm_int_101010:
  default:
    throw RuntimeException("Unsupported Level Zero Image Format");
  }
  return f;
}
} // namespace cassian

#endif
