/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/cli/cli.hpp>
#include <cassian/runtime/access_qualifier.hpp>
#include <cassian/runtime/image_properties.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cassian/test_harness/test_harness.hpp>
#include <cassian/utility/utility.hpp>
#include <catch2/catch.hpp>
#include <cstddef>
#include <string>
#include <test_config.hpp>
#include <vector>

namespace ca = cassian;

void test(const ca::AccessQualifier access, const std::string &access_qualifier,
          ca::Runtime *runtime, const std::string &program_type) {
  const std::string source = ca::load_text_file(
      ca::get_asset("kernels/oclc_access_qualifiers/qualifier.cl"));
  const std::string build_options =
      "-cl-std=CL3.0 -DACCESS_QUALIFIER=" + access_qualifier;
  ca::Kernel kernel = runtime->create_kernel("test_qualifier", source,
                                             build_options, program_type);
  ca::ImageDimensions dim = {2, 2};
  ca::Image image = runtime->create_image(dim, ca::ImageType::t_2d,
                                          ca::ImageFormat::signed_int8,
                                          ca::ImageChannelOrder::r, access);
  runtime->set_kernel_argument(kernel, static_cast<int>(0), image);
  runtime->run_kernel(kernel, {dim.width, dim.height, 1});
  runtime->release_image(image);
  runtime->release_kernel(kernel);
}

TEST_CASE("Oclc access qualifiers", "") {
  const TestConfig &config = get_test_config();
  ca::Requirements requirements;
  requirements.feature(ca::Feature::image2d);
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }
  SECTION("read_only qualifier") {
    const std::string read_only_qualifier = "read_only";
    REQUIRE_NOTHROW(test(ca::AccessQualifier::read_only, read_only_qualifier,
                         config.runtime(), config.program_type()));
    REQUIRE_NOTHROW(test(ca::AccessQualifier::read_write, read_only_qualifier,
                         config.runtime(), config.program_type()));
    if (config.runtime()->name() == "OCL") {
      REQUIRE_THROWS(test(ca::AccessQualifier::write_only, read_only_qualifier,
                          config.runtime(), config.program_type()));
    }
  }
  SECTION("read_write qualifier") {
    requirements.feature(ca::Feature::read_write_images);
    const std::string read_write_qualifier = "read_write";
    if (!ca::should_skip_test(requirements, *config.runtime())) {
      REQUIRE_NOTHROW(test(ca::AccessQualifier::read_only, read_write_qualifier,
                           config.runtime(), config.program_type()));
      REQUIRE_NOTHROW(test(ca::AccessQualifier::write_only,
                           read_write_qualifier, config.runtime(),
                           config.program_type()));
      REQUIRE_NOTHROW(test(ca::AccessQualifier::read_write,
                           read_write_qualifier, config.runtime(),
                           config.program_type()));
    }
  }
  SECTION("write_only qualifier") {
    const std::string write_only_qualifier = "write_only";
    REQUIRE_NOTHROW(test(ca::AccessQualifier::write_only, write_only_qualifier,
                         config.runtime(), config.program_type()));
    REQUIRE_NOTHROW(test(ca::AccessQualifier::read_write, write_only_qualifier,
                         config.runtime(), config.program_type()));
    if (config.runtime()->name() == "OCL") {
      REQUIRE_THROWS(test(ca::AccessQualifier::read_only, write_only_qualifier,
                          config.runtime(), config.program_type()));
    }
  }
}