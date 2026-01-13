/*
 * Copyright (C) 2026 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <bitset>
#include <cassian/catch2_utils/catch2_utils.hpp>
#include <cassian/logging/logging.hpp>
#include <cassian/random/random.hpp>
#include <cassian/reference/extended_bit_operations.hpp>
#include <cassian/runtime/openclc_type_tuples.hpp>
#include <cassian/runtime/openclc_types.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cassian/test_harness/test_harness.hpp>
#include <cassian/utility/metaprogramming.hpp>
#include <cassian/utility/utility.hpp>
#include <cassian/vector/vector.hpp>
#include <catch2/catch.hpp>
#include <test_config.hpp>

namespace ca = cassian;
namespace {

template <typename INPUT_TYPE, typename OUTPUT_TYPE>
std::string get_build_options_basic() {
  std::stringstream stream;
  stream << " -DINPUT_TYPE=" << INPUT_TYPE::device_type
         << " -DOUTPUT_TYPE=" << OUTPUT_TYPE::device_type;
  return stream.str();
}

/**
 * calculate_reference_values unpacks data from vector types, and chooses
 * reference function
 *
 * @tparam INPUT_TYPE the type of input.
 * @tparam OUTPUT_TYPE the type of output.
 * @param[in] input vector of base input values.
 * @param[in] insert vector of bits to be inserted.
 * @param[in] offset vector of offset values.
 * @param[in] count  vector of numbers of bits to be inserted.
 * @param[in] vector_size vector size of cassian::Vector INPUT_TYPE (1 if
 * scalar)
 * @param[in] kernel_name name of kernel used in test.
 * @returns reference values bits.
 */
template <typename INPUT_TYPE, typename OUTPUT_TYPE>
std::vector<OUTPUT_TYPE> calculate_reference_values(
    std::vector<INPUT_TYPE> input, std::vector<INPUT_TYPE> insert,
    std::vector<uint32_t> offset, std::vector<uint32_t> count,
    uint32_t vector_size, const std::string &kernel_name) {
  std::vector<OUTPUT_TYPE> output;
  output.resize(input.size());
  for (size_t vector_id = 0; vector_id < count.size(); vector_id++) {
    for (size_t vector_offset = 0; vector_offset < vector_size;
         vector_offset++) {
      if (kernel_name == "test_basic_bitfield_insert") {
        output[vector_id * vector_size + vector_offset] =
            ca::bitfield_insert(input[vector_id * vector_size + vector_offset],
                                insert[vector_id * vector_size + vector_offset],
                                offset[vector_id], count[vector_id]);
      }
      if (kernel_name == "test_basic_bit_reverse") {
        output[vector_id * vector_size + vector_offset] =
            ca::bit_reverse(input[vector_id * vector_size + vector_offset]);
      }
      if (kernel_name == "test_basic_bitfield_extract_unsigned") {
        output[vector_id * vector_size + vector_offset] =
            ca::bitfield_extract_unsigned(
                input[vector_id * vector_size + vector_offset],
                offset[vector_id], count[vector_id]);
      }
      if (kernel_name == "test_basic_bitfield_extract_signed") {
        output[vector_id * vector_size + vector_offset] =
            ca::bitfield_extract_signed(
                input[vector_id * vector_size + vector_offset],
                offset[vector_id], count[vector_id]);
      }
    }
  }
  return output;
}

