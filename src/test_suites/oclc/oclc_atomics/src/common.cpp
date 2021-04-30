/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/runtime/device_properties.hpp>
#include <cassian/runtime/runtime.hpp>
#include <common.hpp>

std::string to_string(FunctionType function_type) {
  switch (function_type) {
  case FunctionType::implicit:
    return "implicit";
  case FunctionType::explicit_memory_order:
    return "explicit_memory_order";
  case FunctionType::explicit_memory_scope:
    return "explicit_memory_scope";
  default:
    return "unknown";
  }
}

std::string to_string(MemoryOrder memory_order) {
  switch (memory_order) {
  case MemoryOrder::relaxed:
    return "memory_order_relaxed";
  case MemoryOrder::acquire:
    return "memory_order_acquire";
  case MemoryOrder::release:
    return "memory_order_release";
  case MemoryOrder::acq_rel:
    return "memory_order_acq_rel";
  case MemoryOrder::seq_cst:
    return "memory_order_seq_cst";
  default:
    return "unknown";
  }
}

std::string to_string(MemoryScope memory_scope) {
  switch (memory_scope) {
  case MemoryScope::work_item:
    return "memory_scope_work_item";
  case MemoryScope::work_group:
    return "memory_scope_work_group";
  case MemoryScope::device:
    return "memory_scope_device";
  case MemoryScope::all_svm_devices:
    return "memory_scope_all_svm_devices";
  default:
    return "unknown";
  }
}

std::string to_string(Operation operation) {
  switch (operation) {
  case Operation::addition:
    return "add";
  case Operation::subtraction:
    return "sub";
  case Operation::bitwise_or:
    return "or";
  case Operation::bitwise_xor:
    return "xor";
  case Operation::bitwise_and:
    return "and";
  case Operation::compute_min:
    return "min";
  case Operation::compute_max:
    return "max";
  default:
    return "unknown";
  }
}

std::string to_string(MemoryType memory_type) {
  switch (memory_type) {
  case MemoryType::global:
    return "global";
  case MemoryType::local:
    return "local";
  default:
    return "unknown";
  }
}

int get_local_work_size(const int global_work_size,
                        const cassian::Runtime &runtime) {
  const int max_size = runtime.get_device_property(
      cassian::DeviceProperty::max_total_group_size);
  return global_work_size > max_size ? max_size : global_work_size;
}

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
