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

  KernelDescriptor<TEST_TYPE> kernel_description;
  kernel_description.args = 1;
  kernel_description.arg1.data = input_data_values.data();
  kernel_description.arg1.data_count = input_data_values.size();
  kernel_description.arg1.data_size =
      input_data_values.size() * sizeof(uint32_t);
  const std::string name = "sub_group_broadcast";
  kernel_description.kernel_name = get_kernel_name(name);
  kernel_description.kernel_file_name =
      "kernels/oclc_sub_group_functions/" + name + ".cl";
  kernel_description.kernel_func_name = name;
  kernel_description.change_prefix_for_types = true;

  const std::vector<std::vector<uint32_t>> outputs =
      run_test<uint32_t, TEST_TYPE, N>(kernel_description, global_work_size,
                                       local_work_size, runtime, program_type);
  const std::vector<uint32_t> reference(kernel_description.arg1.data_count, 1);
  REQUIRE_THAT(outputs[0], Catch::Equals(reference));
}
using AllTestTypes =
    std::tuple<ca::clc_char_t, ca::clc_uchar_t, ca::clc_short_t,
               ca::clc_ushort_t, ca::clc_int_t, ca::clc_uint_t, ca::clc_long_t,
               ca::clc_ulong_t, ca::clc_float_t, ca::clc_half_t,
               ca::clc_double_t>;
template <typename T> std::string test_name() {
  return std::string(T::type_name);
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("sub_group_broadcast", "", AllTestTypes,
                                    test_name<TestType>) {
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
