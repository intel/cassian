/*
 * Copyright (C) 2022-2024 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <catch2/catch.hpp>
#include <common.hpp>
#include <numeric>
#include <test_config.hpp>
namespace ca = cassian;

namespace {

template <typename TEST_TYPE, size_t N>
void test_num_subgroup(const TestConfig &config, const std::string &name) {
  ca::Runtime *runtime = config.runtime();
  const std::string program_type = config.program_type();

  const size_t global_work_size_per_dimension = config.work_size();
  std::array<size_t, N> global_work_size = {};
  std::array<size_t, N> local_work_size = {};
  size_t global_work_size_total = 1;

  calculate_dimensions(global_work_size, local_work_size,
                       global_work_size_total, global_work_size_per_dimension,
                       runtime);
  LocalWorkgroupInfo workgroup_info =
      calculate_work_groups_num(global_work_size, local_work_size);

  std::vector<uint32_t> input_data_values(global_work_size_total, 1);
  TestCaseDescriptor<TEST_TYPE> test_description;
  TestArguments arg1(input_data_values.data(),
                     input_data_values.size() * sizeof(uint32_t),
                     input_data_values.size());
  TestArguments arg2(input_data_values.data(),
                     input_data_values.size() * sizeof(uint32_t),
                     input_data_values.size());
  TestArguments arg3(input_data_values.data(),
                     input_data_values.size() * sizeof(uint32_t),
                     input_data_values.size());
  test_description.kernel_name = "test_kernel_" + name;
  test_description.kernel_file_name =
      "kernels/oclc_sub_group_functions/" + name + ".cl";
  test_description.kernel_func_names = std::vector{name};
  test_description.test_args.push_back(arg1);
  test_description.test_args.push_back(arg2);
  test_description.test_args.push_back(arg3);

  const std::vector<std::vector<uint32_t>> outputs =
      run_test<uint32_t, TEST_TYPE, N>(test_description, global_work_size,
                                       local_work_size, runtime, program_type);

  std::vector<uint32_t> sub_group_local_ids_ref(arg1.data_count, 0);
  std::vector<uint32_t> group_ids(arg1.data_count, 0);
  std::iota(group_ids.begin(), group_ids.end(), 1);

  std::vector<uint32_t> sub_group_ids_ref(arg2.data_count, 0);
  uint32_t max_sub_group_local_id =
      *std::max_element(outputs[0].begin(), outputs[0].end());
  uint32_t max_sub_group_size =
      *std::max_element(outputs[2].begin(), outputs[2].end());
  for (uint32_t i = 0; i < sub_group_local_ids_ref.size(); i++) {
    sub_group_local_ids_ref[i] = i % (max_sub_group_local_id + 1);
  }
  const std::vector<uint32_t> sub_group_sizes_ref(arg1.data_count,
                                                  max_sub_group_local_id + 1);
  // verify get_sub_group_id using offsets
  std::vector<bool> check_offsets(arg1.data_count, false);
  for (uint32_t wg = 0; wg < workgroup_info.workgroups_count; wg++) {
    for (uint32_t i = 0; i < sub_group_local_ids_ref.size(); i++) {
      size_t offset = outputs[1][i] * max_sub_group_size + outputs[0][i];
      offset = offset + workgroup_info.workgroup_size_scalar * wg;
      check_offsets[offset] = true;
    }
  }
  REQUIRE_THAT(check_offsets, !Catch::VectorContains(false));
  REQUIRE_THAT(outputs[0], Catch::Equals(sub_group_local_ids_ref));
  REQUIRE_THAT(outputs[2], Catch::Equals(sub_group_sizes_ref));
}

using OneTestType = std::tuple<ca::clc_uint_t>;
template <typename T> std::string test_name() {
  return std::string(T::type_name);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("sub_group_id", "", OneTestType,
                                    test_name<TestType>) {
  const TestConfig &config = get_test_config();
  std::string test_func_name = "sub_group_id";
  ca::Requirements requirements;
  requirements.arithmetic_type<TestType>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }
  SECTION("1D") {
    test_num_subgroup<TestType, 1>(get_test_config(), test_func_name);
  }
  SECTION("2D") {
    test_num_subgroup<TestType, 2>(get_test_config(), test_func_name);
  }
  SECTION("3D") {
    test_num_subgroup<TestType, 3>(get_test_config(), test_func_name);
  }
}

} // namespace
