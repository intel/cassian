/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_OCLC_IMAGE_READ_WRITE_FUNC_COMMON_HPP
#define CASSIAN_OCLC_IMAGE_READ_WRITE_FUNC_COMMON_HPP

#include <algorithm>
#include <cassian/runtime/access_qualifier.hpp>
#include <cassian/runtime/image_properties.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cmath>
#include <math.h>
#include <numeric>
#include <tuple>
#include <utility>
#include <vector>

namespace ca = cassian;

#define CASSIAN_REGISTER_FORMAT_ENUM                                           \
  CATCH_REGISTER_ENUM(                                                         \
      ca::ImageChannelOrder, ca::ImageChannelOrder::r,                         \
      ca::ImageChannelOrder::a, ca::ImageChannelOrder::rg,                     \
      ca::ImageChannelOrder::ra, ca::ImageChannelOrder::rgb,                   \
      ca::ImageChannelOrder::rgba, ca::ImageChannelOrder::bgra,                \
      ca::ImageChannelOrder::argb, ca::ImageChannelOrder::abgr)

int suggest_work_size(const std::string &type);

class UnknownTestTypeException : public std::runtime_error {
  using std::runtime_error::runtime_error;
};

struct ImageConfig {
  ca::ImageType type;
  ca::ImageDimensions dim;
  ca::ImageFormat format;
  ca::ImageChannelOrder order;
};

using CustomTuple = std::tuple<ca::ImageChannelOrder, ca::ImageFormat>;

template <typename T>
std::vector<T> run_kernel(const cassian::Kernel &kernel,
                          const std::vector<T> &input, const ImageConfig &conf,
                          cassian::Runtime *runtime) {
  ca::Image input_image =
      runtime->create_image(conf.dim, conf.type, conf.format, conf.order,
                            ca::AccessQualifier::read_only);
  ca::Image output_image =
      runtime->create_image(conf.dim, conf.type, conf.format, conf.order,
                            ca::AccessQualifier::write_only);
  runtime->write_image(input_image, input.data());
  runtime->set_kernel_argument(kernel, 0, input_image);
  runtime->set_kernel_argument(kernel, 1, output_image);
  std::array<size_t, 3> dim_arr = {conf.dim.width,
                                   conf.dim.height != 1 ? conf.dim.height : 0,
                                   conf.dim.depth != 1 ? conf.dim.depth : 0};
  runtime->run_kernel(kernel, dim_arr);
  std::vector<T> vec(input.size(), 0);
  runtime->read_image(output_image, vec.data());

  runtime->release_image(input_image);
  runtime->release_image(output_image);
  runtime->release_kernel(kernel);
  return vec;
}

template <typename T>
std::vector<T>
run_kernel_sampler(const cassian::Kernel &kernel, const std::vector<T> &input,
                   const ImageConfig &conf, cassian::Runtime *runtime) {
  ca::Image input_image =
      runtime->create_image(conf.dim, conf.type, conf.format, conf.order,
                            ca::AccessQualifier::read_only);
  ca::Image output_image =
      runtime->create_image(conf.dim, conf.type, conf.format, conf.order,
                            ca::AccessQualifier::write_only);
  ca::Sampler sampler = runtime->create_sampler();
  runtime->write_image(input_image, input.data());
  runtime->set_kernel_argument(kernel, 0, input_image);
  runtime->set_kernel_argument(kernel, 1, output_image);
  runtime->set_kernel_argument(kernel, 2, sampler);
  std::array<size_t, 3> dim_arr = {conf.dim.width,
                                   conf.dim.height != 1 ? conf.dim.height : 0,
                                   conf.dim.depth != 1 ? conf.dim.depth : 0};
  runtime->run_kernel(kernel, dim_arr);
  std::vector<T> vec(input.size(), 0);
  runtime->read_image(output_image, vec.data());

  runtime->release_image(input_image);
  runtime->release_image(output_image);
  runtime->release_kernel(kernel);
  runtime->release_sampler(sampler);
  return vec;
}

std::string get_build_options(const ImageConfig &config);
std::array<size_t, 2> convert_work_size_2_d(const size_t number);

template <typename T>
std::vector<T> get_vector_data(const size_t section_size) {
  std::vector<T> input(section_size, (T)0);
  std::iota(input.begin(), input.end(), (T)1);
  return input;
}

std::vector<CustomTuple> get_test_case_list(ca::ImageFormat f);
#endif
