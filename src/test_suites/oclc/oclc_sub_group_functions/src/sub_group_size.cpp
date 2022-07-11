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
void test_subgroup_size(const TestConfig &config) {
  const std::string name = "sub_group_size";
  ca::Runtime *runtime = config.runtime();
  const std::string program_type = config.program_type();

  const size_t global_work_size_per_dimension = config.work_size();
  std::array<size_t, N> global_work_size = {};
  std::array<size_t, N> local_work_size = {};
  size_t global_work_size_total = 1;

  calculate_dimensions(global_work_size, local_work_size,
                       global_work_size_total, global_work_size_per_dimension,
                       runtime);

  SECTION("BASIC") {
    std::vector<uint32_t> input_data_values(global_work_size_total, 1);
    TestCaseDescriptor<TEST_TYPE> test_description;
    TestArguments arg1;
    TestArguments arg2;
    test_description.kernel_name = get_kernel_name(name);
    test_description.kernel_file_name =
        "kernels/oclc_sub_group_functions/" + name + ".cl";
    test_description.kernel_func_name = name;

    arg1.data = input_data_values.data();
    arg1.data_count = input_data_values.size();
    arg1.data_size = input_data_values.size() * sizeof(uint32_t);

    arg2.data = input_data_values.data();
    arg2.data_count = input_data_values.size();
    arg2.data_size = input_data_values.size() * sizeof(uint32_t);
    test_description.test_args.push_back(arg1);
    test_description.test_args.push_back(arg2);

    const std::vector<std::vector<uint32_t>> outputs =
        run_test<uint32_t, TEST_TYPE, N>(test_description, global_work_size,
                                         local_work_size, runtime,
                                         program_type);

    uint32_t max_sub_group_id =
        *std::max_element(outputs[1].begin(), outputs[1].end());
    const std::vector<uint32_t> references(arg1.data_count, max_sub_group_id);
    REQUIRE_THAT(outputs[0], Catch::Equals(references));
  }

  SECTION("MAX_VALUE") {
    std::vector<uint32_t> input_data_values(global_work_size_total, 1);
    TestCaseDescriptor<TEST_TYPE> test_description;
    TestArguments arg1;
    test_description.kernel_name = get_kernel_name(name + "_max_value");
    test_description.kernel_file_name =
        "kernels/oclc_sub_group_functions/" + name + ".cl";
    test_description.kernel_func_name = name + "_max_value";

    arg1.data = input_data_values.data();
    arg1.data_count = input_data_values.size();
    arg1.data_size = input_data_values.size() * sizeof(uint32_t);
    test_description.test_args.push_back(arg1);

    const std::vector<std::vector<uint32_t>> outputs =
        run_test<uint32_t, TEST_TYPE, N>(test_description, global_work_size,
                                         local_work_size, runtime,
                                         program_type);
    const std::vector<uint32_t> reference(arg1.data_count, 1);
    REQUIRE_THAT(outputs[0], Catch::Equals(reference));
  }
}

using OneTestType = std::tuple<ca::clc_uint_t>;
template <typename T> std::string test_name() {
  return std::string(T::type_name);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("sub_group_size", "", OneTestType,
                                    test_name<TestType>) {
  const TestConfig &config = get_test_config();

  ca::Requirements requirements;
  requirements.arithmetic_type<TestType>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }
  SECTION("1D") { test_subgroup_size<TestType, 1>(get_test_config()); }
  SECTION("2D") { test_subgroup_size<TestType, 2>(get_test_config()); }
  SECTION("3D") { test_subgroup_size<TestType, 3>(get_test_config()); }
}

} // namespace
