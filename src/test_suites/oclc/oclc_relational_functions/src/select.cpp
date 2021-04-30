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

template <typename T> struct IGenType {};

template <> struct IGenType<ca::clc_char_t> { using value = ca::clc_char_t; };

template <> struct IGenType<ca::clc_char2_t> { using value = ca::clc_char2_t; };

template <> struct IGenType<ca::clc_char3_t> { using value = ca::clc_char3_t; };

template <> struct IGenType<ca::clc_char4_t> { using value = ca::clc_char4_t; };

template <> struct IGenType<ca::clc_char8_t> { using value = ca::clc_char8_t; };

template <> struct IGenType<ca::clc_char16_t> {
  using value = ca::clc_char16_t;
};

template <> struct IGenType<ca::clc_uchar_t> { using value = ca::clc_char_t; };

template <> struct IGenType<ca::clc_uchar2_t> {
  using value = ca::clc_char2_t;
};

template <> struct IGenType<ca::clc_uchar3_t> {
  using value = ca::clc_char3_t;
};

template <> struct IGenType<ca::clc_uchar4_t> {
  using value = ca::clc_char4_t;
};

template <> struct IGenType<ca::clc_uchar8_t> {
  using value = ca::clc_char8_t;
};

template <> struct IGenType<ca::clc_uchar16_t> {
  using value = ca::clc_char16_t;
};

template <> struct IGenType<ca::clc_short_t> { using value = ca::clc_short_t; };

template <> struct IGenType<ca::clc_short2_t> {
  using value = ca::clc_short2_t;
};

template <> struct IGenType<ca::clc_short3_t> {
  using value = ca::clc_short3_t;
};

template <> struct IGenType<ca::clc_short4_t> {
  using value = ca::clc_short4_t;
};

template <> struct IGenType<ca::clc_short8_t> {
  using value = ca::clc_short8_t;
};

template <> struct IGenType<ca::clc_short16_t> {
  using value = ca::clc_short16_t;
};

template <> struct IGenType<ca::clc_ushort_t> {
  using value = ca::clc_short_t;
};

template <> struct IGenType<ca::clc_ushort2_t> {
  using value = ca::clc_short2_t;
};

template <> struct IGenType<ca::clc_ushort3_t> {
  using value = ca::clc_short3_t;
};

template <> struct IGenType<ca::clc_ushort4_t> {
  using value = ca::clc_short4_t;
};

template <> struct IGenType<ca::clc_ushort8_t> {
  using value = ca::clc_short8_t;
};

template <> struct IGenType<ca::clc_ushort16_t> {
  using value = ca::clc_short16_t;
};

template <> struct IGenType<ca::clc_int_t> { using value = ca::clc_int_t; };

template <> struct IGenType<ca::clc_int2_t> { using value = ca::clc_int2_t; };

template <> struct IGenType<ca::clc_int3_t> { using value = ca::clc_int3_t; };

template <> struct IGenType<ca::clc_int4_t> { using value = ca::clc_int4_t; };

template <> struct IGenType<ca::clc_int8_t> { using value = ca::clc_int8_t; };

template <> struct IGenType<ca::clc_int16_t> { using value = ca::clc_int16_t; };

template <> struct IGenType<ca::clc_uint_t> { using value = ca::clc_int_t; };

template <> struct IGenType<ca::clc_uint2_t> { using value = ca::clc_int2_t; };

template <> struct IGenType<ca::clc_uint3_t> { using value = ca::clc_int3_t; };

template <> struct IGenType<ca::clc_uint4_t> { using value = ca::clc_int4_t; };

template <> struct IGenType<ca::clc_uint8_t> { using value = ca::clc_int8_t; };

template <> struct IGenType<ca::clc_uint16_t> {
  using value = ca::clc_int16_t;
};

template <> struct IGenType<ca::clc_long_t> { using value = ca::clc_long_t; };

template <> struct IGenType<ca::clc_long2_t> { using value = ca::clc_long2_t; };

template <> struct IGenType<ca::clc_long3_t> { using value = ca::clc_long3_t; };

template <> struct IGenType<ca::clc_long4_t> { using value = ca::clc_long4_t; };

template <> struct IGenType<ca::clc_long8_t> { using value = ca::clc_long8_t; };

template <> struct IGenType<ca::clc_long16_t> {
  using value = ca::clc_long16_t;
};

template <> struct IGenType<ca::clc_ulong_t> { using value = ca::clc_long_t; };

template <> struct IGenType<ca::clc_ulong2_t> {
  using value = ca::clc_long2_t;
};

