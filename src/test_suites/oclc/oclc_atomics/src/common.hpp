/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_OCLC_ATOMICS_COMMON_HPP
#define CASSIAN_OCLC_ATOMICS_COMMON_HPP

#include <cassian/runtime/runtime.hpp>
#include <cassian/test_harness/test_harness.hpp>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <test_config.hpp>
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
void function_type_requirements(cassian::Requirements &requirements,
                                const std::string &program_type,
                                FunctionType function_types);

enum class MemoryOrder { relaxed, acquire, release, acq_rel, seq_cst };
std::string to_string(MemoryOrder memory_order);
const std::vector<MemoryOrder> memory_orders_all = {
    MemoryOrder::relaxed, MemoryOrder::acquire, MemoryOrder::release,
    MemoryOrder::acq_rel, MemoryOrder::seq_cst};
void memory_order_requirements(cassian::Requirements &requirements,
                               const std::string &program_type,
                               MemoryOrder memory_order);

enum class MemoryScope { work_item, work_group, device, all_svm_devices };
std::string to_string(MemoryScope memory_scope);
const std::vector<MemoryScope> memory_scopes_all = {
    MemoryScope::work_group, MemoryScope::device, MemoryScope::all_svm_devices};
void memory_scope_requirements(cassian::Requirements &requirements,
                               const std::string &program_type,
                               MemoryScope memory_scope);

enum class MemoryFlag { global, local, image };
std::string to_string(MemoryFlag memory_flag);
const std::vector<MemoryFlag> memory_flags_all = {
    MemoryFlag::global, MemoryFlag::local, MemoryFlag::image};

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

enum class ComparisonType { weak, strong };
std::string to_string(ComparisonType comparison_type);
const std::vector<ComparisonType> comparison_types_all = {
    ComparisonType::weak, ComparisonType::strong};

enum class ComparisonResult { success, failure };
std::string to_string(ComparisonResult comparison_result);
const std::vector<ComparisonResult> comparison_results_all = {
    ComparisonResult::success, ComparisonResult::failure};
std::vector<int32_t> to_bool_vector(ComparisonResult comparison_result,
                                    int size);

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

std::string memory_scope_build_option(const MemoryScope memory_scope);

std::string work_group_size_build_option(const int size);

std::string memory_order_build_option(const MemoryOrder memory_order);

cassian::Kernel create_kernel(const std::string &path,
                              const std::string &build_options,
                              cassian::Runtime *runtime,
                              const std::string &program_type);

template <typename TEST_CASE_TYPE>
bool should_skip(const TEST_CASE_TYPE &test_case) {
  cassian::Requirements requirements;
  using test_type = typename TEST_CASE_TYPE::test_type;
  requirements.atomic_type<test_type>();
  return cassian::should_skip_test(requirements, *test_case.runtime);
}

template <typename TEST_CASE_TYPE>
TEST_CASE_TYPE create_test_case(const TestConfig &config) {
  TEST_CASE_TYPE test_case;
  test_case.runtime = config.runtime();
  test_case.global_work_size = config.work_size();
  test_case.local_work_size =
      get_local_work_size(test_case.global_work_size, *test_case.runtime);
  test_case.program_type = config.program_type();
  return test_case;
}

template <typename T> std::string atomic_type_build_option() {
  return std::string(" -D ATOMIC_TYPE=") + T::device_atomic_type;
}

template <typename T> std::string data_type_build_option() {
  return std::string(" -D DATA_TYPE=") + T::device_type;
}

#endif
