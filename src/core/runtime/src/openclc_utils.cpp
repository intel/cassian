/*
 * Copyright (C) 2026 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <string>

#include <cassian/runtime/openclc_utils.hpp>

namespace cassian {

std::string to_string(const AtomicMemoryScope memory_scope) {
  switch (memory_scope) {
  case AtomicMemoryScope::work_item:
    return "memory_scope_work_item";
  case AtomicMemoryScope::work_group:
    return "memory_scope_work_group";
  case AtomicMemoryScope::device:
    return "memory_scope_device";
  case AtomicMemoryScope::all_svm_devices:
    return "memory_scope_all_svm_devices";
  default:
    return "unknown";
  }
}

std::string to_string(const AtomicMemoryOrder memory_order) {
  switch (memory_order) {
  case AtomicMemoryOrder::relaxed:
    return "memory_order_relaxed";
  case AtomicMemoryOrder::acquire:
    return "memory_order_acquire";
  case AtomicMemoryOrder::release:
    return "memory_order_release";
  case AtomicMemoryOrder::acq_rel:
    return "memory_order_acq_rel";
  case AtomicMemoryOrder::seq_cst:
    return "memory_order_seq_cst";
  default:
    return "unknown";
  }
}

} // namespace cassian
