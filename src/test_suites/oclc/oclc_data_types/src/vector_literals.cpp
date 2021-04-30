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
#include <string>
#include <test_config.hpp>
#include <utility>
#include <vector>

namespace ca = cassian;

namespace {

template <size_t N> struct VectorConstructors {
  static std::vector<std::vector<int>> array_;
};

template <>
std::vector<std::vector<int>> VectorConstructors<2>::array_ = {
    {1, 1},
};

template <>
std::vector<std::vector<int>> VectorConstructors<3>::array_ = {
    {1, 1, 1},
    {2, 1},
};

template <>
std::vector<std::vector<int>> VectorConstructors<4>::array_ = {
    {1, 1, 1, 1}, {1, 2, 1}, {1, 3}, {3, 1}};

template <>
std::vector<std::vector<int>> VectorConstructors<8>::array_ = {
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 2, 1, 1, 1, 1, 1},
    {2, 2, 2, 2},
    {2, 2, 1, 1, 2},
    {2, 2, 4},
    {2, 4, 2},
    {4, 2, 2},
    {4, 4}};

template <>
std::vector<std::vector<int>> VectorConstructors<16>::array_ = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {2, 2, 2, 2, 2, 2, 2, 2},
    {4, 4, 4, 4},
    {8, 8}};

template <typename TYPE, typename T = typename TYPE::host_type,
          typename V = typename T::value_type>
std::string constructor(const std::vector<int> &variant,
                        const std::vector<V> &values) {
  std::string s;
  const std::string type = TYPE::scalar_type::device_type;
  if (T::vector_size != 1) {
    s = "(" + type + std::to_string(T::vector_size) + ") (";
  }
  auto values_it = values.cbegin();
  for (const auto &size : variant) {
    if (size != 1) {
      s += "(" + type + std::to_string(size) + ") (";
      for (int i = 0; i < size; ++i) {
        s += std::to_string(*values_it++);
        if (i < size - 1) {
          s += ", ";
        }
      }
      s += ")";
    } else {
      s += std::to_string(*values_it++);
    }
    if (values_it != values.end()) {
      s += ", ";
    }
  }
  if (T::vector_size != 1) {
    s += ")";
  }
  return s;
}

template <typename TYPE, typename V = typename TYPE::host_type::value_type>
void test(const std::vector<int> &variant, const std::vector<V> &values,
          std::vector<V> &output, ca::Runtime *runtime,
          const std::string &program_type) {
  const std::string source = ca::load_text_file(
      ca::get_asset("kernels/oclc_data_types/vector_literals.cl"));
  const std::string build_options =
      "-DDATA_TYPE=" + std::string(TYPE::device_type) + " -DVALUE=\"" +
      constructor<TYPE>(variant, values) + "\"";
  ca::Kernel kernel = runtime->create_kernel("test_kernel", source,
                                             build_options, program_type);
  run_kernel<V>(kernel, output, output.size(), runtime);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("vector literal definition", "",
                                    ca::VectorTypes, test_name<TestType>) {
  using type = typename TestType::host_type;
  using value_type = typename type::value_type;

  const TestConfig &config = get_test_config();

  ca::Requirements requirements;
  requirements.arithmetic_type<TestType>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  SECTION("assignment") {
    std::vector<value_type> values(type::vector_size);
    std::iota(values.begin(), values.end(), value_type(1));
    std::vector<value_type> output(type::vector_size, value_type(0));
    auto variant =
        GENERATE(from_range(VectorConstructors<type::vector_size>::array_));
    test<TestType>(variant, values, output, config.runtime(),
                   config.program_type());
    REQUIRE_THAT(output, Catch::Equals(values));
  }
}

} // namespace
