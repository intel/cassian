#include <cassian/runtime/openclc_types.hpp>
#include <cassian/utility/comparators.hpp>
#include <cassian/utility/utility.hpp>
#include <cassian/vector/vector.hpp>
#include <catch2/catch.hpp>
#include <cstdint>
#include <vector>

namespace {

TEST_CASE("PrecisionComparator matches scalar result for all requirement types",
          "[PrecisionComparator]") {
  const double reference = 10.0;
  const double result = 10.0;

  SECTION("error_value") {
    const double error_value = 1.0;
    PrecisionRequirement<double> error_requirement{
        PrecisionRequirementType::error_value, error_value};
    PrecisionComparator error_comparator(result, reference, error_requirement);

    REQUIRE(error_comparator.match(result));

    const double result_in_range_up = 10.999;
    REQUIRE(error_comparator.match(result_in_range_up));

    const double result_in_range_down = 9.001;
    REQUIRE(error_comparator.match(result_in_range_down));

    const double result_over = 11.001;
    REQUIRE_FALSE(error_comparator.match(result_over));

    const double result_below = 8.999;
    REQUIRE_FALSE(error_comparator.match(result_below));
  }

  SECTION("ulp_value") {
    const double ulp_value = 1.0;
    PrecisionRequirement<double> ulp_requirement{
        PrecisionRequirementType::ulp_value, ulp_value};
    PrecisionComparator ulp_comparator(result, reference, ulp_requirement);

    REQUIRE(ulp_comparator.match(result));

    const double result_outside = 20.0;
    REQUIRE_FALSE(ulp_comparator.match(result_outside));
  }

  SECTION("value_range") {
    const double min_value = 9.0;
    const double max_value = 11.0;
    PrecisionRequirement<double> range_requirement{
        PrecisionRequirementType::value_range, min_value, max_value};
    PrecisionComparator range_comparator(result, reference, range_requirement);

    REQUIRE(range_comparator.match(result));

    const double result_equal_max = max_value;
    REQUIRE(range_comparator.match(result_equal_max));

    const double result_equal_min = min_value;
    REQUIRE(range_comparator.match(result_equal_min));

    const double result_over = 11.1;
    REQUIRE_FALSE(range_comparator.match(result_over));

    const double result_below = 8.9;
    REQUIRE_FALSE(range_comparator.match(result_below));
  }
}

TEST_CASE("PrecisionComparator matches vector result for all requirement types",
          "[PrecisionComparator]") {
  const ca::Vector<double, 3> reference = {10.0, 15.0, 20.0};
  const ca::Vector<double, 3> result = {10.0, 15.0, 20.0};

  SECTION("error_value") {
    double error_value = 1.0;
    std::vector<PrecisionRequirement<double>> error_requirements(
        reference.size(),
        PrecisionRequirement<double>{PrecisionRequirementType::error_value,
                                     error_value});
    PrecisionComparator error_comparator(result, reference, error_requirements);

    REQUIRE(error_comparator.match(result));

    const ca::Vector<double, 3> result_in_range_up_all = {10.999, 15.999,
                                                          20.999};
    REQUIRE(error_comparator.match(result_in_range_up_all));

    const ca::Vector<double, 3> result_in_range_down_all = {9.001, 14.001,
                                                            19.001};
    REQUIRE(error_comparator.match(result_in_range_down_all));

    const ca::Vector<double, 3> result_over_all = {11.001, 16.001, 21.001};
    REQUIRE_FALSE(error_comparator.match(result_over_all));

    const ca::Vector<double, 3> result_below_all = {8.999, 13.999, 18.999};
    REQUIRE_FALSE(error_comparator.match(result_below_all));

    const ca::Vector<double, 3> result_outside_all = {2, 100, 10};
    REQUIRE_FALSE(error_comparator.match(result_outside_all));

    const ca::Vector<double, 3> result_mixed = {10, 16, 10};
    REQUIRE_FALSE(error_comparator.match(result_mixed));
  }

  SECTION("ulp_value") {
    double ulp_value = 1.0;
    std::vector<PrecisionRequirement<double>> ulp_requirements(
        reference.size(), PrecisionRequirement<double>{
                              PrecisionRequirementType::ulp_value, ulp_value});
    PrecisionComparator ulp_comparator(result, reference, ulp_requirements);

    REQUIRE(ulp_comparator.match(result));

    const ca::Vector<double, 3> result_outside_all = {2, 100, 10};
    REQUIRE_FALSE(ulp_comparator.match(result_outside_all));
  }

  SECTION("value_range") {
    double min_value = 9.0;
    double max_value = 21.0;
    std::vector<PrecisionRequirement<double>> range_requirements(
        reference.size(),
        PrecisionRequirement<double>{PrecisionRequirementType::value_range,
                                     min_value, max_value});
    PrecisionComparator range_comparator(result, reference, range_requirements);

    REQUIRE(range_comparator.match(result));

    const ca::Vector<double, 3> result_in_range_all = {max_value, min_value,
                                                       20.5};
    REQUIRE(range_comparator.match(result_in_range_all));

    const ca::Vector<double, 3> result_outside_all = {8, 100, 22};
    REQUIRE_FALSE(range_comparator.match(result_outside_all));

    const ca::Vector<double, 3> result_mixed = {max_value, 20, 22};
    REQUIRE_FALSE(range_comparator.match(result_mixed));
  }
}

TEST_CASE("UlpComparator matches scalar result", "[UlpComparator]") {
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

  SECTION("in range - positive value - large distance") {
    const std::vector<double> reference = {0x1p2};
    const std::vector<double> result = {0x1p1};
    const std::vector<double> ulp_values = {0x1p52};

    UlpComparator comparator(result, reference, ulp_values);

    REQUIRE(comparator.match(result));
  }

  SECTION("in range - negative value") {
    const std::vector<double> reference = {std::nextafter(-4.0, 2.0)};
    const std::vector<double> result = {-4.0};
    const std::vector<double> ulp_values = {1.0};

    UlpComparator comparator(result, reference, ulp_values);

    REQUIRE(comparator.match(result));
  }
}

TEST_CASE("UlpComparator not matches scalar result", "[UlpComparator]") {
  SECTION("outside") {
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

TEST_CASE("UlpComparator matches vector result", "[UlpComparator]") {
  SECTION("in range - equal") {
    const std::vector<ca::Vector<double, 3>> reference = {{10.0, 15.0, 20.0}};
    const std::vector<ca::Vector<double, 3>> result = {{10.0, 15.0, 20.0}};
    const std::vector<double> ulp_values = {0.0};

    UlpComparator comparator(result, reference, ulp_values);

    REQUIRE(comparator.match(result));
  }

  SECTION("in range - positive value") {
    const std::vector<double> reference = {
        2.0, std::nextafter(2.0, 4.0),
        std::nextafter(std::nextafter(2.0, 4.0), 4.0)};
    const std::vector<double> result = {2.0, 2.0, 2.0};
    const std::vector<double> ulp_values = {0.0, 1.0, 2.0};

    UlpComparator comparator(result, reference, ulp_values);

    REQUIRE(comparator.match(result));
  }
}

TEST_CASE("UlpComparator not matches vector result", "[UlpComparator]") {
  SECTION("outside") {
    const std::vector<ca::Vector<double, 3>> reference = {{10.0, 15.0, 20.0}};
    const std::vector<ca::Vector<double, 3>> result = {{20.0, 30.0, 40.0}};
    const std::vector<double> ulp_values = {0.0};

    UlpComparator comparator(result, reference, ulp_values);

    REQUIRE_FALSE(comparator.match(result));
  }

  SECTION("all outside") {
    const std::vector<double> reference = {
        std::nextafter(1.0, 2.0), std::nextafter(2.0, 4.0),
        std::nextafter(std::nextafter(2.0, 4.0), 4.0)};
    const std::vector<double> result = {2.0, 2.0, 2.0};
    const std::vector<double> ulp_values = {0.0, 0.0, 1.0};

    UlpComparator comparator(result, reference, ulp_values);

    REQUIRE_FALSE(comparator.match(result));
  }

  SECTION("mixed outside") {
    const std::vector<double> reference = {
        std::nextafter(1.0, 2.0), std::nextafter(2.0, 4.0),
        std::nextafter(std::nextafter(2.0, 4.0), 4.0)};
    const std::vector<double> result = {2.0, 2.0, 2.0};
    const std::vector<double> ulp_values = {1.0, 1.0, 1.0};

    UlpComparator comparator(result, reference, ulp_values);

    REQUIRE_FALSE(comparator.match(result));
  }
}

TEST_CASE("Ulp Distance equals 0 when values are the same") {
  double reference = 1.0;
  float result = 1.0;

  REQUIRE(0 == calculate_ulp_distance(result, reference));
}
} // namespace