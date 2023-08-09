/*
 * Copyright (C) 2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <catch2/catch.hpp>
#include <common.hpp>
#include <test_config.hpp>
namespace ca = cassian;

namespace {

template <typename TEST_TYPE, size_t N>
void test_enqueued_num_subgroup(const TestConfig &config) {
  const std::string name = "enqueued_num_sub_group";
  ca::Runtime *runtime = config.runtime();
  const std::string program_type = config.program_type();

  const size_t global_work_size_per_dimension = config.work_size();
  std::array<size_t, N> global_work_size = {};
  std::array<size_t, N> local_work_size = {};
  size_t global_work_size_total = 1;

  calculate_dimensions(global_work_size, local_work_size,
                       global_work_size_total, global_work_size_per_dimension,
                       runtime);

  std::vector<uint32_t> input_data_values(global_work_size_total, 1);
  TestCaseDescriptor<TEST_TYPE> test_description;
  TestArguments arg1(input_data_values.data(),
                     input_data_values.size() * sizeof(uint32_t),
                     input_data_values.size());
  TestArguments arg2(input_data_values.data(),
                     input_data_values.size() * sizeof(uint32_t),
                     input_data_values.size());
  test_description.kernel_name = get_kernel_name(name);
  test_description.kernel_file_name =
      "kernels/oclc_sub_group_functions/" + name + ".cl";
  test_description.kernel_func_name = name;
  test_description.test_args.push_back(arg1);
  test_description.test_args.push_back(arg2);

  const std::vector<std::vector<uint32_t>> outputs =
      run_test<uint32_t, TEST_TYPE, N>(test_description, global_work_size,
                                       local_work_size, runtime, program_type);

  REQUIRE_THAT(outputs[0], !Catch::VectorContains(std::uint32_t(0)));
  REQUIRE_THAT(outputs[0], Catch::Equals(outputs[1]));
}

using OneTestType = std::tuple<ca::clc_uint_t>;
template <typename T> std::string test_name() {
  return std::string(T::type_name);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("enqueued_num_sub_group", "", OneTestType,
                                    test_name<TestType>) {
  const TestConfig &config = get_test_config();

  ca::Requirements requirements;
  requirements.arithmetic_type<TestType>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }
  SECTION("1D") { test_enqueued_num_subgroup<TestType, 1>(get_test_config()); }
  SECTION("2D") { test_enqueued_num_subgroup<TestType, 2>(get_test_config()); }
  SECTION("3D") { test_enqueued_num_subgroup<TestType, 3>(get_test_config()); }
}

} // namespace
