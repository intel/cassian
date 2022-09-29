/*
 * Copyright (C) 2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <catch2/catch.hpp>
#include <common.hpp>
namespace ca = cassian;

namespace {

template <typename TEST_TYPE, size_t N>
void recalculate_dimensions(std::array<size_t, N> &global_work_size,
                            std::array<size_t, N> &local_work_size) {
  using vector_type_check = typename TEST_TYPE::host_type;
  if constexpr (ca::is_vector_v<vector_type_check> && N != 1) {
    global_work_size[1] = global_work_size[1] / vector_type_check::vector_size;
    if (local_work_size[1] > global_work_size[1]) {
      local_work_size[1] = global_work_size[1];
    }
  }
}
template <typename TEST_TYPE, size_t N>
void test_subgroup_block(const TestConfig &config,
                         const std::string &func_name) {
  ca::Runtime *runtime = config.runtime();
  const std::string program_type = config.program_type();

  const size_t global_work_size_per_dimension = config.work_size();
  std::array<size_t, N> global_work_size = {};
  std::array<size_t, N> local_work_size = {};
  size_t global_work_size_total = 1;

  calculate_dimensions(global_work_size, local_work_size,
                       global_work_size_total, global_work_size_per_dimension,
                       runtime);

  using scalar_type = typename TEST_TYPE::scalar_type::host_type;

  std::vector<scalar_type> input_data_values(global_work_size_total, 0);
  std::vector<scalar_type> output_data_values(global_work_size_total, 0);
  std::iota(input_data_values.begin(), input_data_values.end(), 0);
  TestCaseDescriptor<TEST_TYPE> test_description;
  TestArguments arg1;
  TestArguments arg2;
  test_description.block_test_type = true;
  test_description.kernel_name = get_kernel_name(func_name);
  test_description.kernel_file_name =
      "kernels/oclc_sub_group_functions/" + func_name + ".cl";
  test_description.kernel_func_name = func_name;
  test_description.change_prefix_for_all_types = false;

  arg1.data = input_data_values.data();
  arg1.data_count = input_data_values.size();
  arg1.data_size = input_data_values.size() * sizeof(scalar_type);

  arg2.data = output_data_values.data();
  arg2.data_count = output_data_values.size();
  arg2.data_size = output_data_values.size() * sizeof(scalar_type);

  test_description.test_args.push_back(arg1);
  test_description.test_args.push_back(arg2);
  recalculate_dimensions<TEST_TYPE, N>(global_work_size, local_work_size);
  const std::vector<std::vector<scalar_type>> outputs =
      run_test<scalar_type, TEST_TYPE, N>(test_description, global_work_size,
                                          local_work_size, runtime,
                                          program_type);
  REQUIRE_THAT(outputs[0], Catch::Equals(outputs[1]));
}

template <typename TEST_TYPE, size_t N>
void test_subgroup_block_image(const TestConfig &config,
                               const std::string &func_name) {
  ca::Runtime *runtime = config.runtime();
  const std::string program_type = config.program_type();

  const size_t global_work_size_per_dimension = config.work_size();
  std::array<size_t, N> global_work_size = {};
  std::array<size_t, N> local_work_size = {};
  size_t global_work_size_total = 1;

  calculate_dimensions(global_work_size, local_work_size,
                       global_work_size_total, global_work_size_per_dimension,
                       runtime);

  using scalar_type = typename TEST_TYPE::scalar_type::host_type;

  std::vector<scalar_type> input_data_values(global_work_size_total, 0);
  std::vector<scalar_type> output_data_values(global_work_size_total, 0);
  std::iota(input_data_values.begin(), input_data_values.end(), 0);
  TestCaseDescriptor<TEST_TYPE> test_description;
  TestArguments arg1;
  TestArguments arg2;

  test_description.block_test_type = true;
  test_description.kernel_name = get_kernel_name(func_name);
  test_description.kernel_file_name =
      "kernels/oclc_sub_group_functions/" + func_name + ".cl";
  test_description.kernel_func_name = func_name;
  test_description.change_prefix_for_all_types = false;

  arg1.is_image = true;
  arg1.data = input_data_values.data();
  arg1.data_count = input_data_values.size();
  arg1.data_size = input_data_values.size() * sizeof(scalar_type);
  arg1.image_dim_0 = global_work_size[0];
  arg1.image_dim_1 = global_work_size[1];

  arg2.data = output_data_values.data();
  arg2.data_count = output_data_values.size();
  arg2.data_size = output_data_values.size() * sizeof(scalar_type);

  test_description.test_args.push_back(arg1);
  test_description.test_args.push_back(arg2);

  recalculate_dimensions<TEST_TYPE, N>(global_work_size, local_work_size);
  const std::vector<std::vector<scalar_type>> outputs =
      run_test<scalar_type, TEST_TYPE, N>(test_description, global_work_size,
                                          local_work_size, runtime,
                                          program_type);

  REQUIRE_THAT(outputs[0], Catch::Equals(input_data_values));
}

template <typename TEST_TYPE, size_t N>
void test_subgroup_block_image_write(const TestConfig &config,
                                     const std::string &func_name) {
  ca::Runtime *runtime = config.runtime();
  const std::string program_type = config.program_type();

  const size_t global_work_size_per_dimension = config.work_size();
  std::array<size_t, N> global_work_size = {};
  std::array<size_t, N> local_work_size = {};
  size_t global_work_size_total = 1;

  calculate_dimensions(global_work_size, local_work_size,
                       global_work_size_total, global_work_size_per_dimension,
                       runtime);

  using scalar_type = typename TEST_TYPE::scalar_type::host_type;

  std::vector<scalar_type> input_data_values(global_work_size_total, 0);
  std::vector<scalar_type> output_data_values(global_work_size_total, 0);
  std::iota(input_data_values.begin(), input_data_values.end(), 0);
  std::iota(output_data_values.begin(), output_data_values.end(), 0);
  std::sort(output_data_values.begin(), output_data_values.end(),
            std::greater<scalar_type>());
  TestCaseDescriptor<TEST_TYPE> test_description;
  TestArguments arg1;
  TestArguments arg2;
  test_description.block_test_type = true;
  test_description.kernel_name = get_kernel_name(func_name);
  test_description.kernel_file_name =
      "kernels/oclc_sub_group_functions/" + func_name + ".cl";
  test_description.kernel_func_name = func_name;
  test_description.change_prefix_for_all_types = false;

  arg1.data = input_data_values.data();
  arg1.data_count = input_data_values.size();
  arg1.data_size = input_data_values.size() * sizeof(scalar_type);
  arg2.is_image = true;
  arg2.data = output_data_values.data();
  arg2.data_count = output_data_values.size();
  arg2.data_size = output_data_values.size() * sizeof(scalar_type);
  arg2.image_dim_0 = global_work_size[0];
  arg2.image_dim_1 = global_work_size[1];

  test_description.test_args.push_back(arg1);
  test_description.test_args.push_back(arg2);

  recalculate_dimensions<TEST_TYPE, N>(global_work_size, local_work_size);
  const std::vector<std::vector<scalar_type>> outputs =
      run_test<scalar_type, TEST_TYPE, N>(test_description, global_work_size,
                                          local_work_size, runtime,
                                          program_type);

  REQUIRE_THAT(outputs[1], Catch::Equals(input_data_values));
}

template <typename TEST_TYPE, size_t N>
void test_subgroup_block_image_read_write(const TestConfig &config,
                                          const std::string &func_name) {
  ca::Runtime *runtime = config.runtime();
  const std::string program_type = config.program_type();

  const size_t global_work_size_per_dimension = config.work_size();
  std::array<size_t, N> global_work_size = {};
  std::array<size_t, N> local_work_size = {};
  size_t global_work_size_total = 1;

  calculate_dimensions(global_work_size, local_work_size,
                       global_work_size_total, global_work_size_per_dimension,
                       runtime);

  using scalar_type = typename TEST_TYPE::scalar_type::host_type;

  std::vector<scalar_type> output_data_values(global_work_size_total, 0);
  std::iota(output_data_values.begin(), output_data_values.end(), 0);
  std::sort(output_data_values.begin(), output_data_values.end(),
            std::greater<scalar_type>());
  TestCaseDescriptor<TEST_TYPE> test_description;
  TestArguments arg1;
  test_description.block_test_type = true;
  test_description.kernel_name = get_kernel_name(func_name);
  test_description.kernel_file_name =
      "kernels/oclc_sub_group_functions/" + func_name + ".cl";
  test_description.kernel_func_name = func_name;
  test_description.change_prefix_for_all_types = false;

  arg1.is_image = true;
  arg1.data = output_data_values.data();
  arg1.data_count = output_data_values.size();
  arg1.data_size = output_data_values.size() * sizeof(scalar_type);

  arg1.image_dim_0 = global_work_size[0];
  arg1.image_dim_1 = global_work_size[1];

  test_description.test_args.push_back(arg1);

  recalculate_dimensions<TEST_TYPE, N>(global_work_size, local_work_size);
  const std::vector<std::vector<scalar_type>> outputs =
      run_test<scalar_type, TEST_TYPE, N>(test_description, global_work_size,
                                          local_work_size, runtime,
                                          program_type);
  REQUIRE_THAT(outputs[0], Catch::Equals(output_data_values));
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("sub_group_block_read", "",
                                    BlockFunctionTestTypes,
                                    test_name<TestType>) {
  const TestConfig &config = get_test_config();

  using scalar_type = typename TestType::scalar_type;
  ca::Requirements requirements;
  requirements.arithmetic_type<scalar_type>();
  std::string func_name = "sub_group_block_read";
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }
  SECTION("1D") {
    test_subgroup_block<TestType, 1>(get_test_config(), func_name);
  }
  SECTION("2D") {
    test_subgroup_block<TestType, 2>(get_test_config(), func_name);
  }
  SECTION("3D") {
    test_subgroup_block<TestType, 3>(get_test_config(), func_name);
  }
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("sub_group_block_write", "",
                                    BlockFunctionTestTypes,
                                    test_name<TestType>) {
  const TestConfig &config = get_test_config();

  using scalar_type = typename TestType::scalar_type;
  ca::Requirements requirements;
  requirements.arithmetic_type<scalar_type>();
  std::string func_name = "sub_group_block_write";
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }
  SECTION("1D") {
    test_subgroup_block<TestType, 1>(get_test_config(), func_name);
  }
  SECTION("2D") {
    test_subgroup_block<TestType, 2>(get_test_config(), func_name);
  }
  SECTION("3D") {
    test_subgroup_block<TestType, 3>(get_test_config(), func_name);
  }
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("sub_group_block_read_write", "",
                                    BlockFunctionTestTypes,
                                    test_name<TestType>) {
  const TestConfig &config = get_test_config();

  using scalar_type = typename TestType::scalar_type;
  ca::Requirements requirements;
  requirements.arithmetic_type<scalar_type>();
  std::string func_name = "sub_group_block_read_write";
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }
  SECTION("1D") {
    test_subgroup_block<TestType, 1>(get_test_config(), func_name);
  }
  SECTION("2D") {
    test_subgroup_block<TestType, 2>(get_test_config(), func_name);
  }
  SECTION("3D") {
    test_subgroup_block<TestType, 3>(get_test_config(), func_name);
  }
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("sub_group_block_read_image", "",
                                    BlockFunctionTestTypes,
                                    test_name<TestType>) {
  const TestConfig &config = get_test_config();

  using scalar_type = typename TestType::scalar_type;
  ca::Requirements requirements;
  requirements.arithmetic_type<scalar_type>();
  std::string func_name = "sub_group_block_read_image";
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  SECTION("2D") {
    test_subgroup_block_image<TestType, 2>(get_test_config(), func_name);
  }
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("sub_group_block_write_image", "",
                                    BlockFunctionTestTypes,
                                    test_name<TestType>) {
  const TestConfig &config = get_test_config();

  using scalar_type = typename TestType::scalar_type;
  ca::Requirements requirements;
  requirements.arithmetic_type<scalar_type>();
  std::string func_name = "sub_group_block_write_image";
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  SECTION("2D") {
    test_subgroup_block_image_write<TestType, 2>(get_test_config(), func_name);
  }
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("sub_group_block_read_write_image", "",
                                    BlockFunctionTestTypes,
                                    test_name<TestType>) {
  const TestConfig &config = get_test_config();

  using scalar_type = typename TestType::scalar_type;
  ca::Requirements requirements;
  requirements.arithmetic_type<scalar_type>();
  std::string func_name = "sub_group_block_read_write_image";
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  SECTION("2D") {
    test_subgroup_block_image_read_write<TestType, 2>(get_test_config(),
                                                      func_name);
  }
}

} // namespace
