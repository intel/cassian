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
void test_subgroup_block(const TestConfig &config,
                         const TestExtensionType test_extension_type,
                         const TestFunctionType test_function_type) {
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
  std::string func_name =
      create_func_name(test_function_type, test_extension_type);

  TestCaseDescriptor<TEST_TYPE> test_description;
  TestArguments arg1;
  TestArguments arg2;
  std::string image_postfix = "";
  if (test_extension_type == block_image) {
    image_postfix = "_image";
  }
  test_description.kernel_name = get_kernel_name(func_name) + image_postfix;
  test_description.kernel_file_name =
      "kernels/oclc_sub_group_functions/" + func_name + image_postfix + ".cl";
  test_description.kernel_func_name = func_name;
  test_description.test_extension_type = test_extension_type;
  test_description.test_function_type = test_function_type;
  std::vector<scalar_type> input_data_values(global_work_size_total, 0);
  std::vector<scalar_type> output_data_values(global_work_size_total, 0);
  std::iota(input_data_values.begin(), input_data_values.end(), 0);

  if (test_extension_type == block_image && test_function_type == read_ft) {
    arg1.is_image = true;
    arg1.image_dim_0 = global_work_size[0];
    arg1.image_dim_1 = global_work_size[1];
  }

  if (test_extension_type == block_image &&
      (test_function_type == write_ft || test_function_type == read_write_ft)) {
    std::iota(output_data_values.begin(), output_data_values.end(), 0);
    std::sort(output_data_values.begin(), output_data_values.end(),
              std::greater<scalar_type>());
  }

  if (test_extension_type == block_image && test_function_type == write_ft) {

    arg2.is_image = true;
    arg2.image_dim_0 = global_work_size[0];
    arg2.image_dim_1 = global_work_size[1];
  }

  test_description.change_prefix_for_all_types = false;
  if (test_extension_type == block_image &&
      test_function_type == read_write_ft) {
    arg1.is_image = true;
    arg1.data = output_data_values.data();
    arg1.data_count = output_data_values.size();
    arg1.data_size = output_data_values.size() * sizeof(scalar_type);
    test_description.test_args.push_back(arg1);
  } else {
    arg1.data = input_data_values.data();
    arg1.data_count = input_data_values.size();
    arg1.data_size = input_data_values.size() * sizeof(scalar_type);

    arg2.data = output_data_values.data();
    arg2.data_count = output_data_values.size();
    arg2.data_size = output_data_values.size() * sizeof(scalar_type);
    test_description.test_args.push_back(arg1);
    test_description.test_args.push_back(arg2);
  }

  recalculate_dimensions<TEST_TYPE, N>(global_work_size, local_work_size);
  const std::vector<std::vector<scalar_type>> outputs =
      run_test<scalar_type, TEST_TYPE, N>(test_description, global_work_size,
                                          local_work_size, runtime,
                                          program_type);

  if (test_extension_type == block) {
    REQUIRE_THAT(outputs[0], Catch::Equals(outputs[1]));
  } else if (test_extension_type == block_image &&
             test_function_type == read_ft) {
    REQUIRE_THAT(outputs[0], Catch::Equals(input_data_values));
  } else if (test_extension_type == block_image &&
             test_function_type == write_ft) {
    REQUIRE_THAT(outputs[1], Catch::Equals(input_data_values));
  } else if (test_extension_type == block_image &&
             test_function_type == read_write_ft) {
    REQUIRE_THAT(outputs[0], Catch::Equals(output_data_values));
  }
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
    test_subgroup_block<TestType, 1>(get_test_config(), block, read_ft);
  }
  SECTION("2D") {
    test_subgroup_block<TestType, 2>(get_test_config(), block, read_ft);
  }
  SECTION("3D") {
    test_subgroup_block<TestType, 3>(get_test_config(), block, read_ft);
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
    test_subgroup_block<TestType, 1>(get_test_config(), block, write_ft);
  }
  SECTION("2D") {
    test_subgroup_block<TestType, 2>(get_test_config(), block, write_ft);
  }
  SECTION("3D") {
    test_subgroup_block<TestType, 3>(get_test_config(), block, write_ft);
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
    test_subgroup_block<TestType, 1>(get_test_config(), block, read_write_ft);
  }
  SECTION("2D") {
    test_subgroup_block<TestType, 2>(get_test_config(), block, read_write_ft);
  }
  SECTION("3D") {
    test_subgroup_block<TestType, 3>(get_test_config(), block, read_write_ft);
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
    test_subgroup_block<TestType, 2>(get_test_config(), block_image, read_ft);
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
    test_subgroup_block<TestType, 2>(get_test_config(), block_image, write_ft);
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
    test_subgroup_block<TestType, 2>(get_test_config(), block_image,
                                     read_write_ft);
  }
}

} // namespace
