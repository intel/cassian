/*
 * Copyright (C) 2022-2024 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <catch2/catch.hpp>
#include <common.hpp>
namespace ca = cassian;

namespace {

std::vector<std::tuple<ca::ImageChannelOrder, ca::ImageFormat>> image_formats{
    {ca::ImageChannelOrder::r, ca::ImageFormat::snorm_int8},
    {ca::ImageChannelOrder::r, ca::ImageFormat::snorm_int16},
    {ca::ImageChannelOrder::r, ca::ImageFormat::unorm_int8},
    {ca::ImageChannelOrder::r, ca::ImageFormat::unorm_int16},
    {ca::ImageChannelOrder::r, ca::ImageFormat::signed_int8},
    {ca::ImageChannelOrder::r, ca::ImageFormat::signed_int16},
    {ca::ImageChannelOrder::r, ca::ImageFormat::signed_int32},
    {ca::ImageChannelOrder::r, ca::ImageFormat::unsigned_int8},
    {ca::ImageChannelOrder::r, ca::ImageFormat::unsigned_int16},
    {ca::ImageChannelOrder::r, ca::ImageFormat::unsigned_int32},
    {ca::ImageChannelOrder::r, ca::ImageFormat::float16},
    {ca::ImageChannelOrder::r, ca::ImageFormat::float32},
    {ca::ImageChannelOrder::rg, ca::ImageFormat::snorm_int8},
    {ca::ImageChannelOrder::rg, ca::ImageFormat::snorm_int16},
    {ca::ImageChannelOrder::rg, ca::ImageFormat::unorm_int8},
    {ca::ImageChannelOrder::rg, ca::ImageFormat::unorm_int16},
    {ca::ImageChannelOrder::rg, ca::ImageFormat::signed_int8},
    {ca::ImageChannelOrder::rg, ca::ImageFormat::signed_int16},
    {ca::ImageChannelOrder::rg, ca::ImageFormat::signed_int32},
    {ca::ImageChannelOrder::rg, ca::ImageFormat::unsigned_int8},
    {ca::ImageChannelOrder::rg, ca::ImageFormat::unsigned_int16},
    {ca::ImageChannelOrder::rg, ca::ImageFormat::unsigned_int32},
    {ca::ImageChannelOrder::rg, ca::ImageFormat::float16},
    {ca::ImageChannelOrder::rg, ca::ImageFormat::float32},
    {ca::ImageChannelOrder::rg, ca::ImageFormat::snorm_int8},
    {ca::ImageChannelOrder::rgba, ca::ImageFormat::snorm_int16},
    {ca::ImageChannelOrder::rgba, ca::ImageFormat::unorm_int8},
    {ca::ImageChannelOrder::rgba, ca::ImageFormat::unorm_int16},
    {ca::ImageChannelOrder::rgba, ca::ImageFormat::signed_int8},
    {ca::ImageChannelOrder::rgba, ca::ImageFormat::signed_int16},
    {ca::ImageChannelOrder::rgba, ca::ImageFormat::signed_int32},
    {ca::ImageChannelOrder::rgba, ca::ImageFormat::unsigned_int8},
    {ca::ImageChannelOrder::rgba, ca::ImageFormat::unsigned_int16},
    {ca::ImageChannelOrder::rgba, ca::ImageFormat::unsigned_int32},
    {ca::ImageChannelOrder::rgba, ca::ImageFormat::float16},
    {ca::ImageChannelOrder::rgba, ca::ImageFormat::float32},
    {ca::ImageChannelOrder::bgra, ca::ImageFormat::unorm_int8}};

template <typename TEST_TYPE>
void print_test_info(TestExtensionType test_extension_type,
                     TestCaseDescriptor<TEST_TYPE> test_description) {
  std::string function_to_print = test_description.kernel_func_names[0];
  if (test_description.kernel_func_names.size() == 2) {
    function_to_print =
        function_to_print + " " + test_description.kernel_func_names[1];
  }

  if (test_extension_type == media_block_image) {
    ca::logging::info() << "Testing image format: "
                        << to_string(test_description.image_config.format)
                        << ", channel order: "
                        << to_string(test_description.image_config.order)
                        << ", datatype: " << TEST_TYPE::device_type
                        << ", function: " << function_to_print
                        << ", block width: " << test_description.block_width
                        << ", block height: " << test_description.block_height
                        << ", subgroup size: "
                        << test_description.sub_group_size << "\n";
  } else if (test_extension_type == block_image) {
    ca::logging::info() << "Testing image format: "
                        << to_string(test_description.image_config.format)
                        << ", channel order: "
                        << to_string(test_description.image_config.order)
                        << ", datatype: " << TEST_TYPE::device_type
                        << ", function: " << function_to_print
                        << ", image_width: "
                        << test_description.image_config.dim.width
                        << ", image_height: "
                        << test_description.image_config.dim.height << "\n";
  } else {
    ca::logging::info() << "Testing datatype: " << TEST_TYPE::device_type
                        << ", function: " << function_to_print << "\n";
  }
}
template <typename TEST_TYPE, size_t N>
void test_subgroup_block(const TestConfig &config,
                         const std::vector<std::string> &names,
                         const TestExtensionType test_extension_type,
                         const TestFunctionType test_function_type) {
  ca::Runtime *runtime = config.runtime();
  const std::string program_type = config.program_type();
  const size_t global_work_size_per_dimension = 256;
  std::array<size_t, N> global_work_size = {};
  std::array<size_t, N> local_work_size = {};
  size_t global_work_size_total = 1;
  TestCaseDescriptor<TEST_TYPE> test_description;
  constexpr size_t initial_image_width = 128;
  constexpr size_t initial_image_height = 128;
  size_t image_width = initial_image_width;
  size_t image_height = initial_image_height;
  if (test_extension_type == media_block_image) {
    size_t workgroup_width = image_width / 8;
    for (size_t i = 0; i < N; ++i) {
      global_work_size.at(i) = (i == 0) ? image_width : image_height;
      local_work_size.at(i) = workgroup_width;
      global_work_size_total = global_work_size_total * global_work_size.at(i);
    }
  } else {
    calculate_dimensions(global_work_size, local_work_size,
                         global_work_size_total, global_work_size_per_dimension,
                         runtime);
    if (test_extension_type == block_image) {
      image_width = global_work_size.at(0);
      image_height = global_work_size.at(1);
    }
  }
  using scalar_type = typename TEST_TYPE::scalar_type::host_type;
  using host_type = typename TEST_TYPE::host_type;

  bool image_case = false;
  std::vector<std::string> modified_names;
  modified_names = names;
  for (auto &func_name : modified_names) {
    std::size_t pos = func_name.find("image");
    if (pos != std::string::npos) {
      func_name = func_name.substr(0, pos - 1);
    }
  }
  std::string func1_name = modified_names[0];
  if (test_function_type == read_write_ft) {
    func1_name += "_write";
  }
  if (test_extension_type == block_image ||
      test_extension_type == media_block_image) {
    func1_name += "_image";
  }

  test_description.kernel_name = "test_kernel_" + func1_name;
  test_description.kernel_file_name =
      "kernels/oclc_sub_group_functions/" + func1_name + ".cl";
  test_description.kernel_func_names = modified_names;
  test_description.test_extension_type = test_extension_type;
  test_description.test_function_type = test_function_type;
  if constexpr (ca::is_vector_v<host_type>) {
    global_work_size_total = global_work_size_total / host_type::vector_size;
  }
  std::vector<host_type> input_data_values(global_work_size_total);
  std::vector<host_type> output_data_values(global_work_size_total);

  if constexpr (ca::is_vector_v<host_type>) {
    for (size_t i = 0; i < input_data_values.size(); ++i) {
      for (int j = 0; j < host_type::vector_size; ++j) {
        input_data_values[i][j] = static_cast<scalar_type>(i);
      }
    }
  } else {
    std::iota(input_data_values.begin(), input_data_values.end(), 0);
  }
  using vector_type_check = typename TEST_TYPE::host_type;
  uint32_t vector_size = 1;
  if constexpr (ca::is_vector_v<vector_type_check>) {
    vector_size = vector_type_check::vector_size;
  }

  TestArguments arg1(input_data_values.data(),
                     input_data_values.size() * sizeof(host_type),
                     input_data_values.size());
  TestArguments arg2(output_data_values.data(),
                     output_data_values.size() * sizeof(host_type),
                     output_data_values.size());

  if ((test_extension_type == block_image ||
       test_extension_type == media_block_image) &&
      test_function_type == read_ft) {
    arg1.is_image = true;
    arg1.image_dim_0 = image_width;
    arg1.image_dim_1 = image_height;
  }

  if ((test_extension_type == block_image ||
       test_extension_type == media_block_image) &&
      (test_function_type == write_ft || test_function_type == read_write_ft)) {
    if constexpr (ca::is_vector_v<host_type>) {
      for (size_t i = 0; i < output_data_values.size(); ++i) {
        for (int j = 0; j < host_type::vector_size; ++j) {
          output_data_values[i][j] = static_cast<scalar_type>(i);
        }
      }
    } else {
      std::iota(output_data_values.begin(), output_data_values.end(), 0);
      std::sort(output_data_values.begin(), output_data_values.end(),
                std::greater<host_type>());
    }
  }

  if ((test_extension_type == block_image ||
       test_extension_type == media_block_image) &&
      test_function_type == write_ft) {

    arg2.is_image = true;
    arg2.image_dim_0 = image_width;
    arg2.image_dim_1 = image_height;
  }

  test_description.change_prefix_for_all_types = false;
  if ((test_extension_type == block_image ||
       test_extension_type == media_block_image) &&
      test_function_type == read_write_ft) {
    arg1.is_image = true;
    arg1.image_dim_0 = image_width;
    arg1.image_dim_1 = image_height;
    arg1.data = output_data_values.data();
    arg1.data_count = output_data_values.size();
    arg1.data_size = output_data_values.size() * sizeof(host_type);
    test_description.test_args.push_back(arg1);
  } else {
    test_description.test_args.push_back(arg1);
    test_description.test_args.push_back(arg2);
  }
  std::vector<size_t> block_widths{1};
  std::vector<size_t> block_heights{1};
  std::vector<size_t> sub_group_sizes{8};

  constexpr uint32_t max_subgroups_size = 16;
  constexpr uint32_t max_block_width = 2 * max_subgroups_size;
  constexpr uint32_t max_block_height = 4 * max_subgroups_size;
  size_t scalar_size = sizeof(scalar_type);
  if (test_extension_type == media_block_image) {
    block_widths.push_back(max_block_width / max_subgroups_size);
    block_widths.push_back(max_block_width / 8);
    block_widths.push_back(max_block_width / 4);
    block_widths.push_back(max_block_width / 2);
    block_widths.push_back(max_block_width);

    block_heights.push_back(max_block_height / max_block_width);
    block_heights.push_back(max_block_height / max_subgroups_size);
    block_heights.push_back(max_block_height / 8);
    block_heights.push_back(max_block_height / 4);
    block_heights.push_back(max_block_height / 2);
    block_heights.push_back(max_block_height);

    sub_group_sizes.push_back(max_subgroups_size);
  }
  if (test_extension_type == block_image ||
      test_extension_type == media_block_image) {
    for (auto &each_block_width : block_widths) {
      for (auto &each_block_height : block_heights) {
        for (auto &each_sub_group_size : sub_group_sizes) {
          for (auto &each_image_format : image_formats) {

            if (test_extension_type == media_block_image) {
              if ((each_block_width * scalar_size) % 4 != 0) {
                continue;
              }
              if ((each_block_width * scalar_size) > max_block_width) {
                continue;
              }
              if ((each_block_width * each_block_height) !=
                  (each_sub_group_size * vector_size)) {
                continue;
              }
              if ((vector_size == 16) && (scalar_size == 4)) {
                continue;
              }
              if (((each_block_width * scalar_size) >= 8) &&
                  (each_block_height > max_block_width)) {
                continue;
              }
              if (((each_block_width * scalar_size) >= max_block_width) &&
                  (each_block_height > 8)) {
                continue;
              }
              if (((each_block_width * scalar_size) >= max_subgroups_size) &&
                  (each_block_height > max_subgroups_size)) {
                continue;
              }
              if (each_block_height < vector_size) {
                continue;
              }
            }
            test_description.block_width = each_block_width;
            test_description.block_height = each_block_height;
            test_description.sub_group_size = each_sub_group_size;
            ImageConfig img_config;
            img_config.format = std::get<1>(each_image_format);
            img_config.order = std::get<0>(each_image_format);
            uint32_t pixel_size =
                ca::get_pixel_size(img_config.format, img_config.order);
            if (test_extension_type == block_image && scalar_size == 8 &&
                pixel_size == scalar_size) {
              continue;
            }
            img_config.dim.width = image_width;
            img_config.dim.height = image_height;
            img_config.type = ca::ImageType::t_2d;
            test_description.image_config = img_config;
            size_t var_size = sizeof(scalar_type);
            test_description.elements_per_pixel =
                (pixel_size != 0) ? var_size / pixel_size : 0;
            if (test_description.elements_per_pixel == 0) {
              continue;
            }

            print_test_info<TEST_TYPE>(test_extension_type, test_description);
            const std::vector<std::vector<host_type>> outputs =
                run_test<host_type, TEST_TYPE, N>(
                    test_description, global_work_size, local_work_size,
                    runtime, program_type);
            if (test_description.test_function_type == read_ft) {
              REQUIRE_THAT(outputs[0], Catch::Equals(input_data_values));
            } else if (test_description.test_function_type == write_ft) {
              REQUIRE_THAT(outputs[1], Catch::Equals(input_data_values));
            } else if (test_description.test_function_type == read_write_ft) {
              REQUIRE_THAT(outputs[0], Catch::Equals(output_data_values));
            }
          }
        }
      }
    }
  } else {
    print_test_info<TEST_TYPE>(test_extension_type, test_description);
    const std::vector<std::vector<host_type>> outputs =
        run_test<host_type, TEST_TYPE, N>(test_description, global_work_size,
                                          local_work_size, runtime,
                                          program_type);
    REQUIRE_THAT(outputs[0], Catch::Equals(outputs[1]));
  }
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("sub_group_block_read", "",
                                    BlockFunctionTestTypes,
                                    test_name<TestType>) {
  const TestConfig &config = get_test_config();

  using scalar_type = typename TestType::scalar_type;
  ca::Requirements requirements;
  requirements.arithmetic_type<scalar_type>();
  std::vector<std::string> names_config;
  names_config.emplace_back("sub_group_block_read");
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }
  SECTION("1D") {
    test_subgroup_block<TestType, 1>(get_test_config(), names_config, block,
                                     read_ft);
  }
  SECTION("2D") {
    test_subgroup_block<TestType, 2>(get_test_config(), names_config, block,
                                     read_ft);
  }
  SECTION("3D") {
    test_subgroup_block<TestType, 3>(get_test_config(), names_config, block,
                                     read_ft);
  }
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("sub_group_block_write", "",
                                    BlockFunctionTestTypes,
                                    test_name<TestType>) {
  const TestConfig &config = get_test_config();

  using scalar_type = typename TestType::scalar_type;
  ca::Requirements requirements;
  requirements.arithmetic_type<scalar_type>();
  std::vector<std::string> names_config;
  names_config.emplace_back("sub_group_block_write");
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }
  SECTION("1D") {
    test_subgroup_block<TestType, 1>(get_test_config(), names_config, block,
                                     write_ft);
  }
  SECTION("2D") {
    test_subgroup_block<TestType, 2>(get_test_config(), names_config, block,
                                     write_ft);
  }
  SECTION("3D") {
    test_subgroup_block<TestType, 3>(get_test_config(), names_config, block,
                                     write_ft);
  }
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("sub_group_block_read_write", "",
                                    BlockFunctionTestTypes,
                                    test_name<TestType>) {
  const TestConfig &config = get_test_config();

  using scalar_type = typename TestType::scalar_type;
  ca::Requirements requirements;
  requirements.arithmetic_type<scalar_type>();
  std::vector<std::string> names_config;
  names_config.emplace_back("sub_group_block_read");
  names_config.emplace_back("sub_group_block_write");

  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }
  SECTION("1D") {
    test_subgroup_block<TestType, 1>(get_test_config(), names_config, block,
                                     read_write_ft);
  }
  SECTION("2D") {
    test_subgroup_block<TestType, 2>(get_test_config(), names_config, block,
                                     read_write_ft);
  }
  SECTION("3D") {
    test_subgroup_block<TestType, 3>(get_test_config(), names_config, block,
                                     read_write_ft);
  }
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("sub_group_block_read_image", "",
                                    BlockFunctionTestTypes,
                                    test_name<TestType>) {
  const TestConfig &config = get_test_config();

  using scalar_type = typename TestType::scalar_type;
  ca::Requirements requirements;
  requirements.arithmetic_type<scalar_type>();
  std::vector<std::string> names_config;
  names_config.emplace_back("sub_group_block_read_image");
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  SECTION("2D") {
    test_subgroup_block<TestType, 2>(get_test_config(), names_config,
                                     block_image, read_ft);
  }
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("sub_group_block_write_image", "",
                                    BlockFunctionTestTypes,
                                    test_name<TestType>) {
  const TestConfig &config = get_test_config();

  using scalar_type = typename TestType::scalar_type;
  ca::Requirements requirements;
  requirements.arithmetic_type<scalar_type>();
  std::vector<std::string> names_config;
  names_config.emplace_back("sub_group_block_write_image");
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  SECTION("2D") {
    test_subgroup_block<TestType, 2>(get_test_config(), names_config,
                                     block_image, write_ft);
  }
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("sub_group_block_read_write_image", "",
                                    BlockFunctionTestTypes,
                                    test_name<TestType>) {
  const TestConfig &config = get_test_config();

  using scalar_type = typename TestType::scalar_type;
  ca::Requirements requirements;
  requirements.arithmetic_type<scalar_type>();
  std::vector<std::string> names_config;
  names_config.emplace_back("sub_group_block_read_image");
  names_config.emplace_back("sub_group_block_write_image");
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  SECTION("2D") {
    test_subgroup_block<TestType, 2>(get_test_config(), names_config,
                                     block_image, read_write_ft);
  }
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("sub_group_media_block_read_image", "",
                                    MediaBlockFunctionTestTypes,
                                    test_name<TestType>) {
  const TestConfig &config = get_test_config();

  using scalar_type = typename TestType::scalar_type;
  ca::Requirements requirements;
  requirements.arithmetic_type<scalar_type>();
  std::vector<std::string> names_config;
  names_config.emplace_back("sub_group_media_block_read_image");
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }
  SECTION("2D") {
    test_subgroup_block<TestType, 2>(get_test_config(), names_config,
                                     media_block_image, read_ft);
  }
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("sub_group_media_block_write_image", "",
                                    MediaBlockFunctionTestTypes,
                                    test_name<TestType>) {
  const TestConfig &config = get_test_config();

  using scalar_type = typename TestType::scalar_type;
  ca::Requirements requirements;
  requirements.arithmetic_type<scalar_type>();
  std::vector<std::string> names_config;
  names_config.emplace_back("sub_group_media_block_write_image");
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }
  SECTION("2D") {
    test_subgroup_block<TestType, 2>(get_test_config(), names_config,
                                     media_block_image, write_ft);
  }
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("sub_group_media_block_read_write_image",
                                    "", MediaBlockFunctionTestTypes,
                                    test_name<TestType>) {
  const TestConfig &config = get_test_config();

  using scalar_type = typename TestType::scalar_type;
  ca::Requirements requirements;
  requirements.arithmetic_type<scalar_type>();
  std::vector<std::string> names_config;
  names_config.emplace_back("sub_group_media_block_read_image");
  names_config.emplace_back("sub_group_media_block_write_image");
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }
  SECTION("2D") {
    test_subgroup_block<TestType, 2>(get_test_config(), names_config,
                                     media_block_image, read_write_ft);
  }
}

} // namespace
