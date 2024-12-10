#include <cassian/fp_types/bfloat16.hpp>
#include <cassian/fp_types/math.hpp>
#include <cassian/logging/logging.hpp>
#include <cassian/runtime/openclc_types.hpp>
#include <cassian/utility/utility.hpp>
#include <catch2/catch.hpp>
#include <limits>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

namespace ca = cassian;

enum class PrecisionRequirementType : uint32_t {
  error_value,
  ulp_value,
  value_range,
  any,
  undefined
};

template <typename T> struct PrecisionRequirement {
  PrecisionRequirementType type = PrecisionRequirementType::ulp_value;
  T value = static_cast<T>(0);
  T value2 = static_cast<T>(0);
};

template <typename T> std::string input_to_string(const std::vector<T> &input) {
  std::stringstream ss;
  ss << "{";
  for (const auto &input_val : input) {
    ss << ca::to_string<T>(input_val) << ", ";
  }
  ss.seekp(-2, std::ios_base::end);
  ss << "}";
  return ss.str();
}

template <typename RESULT_TYPE, typename REFERENCE_TYPE>
REFERENCE_TYPE calculate_ulp_distance(RESULT_TYPE result,
                                      REFERENCE_TYPE reference) {
  if constexpr (!ca::is_floating_point_v<RESULT_TYPE>) {
    static_assert(std::is_same_v<RESULT_TYPE, REFERENCE_TYPE>);
    return ca::abs(reference - result);
  }
  int reference_frexp_exponent;
  REFERENCE_TYPE normalized_fraction =
      std::frexp(reference, &reference_frexp_exponent);
  const int reference_exponent = reference_frexp_exponent - 1;
  const int min_exponent = std::numeric_limits<RESULT_TYPE>::min_exponent - 1;
  const int mantissa_length = std::numeric_limits<RESULT_TYPE>::digits;
  int ulp_exponent;
  if (normalized_fraction != 0.5) {
    ulp_exponent =
        std::max(reference_exponent, min_exponent) - mantissa_length + 1;
  } else {
    ulp_exponent =
        std::max(reference_exponent - 1, min_exponent) - mantissa_length + 1;
  }
  const REFERENCE_TYPE absolute_distance = std::fabs(reference - result);
  const REFERENCE_TYPE ulp_distance =
      std::scalbn(absolute_distance, -ulp_exponent);
  return ulp_distance;
}

template <typename RESULT_TYPE, typename REFERENCE_TYPE>
bool match_results_ulp(const RESULT_TYPE &result,
                       const REFERENCE_TYPE &reference,
                       const RESULT_TYPE ulp_value) {
  // this if constexpr is needed for msvc compatibility, isnan doesn't work with
  // integers
  if constexpr (ca::is_floating_point_v<RESULT_TYPE>) {
    if (cassian::isnan(result) && cassian::isnan(reference)) {
      return true;
    }
    if (result == static_cast<RESULT_TYPE>(reference)) {
      return true;
    }
  }
  const auto ulp_distance = calculate_ulp_distance(result, reference);
  if (ulp_distance <= static_cast<REFERENCE_TYPE>(ulp_value)) {
    return true;
  }
  return false;
}

template <typename TYPE> bool match_range(TYPE result, TYPE start, TYPE end) {
  return result >= start && result <= end;
}

template <typename RESULT_TYPE, typename REFERENCE_TYPE,
          typename cassian::EnableIfIsScalar<RESULT_TYPE> = 0>
bool match_results_error_value(const RESULT_TYPE &result,
                               const REFERENCE_TYPE &reference,
                               RESULT_TYPE error_value) {

  if constexpr (!std::is_integral_v<RESULT_TYPE>) {
    if (ca::isnan(result) && ca::isnan(reference)) {
      return true;
    }
  }
  cassian::logging::debug()
      << "result = " << cassian::to_string(result)
      << "reference = " << cassian::to_string(reference)
      << "error_value = " << cassian::to_string(error_value) << '\n';
  return fabs(static_cast<REFERENCE_TYPE>(result) -
              static_cast<REFERENCE_TYPE>(reference)) <=
         static_cast<REFERENCE_TYPE>(error_value);
}

template <typename RESULT_TYPE, typename REFERENCE_TYPE,
          typename... INPUT_TYPES>
class PrecisionComparator : public Catch::MatcherBase<RESULT_TYPE> {
  RESULT_TYPE result;
  REFERENCE_TYPE reference;
  std::tuple<INPUT_TYPES...> inputs;
  std::vector<PrecisionRequirement<ca::scalar_type_v<RESULT_TYPE>>>
      requirements;

public:
  PrecisionComparator(
      const RESULT_TYPE &result, const REFERENCE_TYPE &reference,
      const PrecisionRequirement<ca::scalar_type_v<RESULT_TYPE>> &requirement,
      INPUT_TYPES... inputs)
      : result(result), reference(reference),
        inputs(inputs...), requirements{requirement} {
    static_assert(!cassian::is_vector_v<REFERENCE_TYPE>,
                  "REFERENCE_TYPE must be a scalar type.");
  }

  PrecisionComparator(
      const RESULT_TYPE &result, const REFERENCE_TYPE &reference,
      const std::vector<PrecisionRequirement<ca::scalar_type_v<RESULT_TYPE>>>
          &requirement,
      INPUT_TYPES... inputs)
      : result(result), reference(reference), requirements(requirement),
        inputs(inputs...) {
    if constexpr (cassian::is_vector_v<RESULT_TYPE>) {
      assert(reference.size() == requirement.size());
    } else {
      assert(requirement.size() == 1);
    }
  }

