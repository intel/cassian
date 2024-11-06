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
  double reference = 10.0;
  double result = 10.0;
  double result_outside_all = 20.0;

  double error_value = 1.0;
  PrecisionRequirement<double> error_requirement{
      PrecisionRequirementType::error_value, error_value};
  PrecisionComparator<double> error_comparator(reference, error_requirement);

  REQUIRE(error_comparator.match(result));
  REQUIRE(!error_comparator.match(result_outside_all));

  double ulp_value = 1.0;
  PrecisionRequirement<double> ulp_requirement{
      PrecisionRequirementType::ulp_value, ulp_value};
  PrecisionComparator<double> ulp_comparator(reference, ulp_requirement);

  REQUIRE(ulp_comparator.match(result));
  REQUIRE(!ulp_comparator.match(result_outside_all));

  double min_value = 9.0;
  double max_value = 11.0;
  PrecisionRequirement<double> range_requirement{
      PrecisionRequirementType::value_range, min_value, max_value};
  PrecisionComparator<double> range_comparator(reference, range_requirement);

  REQUIRE(range_comparator.match(result));
  REQUIRE(!range_comparator.match(result_outside_all));
}

TEST_CASE("PrecisionComparator matches vector result for all requirement types",
          "[PrecisionComparator]") {
  const ca::Vector<double, 3> reference = {10.0, 15.0, 20.0};
  const ca::Vector<double, 3> result = {10.0, 15.0, 20.0};
  const ca::Vector<double, 3> result_outside_all = {20.0, 30.0, 40.0};

  double error_value = 1.0;
  std::vector<PrecisionRequirement<double>> error_requirements(
      reference.size(),
      PrecisionRequirement<double>{PrecisionRequirementType::error_value,
                                   error_value});
  PrecisionComparator<ca::Vector<double, 3>> error_comparator(
      reference, error_requirements);

  REQUIRE(error_comparator.match(result));
  REQUIRE(!error_comparator.match(result_outside_all));

  double ulp_value = 1.0;
  std::vector<PrecisionRequirement<double>> ulp_requirements(
      reference.size(), PrecisionRequirement<double>{
                            PrecisionRequirementType::ulp_value, ulp_value});
  PrecisionComparator<ca::Vector<double, 3>> ulp_comparator(reference,
                                                            ulp_requirements);

  REQUIRE(ulp_comparator.match(result));
  REQUIRE(!ulp_comparator.match(result_outside_all));

  double min_value = 9.0;
  double max_value = 21.0;
  std::vector<PrecisionRequirement<double>> range_requirements(
      reference.size(),
      PrecisionRequirement<double>{PrecisionRequirementType::value_range,
                                   min_value, max_value});
  PrecisionComparator<ca::Vector<double, 3>> range_comparator(
      reference, range_requirements);

  REQUIRE(range_comparator.match(result));
  REQUIRE(!range_comparator.match(result_outside_all));
}

TEST_CASE("UlpComparator matches scalar result", "[UlpComparator]") {
  const std::vector<double> reference = {10.0};
  const std::vector<double> result = {10.0};
  const std::vector<double> ulp_values = {0.0};

  UlpComparator<double> comparator(result, reference, ulp_values);

  REQUIRE(comparator.match(result));
}

TEST_CASE("UlpComparator not matches scalar result", "[UlpComparator]") {
  const std::vector<double> reference = {10.0};
  const std::vector<double> result = {20.0};
  const std::vector<double> ulp_values = {0.0};

  UlpComparator<double> comparator(result, reference, ulp_values);

  REQUIRE(!comparator.match(result));
}

TEST_CASE("UlpComparator matches vector result", "[UlpComparator]") {
  const std::vector<ca::Vector<double, 3>> reference = {{10.0, 15.0, 20.0}};
  const std::vector<ca::Vector<double, 3>> result = {{10.0, 15.0, 20.0}};
  const std::vector<double> ulp_values = {0.0};

  UlpComparator<ca::Vector<double, 3>> comparator(result, reference,
                                                  ulp_values);

  REQUIRE(comparator.match(result));
}

TEST_CASE("UlpComparator not matches vector result", "[UlpComparator]") {
  const std::vector<ca::Vector<double, 3>> reference = {{10.0, 15.0, 20.0}};
  const std::vector<ca::Vector<double, 3>> result = {{20.0, 30.0, 40.0}};
  const std::vector<double> ulp_values = {0.0};

  UlpComparator<ca::Vector<double, 3>> comparator(result, reference,
                                                  ulp_values);

  REQUIRE(!comparator.match(result));
}
} // namespace