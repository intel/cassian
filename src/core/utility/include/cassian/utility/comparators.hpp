#include <cassian/fp_types/bfloat16.hpp>
#include <cassian/fp_types/math.hpp>
#include <cassian/logging/logging.hpp>
#include <cassian/runtime/openclc_types.hpp>
#include <cassian/utility/utility.hpp>
#include <catch2/catch.hpp>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

namespace ca = cassian;

enum class RequirementType : uint32_t {
  error_value,
  ulp_value,
  value_range,
  any,
  undefined
};

template <typename T> struct PrecisionRequirement {
  RequirementType type = RequirementType::ulp_value;
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

template <typename T> T ulp_distance(const T &a, const T &b) {
  if (a == b) {
    return static_cast<T>(0);
  }
  if constexpr (!std::is_integral_v<T>) {
    if (ca::isnan(a) || ca::isnan(b)) {
      return std::numeric_limits<T>::quiet_NaN();
    }
    if (ca::isinf(a) || ca::isinf(b)) {
      return std::numeric_limits<T>::infinity();
    }
  }
  return ca::abs(a - b);
}

template <typename T>
bool match_results_ulp(const T &result, const T &reference, const T ulp_value) {
  if (!std::is_integral_v<T> && cassian::isnan(result) &&
      cassian::isnan(reference)) {
    return true;
  }
  const auto ulp_dist = ulp_distance(result, reference);
  const auto ulp = ulp_value * std::numeric_limits<T>::epsilon();
  return ulp_dist <= cassian::abs(result + reference) * ulp ||
         ulp_dist < std::numeric_limits<T>::min();
}

template <typename TYPE> bool match_range(TYPE result, TYPE start, TYPE end) {
  return result >= start && result <= end;
}

template <typename T, typename cassian::EnableIfIsScalar<T> = 0>
bool match_results_error_value(const T &result, const T &reference,
                               T error_value) {

  if constexpr (!std::is_integral_v<T>) {
    if (ca::isnan(result) && ca::isnan(reference)) {
      return true;
    }
  }
  cassian::logging::debug()
      << "result = " << cassian::to_string(result)
      << "reference = " << cassian::to_string(reference)
      << "error_value = " << cassian::to_string(error_value) << '\n';
  return fabs(static_cast<float>(result) - static_cast<float>(reference)) <=
         static_cast<float>(error_value);
}

template <typename OUTPUT_TYPE, typename... INPUT_TYPES>
class PrecisionComparator : public Catch::MatcherBase<OUTPUT_TYPE> {
  OUTPUT_TYPE reference;
  std::tuple<INPUT_TYPES...> inputs;
  std::vector<PrecisionRequirement<ca::scalar_type_v<OUTPUT_TYPE>>>
      requirements;

public:
  PrecisionComparator(
      const OUTPUT_TYPE &reference,
      const PrecisionRequirement<ca::scalar_type_v<OUTPUT_TYPE>> &requirement,
      INPUT_TYPES... inputs)
      : reference(reference), inputs(inputs...), requirements{requirement} {
    static_assert(!cassian::is_vector_v<OUTPUT_TYPE>,
                  "OUTPUT_TYPE must be a scalar type.");
  }

  PrecisionComparator(
      const OUTPUT_TYPE &reference,
      const std::vector<PrecisionRequirement<ca::scalar_type_v<OUTPUT_TYPE>>>
          &requirement,
      INPUT_TYPES... inputs)
      : reference(reference), requirements(requirement), inputs(inputs...) {
    if constexpr (cassian::is_vector_v<OUTPUT_TYPE>) {
      assert(reference.size() == requirement.size());
    } else {
      assert(requirement.size() == 1);
    }
  }

  bool match(const OUTPUT_TYPE &result) const override {
    bool check = true;
    if constexpr (cassian::is_vector_v<OUTPUT_TYPE>) {
      for (int req = 0; req < requirements.size() && check == true; req++) {
        switch (requirements[req].type) {
        case RequirementType::error_value:
          check = match_results_error_value(result[req], reference[req],
                                            requirements[req].value);
          break;
        case RequirementType::ulp_value:
          check = match_results_ulp(result[req], reference[req],
                                    requirements[req].value);
          break;
        case RequirementType::value_range:
          check = match_range(result[req], requirements[req].value,
                              requirements[req].value2);
          break;
        case RequirementType::any:
          break;
        case RequirementType::undefined:
          check = true;
          break;
        default:
          throw std::invalid_argument("Requirement type not valid");
        }
      }
    } else {
      for (const auto &req : requirements) {
        switch (req.type) {
        case RequirementType::error_value:
          check = match_results_error_value(result, reference, req.value);
          break;
        case RequirementType::ulp_value:
          check = match_results_ulp(result, reference, req.value);
          break;
        case RequirementType::value_range:
          check = match_range(result, req.value, req.value2);
          break;
        case RequirementType::any:
          break;
        case RequirementType::undefined:
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
      case RequirementType::value_range:
        description << "In Range {" << ca::to_string(req.value) << ", "
                    << ca::to_string(req.value2) << "}";
        break;
      case RequirementType::error_value:
        if constexpr (ca::is_vector_v<OUTPUT_TYPE>) {
          description << "\nReference[" << req_index
                      << "]: " << ca::to_string(reference[req_index]);
        } else {
          description << "\nReference: " << ca::to_string(reference);
        }
        description << " Absolute Error within " << ca::to_string(req.value);
        break;
      case RequirementType::ulp_value:
        if constexpr (ca::is_vector_v<OUTPUT_TYPE>) {
          description << "\nReference[" << req_index
                      << "]: " << ca::to_string(reference[req_index]);
        } else {
          description << "\nReference: " << ca::to_string(reference);
        }
        description << " ULP Error within " << ca::to_string(req.value);
        break;
      case RequirementType::any:
        description << "\nAny Value Pass";
        break;
      default:
        description << "\nRequirement Undefined";
        break;
      }
      if (req_index < requirements.size() - 1) {
        description << "\n";
      }
    }

    description << "\n" << inputs_to_string(inputs);
    return description.str();
  }

private:
  template <size_t... I>
  std::string inputs_to_string(std::tuple<INPUT_TYPES...> in) const {
    if constexpr (sizeof...(I) == 0) {
      return "Inputs: None";
    } else {
      return "Inputs: " + ((ca::to_string(std::get<I>(in)) + ",") + ...);
    }
  }
};

template <class OUTPUT_TYPE>
class UlpComparator : public Catch::MatcherBase<std::vector<OUTPUT_TYPE>> {
  using scalar_type = cassian::scalar_type_v<OUTPUT_TYPE>;
  std::vector<OUTPUT_TYPE> reference;
  std::vector<scalar_type> ulp_values;
  std::vector<OUTPUT_TYPE> result;

public:
  UlpComparator(const std::vector<OUTPUT_TYPE> &result,
                const std::vector<OUTPUT_TYPE> &reference,
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
          os << ulp_distance(result[i][j], reference[i][j]) << ", ";
        }
        os << '}';
      } else {
        os << ulp_distance(result[i], reference[i]) << ", ";
      }
    }
    os << '}';
    return "\nreference: " + input_to_string<OUTPUT_TYPE>(reference) +
           "\nULP distance: " + os.str();
  }
};