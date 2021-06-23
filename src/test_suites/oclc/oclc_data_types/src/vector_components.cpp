/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/catch2_utils/catch2_utils.hpp>
#include <cassian/cli/cli.hpp>
#include <cassian/runtime/openclc_type_tuples.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cassian/test_harness/test_harness.hpp>
#include <cassian/utility/metaprogramming.hpp>
#include <cassian/utility/utility.hpp>
#include <cassian/vector/vector.hpp>
#include <catch2/catch.hpp>
#include <common.hpp>
#include <cstddef>
#include <cstdint>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <string>
#include <test_config.hpp>
#include <utility>
#include <vector>

namespace ca = cassian;

namespace {

template <size_t N> struct VectorComponents {
  static std::vector<std::vector<int>> array_;
};

template <>
std::vector<std::vector<int>> VectorComponents<2>::array_ = {
    {0},
    {0, 1},
};

template <>
std::vector<std::vector<int>> VectorComponents<3>::array_ = {
    {0}, {0, 1}, {0, 1, 2}};

template <>
std::vector<std::vector<int>> VectorComponents<4>::array_ = {
    {0, 1, 2, 3}, {1, 2, 1}, {1, 3}};

template <>
std::vector<std::vector<int>> VectorComponents<8>::array_ = {
    {0, 1, 2, 3, 4, 5, 6, 7}, {2, 2, 2, 2}, {2, 2, 4}, {4, 4}};

template <>
std::vector<std::vector<int>> VectorComponents<16>::array_ = {
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}};

template <typename T>
std::vector<T> values_for_components(const std::vector<T> &values,
                                     const std::vector<int> &components) {
  std::vector<T> selected;
  selected.reserve(components.size());
  for (const auto &component : components) {
    selected.push_back(values[component]);
  }
  return selected;
}

template <typename T>
std::vector<T> values_for_components(const std::vector<T> &values,
                                     const std::string &component) {
  std::vector<T> selected;
  selected.reserve(values.size() / 2);
  if (component == "lo") {
    for (int i = 0; i < values.size() / 2; ++i) {
      selected.push_back(values[i]);
    }
  } else if (component == "hi") {
    for (int i = values.size() / 2; i < values.size(); ++i) {
      selected.push_back(values[i]);
    }
  } else if (component == "even") {
    for (int i = 0; i < values.size(); i += 2) {
      selected.push_back(values[i]);
    }
  } else if (component == "odd") {
    for (int i = 1; i < values.size(); i += 2) {
      selected.push_back(values[i]);
    }
  }
  return selected;
}

std::string numeric_components(const std::vector<int> &components) {
  std::stringstream ss;
  ss << "s" << std::hex;
  for (const auto &component : components) {
    ss << component;
  }
  return ss.str();
}

std::string xyzw_components(const std::vector<int> &components) {
  const std::string xyzw = "xyzw";
  std::stringstream ss;
  for (const auto &component : components) {
    if (component > 3) {
      throw std::logic_error("XYZW cannot access components beyond 4th one.");
    }
    ss << xyzw[component];
  }
  return ss.str();
}

template <typename TYPE, typename T = typename TYPE::host_type,
          typename V = typename T::value_type>
std::string constructor(const std::vector<V> &values) {
  std::string s;
  const std::string type = TYPE::scalar_type::device_type;
  s = "(" + type + std::to_string(T::vector_size) + ") (";
  for (auto value = values.begin(); value != values.end(); ++value) {
    s += std::to_string(*value);
    if (value != values.end() - 1) {
      s += ", ";
    }
  }
  s += ")";
  return s;
}

