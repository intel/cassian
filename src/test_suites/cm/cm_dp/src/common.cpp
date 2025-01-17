/*
 * Copyright (C) 2021-2025 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <common.hpp>
#include <cstdio>

namespace ca = cassian;
using ca::test::FlagsBuilder;

std::string operand_to_flag(const OperandOrder order, const InputType type) {
  int num = order == OperandOrder::first ? 0 : 1;
  switch (type) {
  case InputType::vector:
    return "INPUT" + std ::to_string(num) + "_VECTOR";
  case InputType::vector_ref:
    return "INPUT" + std ::to_string(num) + "_VECTOR_REF";
  case InputType::matrix:
    return "INPUT" + std ::to_string(num) + "_MATRIX";
  case InputType::matrix_ref:
    return "INPUT" + std ::to_string(num) + "_MATRIX_REF";
  case InputType::scalar:
    return "INPUT" + std ::to_string(num) + "_SCALAR";
  case InputType::constant:
    return "INPUT" + std ::to_string(num) + "_CONST";
  }
  return "INVALID_TYPE";
}

std::vector<ca::Vector<float, 4>> splitter(std::vector<float> input) {
  const size_t iterations = input.size() / 4;
  std::vector<ca::Vector<float, 4>> output(iterations);
  for (size_t i = 0; i < iterations; ++i) {
    std::vector<float> tmp;
    std::copy_n(std::begin(input) + 4 * i, 4, std::back_inserter(tmp));
    output[i] = ca::Vector<float, 4>(tmp);
  }
  return output;
}

void intput_converter(OperandOrder operand, int simd_u, int simd_v, float a,
                      float b, InputType input_type, std::vector<float> &vec,
                      FlagsBuilder &fb) {
  int num = operand == OperandOrder::first ? 0 : 1;
  switch (input_type) {
  case InputType::vector_ref:
  case InputType::vector: {
    vec = ca::generate_vector<float>(simd_u * simd_v, a, b, 0);
    ca::test::input(vec);
    fb.define("SIMD" + std::to_string(num), std::to_string(simd_u * simd_v));
  } break;
  case InputType::matrix_ref:
  case InputType::matrix: {
    vec = ca::generate_vector<float>(simd_u * simd_v, a, b, 0);
    ca::test::input(vec);
    fb.define("SIMD" + std::to_string(num) + "_U", std::to_string(simd_u));
    fb.define("SIMD" + std::to_string(num) + "_V", std::to_string(simd_v));
  } break;
  case InputType::scalar: {
    float in_scalar = ca::generate_value<float>(a, b, 0);
    std::fill(vec.begin(), vec.end(), in_scalar);
    ca::test::pass(in_scalar);
  } break;
  case InputType::constant: {
    float in_scalar = ca::generate_value<float>(a, b, 0);
    std::fill(vec.begin(), vec.end(), in_scalar);
    const std::size_t buffer_size = 32;
    char float_chars[buffer_size]{};
    // use snprintf because there is no viable alternative for used compiler
    // versions
    int written =
        std::snprintf(float_chars, buffer_size, "%.20g", in_scalar); // NOLINT
    if (written < 0 || written >= buffer_size) {
      throw std::runtime_error("Value could not be written to supplied buffer");
    }
    fb.define("INPUT" + std::to_string(num), float_chars);
  } break;
  }
  fb.define(operand_to_flag(operand, input_type));
}