/*
 * Copyright (C) 2024-2025 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/runtime/openclc_types.hpp>
#include <cassian/utility/comparators.hpp>
#include <cassian/utility/utility.hpp>
#include <cassian/vector/vector.hpp>
#include <catch2/catch.hpp>
#include <cstdint>
#include <limits>
#include <vector>

namespace {

namespace ca = cassian;

// disable buggy clang-tidy checks
using ca::calculate_ulp_distance;
using ca::PrecisionComparator; // NOLINT(misc-unused-using-decls)
using ca::PrecisionRequirement;
using ca::PrecisionRequirementType;
using ca::UlpComparator; // NOLINT(misc-unused-using-decls)

TEST_CASE("PrecisionComparator - scalar", "[PrecisionComparator]") {
  SECTION("error_value") {
    const double reference = 10.0;
    const double result = 10.0;
    const double error_value = 1.0;

    PrecisionRequirement<double> error_requirement{
        PrecisionRequirementType::error_value, error_value};
    PrecisionComparator error_comparator(result, reference, error_requirement);

    SECTION("matches") {
      REQUIRE(error_comparator.match(result));

      const double result_in_range_up = 10.999;
      REQUIRE(error_comparator.match(result_in_range_up));

      const double result_in_range_down = 9.001;
      REQUIRE(error_comparator.match(result_in_range_down));

      SECTION("nan reference - nan result") {
        const double reference_nan = std::numeric_limits<double>::quiet_NaN();
        const double result_nan = std::numeric_limits<double>::quiet_NaN();

        PrecisionRequirement<double> error_requirement{
            PrecisionRequirementType::error_value, error_value};
        PrecisionComparator error_comparator(result_nan, reference_nan,
                                             error_requirement);

        REQUIRE(error_comparator.match(result_nan));
      }
    }

    SECTION("not matches") {
      const double result_over = 11.001;
      REQUIRE_FALSE(error_comparator.match(result_over));

      const double result_below = 8.999;
      REQUIRE_FALSE(error_comparator.match(result_below));

      const double result_nan = std::numeric_limits<double>::quiet_NaN();
      REQUIRE_FALSE(error_comparator.match(result_nan));

      SECTION("nan reference - non nan result") {
        const double reference_nan = std::numeric_limits<double>::quiet_NaN();

        PrecisionRequirement<double> error_requirement{
            PrecisionRequirementType::error_value, error_value};
        PrecisionComparator error_comparator(result, reference_nan,
                                             error_requirement);

        REQUIRE_FALSE(error_comparator.match(result));
      }
    }
  }

  SECTION("ulp_value") {
    SECTION("matches") {
      SECTION("in range - equal") {
        const double reference = 10.0;
        const double result = 10.0;
        const double ulp_value = 0.0;

        PrecisionRequirement<double> ulp_requirement{
            PrecisionRequirementType::ulp_value, ulp_value};
        PrecisionComparator ulp_comparator(result, reference, ulp_requirement);

        REQUIRE(ulp_comparator.match(result));
      }

      SECTION("in range - positive value") {
        const double reference = std::nextafter(2.0, 4.0);
        const double result = 2.0;
        const double ulp_value = 1.0;

        PrecisionRequirement<double> ulp_requirement{
            PrecisionRequirementType::ulp_value, ulp_value};
        PrecisionComparator ulp_comparator(result, reference, ulp_requirement);

        REQUIRE(ulp_comparator.match(result));
      }

      SECTION("in range - positive value - swapped") {
        const double reference = 2.0;
        const double result = std::nextafter(2.0, 4.0);
        const double ulp_value = 2.0;

        PrecisionRequirement<double> ulp_requirement{
            PrecisionRequirementType::ulp_value, ulp_value};
        PrecisionComparator ulp_comparator(result, reference, ulp_requirement);

        REQUIRE(ulp_comparator.match(result));
      }

      SECTION("in range - positive value - swapped - 2") {
        const double reference = std::nextafter(2.0, 4.0);
        const double result = std::nextafter(std::nextafter(2.0, 4.0), 4.0);
        const double ulp_value = 1.0;

        PrecisionRequirement<double> ulp_requirement{
            PrecisionRequirementType::ulp_value, ulp_value};
        PrecisionComparator ulp_comparator(result, reference, ulp_requirement);

        REQUIRE(ulp_comparator.match(result));
      }

      SECTION("in range - positive value - large distance") {
        const double reference = 0x1p2;
        const double result = 0x1p1;
        const double ulp_value = 0x1p52;

        PrecisionRequirement<double> ulp_requirement{
            PrecisionRequirementType::ulp_value, ulp_value};
        PrecisionComparator ulp_comparator(result, reference, ulp_requirement);

        REQUIRE(ulp_comparator.match(result));
      }

      SECTION("in range - negative value") {
        const double reference = std::nextafter(-4.0, 2.0);
        const double result = -4.0;
        const double ulp_value = 1.0;

        PrecisionRequirement<double> ulp_requirement{
            PrecisionRequirementType::ulp_value, ulp_value};
        PrecisionComparator ulp_comparator(result, reference, ulp_requirement);

        REQUIRE(ulp_comparator.match(result));
      }

      SECTION("nan result - nan reference") {
        const double reference = std::numeric_limits<double>::quiet_NaN();
        const double result = std::numeric_limits<double>::quiet_NaN();
        const double ulp_value = 1.0;

        PrecisionRequirement<double> ulp_requirement{
            PrecisionRequirementType::ulp_value, ulp_value};
        PrecisionComparator ulp_comparator(result, reference, ulp_requirement);

        REQUIRE(ulp_comparator.match(result));
      }
    }

    SECTION("not matches") {
      SECTION("outside by a lot") {
        const double reference = 10.0;
        const double result = 20.0;
        const double ulp_value = 0.0;

        PrecisionRequirement<double> ulp_requirement{
            PrecisionRequirementType::ulp_value, ulp_value};
        PrecisionComparator ulp_comparator(result, reference, ulp_requirement);

        REQUIRE_FALSE(ulp_comparator.match(result));
      }

      SECTION("outside - positive value") {
        const double reference = std::nextafter(std::nextafter(2.0, 4.0), 4.0);
        const double result = 2.0;
        const double ulp_value = 1.0;

        PrecisionRequirement<double> ulp_requirement{
            PrecisionRequirementType::ulp_value, ulp_value};
        PrecisionComparator ulp_comparator(result, reference, ulp_requirement);

        REQUIRE_FALSE(ulp_comparator.match(result));
      }

      SECTION("outside - positive value - large distance") {
        const double reference = 0x1p2;
        const double result = 0x1p1;
        const double ulp_value = 0x1p51;

        PrecisionRequirement<double> ulp_requirement{
            PrecisionRequirementType::ulp_value, ulp_value};
        PrecisionComparator ulp_comparator(result, reference, ulp_requirement);

        REQUIRE_FALSE(ulp_comparator.match(result));
      }

      SECTION("outside - negative value") {
        const double reference = std::nextafter(std::nextafter(-4.0, 2.0), 2.0);
        const double result = -4.0;
        const double ulp_value = 1.0;

        PrecisionRequirement<double> ulp_requirement{
            PrecisionRequirementType::ulp_value, ulp_value};
        PrecisionComparator ulp_comparator(result, reference, ulp_requirement);

        REQUIRE_FALSE(ulp_comparator.match(result));
      }

      SECTION("nan result - non nan reference") {
        const double reference = 1.0;
        const double result = std::numeric_limits<double>::quiet_NaN();
        const double ulp_value = 1.0;

        PrecisionRequirement<double> ulp_requirement{
            PrecisionRequirementType::ulp_value, ulp_value};
        PrecisionComparator ulp_comparator(result, reference, ulp_requirement);

        REQUIRE_FALSE(ulp_comparator.match(result));
      }

      SECTION("non nan result - nan reference") {
        const double reference = std::numeric_limits<double>::quiet_NaN();
        const double result = 1.0;
        const double ulp_value = 1.0;

        PrecisionRequirement<double> ulp_requirement{
            PrecisionRequirementType::ulp_value, ulp_value};
        PrecisionComparator ulp_comparator(result, reference, ulp_requirement);

        REQUIRE_FALSE(ulp_comparator.match(result));
      }
    }
  }

  SECTION("value_range") {
    const double reference = 10.0;
    const double result = 10.0;
    const double min_value = 9.0;
    const double max_value = 11.0;

    PrecisionRequirement<double> range_requirement{
        PrecisionRequirementType::value_range, min_value, max_value};
    PrecisionComparator range_comparator(result, reference, range_requirement);

    SECTION("matches") {
      REQUIRE(range_comparator.match(result));

      const double result_equal_max = max_value;
      REQUIRE(range_comparator.match(result_equal_max));

      const double result_equal_min = min_value;
      REQUIRE(range_comparator.match(result_equal_min));
    }

    SECTION("not matches") {
      const double result_over = 11.1;
      REQUIRE_FALSE(range_comparator.match(result_over));

      const double result_below = 8.9;
      REQUIRE_FALSE(range_comparator.match(result_below));
    }
  }

  SECTION("any") {
    const double reference = 10.0;
    const double result = 0.0;

    PrecisionRequirement<double> any_requirement{PrecisionRequirementType::any};
    PrecisionComparator any_comparator(result, reference, any_requirement);

    REQUIRE(any_comparator.match(result));
  }

  SECTION("undefined") {
    const double reference = 10.0;
    const double result = 0.0;

    PrecisionRequirement<double> any_requirement{
        PrecisionRequirementType::undefined};
    PrecisionComparator any_comparator(result, reference, any_requirement);

    REQUIRE(any_comparator.match(result));
  }
}

TEST_CASE("PrecisionComparator - vector", "[PrecisionComparator]") {
  SECTION("error_value") {
    const ca::Vector<double, 3> reference = {10.0, 15.0, 20.0};
    const ca::Vector<double, 3> result = {10.0, 15.0, 20.0};
    double error_value = 1.0;

    std::vector<PrecisionRequirement<double>> error_requirements(
        reference.size(),
        PrecisionRequirement<double>{PrecisionRequirementType::error_value,
                                     error_value});
    PrecisionComparator error_comparator(result, reference, error_requirements);

    SECTION("matches") {
      REQUIRE(error_comparator.match(result));

      const ca::Vector<double, 3> result_in_range_up_all = {10.999, 15.999,
                                                            20.999};
      REQUIRE(error_comparator.match(result_in_range_up_all));

      const ca::Vector<double, 3> result_in_range_down_all = {9.001, 14.001,
                                                              19.001};
      REQUIRE(error_comparator.match(result_in_range_down_all));
    }

    SECTION("not matches") {
      const ca::Vector<double, 3> result_over_all = {11.001, 16.001, 21.001};
      REQUIRE_FALSE(error_comparator.match(result_over_all));

      const ca::Vector<double, 3> result_below_all = {8.999, 13.999, 18.999};
      REQUIRE_FALSE(error_comparator.match(result_below_all));

      const ca::Vector<double, 3> result_outside_all = {2, 100, 10};
      REQUIRE_FALSE(error_comparator.match(result_outside_all));

      const ca::Vector<double, 3> result_mixed = {10, 16, 10};
      REQUIRE_FALSE(error_comparator.match(result_mixed));
    }
  }

  SECTION("ulp_value") {
    SECTION("matches") {
      SECTION("in range - equal") {
        const ca::Vector<double, 3> reference = {10.0, 15.0, 20.0};
        const ca::Vector<double, 3> result = {10.0, 15.0, 20.0};
        const double ulp_value = 0.0;

        std::vector<PrecisionRequirement<double>> ulp_requirements(
            reference.size(),
            PrecisionRequirement<double>{PrecisionRequirementType::ulp_value,
                                         ulp_value});
        PrecisionComparator ulp_comparator(result, reference, ulp_requirements);

        REQUIRE(ulp_comparator.match(result));
      }

      SECTION("in range - positive value") {
        const ca::Vector<double, 3> reference = {
            2.0, std::nextafter(2.0, 4.0),
            std::nextafter(std::nextafter(2.0, 4.0), 4.0)};
        const ca::Vector<double, 3> result = {2.0, 2.0, 2.0};
        const double ulp_value = 2.0;

        std::vector<PrecisionRequirement<double>> ulp_requirements(
            reference.size(),
            PrecisionRequirement<double>{PrecisionRequirementType::ulp_value,
                                         ulp_value});
        PrecisionComparator ulp_comparator(result, reference, ulp_requirements);

        REQUIRE(ulp_comparator.match(result));
      }
    }

    SECTION("not matches") {
      SECTION("outside by a lot") {
        const ca::Vector<double, 3> reference = {10.0, 15.0, 20.0};
        const ca::Vector<double, 3> result = {20.0, 30.0, 40.0};
        const double ulp_value = 0.0;

        std::vector<PrecisionRequirement<double>> ulp_requirements(
            reference.size(),
            PrecisionRequirement<double>{PrecisionRequirementType::ulp_value,
                                         ulp_value});
        PrecisionComparator ulp_comparator(result, reference, ulp_requirements);

        REQUIRE_FALSE(ulp_comparator.match(result));
      }

      SECTION("all outside") {
        const ca::Vector<double, 3> reference = {
            std::nextafter(std::nextafter(1.0, 2.0), 2.0),
            std::nextafter(std::nextafter(2.0, 4.0), 4.0),
            std::nextafter(std::nextafter(2.0, 4.0), 4.0)};
        const ca::Vector<double, 3> result = {2.0, 2.0, 2.0};
        const double ulp_value = 1.0;

        std::vector<PrecisionRequirement<double>> ulp_requirements(
            reference.size(),
            PrecisionRequirement<double>{PrecisionRequirementType::ulp_value,
                                         ulp_value});
        PrecisionComparator ulp_comparator(result, reference, ulp_requirements);

        REQUIRE_FALSE(ulp_comparator.match(result));
      }

      SECTION("mixed outside") {
        const ca::Vector<double, 3> reference = {
            std::nextafter(1.0, 2.0), std::nextafter(2.0, 4.0),
            std::nextafter(std::nextafter(2.0, 4.0), 4.0)};
        const ca::Vector<double, 3> result = {2.0, 2.0, 2.0};
        const double ulp_value = 1.0;

        std::vector<PrecisionRequirement<double>> ulp_requirements(
            reference.size(),
            PrecisionRequirement<double>{PrecisionRequirementType::ulp_value,
                                         ulp_value});
        PrecisionComparator ulp_comparator(result, reference, ulp_requirements);

        REQUIRE_FALSE(ulp_comparator.match(result));
      }
    }
  }

  SECTION("value_range") {
    const ca::Vector<double, 3> reference = {10.0, 15.0, 20.0};
    const ca::Vector<double, 3> result = {10.0, 15.0, 20.0};
    double min_value = 9.0;
    double max_value = 21.0;

    std::vector<PrecisionRequirement<double>> range_requirements(
        reference.size(),
        PrecisionRequirement<double>{PrecisionRequirementType::value_range,
                                     min_value, max_value});
    PrecisionComparator range_comparator(result, reference, range_requirements);

    SECTION("matches") {
      REQUIRE(range_comparator.match(result));

      const ca::Vector<double, 3> result_in_range_all = {max_value, min_value,
                                                         20.5};
      REQUIRE(range_comparator.match(result_in_range_all));
    }

    SECTION("not matches") {
      const ca::Vector<double, 3> result_outside_all = {8, 100, 22};
      REQUIRE_FALSE(range_comparator.match(result_outside_all));

      const ca::Vector<double, 3> result_mixed = {max_value, 20, 22};
      REQUIRE_FALSE(range_comparator.match(result_mixed));
    }
  }

  SECTION("all requirements") {
    const ca::Vector<double, 5> reference = {20.0, 15.0, 10.0, 5.0, 5.0};
    const double error_value = 1.0;
    const double ulp_value = 0.0;
    const double min_value = 9.0;
    const double max_value = 11.0;
    const ca::Vector<double, 5> result = {20.0, 15.0, 10.0, 0.0, 0.0};

    std::vector<PrecisionRequirement<double>> requirements = {
        PrecisionRequirement<double>{PrecisionRequirementType::error_value,
                                     error_value},
        PrecisionRequirement<double>{PrecisionRequirementType::ulp_value,
                                     ulp_value},
        PrecisionRequirement<double>{PrecisionRequirementType::value_range,
                                     min_value, max_value},
        PrecisionRequirement<double>{PrecisionRequirementType::any},
        PrecisionRequirement<double>{PrecisionRequirementType::undefined}};

    PrecisionComparator comparator(result, reference, requirements);

    REQUIRE(comparator.match(result));

    const ca::Vector<double, 5> result_error_outside = {22.0, 15.0, 10.0, 0.0,
                                                        0.0};
    REQUIRE_FALSE(comparator.match(result_error_outside));

    const ca::Vector<double, 5> result_ulp_outside = {
        20.0, std::nextafter(15.0, 16.0), 10.0, 0.0, 0.0};
    REQUIRE_FALSE(comparator.match(result_ulp_outside));

    const ca::Vector<double, 5> result_range_outside = {20.0, 15.0, 12.0, 0.0,
                                                        0.0};
    REQUIRE_FALSE(comparator.match(result_range_outside));
  }
}

TEST_CASE("UlpComparator - scalar", "[UlpComparator]") {
  SECTION("matches") {
    SECTION("in range - equal") {
      const std::vector<double> reference = {10.0};
      const std::vector<double> result = {10.0};
      const std::vector<double> ulp_values = {0.0};

      UlpComparator comparator(result, reference, ulp_values);

      REQUIRE(comparator.match(result));
    }

    SECTION("in range - positive value") {
      const std::vector<double> reference = {std::nextafter(2.0, 4.0)};
      const std::vector<double> result = {2.0};
      const std::vector<double> ulp_values = {1.0};

      UlpComparator comparator(result, reference, ulp_values);

      REQUIRE(comparator.match(result));
    }
  }

  SECTION("not matches") {
    SECTION("outside by a lot") {
      const std::vector<double> reference = {10.0};
      const std::vector<double> result = {20.0};
      const std::vector<double> ulp_values = {0.0};

      UlpComparator comparator(result, reference, ulp_values);

      REQUIRE_FALSE(comparator.match(result));
    }

    SECTION("outside - positive value") {
      const std::vector<double> reference = {
          std::nextafter(std::nextafter(2.0, 4.0), 4.0)};
      const std::vector<double> result = {2.0};
      const std::vector<double> ulp_values = {1.0};

      UlpComparator comparator(result, reference, ulp_values);

      REQUIRE_FALSE(comparator.match(result));
    }

    SECTION("outside - positive value - large distance") {
      const std::vector<double> reference = {0x1p2};
      const std::vector<double> result = {0x1p1};
      const std::vector<double> ulp_values = {0x1p51};

      UlpComparator comparator(result, reference, ulp_values);

      REQUIRE_FALSE(comparator.match(result));
    }

    SECTION("outside - negative value") {
      const std::vector<double> reference = {
          std::nextafter(std::nextafter(-4.0, 2.0), 2.0)};
      const std::vector<double> result = {-4.0};
      const std::vector<double> ulp_values = {1.0};

      UlpComparator comparator(result, reference, ulp_values);

      REQUIRE_FALSE(comparator.match(result));
    }
  }
}

TEST_CASE("UlpComparator - vector", "[UlpComparator]") {
  SECTION("matches") {
    SECTION("in range - equal") {
      const std::vector<ca::Vector<double, 3>> reference = {{10.0, 15.0, 20.0}};
      const std::vector<ca::Vector<double, 3>> result = {{10.0, 15.0, 20.0}};
      const std::vector<double> ulp_values = {0.0};

      UlpComparator comparator(result, reference, ulp_values);

      REQUIRE(comparator.match(result));
    }

    SECTION("in range - positive value") {
      const std::vector<ca::Vector<double, 2>> reference = {
          {2.0, std::nextafter(2.0, 4.0)},
          {2.0, std::nextafter(std::nextafter(2.0, 4.0), 4.0)}};
      const std::vector<ca::Vector<double, 2>> result = {{2.0, 2.0},
                                                         {2.0, 2.0}};
      const std::vector<double> ulp_values = {1.0, 2.0};

      UlpComparator comparator(result, reference, ulp_values);

      REQUIRE(comparator.match(result));
    }
  }

  SECTION("not matches") {
    SECTION("outside by a lot") {
      const std::vector<ca::Vector<double, 3>> reference = {{10.0, 15.0, 20.0}};
      const std::vector<ca::Vector<double, 3>> result = {{20.0, 30.0, 40.0}};
      const std::vector<double> ulp_values = {0.0};

      UlpComparator comparator(result, reference, ulp_values);

      REQUIRE_FALSE(comparator.match(result));
    }

    SECTION("all outside") {
      const std::vector<ca::Vector<double, 2>> reference = {
          {std::nextafter(1.0, 2.0), std::nextafter(2.0, 4.0)},
          {std::nextafter(std::nextafter(2.0, 4.0), 4.0),
           std::nextafter(std::nextafter(3.0, 4.0), 4.0)}};
      const std::vector<ca::Vector<double, 2>> result = {{1.0, 2.0},
                                                         {2.0, 3.0}};
      const std::vector<double> ulp_values = {0.0, 1.0};

      UlpComparator comparator(result, reference, ulp_values);

      REQUIRE_FALSE(comparator.match(result));
    }

    SECTION("mixed outside") {
      const std::vector<ca::Vector<double, 2>> reference = {
          {std::nextafter(1.0, 2.0), std::nextafter(2.0, 4.0)},
          {std::nextafter(std::nextafter(2.0, 4.0), 4.0), 2.0}};
      const std::vector<ca::Vector<double, 2>> result = {{2.0, 2.0},
                                                         {2.0, 2.0}};
      const std::vector<double> ulp_values = {1.0, 1.0};

      UlpComparator comparator(result, reference, ulp_values);

      REQUIRE_FALSE(comparator.match(result));
    }
  }
}

TEST_CASE("Ulp Distance equals 0 when values are the same") {
  double reference = 1.0;
  float result = 1.0;

  REQUIRE(0 == calculate_ulp_distance(result, reference));
}
} // namespace