/*
 * Copyright (C) 2024-2025 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_UTILITY_COMPARATORS_HPP
#define CASSIAN_UTILITY_COMPARATORS_HPP

#include <catch2/catch.hpp>

#include <cassian/fp_types/bfloat16.hpp>
#include <cassian/fp_types/math.hpp>
#include <cassian/logging/logging.hpp>
#include <cassian/runtime/openclc_types.hpp>
#include <cassian/utility/utility.hpp>
#include <cassian/vector/vector.hpp>
#include <catch2/catch.hpp>
#include <cstring>
#include <iomanip>
#include <limits>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

namespace cassian {

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

// --- helpers for hex formatting ---
namespace detail_hex_fmt {
template <class T,
          class U = std::conditional_t<(sizeof(T) <= 4), uint32_t, uint64_t>>
inline std::enable_if_t<std::is_floating_point_v<T>, std::string>
raw_bits(const T &v) {
  U bits{};
  std::memcpy(&bits, &v, sizeof(T));
  std::ostringstream os;
  os << "0x" << std::hex << std::uppercase << std::setw(sizeof(T) * 2)
     << std::setfill('0') << bits;
  return os.str();
}

template <class T>
inline std::enable_if_t<std::is_integral_v<T>, std::string>
raw_bits(const T &v) {
  using Uns = std::make_unsigned_t<T>;
  std::ostringstream os;
  os << "0x" << std::hex << std::uppercase << std::setw(sizeof(T) * 2)
     << std::setfill('0') << static_cast<Uns>(v);
  return os.str();
}

template <class V>
inline std::enable_if_t<is_vector_v<V>, std::string> raw_bits(const V &vec) {
  std::ostringstream os;
  os << '{';
  for (int i = 0; i < vec.size(); ++i) {
    os << raw_bits(vec[i]);
    if (i + 1 < vec.size()) {
      os << ", ";
    }
  }
  os << '}';
  return os.str();
}

template <class T> inline std::string dec_hex_pair(const T &v) {
  if constexpr (std::is_integral_v<T>) {
    return to_string(v) + " (" + raw_bits(v) + ")";
  } else if constexpr (std::is_floating_point_v<T>) {
    std::ostringstream os;
    os.setf(std::ios::scientific);
    os << std::setprecision(std::numeric_limits<T>::max_digits10) << v;
    return os.str() + " (" + raw_bits(v) + ")";
  } else if constexpr (cassian::is_floating_point_v<T>) {
    std::ostringstream os;
    os.setf(std::ios::scientific);
    os << std::setprecision(std::numeric_limits<T>::max_digits10)
       << static_cast<float>(v);
    return os.str() + " (0x" + cassian::to_string(v) + ")";
  } else if constexpr (is_vector_v<T>) {
    std::ostringstream os;
    os << '{';
    for (int i = 0; i < v.size(); ++i) {
      os << dec_hex_pair(v[i]);
      if (i + 1 < v.size()) {
        os << ", ";
      }
    }
    os << '}';
    return os.str();
  } else {
    return to_string(v);
  }
}

} // namespace detail_hex_fmt

template <typename RESULT_TYPE, typename REFERENCE_TYPE>
REFERENCE_TYPE calculate_ulp_distance(RESULT_TYPE result,
                                      REFERENCE_TYPE reference) {
  using cassian::abs;
  using std::abs;
  if constexpr (!is_floating_point_v<RESULT_TYPE>) {
    static_assert(std::is_same_v<RESULT_TYPE, REFERENCE_TYPE>);
    return abs(reference - result);
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
  using std::isinf;
  using std::isnan;
  // this if constexpr is needed for msvc compatibility, isnan doesn't work with
  // integers
  if constexpr (is_floating_point_v<RESULT_TYPE>) {
    if (isinf(result) && isinf(reference)) {
      return true;
    }
    if (isnan(result) && isnan(reference)) {
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
          EnableIfIsScalar<RESULT_TYPE> = 0>
bool match_results_error_value(const RESULT_TYPE &result,
                               const REFERENCE_TYPE &reference,
                               RESULT_TYPE error_value) {
  using std::fabs;
  using std::isnan;
  if constexpr (!std::is_integral_v<RESULT_TYPE>) {
    if (isnan(result) && isnan(reference)) {
      return true;
    }
  }
  logging::debug() << "result = " << detail_hex_fmt::dec_hex_pair(result)
                   << " reference = " << detail_hex_fmt::dec_hex_pair(reference)
                   << " error_value = "
                   << detail_hex_fmt::dec_hex_pair(error_value) << '\n';
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
  std::vector<PrecisionRequirement<scalar_type_v<RESULT_TYPE>>> requirements;

public:
  PrecisionComparator(
      const RESULT_TYPE &result, const REFERENCE_TYPE &reference,
      const PrecisionRequirement<scalar_type_v<RESULT_TYPE>> &requirement,
      INPUT_TYPES... inputs)
      : result(result), reference(reference),
        inputs(inputs...), requirements{requirement} {
    static_assert(!is_vector_v<REFERENCE_TYPE>,
                  "REFERENCE_TYPE must be a scalar type.");
  }

  PrecisionComparator(
      const RESULT_TYPE &result, const REFERENCE_TYPE &reference,
      const std::vector<PrecisionRequirement<scalar_type_v<RESULT_TYPE>>>
          &requirement,
      INPUT_TYPES... inputs)
      : result(result), reference(reference), requirements(requirement),
        inputs(inputs...) {
    if constexpr (is_vector_v<RESULT_TYPE>) {
      assert(reference.size() == requirement.size());
    } else {
      assert(requirement.size() == 1);
    }
  }

  bool match(const RESULT_TYPE &result) const override {
    bool check = true;
    if constexpr (is_vector_v<RESULT_TYPE>) {
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
        description << "In range {" << to_string(req.value) << ", "
                    << to_string(req.value2) << "}";
        break;
      case PrecisionRequirementType::error_value:
        if constexpr (is_vector_v<RESULT_TYPE>) {
          description << "\nReference[" << req_index << "]: "
                      << detail_hex_fmt::dec_hex_pair(reference[req_index]);
        } else {
          description << "\nReference: "
                      << detail_hex_fmt::dec_hex_pair(reference);
        }
        description << " Absolute error within " << to_string(req.value);
        break;
      case PrecisionRequirementType::ulp_value:
        if constexpr (is_vector_v<RESULT_TYPE>) {
          description << "\nReference[" << req_index << "]: "
                      << detail_hex_fmt::dec_hex_pair(reference[req_index])
                      << "\n ULP error "
                      << calculate_ulp_distance(result[req_index],
                                                reference[req_index])
                      << " within " << to_string(req.value);
        } else {
          description << "\nReference: "
                      << detail_hex_fmt::dec_hex_pair(reference)
                      << "\n ULP error "
                      << calculate_ulp_distance(result, reference) << " within "
                      << to_string(req.value);
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
    // Add result values
    if constexpr (is_vector_v<RESULT_TYPE>) {
      description << "\nResult: {";
      for (int i = 0; i < result.size(); ++i) {
        description << detail_hex_fmt::dec_hex_pair(result[i]);
        if (i + 1 < result.size()) {
          description << ", ";
        }
      }
      description << '}';
    } else {
      description << "\nResult: " << detail_hex_fmt::dec_hex_pair(result);
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
      return "Inputs: " + ((std::string(I ? ", " : "") +
                            detail_hex_fmt::dec_hex_pair(std::get<I>(in))) +
                           ...);
    }
  }
};

template <class OUTPUT_TYPE, class REFERENCE_TYPE, class... INPUTS>
class UlpComparator : public Catch::MatcherBase<std::vector<OUTPUT_TYPE>> {
  using scalar_type = scalar_type_v<OUTPUT_TYPE>;
  std::vector<REFERENCE_TYPE> reference;
  std::vector<scalar_type> ulp_values;
  std::vector<OUTPUT_TYPE> result;
  std::tuple<INPUTS...> input;

public:
  UlpComparator(const std::vector<OUTPUT_TYPE> &result,
                const std::vector<REFERENCE_TYPE> &reference,
                const std::vector<scalar_type> &ulp_values, INPUTS... input)
      : reference(reference), result(result), ulp_values(ulp_values),
        input(input...) {}

  bool match(std::vector<OUTPUT_TYPE> const &result) const override {
    for (auto i = 0U; i < result.size(); i++) {
      if constexpr (is_vector_v<OUTPUT_TYPE>) {
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
    os << "\nReference: {";
    for (size_t i = 0; i < reference.size(); ++i) {
      os << detail_hex_fmt::dec_hex_pair(reference[i]);
      if (i + 1 < reference.size()) {
        os << ", ";
      }
    }
    os << "}";

    // Results
    os << "\nResult: {";
    for (size_t i = 0; i < result.size(); ++i) {
      if constexpr (is_vector_v<OUTPUT_TYPE>) {
        os << '{';
        for (int j = 0; j < result[i].size(); ++j) {
          os << detail_hex_fmt::dec_hex_pair(result[i][j]);
          if (j + 1 < result[i].size()) {
            os << ", ";
          }
        }
        os << '}';
      } else {
        os << detail_hex_fmt::dec_hex_pair(result[i]);
      }
      if (i + 1 < result.size()) {
        os << ", ";
      }
    }
    os << "}";

    os << "\nULP distance: ";
    os << '{';
    for (auto i = 0; i < result.size(); i++) {
      if constexpr (is_vector_v<OUTPUT_TYPE>) {
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
    os << "\nInputs: "
       << inputs_to_string(std::make_index_sequence<sizeof...(INPUTS)>());
    return os.str();
  }

private:
  template <size_t... I>
  std::string inputs_to_string(std::index_sequence<I...>) const {
    std::stringstream os;
    if constexpr (sizeof...(I) == 0) {
      os << "None";
    } else {
      os << '{';
      size_t max_size = std::max({std::get<I>(input).size()...});
      for (size_t i = 0; i < max_size; ++i) {
        std::apply(
            [&os, i](const auto &... vecs) {
              os << '{';
              ((vecs.size() > i
                    ? os << detail_hex_fmt::dec_hex_pair(vecs[i]) << ", "
                    : os << ""),
               ...);
              os << '}';
            },
            input);
      }
      os << '}';
    }
    return os.str();
  }
};

} // namespace cassian

#endif
