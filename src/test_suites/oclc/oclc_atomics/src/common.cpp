/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/runtime/device_properties.hpp>
#include <cassian/runtime/openclc_utils.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cassian/utility/utility.hpp>
#include <common.hpp>
#include <stdexcept>
#include <test_config.hpp>

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

std::string to_string(MemoryFlag memory_flag) {
  switch (memory_flag) {
  case MemoryFlag::global:
    return "CLK_GLOBAL_MEM_FENCE";
  case MemoryFlag::local:
    return "CLK_LOCAL_MEM_FENCE";
  case MemoryFlag::image:
    return "CLK_IMAGE_MEM_FENCE";
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

std::string to_string(ComparisonType comparison_type) {
  switch (comparison_type) {
  case ComparisonType::weak:
    return "weak";
  case ComparisonType::strong:
    return "strong";
  default:
    return "unknown";
  }
}

std::string to_string(ComparisonResult comparison_result) {
  switch (comparison_result) {
  case ComparisonResult::success:
    return "success";
  case ComparisonResult::failure:
    return "failure";
  default:
    return "unknown";
  }
}

std::vector<int32_t> to_bool_vector(ComparisonResult comparison_result,
                                    int size) {
  switch (comparison_result) {
  case ComparisonResult::success:
    return std::vector<int32_t>(size, 1);
  case ComparisonResult::failure:
    return std::vector<int32_t>(size, 0);
  default:
    throw std::logic_error("Invalid ComparisonResult");
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

bool is_function_type_supported(cassian::Runtime *runtime,
                                const std::string &program_type,
                                FunctionType function_type) {
  switch (function_type) {
  case FunctionType::implicit:
    return cassian::check_optional_openclc_feature_support(
               runtime, program_type, "__opencl_c_atomic_order_seq_cst") &&
           cassian::check_optional_openclc_feature_support(
               runtime, program_type, "__opencl_c_atomic_scope_device");
  case FunctionType::explicit_memory_order:
    return cassian::check_optional_openclc_feature_support(
        runtime, program_type, "__opencl_c_atomic_scope_device");
  default:
    return true;
  }
}

bool is_memory_order_supported(cassian::Runtime *runtime,
                               const std::string &program_type,
                               MemoryOrder memory_order) {
  switch (memory_order) {
  case MemoryOrder::acquire:
  case MemoryOrder::release:
  case MemoryOrder::acq_rel:
    return cassian::check_optional_openclc_feature_support(
        runtime, program_type, "__opencl_c_atomic_order_acq_rel");
  case MemoryOrder::seq_cst:
    return cassian::check_optional_openclc_feature_support(
        runtime, program_type, "__opencl_c_atomic_order_seq_cst");
  default:
    return true;
  }
}

bool is_memory_scope_supported(cassian::Runtime *runtime,
                               const std::string &program_type,
                               MemoryScope memory_scope) {
  switch (memory_scope) {
  case MemoryScope::device:
    return cassian::check_optional_openclc_feature_support(
        runtime, program_type, "__opencl_c_atomic_scope_device");
  case MemoryScope::all_svm_devices:
    return cassian::check_optional_openclc_feature_support(
        runtime, program_type, "__opencl_c_atomic_scope_all_devices");
  default:
    return true;
  }
}

std::string memory_scope_build_option(const MemoryScope memory_scope) {
  return std::string(" -D MEMORY_SCOPE=") + to_string(memory_scope);
}

std::string work_group_size_build_option(const int size) {
  return std::string(" -D WORK_GROUP_SIZE=") + std::to_string(size);
}

std::string memory_order_build_option(const MemoryOrder memory_order) {
  return std::string(" -D MEMORY_ORDER=") + to_string(memory_order);
}

cassian::Kernel create_kernel(const std::string &path,
                              const std::string &build_options,
                              cassian::Runtime *runtime,
                              const std::string &program_type) {
  const std::string source = cassian::load_text_file(cassian::get_asset(path));
  const std::string kernel_name = "test_kernel";
  return runtime->create_kernel(kernel_name, source, build_options,
                                program_type);
}