template <typename TYPE, typename V = typename TYPE::host_type::value_type>
void test(const std::string &components,
          const std::string &components_data_type, int components_count,
          const std::vector<V> &values, std::vector<V> &output,
          ca::Runtime *runtime, const std::string &program_type) {
  const std::string source = ca::load_text_file(
      ca::get_asset("kernels/oclc_data_types/vector_components.cl"));
  const std::string build_options =
      "-DDATA_TYPE=" + std::string(TYPE::device_type) + " -DVALUE=\"" +
      constructor<TYPE>(values) + "\" -DCOMPONENTS=\"" + components +
      "\" -DCOMPONENTS_DATA_TYPE=\"" + components_data_type + "\"";
  ca::Kernel kernel = runtime->create_kernel("test_kernel", source,
                                             build_options, program_type);
  run_kernel<V>(kernel, output, components_count, runtime);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("vector components - numeric indices", "",
                                    ca::VectorTypes, test_name<TestType>) {
  using type = typename TestType::host_type;
  using value_type = typename type::value_type;

  const TestConfig &config = get_test_config();

  ca::Requirements requirements;
  requirements.arithmetic_type<typename TestType::scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  std::vector<value_type> values(type::vector_size);
  std::iota(values.begin(), values.end(), value_type(1));
  std::vector<value_type> output(type::vector_size, value_type(0));
  auto components =
      GENERATE(from_range(VectorComponents<type::vector_size>::array_));
  const std::string components_data_type =
      std::string(TestType::scalar_type::device_type) +
      (components.size() > 1 ? std::to_string(components.size()) : "");
  test<TestType>(numeric_components(components), components_data_type,
                 components.size(), values, output, config.runtime(),
                 config.program_type());
  REQUIRE_THAT(output,
               Catch::Equals(values_for_components(values, components)));
}

// XYZW supported only for types with up to 4 elements
using XYZWVectorTypes =
    ca::TupleConcat<ca::Vector2Types, ca::Vector3Types, ca::Vector4Types>::type;

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("vector components - xyzw", "",
                                    XYZWVectorTypes, test_name<TestType>) {
  using type = typename TestType::host_type;
  using value_type = typename type::value_type;

  const TestConfig &config = get_test_config();

  ca::Requirements requirements;
  requirements.arithmetic_type<typename TestType::scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  std::vector<value_type> values(type::vector_size);
  std::iota(values.begin(), values.end(), value_type(1));
  std::vector<value_type> output(type::vector_size, value_type(0));
  auto components =
      GENERATE(from_range(VectorComponents<type::vector_size>::array_));
  const std::string components_data_type =
      std::string(TestType::scalar_type::device_type) +
      (components.size() > 1 ? std::to_string(components.size()) : "");
  test<TestType>(xyzw_components(components), components_data_type,
                 components.size(), values, output, config.runtime(),
                 config.program_type());
  REQUIRE_THAT(output,
               Catch::Equals(values_for_components(values, components)));
}

// FIXME: 3 element vectors return undefined values (expected behavior)
using VectorTypesWithout3 =
    ca::TupleConcat<ca::Vector2Types, ca::Vector4Types, ca::Vector8Types,
                    ca::Vector16Types>::type;

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("vector components - lo, hi, even, odd", "",
                                    VectorTypesWithout3, test_name<TestType>) {
  using type = typename TestType::host_type;
  using value_type = typename type::value_type;

  const TestConfig &config = get_test_config();

  ca::Requirements requirements;
  requirements.arithmetic_type<typename TestType::scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }
  const std::vector<std::string> components{{"lo", "hi", "even", "odd"}};
  std::vector<value_type> values(type::vector_size);
  std::iota(values.begin(), values.end(), value_type(1));
  std::vector<value_type> output(type::vector_size, value_type(0));
  auto component = GENERATE_REF(from_range(components));
  const int components_size = TestType::host_type::vector_size != 3
                                  ? TestType::host_type::vector_size / 2
                                  : 2;
  const std::string components_data_type =
      std::string(TestType::scalar_type::device_type) +
      (components_size > 1 ? std::to_string(components_size) : "");
  test<TestType>(component, components_data_type, components_size, values,
                 output, config.runtime(), config.program_type());
  REQUIRE_THAT(output, Catch::Equals(values_for_components(values, component)));
}

// TODO: writing
} // namespace