template <typename INPUT_TYPE, typename OUTPUT_TYPE>
void run_test_bit_instructions(const std::string &kernel_name) {
  using INPUT_HOST_TYPE = typename INPUT_TYPE::host_type;
  using INPUT_SCALAR_TYPE = typename INPUT_TYPE::scalar_type;
  using INPUT_SCALAR_HOST_TYPE = typename INPUT_SCALAR_TYPE::host_type;
  using OUTPUT_HOST_TYPE = typename OUTPUT_TYPE::host_type;
  using OUTPUT_SCALAR_TYPE = typename OUTPUT_TYPE::scalar_type;
  using OUTPUT_SCALAR_HOST_TYPE = typename OUTPUT_SCALAR_TYPE::host_type;

  const TestConfig &test_config = get_test_config();
  ca::Runtime *runtime = test_config.runtime();

  ca::Requirements requirements;
  requirements.arithmetic_type<INPUT_SCALAR_TYPE>();
  requirements.arithmetic_type<OUTPUT_SCALAR_TYPE>();
  requirements.feature(ca::Feature::extended_bit_operations);
  if (ca::should_skip_test(requirements, *runtime)) {
    return;
  }

  const int global_work_size = test_config.work_size();
  const int seed = static_cast<int>(Catch::rngSeed());
  const unsigned int vector_type_size = ca::get_vector_size<INPUT_HOST_TYPE>();
  const unsigned int data_size_in_bits = sizeof(INPUT_SCALAR_HOST_TYPE) * 8;
  std::vector<INPUT_SCALAR_HOST_TYPE> input_values =
      ca::generate_vector<INPUT_SCALAR_HOST_TYPE>(
          global_work_size * vector_type_size, seed);
  std::vector<INPUT_SCALAR_HOST_TYPE> insert_values =
      ca::generate_vector<INPUT_SCALAR_HOST_TYPE>(
          global_work_size * vector_type_size, seed);
  unsigned int reduce_bits = 0;
  if (kernel_name == "test_basic_bitfield_extract_signed") {
    reduce_bits = 1;
  }
  std::vector<uint32_t> offset = ca::generate_vector<uint32_t>(
      global_work_size, 0, data_size_in_bits - 1 - reduce_bits, seed);
  std::vector<uint32_t> count = ca::generate_vector<uint32_t>(
      global_work_size, 0, data_size_in_bits - reduce_bits, seed);
  // sum of count and offset can not be greater than number of bits.
  for (size_t each_c = 0; each_c < count.size(); each_c++) {
    while (offset[each_c] + count[each_c] > data_size_in_bits - reduce_bits) {
      count[each_c]--;
    }
  }
  std::vector<INPUT_HOST_TYPE> input =
      ca::pack_vector<INPUT_HOST_TYPE>(input_values);
  std::vector<INPUT_HOST_TYPE> insert =
      ca::pack_vector<INPUT_HOST_TYPE>(insert_values);

  // Prepare OCL
  ca::Buffer input_buffer =
      runtime->create_buffer(sizeof(INPUT_HOST_TYPE) * input.size());
  ca::Buffer insert_buffer =
      runtime->create_buffer(sizeof(INPUT_HOST_TYPE) * insert.size());
  ca::Buffer output_buffer =
      runtime->create_buffer(sizeof(OUTPUT_HOST_TYPE) * global_work_size);
  ca::Buffer offset_buffer =
      runtime->create_buffer(sizeof(uint32_t) * offset.size());
  ca::Buffer count_buffer =
      runtime->create_buffer(sizeof(uint32_t) * count.size());
  // Prepare kernel
  const std::string kernel_path =
      "kernels/oclc_extended_bit_operations/" + kernel_name + ".cl";
  std::string build_options =
      get_build_options_basic<INPUT_TYPE, OUTPUT_TYPE>();
  ca::logging::info() << "Kernel path: " << kernel_path << "\n";
  ca::logging::info() << "Kernel name: " << kernel_name << "\n";
  ca::logging::info() << "Build options: " << build_options << "\n";

  const std::string source = ca::load_text_file(ca::get_asset(kernel_path));
  ca::Kernel kernel = runtime->create_kernel(kernel_name, source, build_options,
                                             test_config.program_type());

  int index = 0;
  runtime->set_kernel_argument(kernel, index++, input_buffer);
  runtime->set_kernel_argument(kernel, index++, insert_buffer);
  runtime->set_kernel_argument(kernel, index++, output_buffer);
  runtime->set_kernel_argument(kernel, index++, offset_buffer);
  runtime->set_kernel_argument(kernel, index++, count_buffer);

  runtime->write_buffer_from_vector(input_buffer, input);
  runtime->write_buffer_from_vector(insert_buffer, insert);
  runtime->write_buffer_from_vector(offset_buffer, offset);
  runtime->write_buffer_from_vector(count_buffer, count);

  // Run kernel
  runtime->run_kernel(kernel, global_work_size);

  std::vector<OUTPUT_HOST_TYPE> device_results =
      runtime->read_buffer_to_vector<OUTPUT_HOST_TYPE>(output_buffer);

  std::vector<INPUT_SCALAR_HOST_TYPE> initial_input(input_values);
  std::vector<OUTPUT_SCALAR_HOST_TYPE> host_reference_results =
      calculate_reference_values<INPUT_SCALAR_HOST_TYPE,
                                 OUTPUT_SCALAR_HOST_TYPE>(
          input_values, insert_values, offset, count, vector_type_size,
          kernel_name);
  for (size_t vector_id = 0; vector_id < count.size(); vector_id++) {
    for (size_t vector_offset = 0; vector_offset < vector_type_size;
         vector_offset++) {
      ca::logging::debug()
          << "before_insert = "
          << std::bitset<data_size_in_bits>(
                 initial_input[vector_id * vector_type_size + vector_offset])
          << " after_insert = "
          << std::bitset<data_size_in_bits>(
                 host_reference_results[vector_id * vector_type_size +
                                        vector_offset])
          << " insert = "
          << std::bitset<data_size_in_bits>(
                 insert_values[vector_id * vector_type_size + vector_offset])
          << " offset = " << offset[vector_id]
          << " count = " << count[vector_id] << "\n";
    }
  }
  const std::vector<OUTPUT_HOST_TYPE> host_reference_results_packed =
      ca::pack_vector<OUTPUT_HOST_TYPE>(host_reference_results);
  // device_results means 'Actual' when printing mismatches
  REQUIRE(device_results == host_reference_results_packed);

  runtime->release_buffer(input_buffer);
  runtime->release_buffer(insert_buffer);
  runtime->release_buffer(output_buffer);
  runtime->release_buffer(offset_buffer);
  runtime->release_buffer(count_buffer);
}

