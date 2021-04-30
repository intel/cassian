/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "common.hpp"
#include "test_config.hpp"
#include <cassian/cli/cli.hpp>
#include <cassian/runtime/image_properties.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cassian/test_harness/test_harness.hpp>
#include <cassian/utility/utility.hpp>
#include <catch2/catch.hpp>
#include <cstddef>
#include <numeric>
#include <string>
#include <vector>

namespace ca = cassian;
CASSIAN_REGISTER_FORMAT_ENUM; // NOLINT

template <typename T>
std::vector<T> test(const std::vector<T> &input, const ImageConfig &conf,
                    ca::Runtime *runtime, const std::string &program_type) {
  const std::string source = ca::load_text_file(
      ca::get_asset("kernels/oclc_image_read_write_functions/arg_sampler.cl"));
  const std::string build_options = get_build_options(conf);
  ca::Kernel kernel =
      runtime->create_kernel("copy_image", source, build_options, program_type);
  return run_kernel_sampler<T>(kernel, {input}, conf, runtime);
}

TEMPLATE_TEST_CASE_SIG("sampler as argument with 2d image",
                       "[data_type][data_image_type]",
                       ((typename T, ca::ImageFormat V), T, V),
                       (float, ca::ImageFormat::float32),
                       (int8_t, ca::ImageFormat::snorm_int8),
                       (int16_t, ca::ImageFormat::snorm_int16),
                       (uint8_t, ca::ImageFormat::unorm_int8),
                       (uint16_t, ca::ImageFormat::unorm_int16),
                       (uint16_t, ca::ImageFormat::unorm_short_565),
                       (uint16_t, ca::ImageFormat::unorm_short_555),
                       (uint32_t, ca::ImageFormat::unorm_int_101010),
                       (uint32_t, ca::ImageFormat::unorm_int_101010_2),
                       (int8_t, ca::ImageFormat::signed_int8),
                       (int16_t, ca::ImageFormat::signed_int16),
                       (int32_t, ca::ImageFormat::signed_int32),
                       (uint8_t, ca::ImageFormat::unsigned_int8),
                       (uint16_t, ca::ImageFormat::unsigned_int16),
                       (uint32_t, ca::ImageFormat::unsigned_int32)
                       /*(ca::half, ca::ImageFormat::float16)*/
) {
  const TestConfig &config = get_test_config();
  ca::Requirements requirements;
  requirements.feature(ca::Feature::sampling);
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }
  const size_t size = config.work_size();

  ImageConfig image_config;
  image_config.type = ca::ImageType::t_2d;
  image_config.dim = convert_work_size_2_d(size);

  auto test_case_list = get_test_case_list(V);
  for (const auto format : test_case_list) {
    image_config.format = std::get<ca::ImageFormat>(format);
    image_config.order = std::get<ca::ImageChannelOrder>(format);
    DYNAMIC_SECTION("channel order is "
                    << Catch::StringMaker<ca::ImageChannelOrder>::convert(
                           image_config.order)) {
      const size_t section_size =
          size * ca::get_pixel_size(image_config.format, image_config.order);
      auto reference = get_vector_data<T>(section_size / sizeof(T));
      auto golden = test(reference, image_config, config.runtime(),
                         config.program_type());
      REQUIRE_THAT(reference, Catch::Equals(golden));
    }
  }
}