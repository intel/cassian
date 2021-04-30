/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/runtime/feature.hpp>
#include <cassian/runtime/runtime.hpp>
#include <common.hpp>

int suggest_work_size(const std::string &type) {
  const int default_size = 16;
  const int min_size = 1;
  const int max_size = 1024;
  if (type == "min") {
    return min_size;
  }
  if (type == "max") {
    return max_size;
  }
  return default_size;
}

template <> std::string to_cm_string<int64_t>(int64_t value) {
  return std::to_string(value) + "LL";
}

template <> std::string to_cm_string<uint32_t>(uint32_t value) {
  return std::to_string(value) + "U";
}

template <> std::string to_cm_string<uint64_t>(uint64_t value) {
  return std::to_string(value) + "ULL";
}

template <> std::string to_cm_string<float>(float value) {
  return std::to_string(value) + "F";
}

template <> std::string to_cm_string<cassian::Half>(cassian::Half value) {
  return std::to_string(static_cast<float>(value));
}

std::string to_compiler_flag(const OperandType op) {
  switch (op) {
  case OperandType::vector:
    return "VECTOR";
  case OperandType::vector_ref:
    return "VECTOR_REF";
  case OperandType::matrix:
    return "MATRIX";
  case OperandType::matrix_ref:
    return "MATRIX_REF";
  case OperandType::scalar:
    return "SCALAR";
  case OperandType::constant:
    return "CONST";
  }
  return "invalid";
}

UnarySource &UnarySource::instance() {
  static UnarySource src;
  return src;
}

UnaryIntrinsicSource &UnaryIntrinsicSource::instance() {
  static UnaryIntrinsicSource src;
  return src;
}

BinarySource &BinarySource::instance() {
  static BinarySource src;
  return src;
}

BinaryIntrinsicSource &BinaryIntrinsicSource::instance() {
  static BinaryIntrinsicSource src;
  return src;
}