template <typename INPUT_TYPE> std::string sbfe_test_case_name() {
  std::stringstream stream;
  stream << "input: " << INPUT_TYPE::device_type
         << " output: " << INPUT_TYPE::signed_type::device_type;
  return stream.str();
}

template <typename INPUT_TYPE> std::string ubfe_test_case_name() {
  std::stringstream stream;
  stream << "input: " << INPUT_TYPE::device_type
         << " output: " << INPUT_TYPE::unsigned_type::device_type;
  return stream.str();
}

template <typename INPUT_TYPE> std::string bfrev_test_case_name() {
  std::stringstream stream;
  stream << INPUT_TYPE::device_type;
  return stream.str();
}

template <typename INPUT_TYPE> std::string bfi_test_case_name() {
  std::stringstream stream;
  stream << INPUT_TYPE::device_type;
  return stream.str();
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("bitfield_insert", "", ca::IntegerTypes,
                                    bfi_test_case_name<TestType>) {
  run_test_bit_instructions<TestType, TestType>("test_basic_bitfield_insert");
};
TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("bit_reverse", "", ca::IntegerTypes,
                                    bfrev_test_case_name<TestType>) {
  run_test_bit_instructions<TestType, TestType>("test_basic_bit_reverse");
};
TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("bitfield_extract_unsigned", "",
                                    ca::IntegerTypes,
                                    ubfe_test_case_name<TestType>) {
  run_test_bit_instructions<TestType, typename TestType::unsigned_type>(
      "test_basic_bitfield_extract_unsigned");
};
TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("bitfield_extract_signed", "",
                                    ca::IntegerTypes,
                                    sbfe_test_case_name<TestType>) {
  run_test_bit_instructions<TestType, typename TestType::signed_type>(
      "test_basic_bitfield_extract_signed");
};

} // namespace