template <> struct IGenType<ca::clc_ulong3_t> {
  using value = ca::clc_long3_t;
};

template <> struct IGenType<ca::clc_ulong4_t> {
  using value = ca::clc_long4_t;
};

template <> struct IGenType<ca::clc_ulong8_t> {
  using value = ca::clc_long8_t;
};

template <> struct IGenType<ca::clc_ulong16_t> {
  using value = ca::clc_long16_t;
};

template <> struct IGenType<ca::clc_float_t> { using value = ca::clc_int_t; };

template <> struct IGenType<ca::clc_float2_t> { using value = ca::clc_int2_t; };

template <> struct IGenType<ca::clc_float3_t> { using value = ca::clc_int3_t; };

template <> struct IGenType<ca::clc_float4_t> { using value = ca::clc_int4_t; };

template <> struct IGenType<ca::clc_float8_t> { using value = ca::clc_int8_t; };

template <> struct IGenType<ca::clc_float16_t> {
  using value = ca::clc_int16_t;
};

template <> struct IGenType<ca::clc_double_t> { using value = ca::clc_long_t; };

template <> struct IGenType<ca::clc_double2_t> {
  using value = ca::clc_long2_t;
};

template <> struct IGenType<ca::clc_double3_t> {
  using value = ca::clc_long3_t;
};

template <> struct IGenType<ca::clc_double4_t> {
  using value = ca::clc_long4_t;
};

template <> struct IGenType<ca::clc_double8_t> {
  using value = ca::clc_long8_t;
};

template <> struct IGenType<ca::clc_double16_t> {
  using value = ca::clc_long16_t;
};

template <typename T> struct UGenType {};

template <> struct UGenType<ca::clc_char_t> { using value = ca::clc_uchar_t; };

template <> struct UGenType<ca::clc_char2_t> {
  using value = ca::clc_uchar2_t;
};

template <> struct UGenType<ca::clc_char3_t> {
  using value = ca::clc_uchar3_t;
};

template <> struct UGenType<ca::clc_char4_t> {
  using value = ca::clc_uchar4_t;
};

template <> struct UGenType<ca::clc_char8_t> {
  using value = ca::clc_uchar8_t;
};

template <> struct UGenType<ca::clc_char16_t> {
  using value = ca::clc_uchar16_t;
};

template <> struct UGenType<ca::clc_uchar_t> { using value = ca::clc_uchar_t; };

template <> struct UGenType<ca::clc_uchar2_t> {
  using value = ca::clc_uchar2_t;
};

template <> struct UGenType<ca::clc_uchar3_t> {
  using value = ca::clc_uchar3_t;
};

template <> struct UGenType<ca::clc_uchar4_t> {
  using value = ca::clc_uchar4_t;
};

template <> struct UGenType<ca::clc_uchar8_t> {
  using value = ca::clc_uchar8_t;
};

template <> struct UGenType<ca::clc_uchar16_t> {
  using value = ca::clc_uchar16_t;
};

template <> struct UGenType<ca::clc_short_t> {
  using value = ca::clc_ushort_t;
};

template <> struct UGenType<ca::clc_short2_t> {
  using value = ca::clc_ushort2_t;
};

template <> struct UGenType<ca::clc_short3_t> {
  using value = ca::clc_ushort3_t;
};

template <> struct UGenType<ca::clc_short4_t> {
  using value = ca::clc_ushort4_t;
};

template <> struct UGenType<ca::clc_short8_t> {
  using value = ca::clc_ushort8_t;
};

template <> struct UGenType<ca::clc_short16_t> {
  using value = ca::clc_ushort16_t;
};

template <> struct UGenType<ca::clc_ushort_t> {
  using value = ca::clc_ushort_t;
};

template <> struct UGenType<ca::clc_ushort2_t> {
  using value = ca::clc_ushort2_t;
};

template <> struct UGenType<ca::clc_ushort3_t> {
  using value = ca::clc_ushort3_t;
};

template <> struct UGenType<ca::clc_ushort4_t> {
  using value = ca::clc_ushort4_t;
};

template <> struct UGenType<ca::clc_ushort8_t> {
  using value = ca::clc_ushort8_t;
};

template <> struct UGenType<ca::clc_ushort16_t> {
  using value = ca::clc_ushort16_t;
};

template <> struct UGenType<ca::clc_int_t> { using value = ca::clc_uint_t; };

template <> struct UGenType<ca::clc_int2_t> { using value = ca::clc_uint2_t; };

template <> struct UGenType<ca::clc_int3_t> { using value = ca::clc_uint3_t; };

