/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/random/random.hpp>
#include <cassian/runtime/openclc_types.hpp>
#include <cassian/test_harness/test_harness.hpp>
#include <cassian/utility/utility.hpp>
#include <cassian/vector/vector.hpp>
#include <catch2/catch.hpp>
#include <common.hpp>
#include <test_config.hpp>

namespace ca = cassian;

namespace {

template <typename INPUT_TYPE, typename OUTPUT_TYPE>
std::string get_build_options() {
  const std::string clc_input_type = INPUT_TYPE::device_type;
  const std::string clc_output_type = OUTPUT_TYPE::device_type;
  std::string build_options =
      " -D INPUT_TYPE=" + clc_input_type + " -D OUTPUT_TYPE=" + clc_output_type;
  return build_options;
}

ca::Kernel create_kernel(const std::string &path,
                         const std::string &build_options, ca::Runtime *runtime,
                         const std::string &program_type) {
  const std::string source = ca::load_text_file(ca::get_asset(path));
  const std::string kernel_name = "test_kernel";
  return runtime->create_kernel(kernel_name, source, build_options,
                                program_type);
}

template <typename OUTPUT_TYPE, typename INPUT_TYPE>
std::vector<OUTPUT_TYPE>
run_kernel(const ca::Kernel &kernel, const int global_work_size,
           const int local_work_size, const std::vector<INPUT_TYPE> &input_x,
           const std::vector<INPUT_TYPE> &input_y, ca::Runtime *runtime) {
  std::vector<ca::Buffer> buffers;

  ca::Buffer input_x_buffer =
      runtime->create_buffer(sizeof(INPUT_TYPE) * input_x.size());
  runtime->write_buffer_from_vector(input_x_buffer, input_x);
  buffers.push_back(input_x_buffer);

  ca::Buffer input_y_buffer =
      runtime->create_buffer(sizeof(INPUT_TYPE) * input_y.size());
  runtime->write_buffer_from_vector(input_y_buffer, input_y);
  buffers.push_back(input_y_buffer);

  ca::Buffer output_buffer =
      runtime->create_buffer(sizeof(OUTPUT_TYPE) * global_work_size);
  buffers.push_back(output_buffer);

  for (size_t i = 0; i < buffers.size(); ++i) {
    runtime->set_kernel_argument(kernel, static_cast<int>(i), buffers[i]);
  }

  runtime->run_kernel(kernel, global_work_size, local_work_size);

  std::vector<OUTPUT_TYPE> output =
      runtime->read_buffer_to_vector<OUTPUT_TYPE>(output_buffer);

  for (auto &buffer : buffers) {
    runtime->release_buffer(buffer);
  }

  return output;
}

template <typename OUTPUT_TYPE, typename INPUT_TYPE>
std::vector<OUTPUT_TYPE> get_reference(const std::vector<INPUT_TYPE> &input_a,
                                       const std::vector<INPUT_TYPE> &input_b) {
  std::vector<OUTPUT_TYPE> output(input_a.size());
  for (size_t i = 0; i < output.size(); ++i) {
    if constexpr (ca::is_vector_v<INPUT_TYPE>) {
      output[i] =
          ca::not_equal_component_wise<OUTPUT_TYPE>(input_a[i], input_b[i]);
    } else {
      output[i] = input_a[i] != input_b[i];
    }
  }
  return output;
}

template <typename OUTPUT_TYPE, typename INPUT_TYPE>
void run_test(const std::vector<INPUT_TYPE> &input_x,
              const std::vector<INPUT_TYPE> &input_y, const ca::Kernel &kernel,
              const int global_work_size, const int local_work_size,
              ca::Runtime *runtime) {
  const std::vector<OUTPUT_TYPE> output = run_kernel<OUTPUT_TYPE>(
      kernel, global_work_size, local_work_size, input_x, input_y, runtime);
  const std::vector<OUTPUT_TYPE> reference =
      get_reference<OUTPUT_TYPE>(input_x, input_y);
  REQUIRE_THAT(output, Catch::Equals(reference));
}

TEMPLATE_TEST_CASE("isnotequal", "", ca::clc_float_t, ca::clc_float2_t,
                   ca::clc_float3_t, ca::clc_float4_t, ca::clc_float8_t,
                   ca::clc_float16_t, ca::clc_double_t, ca::clc_double2_t,
                   ca::clc_double3_t, ca::clc_double4_t, ca::clc_double8_t,
                   ca::clc_double16_t) {
  const TestConfig &config = get_test_config();
  ca::Runtime *runtime = config.runtime();
  const std::string program_type = config.program_type();

  ca::Requirements requirements;
  requirements.arithmetic_type<TestType>();
  if (ca::should_skip_test(requirements, *runtime)) {
    return;
  }

  using input_type = TestType;
  using input_host_type = typename input_type::host_type;
  using output_type = typename input_type::logical_type;
  using output_host_type = typename output_type::host_type;

  const std::string build_options =
      get_build_options<input_type, output_type>();
  const ca::Kernel kernel =
      create_kernel("kernels/oclc_relational_functions/isnotequal.cl",
                    build_options, runtime, program_type);

  const int global_work_size = config.work_size();
  const int local_work_size = get_local_work_size(global_work_size, *runtime);

  SECTION("all components of x are equal to y") {
    const auto value_x = input_host_type(2);
    const auto value_y = value_x;
    const std::vector<input_host_type> input_x(global_work_size, value_x);
    const std::vector<input_host_type> input_y(global_work_size, value_y);
    run_test<output_host_type>(input_x, input_y, kernel, global_work_size,
                               local_work_size, runtime);
  }

  SECTION("all components of x are not equal to y") {
    const auto value_x = input_host_type(2);
    const auto value_y = input_host_type(3);
    const std::vector<input_host_type> input_x(global_work_size, value_x);
    const std::vector<input_host_type> input_y(global_work_size, value_y);
    run_test<output_host_type>(input_x, input_y, kernel, global_work_size,
                               local_work_size, runtime);
  }

  SECTION("all components of x and y are positive zero") {
    const auto value_x = input_host_type(0.0);
    const auto value_y = input_host_type(0.0);
    const std::vector<input_host_type> input_x(global_work_size, value_x);
    const std::vector<input_host_type> input_y(global_work_size, value_y);
    run_test<output_host_type>(input_x, input_y, kernel, global_work_size,
                               local_work_size, runtime);
  }

  SECTION("all components of x and y are negative zero") {
    const auto value_x = input_host_type(-0.0);
    const auto value_y = input_host_type(-0.0);
    const std::vector<input_host_type> input_x(global_work_size, value_x);
    const std::vector<input_host_type> input_y(global_work_size, value_y);
    run_test<output_host_type>(input_x, input_y, kernel, global_work_size,
                               local_work_size, runtime);
  }

  SECTION("all components of x are positive zero, and all components of y are "
          "negative zero") {
    const auto value_x = input_host_type(0.0);
    const auto value_y = input_host_type(-0.0);
    const std::vector<input_host_type> input_x(global_work_size, value_x);
    const std::vector<input_host_type> input_y(global_work_size, value_y);
    run_test<output_host_type>(input_x, input_y, kernel, global_work_size,
                               local_work_size, runtime);
  }

  SECTION("all components of x and y are quiet NaN") {
    const auto value_x =
        input_host_type(std::numeric_limits<input_host_type>::quiet_NaN());
    const auto value_y =
        input_host_type(std::numeric_limits<input_host_type>::quiet_NaN());
    const std::vector<input_host_type> input_x(global_work_size, value_x);
    const std::vector<input_host_type> input_y(global_work_size, value_y);
    run_test<output_host_type>(input_x, input_y, kernel, global_work_size,
                               local_work_size, runtime);
  }

  SECTION("all components of x are quiet NaN, all components of y are normal") {
    const auto value_x =
        input_host_type(std::numeric_limits<input_host_type>::quiet_NaN());
    const auto value_y = input_host_type(2);
    const std::vector<input_host_type> input_x(global_work_size, value_x);
    const std::vector<input_host_type> input_y(global_work_size, value_y);
    run_test<output_host_type>(input_x, input_y, kernel, global_work_size,
                               local_work_size, runtime);
  }

  SECTION(
      "all components of x are quiet NaN, all components of y are infinity") {
    const auto value_x =
        input_host_type(std::numeric_limits<input_host_type>::quiet_NaN());
    const auto value_y =
        input_host_type(std::numeric_limits<input_host_type>::infinity());
    const std::vector<input_host_type> input_x(global_work_size, value_x);
    const std::vector<input_host_type> input_y(global_work_size, value_y);
    run_test<output_host_type>(input_x, input_y, kernel, global_work_size,
                               local_work_size, runtime);
  }

  SECTION("all components of x and y are signaling NaN") {
    const auto value_x =
        input_host_type(std::numeric_limits<input_host_type>::signaling_NaN());
    const auto value_y =
        input_host_type(std::numeric_limits<input_host_type>::signaling_NaN());
    const std::vector<input_host_type> input_x(global_work_size, value_x);
    const std::vector<input_host_type> input_y(global_work_size, value_y);
    run_test<output_host_type>(input_x, input_y, kernel, global_work_size,
                               local_work_size, runtime);
  }

  SECTION(
      "all components of x are signaling NaN, all components of y are normal") {
    const auto value_x =
        input_host_type(std::numeric_limits<input_host_type>::signaling_NaN());
    const auto value_y = input_host_type(2);
    const std::vector<input_host_type> input_x(global_work_size, value_x);
    const std::vector<input_host_type> input_y(global_work_size, value_y);
    run_test<output_host_type>(input_x, input_y, kernel, global_work_size,
                               local_work_size, runtime);
  }

  SECTION("all components of x are signaling NaN, all components of y are "
          "infinity") {
    const auto value_x =
        input_host_type(std::numeric_limits<input_host_type>::signaling_NaN());
    const auto value_y =
        input_host_type(std::numeric_limits<input_host_type>::infinity());
    const std::vector<input_host_type> input_x(global_work_size, value_x);
    const std::vector<input_host_type> input_y(global_work_size, value_y);
    run_test<output_host_type>(input_x, input_y, kernel, global_work_size,
                               local_work_size, runtime);
  }

  SECTION("all components of x and y are infinity") {
    const auto value_x =
        input_host_type(std::numeric_limits<input_host_type>::infinity());
    const auto value_y = value_x;
    const std::vector<input_host_type> input_x(global_work_size, value_x);
    const std::vector<input_host_type> input_y(global_work_size, value_y);
    run_test<output_host_type>(input_x, input_y, kernel, global_work_size,
                               local_work_size, runtime);
  }

  SECTION("all components of x and y are denormalized") {
    const auto value_x =
        input_host_type(std::numeric_limits<input_host_type>::denorm_min());
    const auto value_y =
        input_host_type(std::numeric_limits<input_host_type>::denorm_min());
    const std::vector<input_host_type> input_x(global_work_size, value_x);
    const std::vector<input_host_type> input_y(global_work_size, value_y);
    run_test<output_host_type>(input_x, input_y, kernel, global_work_size,
                               local_work_size, runtime);
  }

  SECTION("all components of x and y differ by epsilon") {
    const auto value_x = input_host_type(2);
    const auto value_y =
        value_x + std::numeric_limits<input_host_type>::epsilon();
    const std::vector<input_host_type> input_x(global_work_size, value_x);
    const std::vector<input_host_type> input_y(global_work_size, value_y);
    run_test<output_host_type>(input_x, input_y, kernel, global_work_size,
                               local_work_size, runtime);
  }

  if constexpr (ca::is_vector_v<input_host_type>) {
    SECTION("some components of x are equal to y, others are not") {
      const auto value_x = input_host_type(2);
      auto value_y = input_host_type(3);
      value_y[0] = value_x[0];
      const std::vector<input_host_type> input_x(global_work_size, value_x);
      std::vector<input_host_type> input_y(global_work_size, value_y);
      run_test<output_host_type>(input_x, input_y, kernel, global_work_size,
                                 local_work_size, runtime);
    }
  }

  SECTION("random values") {
    const std::vector<input_host_type> input_x =
        ca::generate_vector<input_host_type>(global_work_size, 0);
    const std::vector<input_host_type> input_y =
        ca::generate_vector<input_host_type>(global_work_size, 0);
    run_test<output_host_type>(input_x, input_y, kernel, global_work_size,
                               local_work_size, runtime);
  }

  runtime->release_kernel(kernel);
}

} // namespace
