/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/catch2_utils/catch2_utils.hpp>
#include <cassian/logging/logging.hpp>
#include <cassian/random/random.hpp>
#include <cassian/runtime/openclc_type_tuples.hpp>
#include <cassian/runtime/openclc_types.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cassian/test_harness/test_harness.hpp>
#include <cassian/utility/metaprogramming.hpp>
#include <cassian/utility/utility.hpp>
#include <cassian/vector/type_helpers.hpp>
#include <cassian/vector/vector.hpp>
#include <catch2/catch.hpp>
#include <common.hpp>
#include <cstddef>
#include <enum_definitions.hpp>
#include <integer_functions.hpp>
#include <map>
#include <string>
#include <test_config.hpp>
#include <utility>
#include <vector>

namespace ca = cassian;

namespace {

template <typename T>
void create_and_set_buffers(const std::map<int, T> &input_map,
                            ca::Runtime *runtime,
                            std::vector<ca::Buffer> *buffers,
                            const ca::Kernel &kernel) {
  for (const auto &[index, input] : input_map) {
    auto &buffer = buffers->at(index + 1);
    buffer = runtime->create_buffer(sizeof(input));
    runtime->write_buffer(buffer, &input);
    runtime->set_kernel_argument(kernel, index + 1, buffer);
  }
}

template <typename T, typename A, typename B>
std::vector<T>
run_gentype_kernel(const size_t &size, const std::map<int, A> &input_map_1,
                   const std::map<int, B> &input_map_2, ca::Runtime *runtime,
                   const ca::Kernel &kernel) {
  std::vector<ca::Buffer> buffers(input_map_1.size() + input_map_2.size() + 1);
  auto &buffer_out = buffers[0];
  buffer_out = runtime->create_buffer(size * sizeof(T));
  runtime->set_kernel_argument(kernel, 0, buffer_out);

  create_and_set_buffers(input_map_1, runtime, &buffers, kernel);
  create_and_set_buffers(input_map_2, runtime, &buffers, kernel);

  runtime->run_kernel(kernel, size);
  auto kernel_output = runtime->read_buffer_to_vector<T>(buffer_out);
  for (auto &buffer : buffers) {
    runtime->release_buffer(buffer);
  }
  return kernel_output;
}

template <typename OUTPUT_TYPE, typename A, typename B>
std::vector<OUTPUT_TYPE> test_gentype(const std::map<int, A> &input_map_1,
                                      const std::map<int, B> &input_map_2,
                                      const std::string &build_options,
                                      const TestConfig &config) {
  auto *runtime = config.runtime();
  const std::string source = ca::load_text_file(
      ca::get_asset("kernels/oclc_integer_functions/integer_functions.cl"));
  ca::Kernel kernel = runtime->create_kernel("test", source, build_options,
                                             config.program_type());
  auto output = run_gentype_kernel<OUTPUT_TYPE, A, B>(
      config.work_size(), input_map_1, input_map_2, runtime, kernel);
  runtime->release_kernel(kernel);
  return output;
}

std::string create_section_name(const std::string &function_string,
                                const int &version) {
  std::stringstream ss;
  ss << "Function: " << function_string
     << (version != 0 ? " version " + std::to_string(version) : "");
  return ss.str();
}

using GentypeTypes =
    ca::TupleConcat<ca::TypesInt, ca::TypesUint, ca::TypesLong, ca::TypesUlong,
                    ca::TypesChar, ca::TypesUchar, ca::TypesShort,
                    ca::TypesUshort>::type;

template <typename T, typename GENTYPE, typename UGENTYPE,
          typename INPUT_MAP_TYPE_1, typename INPUT_MAP_TYPE_2>
void run_test(const std::vector<T> &list_of_functions,
              const TestConfig &config) {
  const size_t work_size = config.work_size();
  for (auto &oclc_function : list_of_functions) {
    const auto function_string =
        Catch::StringMaker<Function>::convert(oclc_function.get_function());
    const auto section_name =
        create_section_name(function_string, oclc_function.get_version());
    DYNAMIC_SECTION(section_name) {
      ca::logging::debug() << "Input values:" << oclc_function.input_to_string()
                           << '\n';
      const std::string build_options =
          oclc_function.get_build_options(function_string);
      const auto reference = oclc_function.get_reference_value();
      if (oclc_function.get_output_type() == OutputType::ugentype) {
        const auto result =
            test_gentype<UGENTYPE, INPUT_MAP_TYPE_1, INPUT_MAP_TYPE_2>(
                oclc_function.get_input_map_1(),
                oclc_function.get_input_map_2(), build_options, config);
        const auto reference_vector =
            std::vector<UGENTYPE>(work_size, UGENTYPE(reference));
        REQUIRE_THAT(result, Catch::Equals(reference_vector));
      } else {
        const auto result =
            test_gentype<GENTYPE, INPUT_MAP_TYPE_1, INPUT_MAP_TYPE_2>(
                oclc_function.get_input_map_1(),
                oclc_function.get_input_map_2(), build_options, config);
        const auto reference_vector =
            std::vector<GENTYPE>(work_size, GENTYPE(reference));
        REQUIRE_THAT(result, Catch::Equals(reference_vector));
      }
    }
  }
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("gentype_integer_functions", "",
                                    GentypeTypes, test_name<TestType>) {
  const TestConfig &config = get_test_config();
  using scalar_type = typename TestType::scalar_type;
  using gentype = typename TestType::host_type;
  using ugentype = typename TestType::unsigned_type::host_type;
  using sgentype = typename scalar_type::host_type;
  ca::Requirements requirements;
  requirements.arithmetic_type<scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }
  using OclcFunc = OclcGentypeFunction<TestType, gentype, sgentype>;
  const std::initializer_list<OclcFunc> list_of_gentype_functions{
      OclcFunc(Function::abs, {gentype(10)}),
      OclcFunc(Function::abs_diff, {gentype(10), gentype(5)}),
      OclcFunc(Function::add_sat, {gentype(15), gentype(5)}),
      OclcFunc(Function::hadd, {gentype(15), gentype(10)}),
      OclcFunc(Function::rhadd, {gentype(15), gentype(10)}),
      OclcFunc(1, Function::clamp, {gentype(15), gentype(5), gentype(20)}),
      OclcFunc(2, Function::clamp, {gentype(15)}, {sgentype(5), sgentype(20)}),
      OclcFunc(Function::clz, {gentype(15)})};
  run_test<OclcFunc, gentype, ugentype, gentype, sgentype>(
      list_of_gentype_functions, config);
}

using SpecialTypes =
    ca::TupleConcat<ca::TypesInt, ca::TypesUint, ca::TypesLong, ca::TypesUlong,
                    ca::TypesShort, ca::TypesUshort>::type;
template <class OclcFunc, typename T, typename regressed_type,
          typename uregressed_type>
constexpr std::vector<OclcFunc> get_special_functions() {
  if constexpr (std::is_unsigned<T>::value) {
    return {OclcFunc(Function::upsample, std::vector<regressed_type>(),
                     {uregressed_type(1), uregressed_type(2)})};
  } else {
    return {OclcFunc(Function::upsample, {regressed_type(1)},
                     {uregressed_type(2)})};
  }
};
TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("special_functions", "", SpecialTypes,
                                    test_name<TestType>) {
  using scalar_type = typename TestType::scalar_type;
  using gentype = typename TestType::host_type;
  using sgentype = typename scalar_type::host_type;
  using ugentype = typename TestType::unsigned_type::host_type;
  using regressed_type = typename TestType::regressed_type::host_type;
  using uregressed_type =
      typename TestType::regressed_type::unsigned_type::host_type;

  ca::Requirements requirements;
  requirements.arithmetic_type<scalar_type>();
  const TestConfig &config = get_test_config();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }
  using OclcFunc =
      SpecialRegressedFunction<TestType, regressed_type, uregressed_type>;
  const auto list_of_special_func =
      get_special_functions<OclcFunc, sgentype, regressed_type,
                            uregressed_type>();
  run_test<OclcFunc, gentype, ugentype, regressed_type, uregressed_type>(
      list_of_special_func, config);
}

} // namespace