template <> struct UGenType<ca::clc_int4_t> { using value = ca::clc_uint4_t; };

template <> struct UGenType<ca::clc_int8_t> { using value = ca::clc_uint8_t; };

template <> struct UGenType<ca::clc_int16_t> {
  using value = ca::clc_uint16_t;
};

template <> struct UGenType<ca::clc_uint_t> { using value = ca::clc_uint_t; };

template <> struct UGenType<ca::clc_uint2_t> { using value = ca::clc_uint2_t; };

template <> struct UGenType<ca::clc_uint3_t> { using value = ca::clc_uint3_t; };

template <> struct UGenType<ca::clc_uint4_t> { using value = ca::clc_uint4_t; };

template <> struct UGenType<ca::clc_uint8_t> { using value = ca::clc_uint8_t; };

template <> struct UGenType<ca::clc_uint16_t> {
  using value = ca::clc_uint16_t;
};

template <> struct UGenType<ca::clc_long_t> { using value = ca::clc_ulong_t; };

template <> struct UGenType<ca::clc_long2_t> {
  using value = ca::clc_ulong2_t;
};

template <> struct UGenType<ca::clc_long3_t> {
  using value = ca::clc_ulong3_t;
};

template <> struct UGenType<ca::clc_long4_t> {
  using value = ca::clc_ulong4_t;
};

template <> struct UGenType<ca::clc_long8_t> {
  using value = ca::clc_ulong8_t;
};

template <> struct UGenType<ca::clc_long16_t> {
  using value = ca::clc_ulong16_t;
};

template <> struct UGenType<ca::clc_ulong_t> { using value = ca::clc_ulong_t; };

template <> struct UGenType<ca::clc_ulong2_t> {
  using value = ca::clc_ulong2_t;
};

template <> struct UGenType<ca::clc_ulong3_t> {
  using value = ca::clc_ulong3_t;
};

template <> struct UGenType<ca::clc_ulong4_t> {
  using value = ca::clc_ulong4_t;
};

template <> struct UGenType<ca::clc_ulong8_t> {
  using value = ca::clc_ulong8_t;
};

template <> struct UGenType<ca::clc_ulong16_t> {
  using value = ca::clc_ulong16_t;
};

template <> struct UGenType<ca::clc_float_t> { using value = ca::clc_uint_t; };

template <> struct UGenType<ca::clc_float2_t> {
  using value = ca::clc_uint2_t;
};

template <> struct UGenType<ca::clc_float3_t> {
  using value = ca::clc_uint3_t;
};

template <> struct UGenType<ca::clc_float4_t> {
  using value = ca::clc_uint4_t;
};

template <> struct UGenType<ca::clc_float8_t> {
  using value = ca::clc_uint8_t;
};

template <> struct UGenType<ca::clc_float16_t> {
  using value = ca::clc_uint16_t;
};

template <> struct UGenType<ca::clc_double_t> {
  using value = ca::clc_ulong_t;
};

template <> struct UGenType<ca::clc_double2_t> {
  using value = ca::clc_ulong2_t;
};

template <> struct UGenType<ca::clc_double3_t> {
  using value = ca::clc_ulong3_t;
};

template <> struct UGenType<ca::clc_double4_t> {
  using value = ca::clc_ulong4_t;
};

template <> struct UGenType<ca::clc_double8_t> {
  using value = ca::clc_ulong8_t;
};

template <> struct UGenType<ca::clc_double16_t> {
  using value = ca::clc_ulong16_t;
};

