/*
 * Copyright (C) 2021-2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

// Enable math defines from cmath module
#define _USE_MATH_DEFINES

#include <algorithm>
#include <cassian/catch2_utils/catch2_utils.hpp>
#include <cassian/logging/logging.hpp>
#include <cassian/runtime/openclc_type_tuples.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cassian/test_harness/test_harness.hpp>
#include <cassian/utility/metaprogramming.hpp>
#include <cassian/utility/utility.hpp>
#include <catch2/catch.hpp>
#include <common.hpp>
#include <cstddef>
#include <cstdint>
#include <enum_definitions.hpp>
#include <math_functions.hpp>
#include <string>
#include <test_config.hpp>
#include <vector>
namespace ca = cassian;

namespace {
template <typename T_1, typename T_2, typename T_3> struct Input {
  std::vector<T_1> input_a;
  std::vector<T_2> input_b;
  std::vector<T_3> input_c;
  Input(std::vector<T_1> input_a, std::vector<T_2> input_b,
        std::vector<T_3> input_c)
      : input_a(std::move(input_a)), input_b(std::move(input_b)),
        input_c(std::move(input_c)) {}
};

template <typename T>
void create_and_set_buffer(const std::vector<T> &input, ca::Runtime *runtime,
                           std::vector<ca::Buffer> *buffers,
                           const ca::Kernel &kernel) {
  const auto buffer = runtime->create_buffer(input.size() * sizeof(T));
  runtime->write_buffer(buffer, input.data());
  buffers->push_back(buffer);
  runtime->set_kernel_argument(kernel, buffers->size() - 1, buffer);
}

template <typename T, typename INPUT, typename INPUT_B, typename INPUT_C>
std::vector<T> run_gentype_kernel(const size_t &work_size, const INPUT &input,
                                  std::vector<INPUT_B> &argument_2_output,
                                  std::vector<INPUT_C> &argument_3_output,
                                  ca::Runtime *runtime,
                                  const ca::Kernel &kernel,
                                  const bool is_store) {
  std::vector<ca::Buffer> buffers;
  const auto buffer_out = runtime->create_buffer(work_size * sizeof(T));
  buffers.push_back(buffer_out);
  runtime->set_kernel_argument(kernel, 0, buffer_out);
  create_and_set_buffer(input.input_a, runtime, &buffers, kernel);
  if (!input.input_b.empty()) {
    create_and_set_buffer(input.input_b, runtime, &buffers, kernel);
  }
  if (!input.input_c.empty()) {
    create_and_set_buffer(input.input_c, runtime, &buffers, kernel);
  }
  runtime->run_kernel(kernel, work_size);
  auto kernel_output = runtime->read_buffer_to_vector<T>(buffer_out);
  if (!input.input_b.empty() && is_store) {
    argument_2_output = runtime->read_buffer_to_vector<INPUT_B>(buffers.at(2));
  }
  if (!input.input_c.empty() && is_store) {
    argument_3_output = runtime->read_buffer_to_vector<INPUT_C>(buffers.at(3));
  }

  for (const auto &buffer : buffers) {
    runtime->release_buffer(buffer);
  }
  return kernel_output;
}

template <typename GENTYPE, typename INPUT, typename INPUT_B, typename INPUT_C>
std::vector<GENTYPE>
test_gentype(const INPUT &input, std::vector<INPUT_B> &argument_2_output,
             std::vector<INPUT_C> &argument_3_output,
             const std::string &build_options, const TestConfig &config,
             const bool is_store) {
  auto *runtime = config.runtime();
  std::string path = is_store
                         ? "kernels/oclc_math_functions/math_functions_store.cl"
                         : "kernels/oclc_math_functions/math_functions.cl";
  const std::string source = ca::load_text_file(ca::get_asset(path));
  ca::Kernel kernel = runtime->create_kernel("test", source, build_options,
                                             config.program_type());
  auto output = run_gentype_kernel<GENTYPE, INPUT, INPUT_B>(
      config.work_size(), input, argument_2_output, argument_3_output, runtime,
      kernel, is_store);
  runtime->release_kernel(kernel);
  return output;
}

template <class T, typename INPUT>
void run_section(const T &oclc_function, INPUT &input,
                 const TestConfig &config) {
  using output_type = typename T::output_type;
  using input_b_type = typename T::input_type_2;
  using input_c_type = typename T::input_type_3;
  const auto work_size = config.work_size();

  auto reference_vector = std::vector<output_type>(work_size);
  auto reference_vector_2 = std::vector<input_b_type>(
      work_size); // vector for reference values returned by function argument
  auto reference_vector_3 = std::vector<input_c_type>(work_size);
  for (auto j = 0; j < work_size; j++) {
    if constexpr (T::arg_num == 3) {
      reference_vector[j] = T::calculate_reference(
          input.input_a[j], input.input_b[j], input.input_c[j]);
      reference_vector_3[j] = input.input_c[j];
    } else if constexpr (T::arg_num == 2) {
      reference_vector[j] =
          T::calculate_reference(input.input_a[j], input.input_b[j]);
      reference_vector_2[j] = input.input_b[j];
    } else {
      reference_vector[j] = T::calculate_reference(input.input_a[j]);
    }
  }
  std::vector<input_b_type> argument_2_output(work_size);
  std::vector<input_c_type> argument_3_output(work_size);
  const auto result = test_gentype<output_type, INPUT, input_b_type>(
      input, argument_2_output, argument_3_output,
      oclc_function.get_build_options(), config, oclc_function.get_is_store());
  REQUIRE_THAT(result, UlpComparator<output_type>(reference_vector, work_size,
                                                  T::function));
  if (oclc_function.get_is_store() && T::arg_num == 2) {
    REQUIRE_THAT(argument_2_output,
                 UlpComparator<input_b_type>(reference_vector_2, work_size,
                                             T::function));
  }
  if (oclc_function.get_is_store() && T::arg_num == 3) {
    REQUIRE_THAT(argument_3_output,
                 UlpComparator<input_c_type>(reference_vector_3, work_size,
                                             T::function));
  }
}

template <typename T>
std::vector<T> create_input_vector(const T &input, const uint32_t &arg_index,
                                   const uint32_t &arg_num,
                                   const size_t &work_size) {
  if (arg_num < arg_index + 1) {
    return std::vector<T>();
  }
  std::vector<T> input_vector(work_size);
  std::fill(input_vector.begin(), input_vector.end(), input);
  return input_vector;
}

template <class T, SectionType section_type>
void run_specific_section(const T &oclc_function,
                          const std::vector<Value<T>> &input_values) {
  const auto function_string = oclc_function.get_function_string();
  const auto build_options = oclc_function.get_build_options();
  const TestConfig &config = get_test_config();
  const auto work_size = config.work_size();
  using input_type_1 = typename T::input_type_1;
  using input_type_2 = typename T::input_type_2;
  using input_type_3 = typename T::input_type_3;
  for (auto i = 0U; i < input_values.size(); i++) {
    const auto section_name = create_section_name<section_type>(
        function_string, i, input_values.size());
    SECTION(section_name) {
      auto input_a = create_input_vector<input_type_1>(
          input_values[i].get_val_1(), 0, T::arg_num, work_size);
      auto input_b = create_input_vector<input_type_2>(
          input_values[i].get_val_2(), 1, T::arg_num, work_size);
      auto input_c = create_input_vector<input_type_3>(
          input_values[i].get_val_3(), 2, T::arg_num, work_size);
      if (section_type == SectionType::random) {
        input_a = randomize_input(input_a);
        input_b = randomize_input(input_b);
        input_c = randomize_input(input_c);
      }
      ca::logging::debug() << "Build options: " << build_options << '\n';
      ca::logging::debug() << "Input A: " << ca::to_string(input_a) << '\n';
      ca::logging::debug() << "Input B: " << ca::to_string(input_b) << '\n';
      ca::logging::debug() << "Input C: " << ca::to_string(input_c) << '\n';
      auto input = Input(input_a, input_b, input_c);
      run_section(oclc_function, input, config);
    }
  }
}

template <class T> void run_multiple_test_sections(const T &oclc_function) {
  const auto input = get_gentype_values<T>();
  run_specific_section<T, SectionType::random>(oclc_function,
                                               input.random_values);
  run_specific_section<T, SectionType::edge>(oclc_function,
                                             input.edge_case_values);
}

auto run_each_test = [](auto &&... oclc_function) {
  (run_multiple_test_sections(oclc_function), ...);
};

using Gentype = ca::TupleConcat<ca::TypesFloat, ca::TypesDouble>::type;

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("math_functions_gentype", "", Gentype,
                                    test_name<TestType>) {
  const TestConfig &config = get_test_config();
  using scalar_type = typename TestType::scalar_type;
  ca::Requirements requirements;
  requirements.arithmetic_type<scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }
  using host_type = typename TestType::host_type;
  run_each_test(
      OclcFunction<Function::acos, 1, calculate_acos<host_type>, TestType>(),
      OclcFunction<Function::acosh, 1, calculate_acosh<host_type>, TestType>(),
      OclcFunction<Function::acospi, 1, calculate_acospi<host_type>,
                   TestType>(),
      OclcFunction<Function::asin, 1, calculate_asin<host_type>, TestType>(),
      OclcFunction<Function::asinh, 1, calculate_asinh<host_type>, TestType>(),
      OclcFunction<Function::asinpi, 1, calculate_asinpi<host_type>,
                   TestType>(),
      OclcFunction<Function::atan, 1, calculate_atan<host_type>, TestType>(),
      OclcFunction<Function::atan2, 2, calculate_atan2<host_type>, TestType>(),
      OclcFunction<Function::atanh, 1, calculate_atanh<host_type>, TestType>(),
      OclcFunction<Function::atanpi, 1, calculate_atanpi<host_type>,
                   TestType>(),
      OclcFunction<Function::atan2pi, 2, calculate_atan2pi<host_type>,
                   TestType>(),
      OclcFunction<Function::cbrt, 1, calculate_cbrt<host_type>, TestType>(),
      OclcFunction<Function::ceil, 1, calculate_ceil<host_type>, TestType>(),
      OclcFunction<Function::copysign, 2, calculate_copysign<host_type>,
                   TestType>(),
      OclcFunction<Function::cos, 1, calculate_cos<host_type>, TestType>(),
      OclcFunction<Function::cosh, 1, calculate_cosh<host_type>, TestType>(),
      OclcFunction<Function::cospi, 1, calculate_cospi<host_type>, TestType>(),
      OclcFunction<Function::sqrt, 1, calculate_sqrt<host_type>, TestType>(),
      OclcFunction<Function::mad, 3, calculate_mad<host_type>, TestType>(),
      OclcFunction<Function::maxmag, 2, calculate_maxmag<host_type>,
                   TestType>(),
      OclcFunction<Function::minmag, 2, calculate_minmag<host_type>,
                   TestType>(),
      OclcFunction<Function::nextafter, 2, calculate_nextafter<host_type>,
                   TestType>(),
      OclcFunction<Function::pow, 2, calculate_pow<host_type>, TestType>(),
      OclcFunction<Function::remainder, 2, calculate_remainder<host_type>,
                   TestType>(),
      OclcFunction<Function::rint, 1, calculate_rint<host_type>, TestType>(),
      OclcFunction<Function::round, 1, calculate_round<host_type>, TestType>(),
      OclcFunction<Function::rsqrt, 1, calculate_rsqrt<host_type>, TestType>(),
      OclcFunction<Function::sin, 1, calculate_sin<host_type>, TestType>(),
      OclcFunction<Function::sinh, 1, calculate_sinh<host_type>, TestType>(),
      OclcFunction<Function::sinpi, 1, calculate_sinpi<host_type>, TestType>(),
      OclcFunction<Function::tan, 1, calculate_tan<host_type>, TestType>(),
      OclcFunction<Function::tanh, 1, calculate_tanh<host_type>, TestType>(),
      OclcFunction<Function::tanpi, 1, calculate_tanpi<host_type>, TestType>(),
      OclcFunction<Function::tgamma, 1, calculate_tgamma<host_type>,
                   TestType>(),
      OclcFunction<Function::trunc, 1, calculate_trunc<host_type>, TestType>(),
      OclcFunction<Function::erfc, 1, calculate_erfc<host_type>, TestType>(),
      OclcFunction<Function::erf, 1, calculate_erf<host_type>, TestType>(),
      OclcFunction<Function::exp, 1, calculate_exp<host_type>, TestType>(),
      OclcFunction<Function::exp2, 1, calculate_exp2<host_type>, TestType>(),
      OclcFunction<Function::exp10, 1, calculate_exp10<host_type>, TestType>(),
      OclcFunction<Function::expm1, 1, calculate_expm1<host_type>, TestType>(),
      OclcFunction<Function::fabs, 1, calculate_fabs<host_type>, TestType>(),
      OclcFunction<Function::fdim, 2, calculate_fdim<host_type>, TestType>(),
      OclcFunction<Function::floor, 1, calculate_floor<host_type>, TestType>(),
      OclcFunction<Function::fma, 3, calculate_fma<host_type>, TestType>(),
      OclcFunction<Function::fmax, 2, calculate_fmax<host_type>, TestType>(),
      OclcFunction<Function::fmin, 2, calculate_fmin<host_type>, TestType>(),
      OclcFunction<Function::fmod, 2, calculate_fmod<host_type>, TestType>(),
      OclcFunction<Function::hypot, 2, calculate_hypot<host_type>, TestType>(),
      OclcFunction<Function::lgamma, 1, calculate_lgamma<host_type>,
                   TestType>(),
      OclcFunction<Function::log, 1, calculate_log<host_type>, TestType>(),
      OclcFunction<Function::log2, 1, calculate_log2<host_type>, TestType>(),
      OclcFunction<Function::log10, 1, calculate_log10<host_type>, TestType>(),
      OclcFunction<Function::log1p, 1, calculate_log1p<host_type>, TestType>(),
      OclcFunction<Function::logb, 1, calculate_logb<host_type>, TestType>(),
      OclcFunction<Function::powr, 2, calculate_powr<host_type>, TestType>());

  if constexpr (std::is_same_v<typename scalar_type::host_type, double>) {
    using int_type = typename TestType::logical_type::regressed_type;
    using uint_type = typename TestType::logical_type::unsigned_type;
    run_each_test(
        OclcFunction<Function::ilogb, 1,
                     calculate_ilogb<typename int_type::host_type, host_type>,
                     int_type, AddressSpace::clc_global, TestType>(),
        OclcFunction<Function::ldexp, 2,
                     calculate_ldexp<host_type, typename int_type::host_type>,
                     TestType, AddressSpace::clc_global, TestType, int_type>(),
        OclcFunction<Function::nan, 1,
                     calculate_nan<host_type, typename uint_type::host_type>,
                     TestType, AddressSpace::clc_global, uint_type>(),
        OclcFunction<Function::rootn, 2,
                     calculate_rootn<host_type, typename int_type::host_type>,
                     TestType, AddressSpace::clc_global, TestType, int_type>(),
        OclcFunction<Function::pown, 2,
                     calculate_pown<host_type, typename int_type::host_type>,
                     TestType, AddressSpace::clc_global, TestType, int_type>());
  } else {
    using int_type = typename TestType::logical_type;
    using uint_type = typename TestType::logical_type::unsigned_type;
    run_each_test(
        OclcFunction<Function::ilogb, 1,
                     calculate_ilogb<typename int_type::host_type, host_type>,
                     int_type, AddressSpace::clc_global, TestType>(),
        OclcFunction<Function::ldexp, 2,
                     calculate_ldexp<host_type, typename int_type::host_type>,
                     TestType, AddressSpace::clc_global, TestType, int_type>(),
        OclcFunction<Function::nan, 1,
                     calculate_nan<host_type, typename uint_type::host_type>,
                     TestType, AddressSpace::clc_global, uint_type>(),
        OclcFunction<Function::rootn, 2,
                     calculate_rootn<host_type, typename int_type::host_type>,
                     TestType, AddressSpace::clc_global, TestType, int_type>(),
        OclcFunction<Function::pown, 2,
                     calculate_pown<host_type, typename int_type::host_type>,
                     TestType, AddressSpace::clc_global, TestType, int_type>(),
        OclcFunction<Function::native_cos, 1, calculate_cos<host_type>,
                     TestType>(),
        OclcFunction<Function::native_divide, 2, calculate_divide<host_type>,
                     TestType>(),
        OclcFunction<Function::native_exp, 1, calculate_exp<host_type>,
                     TestType>(),
        OclcFunction<Function::native_exp2, 1, calculate_exp2<host_type>,
                     TestType>(),
        OclcFunction<Function::native_exp10, 1, calculate_exp10<host_type>,
                     TestType>(),
        OclcFunction<Function::native_log, 1, calculate_log<host_type>,
                     TestType>(),
        OclcFunction<Function::native_log2, 1, calculate_log2<host_type>,
                     TestType>(),
        OclcFunction<Function::native_log10, 1, calculate_log10<host_type>,
                     TestType>(),
        OclcFunction<Function::native_powr, 2, calculate_pow<host_type>,
                     TestType>(),
        OclcFunction<Function::native_recip, 1, calculate_recip<host_type>,
                     TestType>(),
        OclcFunction<Function::native_rsqrt, 1, calculate_rsqrt<host_type>,
                     TestType>(),
        OclcFunction<Function::native_sin, 1, calculate_sin<host_type>,
                     TestType>(),
        OclcFunction<Function::native_sqrt, 1, calculate_sqrt<host_type>,
                     TestType>(),
        OclcFunction<Function::native_tan, 1, calculate_tan<host_type>,
                     TestType>(),
        OclcFunction<Function::half_cos, 1, calculate_cos<host_type>,
                     TestType>(),
        OclcFunction<Function::half_divide, 2, calculate_divide<host_type>,
                     TestType>(),
        OclcFunction<Function::half_exp, 1, calculate_exp<host_type>,
                     TestType>(),
        OclcFunction<Function::half_exp2, 1, calculate_exp2<host_type>,
                     TestType>(),
        OclcFunction<Function::half_exp10, 1, calculate_exp10<host_type>,
                     TestType>(),
        OclcFunction<Function::half_log, 1, calculate_log<host_type>,
                     TestType>(),
        OclcFunction<Function::half_log2, 1, calculate_log2<host_type>,
                     TestType>(),
        OclcFunction<Function::half_log10, 1, calculate_log10<host_type>,
                     TestType>(),
        OclcFunction<Function::half_powr, 2, calculate_powr<host_type>,
                     TestType>(),
        OclcFunction<Function::half_recip, 1, calculate_recip<host_type>,
                     TestType>(),
        OclcFunction<Function::half_rsqrt, 1, calculate_rsqrt<host_type>,
                     TestType>(),
        OclcFunction<Function::half_sin, 1, calculate_sin<host_type>,
                     TestType>(),
        OclcFunction<Function::half_sqrt, 1, calculate_sqrt<host_type>,
                     TestType>(),
        OclcFunction<Function::half_tan, 1, calculate_tan<host_type>,
                     TestType>());
  }
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("math_functions_specializations", "",
                                    Gentype, test_name<TestType>) {
  const TestConfig &config = get_test_config();
  using scalar_type = typename TestType::scalar_type;
  ca::Requirements requirements;
  requirements.arithmetic_type<scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }
  using host_type = typename TestType::host_type;
  run_each_test(
      OclcFunction<Function::fmax, 2,
                   calculate_fmax<host_type, typename scalar_type::host_type>,
                   TestType, AddressSpace::clc_global, TestType, scalar_type>(),
      OclcFunction<Function::fmin, 2,
                   calculate_fmin<host_type, typename scalar_type::host_type>,
                   TestType, AddressSpace::clc_global, TestType,
                   scalar_type>());
  if constexpr (std::is_same_v<typename scalar_type::host_type, double>) {
    using int_type =
        typename TestType::logical_type::regressed_type::scalar_type;
    run_each_test(
        OclcFunction<Function::ldexp, 2,
                     calculate_ldexp<host_type, typename int_type::host_type>,
                     TestType, AddressSpace::clc_global, TestType, int_type>());
  } else {
    using int_type = typename TestType::logical_type::scalar_type;
    run_each_test(
        OclcFunction<Function::ldexp, 2,
                     calculate_ldexp<host_type, typename int_type::host_type>,
                     TestType, AddressSpace::clc_global, TestType, int_type>());
  }
}

TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("math_functions_gentype - store", "",
                                    Gentype, test_name<TestType>) {
  const TestConfig &config = get_test_config();
  using scalar_type = typename TestType::scalar_type;
  ca::Requirements requirements;
  requirements.arithmetic_type<scalar_type>();
  if (ca::should_skip_test(requirements, *config.runtime())) {
    return;
  }
  using host_type = typename TestType::host_type;
  run_each_test(
      OclcFunction<Function::fract, 2, calculate_fract<host_type>, TestType>(),
      OclcFunction<Function::sincos, 2, calculate_sincos<host_type>,
                   TestType>(),
      OclcFunction<Function::modf, 2, calculate_modf<host_type>, TestType>(),
      OclcFunction<Function::fract, 2, calculate_fract<host_type>, TestType,
                   AddressSpace::clc_local>(),
      OclcFunction<Function::sincos, 2, calculate_sincos<host_type>, TestType,
                   AddressSpace::clc_local>(),
      OclcFunction<Function::modf, 2, calculate_modf<host_type>, TestType,
                   AddressSpace::clc_local>(),
      OclcFunction<Function::fract, 2, calculate_fract<host_type>, TestType,
                   AddressSpace::clc_private>(),
      OclcFunction<Function::sincos, 2, calculate_sincos<host_type>, TestType,
                   AddressSpace::clc_private>(),
      OclcFunction<Function::modf, 2, calculate_modf<host_type>, TestType,
                   AddressSpace::clc_private>());
  if constexpr (std::is_same_v<typename scalar_type::host_type, double>) {
    using int_type = typename TestType::logical_type::regressed_type;
    run_each_test(
        OclcFunction<Function::frexp, 2,
                     calculate_frexp<host_type, typename int_type::host_type>,
                     TestType, AddressSpace::clc_global, TestType, int_type>(),
        OclcFunction<Function::remquo, 3,
                     calculate_remquo<host_type, typename int_type::host_type>,
                     TestType, AddressSpace::clc_global, TestType, TestType,
                     int_type>(),
        OclcFunction<
            Function::lgamma_r, 2,
            calculate_lgamma_r<host_type, typename int_type::host_type>,
            TestType, AddressSpace::clc_global, TestType, int_type>(),
        OclcFunction<Function::frexp, 2,
                     calculate_frexp<host_type, typename int_type::host_type>,
                     TestType, AddressSpace::clc_local, TestType, int_type>(),
        OclcFunction<Function::remquo, 3,
                     calculate_remquo<host_type, typename int_type::host_type>,
                     TestType, AddressSpace::clc_local, TestType, TestType,
                     int_type>(),
        OclcFunction<
            Function::lgamma_r, 2,
            calculate_lgamma_r<host_type, typename int_type::host_type>,
            TestType, AddressSpace::clc_local, TestType, int_type>(),
        OclcFunction<Function::frexp, 2,
                     calculate_frexp<host_type, typename int_type::host_type>,
                     TestType, AddressSpace::clc_private, TestType, int_type>(),
        OclcFunction<Function::remquo, 3,
                     calculate_remquo<host_type, typename int_type::host_type>,
                     TestType, AddressSpace::clc_private, TestType, TestType,
                     int_type>(),
        OclcFunction<
            Function::lgamma_r, 2,
            calculate_lgamma_r<host_type, typename int_type::host_type>,
            TestType, AddressSpace::clc_private, TestType, int_type>());
  } else {
    using int_type = typename TestType::logical_type;
    run_each_test(
        OclcFunction<Function::frexp, 2,
                     calculate_frexp<host_type, typename int_type::host_type>,
                     TestType, AddressSpace::clc_global, TestType, int_type>(),
        OclcFunction<Function::remquo, 3,
                     calculate_remquo<host_type, typename int_type::host_type>,
                     TestType, AddressSpace::clc_global, TestType, TestType,
                     int_type>(),
        OclcFunction<
            Function::lgamma_r, 2,
            calculate_lgamma_r<host_type, typename int_type::host_type>,
            TestType, AddressSpace::clc_global, TestType, int_type>(),
        OclcFunction<Function::frexp, 2,
                     calculate_frexp<host_type, typename int_type::host_type>,
                     TestType, AddressSpace::clc_local, TestType, int_type>(),
        OclcFunction<Function::remquo, 3,
                     calculate_remquo<host_type, typename int_type::host_type>,
                     TestType, AddressSpace::clc_local, TestType, TestType,
                     int_type>(),
        OclcFunction<
            Function::lgamma_r, 2,
            calculate_lgamma_r<host_type, typename int_type::host_type>,
            TestType, AddressSpace::clc_local, TestType, int_type>(),
        OclcFunction<Function::frexp, 2,
                     calculate_frexp<host_type, typename int_type::host_type>,
                     TestType, AddressSpace::clc_private, TestType, int_type>(),
        OclcFunction<Function::remquo, 3,
                     calculate_remquo<host_type, typename int_type::host_type>,
                     TestType, AddressSpace::clc_private, TestType, TestType,
                     int_type>(),
        OclcFunction<
            Function::lgamma_r, 2,
            calculate_lgamma_r<host_type, typename int_type::host_type>,
            TestType, AddressSpace::clc_private, TestType, int_type>());
  }
}
} // namespace
