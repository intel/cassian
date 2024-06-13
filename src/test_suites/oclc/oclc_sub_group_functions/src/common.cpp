/*
 * Copyright (C) 2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <common.hpp>
#include <string>

int suggest_work_size(const std::string &type) {
  const int default_size = 64;
  const int min_size = 1;
  const int max_size = 1024;
  if (type == "min") {
    return min_size;
  }
  if (type == "max") {
    return max_size;
  }
  return default_size;
}

ca::Kernel create_kernel(const std::string &path,
                         const std::string &kernel_name,
                         const std::string &build_options, ca::Runtime *runtime,
                         const std::string &program_type) {
  const std::string source = ca::load_text_file(ca::get_asset(path));
  return runtime->create_kernel(kernel_name, source, build_options,
                                program_type);
}

std::string function_type(const TestFunctionType &test_function_type) {
  switch (test_function_type) {
  case read_ft:
    return "read";
  case write_ft:
    return "write";
  case read_write_ft:
    return "read_write";
  default:
    return "";
  }
}

std::string to_string(ca::ImageChannelOrder channel_order) {
  switch (channel_order) {
  case ca::ImageChannelOrder::r:
    return "r";
  case ca::ImageChannelOrder::a:
    return "a";
  case ca::ImageChannelOrder::rg:
    return "rg";
  case ca::ImageChannelOrder::ra:
    return "ra";
  case ca::ImageChannelOrder::rgb:
    return "rgb";
  case ca::ImageChannelOrder::rgba:
    return "rgba";
  case ca::ImageChannelOrder::bgra:
    return "bgra";
  case ca::ImageChannelOrder::argb:
    return "argb";
  case ca::ImageChannelOrder::abgr:
    return "abgr";
  case ca::ImageChannelOrder::nv12:
    return "nv12";
  default:
    return "unknown";
  }
}

std::string to_string(ca::ImageFormat image_format) {
  switch (image_format) {
  case ca::ImageFormat::snorm_int8:
    return "snorm_int8";
  case ca::ImageFormat::snorm_int16:
    return "snorm_int16";
  case ca::ImageFormat::unorm_int8:
    return "unorm_int8";
  case ca::ImageFormat::unorm_int16:
    return "unorm_int16";
  case ca::ImageFormat::unorm_short_565:
    return "unorm_short_565";
  case ca::ImageFormat::unorm_short_555:
    return "unorm_short_555";
  case ca::ImageFormat::unorm_int_101010:
    return "unorm_int_101010";
  case ca::ImageFormat::unorm_int_101010_2:
    return "unorm_int_101010_2";
  case ca::ImageFormat::signed_int8:
    return "signed_int8";
  case ca::ImageFormat::signed_int16:
    return "signed_int16";
  case ca::ImageFormat::signed_int32:
    return "signed_int32";
  case ca::ImageFormat::unsigned_int8:
    return "unsigned_int8";
  case ca::ImageFormat::unsigned_int16:
    return "unsigned_int16";
  case ca::ImageFormat::unsigned_int32:
    return "unsigned_int32";
  case ca::ImageFormat::float16:
    return "float16";
  case ca::ImageFormat::float32:
    return "float32";
  default:
    return "unknown";
  }
}
