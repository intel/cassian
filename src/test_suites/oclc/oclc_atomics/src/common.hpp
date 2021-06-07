/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_OCLC_ATOMICS_COMMON_HPP
#define CASSIAN_OCLC_ATOMICS_COMMON_HPP

#include <cassian/runtime/runtime.hpp>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

enum class FunctionType {
  implicit,
  explicit_memory_order,
  explicit_memory_scope
};
std::string to_string(FunctionType function_type);
const std::vector<FunctionType> function_types_all = {
    FunctionType::implicit, FunctionType::explicit_memory_order,
    FunctionType::explicit_memory_scope};
bool is_function_type_supported(cassian::Runtime *runtime,
                                const std::string &program_type,
                                FunctionType function_types);

enum class MemoryOrder { relaxed, acquire, release, acq_rel, seq_cst };
std::string to_string(MemoryOrder memory_order);
const std::vector<MemoryOrder> memory_orders_all = {
    MemoryOrder::relaxed, MemoryOrder::acquire, MemoryOrder::release,
    MemoryOrder::acq_rel, MemoryOrder::seq_cst};
bool is_memory_order_supported(cassian::Runtime *runtime,
                               const std::string &program_type,
                               MemoryOrder memory_order);

enum class MemoryScope { work_item, work_group, device, all_svm_devices };
std::string to_string(MemoryScope memory_scope);
const std::vector<MemoryScope> memory_scopes_all = {
    MemoryScope::work_group, MemoryScope::device, MemoryScope::all_svm_devices};
bool is_memory_scope_supported(cassian::Runtime *runtime,
                               const std::string &program_type,
                               MemoryScope memory_scope);

enum class Operation {
  addition,
  subtraction,
  bitwise_or,
  bitwise_xor,
  bitwise_and,
  compute_min,
  compute_max
};
std::string to_string(Operation operation);

class UnknownOperationException : public std::runtime_error {
  using std::runtime_error::runtime_error;
};

enum class MemoryType { global, local };
std::string to_string(MemoryType memory_type);
const std::vector<MemoryType> memory_types_all = {MemoryType::global,
                                                  MemoryType::local};

int get_local_work_size(const int global_work_size,
                        const cassian::Runtime &runtime);

int suggest_work_size(const std::string &type);

#endif
