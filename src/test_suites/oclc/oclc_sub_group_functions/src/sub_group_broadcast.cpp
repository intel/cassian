/*
 * Copyright (C) 2022 Intel Corporation
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
void test_subgroup_broadcast(const TestConfig &config) {

  ca::Runtime *runtime = config.runtime();
  const std::string program_type = config.program_type();

  const size_t global_work_size_per_dimension = config.work_size();
  std::array<size_t, N> global_work_size = {};
  std::array<size_t, N> local_work_size = {};
  size_t global_work_size_total = 1;

  calculate_dimensions(global_work_size, local_work_size,
                       global_work_size_total, global_work_size_per_dimension,
                       runtime);

  std::vector<uint32_t> input_data_values(global_work_size_total);
  std::iota(input_data_values.begin(), input_data_values.end(), 0);

  TestCaseDescriptor<TEST_TYPE> test_description;
  TestArguments arg1(input_data_values.data(),
                     input_data_values.size() * sizeof(uint32_t),
                     input_data_values.size());
  const std::string name = "sub_group_broadcast";
  test_description.kernel_name = get_kernel_name(name);
  test_description.kernel_file_name =
      "kernels/oclc_sub_group_functions/" + name + ".cl";
  test_description.kernel_func_name = name;
  test_description.change_prefix_for_types = true;

  test_description.test_args.push_back(arg1);

  const std::vector<std::vector<uint32_t>> outputs =
      run_test<uint32_t, TEST_TYPE, N>(test_description, global_work_size,
                                       local_work_size, runtime, program_type);
  const std::vector<uint32_t> reference(arg1.data_count, 1);
  REQUIRE_THAT(outputs[0], Catch::Equals(reference));
}
template <typename T> std::string test_name() {
  return std::string(T::type_name);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("sub_group_broadcast", "",
                                    AllScalarTestTypes, test_name<TestType>) {
  const TestConfig &config = get_test_config();

  ca::Requirements requirements;
  requirements.arithmetic_type<TestType>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }
  SECTION("1D") { test_subgroup_broadcast<TestType, 1>(get_test_config()); }
  SECTION("2D") { test_subgroup_broadcast<TestType, 2>(get_test_config()); }
  SECTION("3D") { test_subgroup_broadcast<TestType, 3>(get_test_config()); }
}

} // namespace
