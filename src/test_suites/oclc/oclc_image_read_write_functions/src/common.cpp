/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "common.hpp"
#include <cassian/runtime/feature.hpp>
#include <cassian/runtime/image_properties.hpp>
#include <cassian/runtime/runtime.hpp>

namespace ca = cassian;

namespace {
std::string get_oclc_read_function(const ca::ImageFormat &format) {
  switch (format) {
  case ca::ImageFormat::snorm_int8:
  case ca::ImageFormat::snorm_int16:
  case ca::ImageFormat::unorm_int8:
  case ca::ImageFormat::unorm_int16:
  case ca::ImageFormat::unorm_short_565:
  case ca::ImageFormat::unorm_short_555:
  case ca::ImageFormat::unorm_int_101010:
  case ca::ImageFormat::unorm_int_101010_2:
  case ca::ImageFormat::float16:
  case ca::ImageFormat::float32:
    return "read_imagef";
  case ca::ImageFormat::signed_int8:
  case ca::ImageFormat::signed_int16:
  case ca::ImageFormat::signed_int32:
    return "read_imagei";
  case ca::ImageFormat::unsigned_int8:
  case ca::ImageFormat::unsigned_int16:
  case ca::ImageFormat::unsigned_int32:
    return "read_imageui";
  }
  throw UnknownTestTypeException("Test doesn't support image format");
}

std::string get_oclc_write_function(const ca::ImageFormat &format) {
  switch (format) {
  case ca::ImageFormat::snorm_int8:
  case ca::ImageFormat::snorm_int16:
  case ca::ImageFormat::unorm_int8:
  case ca::ImageFormat::unorm_int16:
  case ca::ImageFormat::unorm_short_565:
  case ca::ImageFormat::unorm_short_555:
  case ca::ImageFormat::unorm_int_101010:
  case ca::ImageFormat::unorm_int_101010_2:
  case ca::ImageFormat::float16:
  case ca::ImageFormat::float32:
    return "write_imagef";
  case ca::ImageFormat::signed_int8:
  case ca::ImageFormat::signed_int16:
  case ca::ImageFormat::signed_int32:
    return "write_imagei";
  case ca::ImageFormat::unsigned_int8:
  case ca::ImageFormat::unsigned_int16:
  case ca::ImageFormat::unsigned_int32:
    return "write_imageui";
  }
  throw UnknownTestTypeException("Test doesn't support image format");
}

std::string get_oclc_color_type(const ca::ImageFormat &format) {
  switch (format) {
  case ca::ImageFormat::snorm_int8:
  case ca::ImageFormat::snorm_int16:
  case ca::ImageFormat::unorm_int8:
  case ca::ImageFormat::unorm_int16:
  case ca::ImageFormat::unorm_short_565:
  case ca::ImageFormat::unorm_short_555:
  case ca::ImageFormat::unorm_int_101010:
  case ca::ImageFormat::unorm_int_101010_2:
  case ca::ImageFormat::float16:
  case ca::ImageFormat::float32:
    return "float4";
  case ca::ImageFormat::signed_int8:
  case ca::ImageFormat::signed_int16:
  case ca::ImageFormat::signed_int32:
    return "int4";
  case ca::ImageFormat::unsigned_int8:
  case ca::ImageFormat::unsigned_int16:
  case ca::ImageFormat::unsigned_int32:
    return "uint4";
  }
  throw UnknownTestTypeException("Test doesn't support image format");
}

std::string get_oclc_image_type(const ca::ImageType &type) {
  switch (type) {
  case ca::ImageType::t_1d:
    return "image1d_t";
  case ca::ImageType::t_1d_array:
    return "image1d_array_t";
  case ca::ImageType::t_1d_buffer:
    return "image1d_buffer_t";
  case ca::ImageType::t_2d:
    return "image2d_t";
  case ca::ImageType::t_2d_array:
    return "image2d_array_t";
  case ca::ImageType::t_2d_depth:
    return "image2d_depth_t";
  case ca::ImageType::t_2d_array_depth:
    return "image2d_array_depth_t";
  case ca::ImageType::t_3d:
    return "image3d_t";
  }
  throw UnknownTestTypeException("Test doesn't support image type");
}

std::string get_oclc_coord_type(const ca::ImageType &type) {
  switch (type) {
  case ca::ImageType::t_1d:
  case ca::ImageType::t_1d_buffer:
    return "int";
  case ca::ImageType::t_1d_array:
  case ca::ImageType::t_2d:
  case ca::ImageType::t_2d_depth:
    return "int2";
  case ca::ImageType::t_2d_array:
  case ca::ImageType::t_2d_array_depth:
  case ca::ImageType::t_3d:
    return "int4";
  }
  throw UnknownTestTypeException("Test doesn't support image type");
}

std::string get_dimension(const ca::ImageType &type) {
  switch (type) {
  case ca::ImageType::t_1d:
  case ca::ImageType::t_1d_buffer:
    return "T_1D";
  case ca::ImageType::t_1d_array:
  case ca::ImageType::t_2d:
  case ca::ImageType::t_2d_depth:
    return "T_2D";
  case ca::ImageType::t_2d_array:
  case ca::ImageType::t_2d_array_depth:
  case ca::ImageType::t_3d:
    return "T_3D";
  }
  throw UnknownTestTypeException("Test doesn't support image type");
}

std::vector<size_t> create_divisors_vector(const size_t number) {
  std::vector<size_t> v;
  for (auto i = 2U; i <= number / 2; i++) {
    if (number % i == 0) {
      v.push_back(i);
    }
  }
  return v;
}

constexpr std::initializer_list<CustomTuple> format_list = {
    {ca::ImageChannelOrder::r, ca::ImageFormat::snorm_int8},
    {ca::ImageChannelOrder::r, ca::ImageFormat::snorm_int16},
    {ca::ImageChannelOrder::r, ca::ImageFormat::unorm_int8},
    {ca::ImageChannelOrder::r, ca::ImageFormat::unorm_int16},
    {ca::ImageChannelOrder::r, ca::ImageFormat::signed_int8},
    {ca::ImageChannelOrder::r, ca::ImageFormat::signed_int16},
    {ca::ImageChannelOrder::r, ca::ImageFormat::signed_int32},
    {ca::ImageChannelOrder::r, ca::ImageFormat::unsigned_int8},
    {ca::ImageChannelOrder::r, ca::ImageFormat::unsigned_int16},
    {ca::ImageChannelOrder::r, ca::ImageFormat::unsigned_int32},
    {ca::ImageChannelOrder::r, ca::ImageFormat::float16},
    {ca::ImageChannelOrder::r, ca::ImageFormat::float32},
    {ca::ImageChannelOrder::rg, ca::ImageFormat::snorm_int8},
    {ca::ImageChannelOrder::rg, ca::ImageFormat::snorm_int16},
    {ca::ImageChannelOrder::rg, ca::ImageFormat::unorm_int8},
    {ca::ImageChannelOrder::rg, ca::ImageFormat::unorm_int16},
    {ca::ImageChannelOrder::rg, ca::ImageFormat::signed_int8},
    {ca::ImageChannelOrder::rg, ca::ImageFormat::signed_int16},
    {ca::ImageChannelOrder::rg, ca::ImageFormat::signed_int32},
    {ca::ImageChannelOrder::rg, ca::ImageFormat::unsigned_int8},
    {ca::ImageChannelOrder::rg, ca::ImageFormat::unsigned_int16},
    {ca::ImageChannelOrder::rg, ca::ImageFormat::unsigned_int32},
    {ca::ImageChannelOrder::rg, ca::ImageFormat::float16},
    {ca::ImageChannelOrder::rg, ca::ImageFormat::float32},
    {ca::ImageChannelOrder::rg, ca::ImageFormat::snorm_int8},
    {ca::ImageChannelOrder::rgba, ca::ImageFormat::snorm_int16},
    {ca::ImageChannelOrder::rgba, ca::ImageFormat::unorm_int8},
    {ca::ImageChannelOrder::rgba, ca::ImageFormat::unorm_int16},
    {ca::ImageChannelOrder::rgba, ca::ImageFormat::signed_int8},
    {ca::ImageChannelOrder::rgba, ca::ImageFormat::signed_int16},
    {ca::ImageChannelOrder::rgba, ca::ImageFormat::signed_int32},
    {ca::ImageChannelOrder::rgba, ca::ImageFormat::unsigned_int8},
    {ca::ImageChannelOrder::rgba, ca::ImageFormat::unsigned_int16},
    {ca::ImageChannelOrder::rgba, ca::ImageFormat::unsigned_int32},
    {ca::ImageChannelOrder::rgba, ca::ImageFormat::float16},
    {ca::ImageChannelOrder::rgba, ca::ImageFormat::float32},
    {ca::ImageChannelOrder::bgra, ca::ImageFormat::unorm_int8}};
} // namespace

