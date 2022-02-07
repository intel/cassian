/*
 * Copyright (C) 2021-2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_OPENCL_RUNTIME_UTILS_HPP
#define CASSIAN_OPENCL_RUNTIME_UTILS_HPP

#include <CL/cl.h>
#include <CL/cl_platform.h>
#include <cassian/runtime/access_qualifier.hpp>
#include <cassian/runtime/image_properties.hpp>
#include <cassian/runtime/runtime.hpp>

/**
 * Cassian namespace.
 */
namespace cassian {

/**
 * Adds access qualifier flags to the current cl_mem_flags
 *
 * @param[in, out] flags cl_mem_flags
 * @param[in] access AccessQualifier
 */
cl_mem_flags append_access_qualifier_flags(cl_mem_flags flags,
                                           const AccessQualifier &access) {
  switch (access) {
  case AccessQualifier::read_write:
    flags |= CL_MEM_READ_WRITE;
    break;
  case AccessQualifier::read_only:
    flags |= CL_MEM_READ_ONLY;
    break;
  case AccessQualifier::write_only:
    flags |= CL_MEM_WRITE_ONLY;
    break;
  }
  return flags;
}

/**
 * Get OpenCL image type
 *
 * @param[in] type ImageType
 * @throws cassian::RuntimeException Thrown if type is unsupported
 */
cl_mem_object_type cl_get_image_type(const ImageType type) {
  switch (type) {
  case ImageType::t_1d:
    return CL_MEM_OBJECT_IMAGE1D;
  case ImageType::t_1d_array:
    return CL_MEM_OBJECT_IMAGE1D_ARRAY;
  case ImageType::t_2d:
    return CL_MEM_OBJECT_IMAGE2D;
  case ImageType::t_2d_array:
    return CL_MEM_OBJECT_IMAGE2D_ARRAY;
  case ImageType::t_3d:
    return CL_MEM_OBJECT_IMAGE3D;
  case ImageType::t_1d_buffer:
    return CL_MEM_OBJECT_IMAGE1D_BUFFER;
  default:
    throw RuntimeException("Unsupported image type for OpenCL");
  }
}

/**
 * Create OpenCL image format
 *
 * @param[in] format ImageFormat
 * @param[in] order ImageChannelOrder
 * @throws cassian::RuntimeException Thrown if type is unsupported
 */
cl_image_format cl_create_image_format(const ImageFormat format,
                                       const ImageChannelOrder order) {
  cl_image_format f = {};
  switch (order) {
  case ImageChannelOrder::r:
    f.image_channel_order = CL_R;
    break;
  case ImageChannelOrder::a:
    f.image_channel_order = CL_A;
    break;
  case ImageChannelOrder::rg:
    f.image_channel_order = CL_RG;
    break;
  case ImageChannelOrder::ra:
    f.image_channel_order = CL_RA;
    break;
  case ImageChannelOrder::rgb:
    f.image_channel_order = CL_RGB;
    break;
  case ImageChannelOrder::rgba:
    f.image_channel_order = CL_RGBA;
    break;
  case ImageChannelOrder::bgra:
    f.image_channel_order = CL_BGRA;
    break;
  case ImageChannelOrder::argb:
    f.image_channel_order = CL_ARGB;
    break;
  case ImageChannelOrder::abgr:
    f.image_channel_order = CL_ABGR;
    break;
  case ImageChannelOrder::nv12:
    f.image_channel_order = CL_NV12_INTEL;
    break;
  default:
    throw RuntimeException("Unsupported channel order for OpenCL");
  }
  switch (format) {
  case ImageFormat::snorm_int8:
    f.image_channel_data_type = CL_SNORM_INT8;
    break;
  case ImageFormat::snorm_int16:
    f.image_channel_data_type = CL_SNORM_INT16;
    break;
  case ImageFormat::unorm_int8:
    f.image_channel_data_type = CL_UNORM_INT8;
    break;
  case ImageFormat::unorm_int16:
    f.image_channel_data_type = CL_UNORM_INT16;
    break;
  case ImageFormat::unorm_short_565:
    f.image_channel_data_type = CL_UNORM_SHORT_565;
    break;
  case ImageFormat::unorm_short_555:
    f.image_channel_data_type = CL_UNORM_SHORT_555;
    break;
  case ImageFormat::unorm_int_101010:
    f.image_channel_data_type = CL_UNORM_INT_101010;
    break;
  case ImageFormat::unorm_int_101010_2:
    f.image_channel_data_type = CL_UNORM_INT_101010_2;
    break;
  case ImageFormat::signed_int8:
    f.image_channel_data_type = CL_SIGNED_INT8;
    break;
  case ImageFormat::signed_int16:
    f.image_channel_data_type = CL_SIGNED_INT16;
    break;
  case ImageFormat::signed_int32:
    f.image_channel_data_type = CL_SIGNED_INT32;
    break;
  case ImageFormat::unsigned_int8:
    f.image_channel_data_type = CL_UNSIGNED_INT8;
    break;
  case ImageFormat::unsigned_int16:
    f.image_channel_data_type = CL_UNSIGNED_INT16;
    break;
  case ImageFormat::unsigned_int32:
    f.image_channel_data_type = CL_UNSIGNED_INT32;
    break;
  case ImageFormat::float16:
    f.image_channel_data_type = CL_HALF_FLOAT;
    break;
  case ImageFormat::float32:
    f.image_channel_data_type = CL_FLOAT;
    break;
  default:
    throw RuntimeException("Unsupported image format for OpenCL");
  }
  return f;
}
} // namespace cassian

#endif
