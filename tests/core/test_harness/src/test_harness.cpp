/*
 * Copyright (C) 2021 Intel Corporation
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
}

} // namespace