int suggest_work_size(const std::string &type) {
  const int default_size = 16;
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

std::array<size_t, 2> convert_work_size_2_d(const size_t number) {
  auto v = create_divisors_vector(number);
  if (v.empty()) {
    return {number, 1};
  }
  if (v.size() == 1) {
    return {v.front(), v.front()};
  }
  const size_t middle = v.size() / 2UL;
  if (v.size() % 2 == 1) {
    const auto mid = v.begin() + middle;
    return {*mid, *mid};
  }
  const auto x = v.begin() + middle;
  const auto y = v.begin() + middle - 1UL;
  return {*x, *y};
}

std::string get_build_options(const ImageConfig &config) {
  std::stringstream build_options;
  build_options << "-DCOLOR_TYPE=" << get_oclc_color_type(config.format)
                << " -DCOORD_TYPE=" << get_oclc_coord_type(config.type)
                << " -DIMAGE_TYPE=" << get_oclc_image_type(config.type)
                << " -DREAD_FUNC=" << get_oclc_read_function(config.format)
                << " -DWRITE_FUNC=" << get_oclc_write_function(config.format)
                << " -D" << get_dimension(config.type);
  return build_options.str();
}

std::vector<CustomTuple> get_test_case_list(ca::ImageFormat f) {
  std::vector<CustomTuple> test_case_list;
  const auto predicate = [&f](CustomTuple tuple) {
    return std::get<ca::ImageFormat>(tuple) == f;
  };
  const auto *it_list =
      std::find_if(format_list.begin(), format_list.end(), predicate);
  while (it_list != format_list.end()) {
    test_case_list.emplace_back(*it_list);
    it_list = std::find_if(++it_list, format_list.end(), predicate);
  }
  return test_case_list;
}
