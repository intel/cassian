/*
 * Copyright (C) 2021-2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/runtime/feature.hpp>
#include <cassian/runtime/mocks/stub_runtime.hpp>
#include <cassian/runtime/openclc_types.hpp>
#include <cassian/test_harness/test_harness.hpp>
#include <catch2/catch.hpp>
#include <cstdint>
#include <string>

namespace ca = cassian;

namespace {

TEST_CASE("Requirements::check") {
  ca::Requirements requirements;
  ca::StubRuntime runtime;

  SECTION("no requirements") {
    const std::string reason = requirements.check(runtime);
    REQUIRE(reason.empty());
  }

  SECTION("feature") {
    SECTION("fp16 - supported") {
      runtime.is_fp16_supported_ = true;
      requirements.feature(ca::Feature::fp16);
      const std::string reason = requirements.check(runtime);
      REQUIRE(reason.empty());
    }

    SECTION("fp16 - not supported") {
      runtime.is_fp16_supported_ = false;
      requirements.feature(ca::Feature::fp16);
      const std::string reason = requirements.check(runtime);
      REQUIRE(reason == "Feature not supported: fp16");
    }
  }

  SECTION("arithmetic_type") {
    SECTION("core scalar") {
      requirements.arithmetic_type<ca::clc_int_t>();
      const std::string reason = requirements.check(runtime);
      REQUIRE(reason.empty());
    }

    SECTION("fp16 - supported") {
      runtime.is_fp16_supported_ = true;
      requirements.arithmetic_type<ca::clc_half_t>();
      const std::string reason = requirements.check(runtime);
      REQUIRE(reason.empty());
    }

    SECTION("fp16 - not supported") {
      runtime.is_fp16_supported_ = false;
      requirements.arithmetic_type<ca::clc_half_t>();
      const std::string reason = requirements.check(runtime);
      REQUIRE(reason == "Feature not supported: fp16");
    }

    SECTION("fp64 - supported") {
      runtime.is_fp64_supported_ = true;
      requirements.arithmetic_type<ca::clc_double_t>();
      const std::string reason = requirements.check(runtime);
      REQUIRE(reason.empty());
    }

    SECTION("fp64 - not supported") {
      runtime.is_fp64_supported_ = false;
      requirements.arithmetic_type<ca::clc_double_t>();
      const std::string reason = requirements.check(runtime);
      REQUIRE(reason == "Feature not supported: fp64");
    }
  }

  SECTION("atomic_type") {
    SECTION("int - supported") {
      requirements.atomic_type<ca::clc_int_t>();
      const std::string reason = requirements.check(runtime);
      REQUIRE(reason.empty());
    }

    SECTION("float - supported") {
      requirements.atomic_type<ca::clc_float_t>();
      const std::string reason = requirements.check(runtime);
      REQUIRE(reason.empty());
    }

    SECTION("long - supported") {
      runtime.is_int64_atomics_supported_ = true;
      requirements.atomic_type<ca::clc_long_t>();
      const std::string reason = requirements.check(runtime);
      REQUIRE(reason.empty());
    }

    SECTION("long - not supported") {
      runtime.is_int64_atomics_supported_ = false;
      requirements.atomic_type<ca::clc_long_t>();
      const std::string reason = requirements.check(runtime);
      REQUIRE(reason == "Feature not supported: int64_atomics");
    }

    SECTION("ulong - supported") {
      runtime.is_int64_atomics_supported_ = true;
      requirements.atomic_type<ca::clc_ulong_t>();
      const std::string reason = requirements.check(runtime);
      REQUIRE(reason.empty());
    }

    SECTION("ulong - not supported") {
      runtime.is_int64_atomics_supported_ = false;
      requirements.atomic_type<ca::clc_ulong_t>();
      const std::string reason = requirements.check(runtime);
      REQUIRE(reason == "Feature not supported: int64_atomics");
    }

    SECTION("double - supported") {
      runtime.is_fp64_supported_ = true;
      requirements.atomic_type<ca::clc_double_t>();
      const std::string reason = requirements.check(runtime);
      REQUIRE(reason.empty());
    }

    SECTION("double - not supported fp64") {
      runtime.is_fp64_supported_ = false;
      requirements.atomic_type<ca::clc_double_t>();
      const std::string reason = requirements.check(runtime);
      REQUIRE(reason == "Feature not supported: fp64");
    }

    SECTION("double - not supported int64 atomics") {
      runtime.is_int64_atomics_supported_ = false;
      requirements.atomic_type<ca::clc_double_t>();
      const std::string reason = requirements.check(runtime);
      REQUIRE(reason == "Feature not supported: int64_atomics");
    }
  }

  SECTION("atomic_add") {
    SECTION("int - supported") {
      requirements.atomic_add<ca::clc_int_t>();
      const std::string reason = requirements.check(runtime);
      REQUIRE(reason.empty());
    }
  }

  SECTION("sub_group_size") {
    runtime.is_simd8_supported_ = true;
    runtime.is_simd16_supported_ = false;
    runtime.is_simd32_supported_ = false;
    SECTION("can be 8") {
      requirements.sub_group_size<8>();
      const std::string reason = requirements.check(runtime);
      REQUIRE(reason.empty());
    }
    SECTION("can't be 8") {
      runtime.is_simd8_supported_ = false;
      requirements.sub_group_size<8>();
      const std::string reason = requirements.check(runtime);
      REQUIRE(reason == "Feature not supported: simd8");
    }
    SECTION("can't be 16") {
      requirements.sub_group_size<16>();
      const std::string reason = requirements.check(runtime);
      REQUIRE(reason == "Feature not supported: simd16");
    }
    SECTION("can't be 32") {
      requirements.sub_group_size<32>();
      const std::string reason = requirements.check(runtime);
      REQUIRE(reason == "Feature not supported: simd32");
    }
  }

  SECTION("min_work_group_size") {
    runtime.max_group_size_x_ = 1024;
    runtime.max_group_size_y_ = 1024;
    runtime.max_group_size_z_ = 1024;
    SECTION("can be (1, 1, 1)") {
      requirements.min_work_group_size(1, 1, 1);
      const std::string reason = requirements.check(runtime);
      REQUIRE(reason.empty());
    }
    SECTION("can be (1024, 1024, 1024)") {
      requirements.min_work_group_size(1024, 1024, 1024);
      const std::string reason = requirements.check(runtime);
      REQUIRE(reason.empty());
    }
    SECTION("can't be (1025, 1025, 1025)") {
      requirements.min_work_group_size(1025, 1025, 1025);
      const std::string reason = requirements.check(runtime);
      REQUIRE(reason == "Test case requirements not met: minimum workgroup "
                        "size (1025, 1025, 1025)");
    }
  }
}

TEST_CASE("should_skip_test") {
  ca::Requirements requirements;
  ca::StubRuntime runtime;

  SECTION("feature supported") {
    runtime.is_fp16_supported_ = true;
    requirements.feature(ca::Feature::fp16);
    REQUIRE_FALSE(ca::should_skip_test(requirements, runtime));
  }

  SECTION("feature not supported") {
    runtime.is_fp16_supported_ = false;
    requirements.feature(ca::Feature::fp16);
    REQUIRE(ca::should_skip_test(requirements, runtime));
  }

  SECTION("min_work_group_size") {
    runtime.max_group_size_x_ = 1024;
    runtime.max_group_size_y_ = 1024;
    runtime.max_group_size_z_ = 1024;
    SECTION("correct") {
      requirements.min_work_group_size(1024, 1024, 1024);
      REQUIRE_FALSE(ca::should_skip_test(requirements, runtime));
    }
    SECTION("incorrect") {
      requirements.min_work_group_size(1025, 1025, 1025);
      REQUIRE(ca::should_skip_test(requirements, runtime));
    }
  }

  SECTION("sub_group_size") {
    runtime.is_simd8_supported_ = true;
    runtime.is_simd32_supported_ = false;
    SECTION("correct") {
      requirements.sub_group_size<8>();
      REQUIRE_FALSE(ca::should_skip_test(requirements, runtime));
    }
    SECTION("incorrect") {
      requirements.sub_group_size<32>();
      REQUIRE(ca::should_skip_test(requirements, runtime));
    }
  }
}

} // namespace