  bool match(const RESULT_TYPE &result) const override {
    bool check = true;
    if constexpr (cassian::is_vector_v<RESULT_TYPE>) {
      for (int req = 0; req < requirements.size() && check == true; req++) {
        if (result[req] == static_cast<RESULT_TYPE>(reference[req])) {
          continue;
        }
        switch (requirements[req].type) {
        case PrecisionRequirementType::error_value:
          check = match_results_error_value(result[req], reference[req],
                                            requirements[req].value);
          break;
        case PrecisionRequirementType::ulp_value:
          check = match_results_ulp(result[req], reference[req],
                                    requirements[req].value);
          break;
        case PrecisionRequirementType::value_range:
          check = match_range(result[req], requirements[req].value,
                              requirements[req].value2);
          break;
        case PrecisionRequirementType::any:
          break;
        case PrecisionRequirementType::undefined:
          check = true;
          break;
        default:
          throw std::invalid_argument("Requirement type not valid");
        }
      }
    } else {
      for (const auto &req : requirements) {
        if (result == static_cast<RESULT_TYPE>(reference)) {
          continue;
        }
        switch (req.type) {
        case PrecisionRequirementType::error_value:
          check = match_results_error_value(result, reference, req.value);
          break;
        case PrecisionRequirementType::ulp_value:
          check = match_results_ulp(result, reference, req.value);
          break;
        case PrecisionRequirementType::value_range:
          check = match_range(result, req.value, req.value2);
          break;
        case PrecisionRequirementType::any:
          break;
        case PrecisionRequirementType::undefined:
          check = true;
          break;
        default:
          throw std::invalid_argument("Requirement type not valid");
        }
        if (!check) {
          break;
        }
      }
    }
    return check;
  }

  std::string describe() const override {
    std::ostringstream description;
    for (size_t req_index = 0; req_index < requirements.size(); ++req_index) {
      const auto &req = requirements[req_index];
      switch (req.type) {
      case PrecisionRequirementType::value_range:
        description << "In range {" << ca::to_string(req.value) << ", "
                    << ca::to_string(req.value2) << "}";
        break;
      case PrecisionRequirementType::error_value:
        if constexpr (ca::is_vector_v<RESULT_TYPE>) {
          description << "\nReference[" << req_index
                      << "]: " << ca::to_string(reference[req_index]);
        } else {
          description << "\nReference: " << ca::to_string(reference);
        }
        description << " Absolute error within " << ca::to_string(req.value);
        break;
      case PrecisionRequirementType::ulp_value:
        if constexpr (ca::is_vector_v<RESULT_TYPE>) {
          description << "\nReference[" << req_index
                      << "]: " << ca::to_string(reference[req_index])
                      << "\n ULP error "
                      << calculate_ulp_distance(result[req_index],
                                                reference[req_index])
                      << " within " << ca::to_string(req.value);
        } else {
          description << "\nReference: " << ca::to_string(reference)
                      << "\n ULP error "
                      << calculate_ulp_distance(result, reference) << " within "
                      << ca::to_string(req.value);
        }

        break;
      case PrecisionRequirementType::any:
        description << "\nAny value pass";
        break;
      default:
        description << "\nRequirement undefined";
        break;
      }
      if (req_index < requirements.size() - 1) {
        description << "\n";
      }
    }

    description << "\n"
                << inputs_to_string(
                       inputs,
                       std::make_index_sequence<sizeof...(INPUT_TYPES)>());
    return description.str();
  }

private:
  template <size_t... I>
  std::string inputs_to_string(std::tuple<INPUT_TYPES...> in,
                               std::index_sequence<I...> /*unused*/) const {
    if constexpr (sizeof...(I) == 0) {
      return "Inputs: None";
    } else {
      return "Inputs: " + ((ca::to_string(std::get<I>(in)) + ",") + ...);
    }
  }
};

template <class OUTPUT_TYPE, class REFERENCE_TYPE>
class UlpComparator : public Catch::MatcherBase<std::vector<OUTPUT_TYPE>> {
  using scalar_type = cassian::scalar_type_v<OUTPUT_TYPE>;
  std::vector<REFERENCE_TYPE> reference;
  std::vector<scalar_type> ulp_values;
  std::vector<OUTPUT_TYPE> result;

public:
  UlpComparator(const std::vector<OUTPUT_TYPE> &result,
                const std::vector<REFERENCE_TYPE> &reference,
                const std::vector<scalar_type> &ulp_values)
      : reference(reference), result(result), ulp_values(ulp_values) {}
  bool match(std::vector<OUTPUT_TYPE> const &result) const override {
    for (auto i = 0U; i < result.size(); i++) {
      if constexpr (cassian::is_vector_v<OUTPUT_TYPE>) {
        for (int j = 0; j < result[i].size(); j++) {
          if (!match_results_ulp(result[i][j], reference[i][j],
                                 ulp_values[i])) {
            return false;
          }
        }
      } else {
        if (!match_results_ulp(result[i], reference[i], ulp_values[i])) {
          return false;
        }
      }
    }
    return true;
  }

  std::string describe() const override {
    auto ulp_diffs = result;
    std::stringstream os;
    os.precision(std::numeric_limits<double>::max_digits10);
    os << '{';
    for (auto i = 0; i < result.size(); i++) {
      if constexpr (cassian::is_vector_v<OUTPUT_TYPE>) {
        os << '{';
        for (int j = 0; j < result[i].size(); j++) {
          os << calculate_ulp_distance(result[i][j], reference[i][j]) << ", ";
        }
        os << '}';
      } else {
        os << calculate_ulp_distance(result[i], reference[i]) << ", ";
      }
    }
    os << '}';
    return "\nReference: " + input_to_string<REFERENCE_TYPE>(reference) +
           "\nULP distance: " + os.str();
  }
};