template <typename DATA_TYPE, typename SELECT_TYPE>
std::string get_build_options() {
  const std::string clc_data_type = DATA_TYPE::device_type;
  const std::string clc_select_type = SELECT_TYPE::device_type;
  std::string build_options =
      " -D DATA_TYPE=" + clc_data_type + " -D SELECT_TYPE=" + clc_select_type;
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

template <typename DATA_TYPE, typename SELECT_TYPE>
std::vector<DATA_TYPE>
run_kernel(const ca::Kernel &kernel, const int global_work_size,
           const int local_work_size, const std::vector<DATA_TYPE> &input_a,
           const std::vector<DATA_TYPE> &input_b,
           const std::vector<SELECT_TYPE> &input_c, ca::Runtime *runtime) {
  std::vector<ca::Buffer> buffers;

  ca::Buffer input_a_buffer =
      runtime->create_buffer(sizeof(DATA_TYPE) * input_a.size());
  runtime->write_buffer_from_vector(input_a_buffer, input_a);
  buffers.push_back(input_a_buffer);

  ca::Buffer input_b_buffer =
      runtime->create_buffer(sizeof(DATA_TYPE) * input_b.size());
  runtime->write_buffer_from_vector(input_b_buffer, input_b);
  buffers.push_back(input_b_buffer);

  ca::Buffer input_c_buffer =
      runtime->create_buffer(sizeof(SELECT_TYPE) * input_c.size());
  runtime->write_buffer_from_vector(input_c_buffer, input_c);
  buffers.push_back(input_c_buffer);

  ca::Buffer output_buffer =
      runtime->create_buffer(sizeof(DATA_TYPE) * global_work_size);
  buffers.push_back(output_buffer);

  for (size_t i = 0; i < buffers.size(); ++i) {
    runtime->set_kernel_argument(kernel, static_cast<int>(i), buffers[i]);
  }

  runtime->run_kernel(kernel, global_work_size, local_work_size);

  std::vector<DATA_TYPE> output =
      runtime->read_buffer_to_vector<DATA_TYPE>(output_buffer);

  for (auto &buffer : buffers) {
    runtime->release_buffer(buffer);
  }

  return output;
}

template <typename T> bool is_msb_set(const T &input) {
  using unsigned_type = typename std::make_unsigned_t<T>;
  const size_t bits_in_byte = 8;
  const size_t number_of_bits = sizeof(unsigned_type) * bits_in_byte;
  unsigned_type value = (input >> number_of_bits - 1) & 1U;
  return value != 0;
}

template <typename T, typename U> T select(const T &a, const T &b, const U &c) {
  auto result = T(0);
  if constexpr (ca::is_vector_v<T>) {
    for (int i = 0; i < a.size(); ++i) {
      result[i] = is_msb_set(c[i]) ? b[i] : a[i];
    }
  } else {
    result = c ? b : a;
  }
  return result;
}

template <typename DATA_TYPE, typename SELECT_TYPE>
std::vector<DATA_TYPE> get_reference(const std::vector<DATA_TYPE> &input_a,
                                     const std::vector<DATA_TYPE> &input_b,
                                     const std::vector<SELECT_TYPE> &input_c) {
  std::vector<DATA_TYPE> output(input_a.size());
  for (size_t i = 0; i < output.size(); ++i) {
    output[i] = select(input_a[i], input_b[i], input_c[i]);
  }
  return output;
}

template <typename DATA_TYPE, typename SELECT_TYPE>
void run_test(const std::vector<DATA_TYPE> &input_a,
              const std::vector<DATA_TYPE> &input_b,
              const std::vector<SELECT_TYPE> &input_c, const ca::Kernel &kernel,
              const int global_work_size, const int local_work_size,
              ca::Runtime *runtime) {
  const std::vector<DATA_TYPE> output =
      run_kernel(kernel, global_work_size, local_work_size, input_a, input_b,
                 input_c, runtime);
  const std::vector<DATA_TYPE> reference =
      get_reference(input_a, input_b, input_c);
  REQUIRE_THAT(output, Catch::Equals(reference));
}

template <typename T> T value_with_msb_set() {
  if constexpr (ca::is_vector_v<T>) {
    using unsigned_type = typename std::make_unsigned_t<ca::scalar_type_v<T>>;
    return T(value_with_msb_set<unsigned_type>());
  } else {
    using unsigned_type = typename std::make_unsigned_t<T>;
    const size_t bits_in_byte = 8;
    const size_t number_of_bits = sizeof(unsigned_type) * bits_in_byte;
    unsigned_type value = 0;
    value = 1ULL << number_of_bits - 1;
    return T(value);
  }
}

template <typename T> T value_with_msb_not_set() {
  if constexpr (ca::is_vector_v<T>) {
    using unsigned_type = typename std::make_unsigned_t<ca::scalar_type_v<T>>;
    return T(value_with_msb_not_set<unsigned_type>());
  } else {
    using unsigned_type = typename std::make_unsigned_t<T>;
    const size_t bits_in_byte = 8;
    const size_t number_of_bits = sizeof(unsigned_type) * bits_in_byte;
    auto value = value_with_msb_set<unsigned_type>();
    value = ~value;
    return T(value);
  }
}

template <typename TEST_TYPE, typename SELECT_TYPE>
void test_select(const TestConfig &config) {
  using data_host_type = typename TEST_TYPE::host_type;
  using select_host_type = typename SELECT_TYPE::host_type;

  ca::Runtime *runtime = config.runtime();
  const std::string program_type = config.program_type();

  const std::string build_options = get_build_options<TEST_TYPE, SELECT_TYPE>();
  const ca::Kernel kernel =
      create_kernel("kernels/oclc_relational_functions/select.cl",
                    build_options, runtime, program_type);

  const int global_work_size = config.work_size();
  const int local_work_size = get_local_work_size(global_work_size, *runtime);

  SECTION("most significant bit in all components of c are set") {
    const std::vector<data_host_type> input_a(global_work_size,
                                              data_host_type(2));
    const std::vector<data_host_type> input_b(global_work_size,
                                              data_host_type(3));
    const std::vector<select_host_type> input_c(
        global_work_size, value_with_msb_set<select_host_type>());
    run_test(input_a, input_b, input_c, kernel, global_work_size,
             local_work_size, runtime);
  }

  SECTION("most significant bit in all components of c are not set") {
    const std::vector<data_host_type> input_a(global_work_size,
                                              data_host_type(2));
    const std::vector<data_host_type> input_b(global_work_size,
                                              data_host_type(3));
    const std::vector<select_host_type> input_c(
        global_work_size, value_with_msb_not_set<select_host_type>());
    run_test(input_a, input_b, input_c, kernel, global_work_size,
             local_work_size, runtime);
  }

  SECTION("most significant bit in even components of c are set, "
          "most significant bit in odd components of c are not set") {
    const std::vector<data_host_type> input_a(global_work_size,
                                              data_host_type(2));
    const std::vector<data_host_type> input_b(global_work_size,
                                              data_host_type(3));
    std::vector<select_host_type> input_c(global_work_size);
    for (size_t i = 0; i < input_c.size(); ++i) {
      input_c[i] = i % 2 ? value_with_msb_set<select_host_type>()
                         : value_with_msb_not_set<select_host_type>();
    }
    run_test(input_a, input_b, input_c, kernel, global_work_size,
             local_work_size, runtime);
  }

  SECTION("random values") {
    const std::vector<data_host_type> input_a =
        ca::generate_vector<data_host_type>(global_work_size, 0);
    const std::vector<data_host_type> input_b =
        ca::generate_vector<data_host_type>(global_work_size, 0);
    const std::vector<select_host_type> input_c =
        ca::generate_vector<select_host_type>(global_work_size, 0);
    run_test(input_a, input_b, input_c, kernel, global_work_size,
             local_work_size, runtime);
  }

  runtime->release_kernel(kernel);
}

TEMPLATE_TEST_CASE(
    "select", "", ca::clc_char_t, ca::clc_char2_t, ca::clc_char3_t,
    ca::clc_char4_t, ca::clc_char8_t, ca::clc_char16_t, ca::clc_uchar_t,
    ca::clc_uchar2_t, ca::clc_uchar3_t, ca::clc_uchar4_t, ca::clc_uchar8_t,
    ca::clc_uchar16_t, ca::clc_short_t, ca::clc_short2_t, ca::clc_short3_t,
    ca::clc_short4_t, ca::clc_short8_t, ca::clc_short16_t, ca::clc_ushort_t,
    ca::clc_ushort2_t, ca::clc_ushort3_t, ca::clc_ushort4_t, ca::clc_ushort8_t,
    ca::clc_ushort16_t, ca::clc_int_t, ca::clc_int2_t, ca::clc_int3_t,
    ca::clc_int4_t, ca::clc_int8_t, ca::clc_int16_t, ca::clc_uint_t,
    ca::clc_uint2_t, ca::clc_uint3_t, ca::clc_uint4_t, ca::clc_uint8_t,
    ca::clc_uint16_t, ca::clc_long_t, ca::clc_long2_t, ca::clc_long3_t,
    ca::clc_long4_t, ca::clc_long8_t, ca::clc_long16_t, ca::clc_ulong_t,
    ca::clc_ulong2_t, ca::clc_ulong3_t, ca::clc_ulong4_t, ca::clc_ulong8_t,
    ca::clc_ulong16_t, ca::clc_float_t, ca::clc_float2_t, ca::clc_float3_t,
    ca::clc_float4_t, ca::clc_float8_t, ca::clc_float16_t, ca::clc_double_t,
    ca::clc_double2_t, ca::clc_double3_t, ca::clc_double4_t, ca::clc_double8_t,
    ca::clc_double16_t) {
  const TestConfig &config = get_test_config();

  ca::Requirements requirements;
  requirements.arithmetic_type<TestType>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }

  SECTION("signed") {
    using select_type = typename IGenType<TestType>::value;
    test_select<TestType, select_type>(config);
  }

  SECTION("unsigned") {
    using select_type = typename UGenType<TestType>::value;
    test_select<TestType, select_type>(config);
  }
}

} // namespace
